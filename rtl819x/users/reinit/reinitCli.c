/*
 * This file is the entrance of reinit client, it corresponding with reinit server
 */

#include <sys/time.h>
#include <unistd.h>

#include <libubox/ustream.h>

#include "libubus.h"
#include "apmib.h"
#define log_fprintf if(logoutput)fprintf

static struct ubus_context *ctx;
static struct blob_buf b;

static int logoutput=0;
enum {
	RETURN_CODE,
	RERURN_MSG,
	__RETURN_MAX
};

static const struct blobmsg_policy return_policy[__RETURN_MAX] = {
	[RETURN_CODE] = { .name = "rc", .type = BLOBMSG_TYPE_INT32 },
	[RERURN_MSG] = { .name = "msg", .type = BLOBMSG_TYPE_STRING}
};

static void reinit_return_data_cb(struct ubus_request *req,
				    int type, struct blob_attr *msg)
{
	struct blob_attr *tb[__RETURN_MAX];
	int rc;
	char*out_msg=NULL;

	blobmsg_parse(return_policy, __RETURN_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[RETURN_CODE]) {
		fprintf(stderr, "No return code received from server\n");
		return;
	}
	rc = blobmsg_get_u32(tb[RETURN_CODE]);
	log_fprintf(stderr,"return value form server is %d\n",rc);
	out_msg= blobmsg_get_string(tb[RERURN_MSG]);
	log_fprintf(stderr,"return msg form server is %s\n",out_msg);
	
}


static void reinit_client_main(REINIT_EVENT_ID reinit_event,int dataLen, char* databuf)
{
	static struct ubus_request req;
	uint32_t id;
	int rv=0;
	
	if (ubus_lookup_id(ctx, "reinit", &id)) {
		fprintf(stderr, "Failed to look up reinit object\n");
		return;
	}

	blob_buf_init(&b, 0);
	switch(reinit_event)
	{
		case REINIT_EVENT_MIBCHANGE:			
		case REINIT_EVENT_APPLYCAHNGES:
		case REINIT_EVENT_WAN_POWER_ON:			
		default:
			//fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);
			blobmsg_add_u32(&b, "reinit_event_id", reinit_event);
			blobmsg_add_u32(&b, "data_len", dataLen);
			blobmsg_add_string(&b, "data", databuf);
			log_fprintf(stderr,"%s:%d reinit_event=%d data=%s\n",__FUNCTION__,__LINE__,reinit_event,databuf);
			rv=ubus_invoke(ctx, id, "reinitSer", b.head, reinit_return_data_cb, 0, 0);
			log_fprintf(stderr,"%s:%d rv=%d\n",__FUNCTION__,__LINE__,rv);
			break;

	}
	//fprintf(stderr,"%s:%d\n",__FUNCTION__,__LINE__);

}

void reinitCliHelp()
{
	printf("Usage: reinitCli command\
				-e: reinit event id %d(REINIT_EVENT_MIBCHANGE) %d(REINIT_EVENT_APPLYCAHNGES)...\n",REINIT_EVENT_MIBCHANGE,REINIT_EVENT_APPLYCAHNGES);
	printf("	-l: data length,must set before data\
				-d: reinit parm data\
				-D: debug mode\n");
	exit(0);
}

int main(int argc, char **argv)
{
	char *file = NULL;
	char *strTmp=NULL;
	int intTmp=0;
	REINIT_EVENT_ID reinit_event=0;
	char *reinit_change_file=NULL;
	//char * data=NULL;
	char *dataBuf="";
	int dataBufLen=0;
	
	int ch;
	MibChangeNode_t changeNode={0};

	while ((ch = getopt(argc, argv, "e:d:l:D")) != -1) {
		switch (ch) {
		case 'e':
			intTmp = atoi(optarg);
			if(intTmp<=REINIT_EVENT_BEGIN||intTmp>=REINIT_EVENT_END)
				reinitCliHelp();
			reinit_event=intTmp;
			break;
		case 'l':
			dataBufLen=atoi(optarg);
			//must malloc (dataBufLen+1), and dataBuf[dataBufLen] is '\0'
			dataBuf=(char *)malloc(dataBufLen+1);
			if(!dataBuf) { fprintf(stderr,"malloc fail!\n"); exit(0);}
			bzero(dataBuf,dataBufLen+1);
			break;
		case 'd':
			//data=optarg;
			if(!dataBufLen) 
				reinitCliHelp();
			strncpy(dataBuf,optarg,dataBufLen);
			break;		
		case 'D':
			logoutput=1;
			break;
		default:
			reinitCliHelp();
			break;
		}
	}

	log_fprintf(stderr,"get event is %d\n",reinit_event);
	log_fprintf(stderr,"%s:%d get data=%s\n",__FUNCTION__,__LINE__,dataBuf);

	uloop_init();

	ctx = ubus_connect(NULL);
	if (!ctx) {
		fprintf(stderr, "Failed to connect to ubus\n");
		return -1;
	}

	ubus_add_uloop(ctx);
	//fprintf(stderr,"%s:%d dataBuf=%s\n",__FUNCTION__,__LINE__,dataBuf);

	reinit_client_main(reinit_event,dataBufLen,dataBuf);

	ubus_free(ctx);
	uloop_done();

	return 0;
}

