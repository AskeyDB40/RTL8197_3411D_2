/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2011  Nokia Corporation
 *  Copyright (C) 2011  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <glib.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#include "src/adapter.h"
#include "src/device.h"
#include "src/profile.h"
#include "src/plugin.h"

#include "lib/uuid.h"
#include "attrib/gattrib.h"
#include "attrib/att.h"
#include "attrib/gatt.h"
#include "attrib/att-database.h"
#include "src/shared/util.h"
#include "src/attrib-server.h"
#include "attrib/gatt-service.h"
#include "src/log.h"

#define CURRENT_TIME_SVC_UUID		0x1805
#define REF_TIME_UPDATE_SVC_UUID	0x1806

#define LOCAL_TIME_INFO_CHR_UUID	0x2A0F
#define TIME_UPDATE_CTRL_CHR_UUID	0x2A16
#define TIME_UPDATE_STAT_CHR_UUID	0x2A17
#define CT_TIME_CHR_UUID		0x2A2B

enum {
	UPDATE_RESULT_SUCCESSFUL = 0,
	UPDATE_RESULT_CANCELED = 1,
	UPDATE_RESULT_NO_CONN = 2,
	UPDATE_RESULT_ERROR = 3,
	UPDATE_RESULT_TIMEOUT = 4,
	UPDATE_RESULT_NOT_ATTEMPTED = 5,
};

enum {
	UPDATE_STATE_IDLE = 0,
	UPDATE_STATE_PENDING = 1,
};

enum {
	GET_REFERENCE_UPDATE = 1,
	CANCEL_REFERENCE_UPDATE = 2,
};

struct alert_adapter {
	struct btd_adapter *adapter;
	uint16_t hnd_ccc[1];
	uint16_t hnd_value[1];
	guint disc_id;

	struct btd_device *device;

	bool ccc_enable;
	unsigned int timeout_id;
};

struct notify_data {
	struct alert_adapter *al_adapter;
};

struct notify_callback {
	struct notify_data *notify_data;
	struct btd_device *device;
	guint id;
};

static GSList *alert_adapters = NULL;

static int encode_current_time(uint8_t value[10])
{
	struct timespec tp;
	struct tm tm;

	if (clock_gettime(CLOCK_REALTIME, &tp) == -1) {
		int err = -errno;

		error("clock_gettime: %s", strerror(-err));
		return err;
	}

	if (localtime_r(&tp.tv_sec, &tm) == NULL) {
		error("localtime_r() failed");
		/* localtime_r() does not set errno */
		return -EINVAL;
	}

	put_le16(1900 + tm.tm_year, &value[0]); /* Year */
	value[2] = tm.tm_mon + 1; /* Month */
	value[3] = tm.tm_mday; /* Day */
	value[4] = tm.tm_hour; /* Hours */
	value[5] = tm.tm_min; /* Minutes */
	value[6] = tm.tm_sec; /* Seconds */
	value[7] = tm.tm_wday == 0 ? 7 : tm.tm_wday; /* Day of Week */
	/* From Time Profile spec: "The number of 1/256 fractions of a second."
	 * In 1s there are 256 fractions, in 1ns there are 256/10^9 fractions.
	 * To avoid integer overflow, we use the equivalent 1/3906250 ratio. */
	value[8] = tp.tv_nsec / 3906250; /* Fractions256 */
	value[9] = 0x00; /* Adjust Reason */

	return 0;
}

static int adapter_cmp(gconstpointer a, gconstpointer b)
{
	const struct alert_adapter *al_adapter = a;
	const struct btd_adapter *adapter = b;

	return al_adapter->adapter == adapter ? 0 : -1;
}

static struct alert_adapter *find_alert_adapter(struct btd_adapter *adapter)
{
	GSList *l = g_slist_find_custom(alert_adapters, adapter, adapter_cmp);

	return l ? l->data : NULL;
}

static void attio_disconnected_cb(gpointer user_data)
{
	struct alert_adapter *adapter  = user_data;

	DBG("");

	adapter->ccc_enable = false;
	if (adapter->timeout_id) {
		timeout_remove(adapter->timeout_id);
		adapter->timeout_id = 0;
	}

	if (adapter->disc_id > 0) {
		DBG("Remmove attio callback and unref device");
		btd_device_remove_attio_callback(adapter->device,
						 adapter->disc_id);
		adapter->disc_id = 0;

		btd_device_unref(adapter->device);
		adapter->device = NULL;
	}
}

static void destroy_notify_callback(gpointer user_data)
{
	struct notify_callback *cb = user_data;

	DBG("");

	btd_device_remove_attio_callback(cb->device, cb->id);
	btd_device_unref(cb->device);
	g_free(cb->notify_data);
	g_free(cb);
}

static void attio_connected_cb(GAttrib *attrib, gpointer user_data)
{
	struct notify_callback *cb = user_data;
	struct notify_data *nd = cb->notify_data;
	struct alert_adapter *al_adapter = nd->al_adapter;
	size_t len;
	uint8_t *pdu = g_attrib_get_buffer(attrib, &len);
	uint8_t value[10];

	DBG("");

	if (encode_current_time(value) < 0)
		return;

	len = enc_notification(al_adapter->hnd_value[0], value, sizeof(value),
			pdu, len);

	DBG("Send notification for handle: 0x%04x, ccc 0x%04x",
			al_adapter->hnd_value[0],
			al_adapter->hnd_ccc[0]);

	g_attrib_send(attrib, 0, pdu, len, NULL, cb, destroy_notify_callback);

	return;

end:
	btd_device_remove_attio_callback(cb->device, cb->id);
	btd_device_unref(cb->device);
	g_free(cb->notify_data);
	g_free(cb);
}

static void filter_devices_notify(struct btd_device *device, void *user_data)
{
	struct notify_data *notify_data = user_data;
	struct alert_adapter *al_adapter = notify_data->al_adapter;
	struct notify_callback *cb;

	/* We only notify the current connected device */
	if (device != al_adapter->device)
		return;

	cb = g_new0(struct notify_callback, 1);
	cb->notify_data = notify_data;
	cb->device = btd_device_ref(device);
	cb->id = btd_device_add_attio_callback(device, attio_connected_cb, NULL,
					       cb);
}

static void notify_devices(struct alert_adapter *al_adapter)
{
	struct notify_data *notify_data;

	notify_data = g_new0(struct notify_data, 1);
	notify_data->al_adapter = al_adapter;

	btd_adapter_for_each_device(al_adapter->adapter, filter_devices_notify,
					notify_data);
}


static bool ct_timeout_cb(void *user_data)
{
	struct alert_adapter *adapter = user_data;

	DBG("");

	notify_devices(adapter);

	return true;
}

static void disconnect_cb(struct btd_device *dev, uint8_t reason)
{
	DBG("");
}

static uint8_t current_time_cccd_cb(struct attribute *a,
				struct btd_device *device,
				gpointer user_data)
{
	struct alert_adapter *adapter = user_data;

	DBG("handle 0x%04x", a->handle);

	if (a->len != 2)
		DBG("Invalid cccd value size: %zu", a->len);

	DBG("a->data[]: 0x%2.2x, 0x%2.2x", a->data[0], a->data[1]);

	if (a->data[0] == 0x01) {
		DBG("Enable current time notification");
		adapter->ccc_enable = true;
		if (!adapter->timeout_id)
			adapter->timeout_id = timeout_add(1000, ct_timeout_cb,
						    adapter, NULL);

		if (!adapter->disc_id) {
			adapter->device = btd_device_ref(device);
			adapter->disc_id =
				btd_device_add_attio_callback(device, NULL,
						attio_disconnected_cb, adapter);
		}

	} else if (a->data[0] == 0x00) {
		DBG("Disable current time notification");
		adapter->ccc_enable = false;
		if (adapter->timeout_id) {
			timeout_remove(adapter->timeout_id);
			adapter->timeout_id = 0;
		}
	}

	return 0;
}


static uint8_t current_time_read(struct attribute *a,
				 struct btd_device *device, gpointer user_data)
{
	struct btd_adapter *adapter = user_data;
	uint8_t value[10];

	if (encode_current_time(value) < 0)
		return ATT_ECODE_IO;

	attrib_db_update(adapter, a->handle, NULL, value, sizeof(value), NULL);

	return 0;
}

static uint8_t local_time_info_read(struct attribute *a,
				struct btd_device *device, gpointer user_data)
{
	struct btd_adapter *adapter = user_data;
	uint8_t value[2];

	DBG("a=%p", a);

	tzset();

	/* Convert POSIX "timezone" (seconds West of GMT) to Time Profile
	 * format (offset from UTC in number of 15 minutes increments). */
	value[0] = (uint8_t) (-1 * timezone / (60 * 15));

	/* FIXME: POSIX "daylight" variable only indicates whether there
	 * is DST for the local time or not. The offset is unknown. */
	value[1] = daylight ? 0xff : 0x00;

	attrib_db_update(adapter, a->handle, NULL, value, sizeof(value), NULL);

	return 0;
}

static gboolean register_current_time_service(struct alert_adapter *adap)
{
	struct btd_adapter *adapter = adap->adapter;
	bt_uuid_t uuid;

	bt_uuid16_create(&uuid, CURRENT_TIME_SVC_UUID);

	/* Current Time service */
	return gatt_service_add(adapter, GATT_PRIM_SVC_UUID, &uuid,
				/* CT Time characteristic */
				GATT_OPT_CHR_UUID16, CT_TIME_CHR_UUID,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ |
							GATT_CHR_PROP_NOTIFY,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						current_time_read, adapter,
						//current_time_read, adap,
				GATT_OPT_CCC_CB, ATTRIB_NOTIFY,
						current_time_cccd_cb, adap,
				GATT_OPT_CCC_GET_HANDLE, &adap->hnd_ccc[0],
				GATT_OPT_CHR_VALUE_GET_HANDLE, &adap->hnd_value[0],

				/* Local Time Information characteristic */
				GATT_OPT_CHR_UUID16, LOCAL_TIME_INFO_CHR_UUID,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						local_time_info_read, adapter,

				GATT_OPT_INVALID);
}

static uint8_t time_update_control(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
	DBG("handle 0x%04x", a->handle);

	if (a->len != 1)
		DBG("Invalid control point value size: %zu", a->len);

	switch (a->data[0]) {
	case GET_REFERENCE_UPDATE:
		DBG("Get Reference Update");
		break;
	case CANCEL_REFERENCE_UPDATE:
		DBG("Cancel Reference Update");
		break;
	default:
		DBG("Unknown command: 0x%02x", a->data[0]);
	}

	return 0;
}

static uint8_t time_update_status(struct attribute *a,
						struct btd_device *device,
						gpointer user_data)
{
	struct btd_adapter *adapter = user_data;
	uint8_t value[2];

	DBG("handle 0x%04x", a->handle);

	value[0] = UPDATE_STATE_IDLE;
	value[1] = UPDATE_RESULT_SUCCESSFUL;
	attrib_db_update(adapter, a->handle, NULL, value, sizeof(value), NULL);

	return 0;
}

static gboolean register_ref_time_update_service(struct btd_adapter *adapter)
{
	bt_uuid_t uuid;

	bt_uuid16_create(&uuid, REF_TIME_UPDATE_SVC_UUID);

	/* Reference Time Update service */
	return gatt_service_add(adapter, GATT_PRIM_SVC_UUID, &uuid,
				/* Time Update control point */
				GATT_OPT_CHR_UUID16, TIME_UPDATE_CTRL_CHR_UUID,
				GATT_OPT_CHR_PROPS,
					GATT_CHR_PROP_WRITE_WITHOUT_RESP,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_WRITE,
						time_update_control, adapter,

				/* Time Update status */
				GATT_OPT_CHR_UUID16, TIME_UPDATE_STAT_CHR_UUID,
				GATT_OPT_CHR_PROPS, GATT_CHR_PROP_READ,
				GATT_OPT_CHR_VALUE_CB, ATTRIB_READ,
						time_update_status, adapter,

				GATT_OPT_INVALID);
}

static int time_server_init(struct btd_profile *p, struct btd_adapter *adapter)
{
	const char *path = adapter_get_path(adapter);
	struct alert_adapter *al_adapter;

	DBG("path %s", path);

	al_adapter = g_new0(struct alert_adapter, 1);
	al_adapter->adapter = btd_adapter_ref(adapter);

	alert_adapters = g_slist_append(alert_adapters, al_adapter);

	if (!register_current_time_service(al_adapter)) {
		error("Current Time Service could not be registered");
		return -EIO;
	}

	if (!register_ref_time_update_service(adapter)) {
		error("Reference Time Update Service could not be registered");
		return -EIO;
	}

	return 0;
}

static void time_server_exit(struct btd_profile *p,
						struct btd_adapter *adapter)
{
	const char *path = adapter_get_path(adapter);
	struct alert_adapter *al_adapter;

	DBG("path %s", path);

	al_adapter = find_alert_adapter(adapter);
	if (!al_adapter)
		return;

	alert_adapters = g_slist_remove(alert_adapters, al_adapter);
	btd_adapter_unref(al_adapter->adapter);

	g_free(al_adapter);

}

struct btd_profile time_profile = {
	.name		= "gatt-time-server",
	.adapter_probe	= time_server_init,
	.adapter_remove	= time_server_exit,
};

static int time_init(void)
{
	return btd_profile_register(&time_profile);
}

static void time_exit(void)
{
	btd_profile_unregister(&time_profile);
}

BLUETOOTH_PLUGIN_DEFINE(time, VERSION,
			BLUETOOTH_PLUGIN_PRIORITY_DEFAULT,
			time_init, time_exit)
