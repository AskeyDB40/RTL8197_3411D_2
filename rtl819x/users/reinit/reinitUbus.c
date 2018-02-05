/*
 * Copyright (C) 2011-2014 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <unistd.h>
#include <signal.h>
#include <syslog.h>

#include <libubox/blobmsg_json.h>
#include "libubus.h"
#include "reinitSer.h"
#include "reinitEvent.h"
#include "reinitHandle.h"

static struct ubus_context *ctx;

static struct blob_buf b;

enum {
	LOG_LEVEL,
	__LOG_LEVEL_MAX
};

static const struct blobmsg_policy setLogLevel_policy[]={
	[LOG_LEVEL] = { .name = "level", .type = BLOBMSG_TYPE_INT32 },
};

void logLevelShow()
{
	fprintf(stderr,"log level must between 0-7:\n 0(LOG_EMERG)\n 1(LOG_ALERT)\n 2(LOG_CRIT)\n 3(LOG_ERR)\n 4(LOG_WARNING)\n 5(LOG_NOTICE)\n 6(LOG_INFO)\n 7(LOG_DEBUG)\n");
}
static int setLogLevel(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_attr *tb[__LOG_LEVEL_MAX];
	int logLevel=-1;

	blobmsg_parse(setLogLevel_policy, ARRAY_SIZE(setLogLevel_policy), tb, blob_data(msg), blob_len(msg));

	if(!tb[LOG_LEVEL])
	{
		logLevelShow();
		return UBUS_STATUS_INVALID_ARGUMENT;
	}

	logLevel=blobmsg_get_u32(tb[LOG_LEVEL]);
	if(logLevel<LOG_EMERG || logLevel>LOG_DEBUG)
		logLevelShow();

	debugLevel=logLevel;
	reinitSer_printf(LOG_DEBUG,"%s:%d pid=%d\n",__FUNCTION__,__LINE__,getpid());

	return 0;
}

enum {
	DUMP_CMD,
	__DUMP_MAX
};

static const struct blobmsg_policy dump_policy[] = {
	[DUMP_CMD] = { .name = "cmd", .type = BLOBMSG_TYPE_STRING },
};


void dump_show()
{
	fprintf(stderr,"\
		dump show cmd:\
		mclist: mib change list\
		getLogLevel: show log level");
}

static int dump_info(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_attr *tb[__DUMP_MAX];
	char * in_cmd=NULL;

	blobmsg_parse(dump_policy, ARRAY_SIZE(dump_policy), tb, blob_data(msg), blob_len(msg));

	if(!tb[DUMP_CMD])
	{
		dump_show();
		return UBUS_STATUS_INVALID_ARGUMENT;
	}

	in_cmd=blobmsg_get_string(tb[DUMP_CMD]);

	if(strcmp(in_cmd,"mclist")==0)
	{
		MibChangeNode_tp pnode=NULL;
		fprintf(stderr,"\n total %d mib changed:\n",mibChangelist.count);
		if(mibChangelist.count==0) return 0;
		
		list_for_each_entry(pnode,&(mibChangelist.changeNodeHead.list),list)
		{
			dumpMibChangeListInfo(pnode);
		}
	}else
	if(strcmp(in_cmd,"getLogLevel")==0)
	{
		fprintf(stderr,"\ncurrent log level is %d\n",debugLevel);
	}else
	{
		dump_show();
	}
	reinitSer_printf(LOG_DEBUG,"%s:%d pid=%d\n",__FUNCTION__,__LINE__,getpid());

	return 0;
}

enum {
	REINIT_ID,
	REINIT_DATA_LEN,
	REINIT_DATA,
	__REINIT_MAX
};

static const struct blobmsg_policy reinitSer_policy[__REINIT_MAX] = {
	[REINIT_ID] = { .name = "reinit_event_id", .type = BLOBMSG_TYPE_INT32 },
	[REINIT_DATA_LEN] = { .name = "data_len", .type = BLOBMSG_TYPE_INT32 },		
	[REINIT_DATA] = { .name = "data", .type = BLOBMSG_TYPE_STRING },
};


static int reinit_server(struct ubus_context *ctx, struct ubus_object *obj,
		      struct ubus_request_data *req, const char *method,
		      struct blob_attr *msg)
{
	struct blob_attr *tb[__REINIT_MAX];
	int retVal=0;
	int in_len=0;
	int in_id=0;
	char * in_msg=NULL;
	char *outMsg=(char*)malloc(256);
	if(!outMsg){reinitSer_printf(LOG_EMERG,"%s:%d malloc fail!\n",__FUNCTION__,__LINE__);exit(0);}
	bzero(outMsg,256);
	reinitSer_printf(LOG_DEBUG,"%s:%d pid=%d\n",__FUNCTION__,__LINE__,getpid());

	blobmsg_parse(reinitSer_policy, __REINIT_MAX, tb, blob_data(msg), blob_len(msg));
	if (!tb[REINIT_ID])
	{
		free(outMsg);
		return UBUS_STATUS_INVALID_ARGUMENT;
	}

	if(!tb[REINIT_DATA_LEN]||!tb[REINIT_DATA])
	{
		fprintf(stderr,"invalid input\n");
		
		return UBUS_STATUS_INVALID_ARGUMENT;
	}
	in_id = blobmsg_get_u32(tb[REINIT_ID]);
	in_len= blobmsg_get_u32(tb[REINIT_DATA_LEN]);
	in_msg=blobmsg_get_string(tb[REINIT_DATA]);

	reinitSer_printf(LOG_DEBUG,"%s:%d id=%d\n",__FUNCTION__,__LINE__,in_id);
	reinitSer_printf(LOG_DEBUG,"%s:%d msgLen=%d\n",__FUNCTION__,__LINE__,in_len);
	reinitSer_printf(LOG_DEBUG,"%s:%d msg=%s\n",__FUNCTION__,__LINE__,in_msg);
	blob_buf_init(&b, 0);

	
	retVal=reinit_event_process(in_id,in_msg,in_len,outMsg);
	
	//blobmsg_add_u32(&b, "rc", retVal);
	//blobmsg_add_string(&b,"msg",outMsg);

	
	//ubus_send_reply(ctx, req, b.head);
	
	reinitSer_printf(LOG_DEBUG,"%s:%d outMsg=%s\n",__FUNCTION__,__LINE__,outMsg);
	free(outMsg);
	reinitSer_printf(LOG_DEBUG,"%s:%d pid=%d\n",__FUNCTION__,__LINE__,getpid());
	return 0;
}



static const struct ubus_method reinit_methods[] = {
	UBUS_METHOD("dump", dump_info, dump_policy),
	UBUS_METHOD("reinitSer", reinit_server, reinitSer_policy),
	UBUS_METHOD("setLogLevel", setLogLevel, setLogLevel_policy),

};

static struct ubus_object_type reinit_object_type =
	UBUS_OBJECT_TYPE("reinit", reinit_methods);

static struct ubus_object reinit_object = {
	.name = "reinit",
	.type = &reinit_object_type,
	.methods = reinit_methods,
	.n_methods = ARRAY_SIZE(reinit_methods),
};

static void regUbusServer(void)
{
	int ret;

	ret = ubus_add_object(ctx, &reinit_object);
	if (ret)
		reinitSer_printf(LOG_ERR, "Failed to add object: %s\n", ubus_strerror(ret));

	uloop_run();
}

int reinitUbusMain()
{

	uloop_init();

	ctx = ubus_connect(NULL);
	if (!ctx) {
		reinitSer_printf(LOG_ERR, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);

	regUbusServer();

	ubus_free(ctx);
	uloop_done();

	return 0;
}
