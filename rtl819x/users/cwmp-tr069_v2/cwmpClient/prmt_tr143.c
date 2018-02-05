#include <sys/time.h>
#include <signal.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "apmib.h"
#include "prmt_tr143.h"
#include "prmt_utility.h"

extern void *shmat(int shmid, const void *shmaddr, int shmflg);

#ifdef _PRMT_TR143_


int clearWanUDPEchoItf( void );
int setWanUDPEchoItf( unsigned char ifindex );
int getWanUDPEchoItf( unsigned char *pifindex);
/*****************************************************************************************************/
char *strTR143State[] =
{
	"None",
	"Requested",
	"Completed",
	"Error_InitConnectionFailed",
	"Error_NoResponse",
	"Error_PasswordRequestFailed",
	"Error_LoginFailed",
	"Error_NoTransferMode",
	"Error_NoPASV",
	//download
	"Error_TransferFailed",
	"Error_IncorrectSize",
	"Error_Timeout",
	//upload
	"Error_NoCWD",
	"Error_NoSTOR",
	"Error_NoTransferComplete",
	
	"" /*eTR143_End, last one*/
};

struct TR143_Diagnostics gDownloadDiagnostics =
{
	DLWAY_DOWN,	/*Way*/
	eTR143_None,	/*DiagnosticsState*/
	NULL,		/*pInterface*/
	"",		/*IfName*/
	NULL,		/*pURL*/
	0,		/*DSCP*/
	0,		/*EthernetPriority*/
	0,		/*TestFileLength*/
	{0,0},		/*ROMTime*/
	{0,0},		/*BOMTime*/
	{0,0},		/*EOMTime*/
	0,		/*TestBytesReceived*/
	0,		/*TotalBytesReceived*/
	0,		/*TotalBytesSent*/
	{0,0},		/*TCPOpenRequestTime*/
	{0,0},		/*TCPOpenResponseTime*/	
	0,		/*http_pid*/
	0		/*ftp_pid*/
};
int gStartTR143DownloadDiag=0;

struct TR143_Diagnostics gUploadDiagnostics =
{
	DLWAY_UP,	/*Way*/
	eTR143_None,	/*DiagnosticsState*/
	NULL,		/*pInterface*/
	"",		/*IfName*/
	NULL,		/*pURL*/
	0,		/*DSCP*/
	0,		/*EthernetPriority*/
	0,		/*TestFileLength*/
	{0,0},		/*ROMTime*/
	{0,0},		/*BOMTime*/
	{0,0},		/*EOMTime*/
	0,		/*TestBytesReceived*/
	0,		/*TotalBytesReceived*/
	0,		/*TotalBytesSent*/
	{0,0},		/*TCPOpenRequestTime*/
	{0,0},		/*TCPOpenResponseTime*/	
	0,		/*http_pid*/
	0		/*ftp_pid*/
};
int gStartTR143UploadDiag=0;

/******UDPEchoConfig***************************************************************************/
struct CWMP_OP tUDPEchoConfigLeafOP = { getUDPEchoConfig, setUDPEchoConfig };
struct CWMP_PRMT tUDPEchoConfigLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tUDPEchoConfigLeafOP},
{"Interface",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tUDPEchoConfigLeafOP},
{"SourceIPAddress",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tUDPEchoConfigLeafOP},
{"UDPPort",			eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,	&tUDPEchoConfigLeafOP},
{"EchoPlusEnabled",		eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tUDPEchoConfigLeafOP},
{"EchoPlusSupported",		eCWMP_tBOOLEAN,	CWMP_READ,		&tUDPEchoConfigLeafOP},
{"PacketsReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tUDPEchoConfigLeafOP},
{"PacketsResponded",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tUDPEchoConfigLeafOP},
{"BytesReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tUDPEchoConfigLeafOP},
{"BytesResponded",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,&tUDPEchoConfigLeafOP},
{"TimeFirstPacketReceived",	eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,&tUDPEchoConfigLeafOP},
{"TimeLastPacketReceived",	eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,&tUDPEchoConfigLeafOP},
};
enum eUDPEchoConfigLeaf
{
	eUEC_Enable,
	eUEC_Interface,
	eUEC_SourceIPAddress,
	eUEC_UDPPort,
	eUEC_EchoPlusEnabled,
	eUEC_EchoPlusSupported,
	eUEC_PacketsReceived,
	eUEC_PacketsResponded,
	eUEC_BytesReceived,
	eUEC_BytesResponded,
	eUEC_TimeFirstPacketReceived,
	eUEC_TimeLastPacketReceived
};
struct CWMP_LEAF tUDPEchoConfigLeaf[] =
{
{ &tUDPEchoConfigLeafInfo[eUEC_Enable] },
{ &tUDPEchoConfigLeafInfo[eUEC_Interface] },
{ &tUDPEchoConfigLeafInfo[eUEC_SourceIPAddress] },
{ &tUDPEchoConfigLeafInfo[eUEC_UDPPort] },
{ &tUDPEchoConfigLeafInfo[eUEC_EchoPlusEnabled] },
{ &tUDPEchoConfigLeafInfo[eUEC_EchoPlusSupported] },
{ &tUDPEchoConfigLeafInfo[eUEC_PacketsReceived] },
{ &tUDPEchoConfigLeafInfo[eUEC_PacketsResponded] },
{ &tUDPEchoConfigLeafInfo[eUEC_BytesReceived] },
{ &tUDPEchoConfigLeafInfo[eUEC_BytesResponded] },
{ &tUDPEchoConfigLeafInfo[eUEC_TimeFirstPacketReceived] },
{ &tUDPEchoConfigLeafInfo[eUEC_TimeLastPacketReceived] },
{ NULL }
};

/******UploadDiagnostics***************************************************************************/
struct CWMP_OP tUploadDiagnosticsLeafOP = { getUploadDiagnostics, setUploadDiagnostics };
struct CWMP_PRMT tUploadDiagnosticsLeafInfo[] =
{
/*(name,			type,		flag,					op)*/
{"DiagnosticsState",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ|CWMP_DENY_ACT,	&tUploadDiagnosticsLeafOP},
{"Interface",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,			&tUploadDiagnosticsLeafOP},
{"UploadURL",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,			&tUploadDiagnosticsLeafOP},
//{"UploadTransports",	eCWMP_tSTRING,	CWMP_READ,			&tUploadDiagnosticsLeafOP},
{"DSCP",			eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,			&tUploadDiagnosticsLeafOP},
{"EthernetPriority",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,			&tUploadDiagnosticsLeafOP},
{"TestFileLength",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,			&tUploadDiagnosticsLeafOP},
{"ROMTime",			eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tUploadDiagnosticsLeafOP},
{"BOMTime",			eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tUploadDiagnosticsLeafOP},
{"EOMTime",			eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tUploadDiagnosticsLeafOP},
{"TotalBytesSent",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,		&tUploadDiagnosticsLeafOP},
{"TCPOpenRequestTime",		eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tUploadDiagnosticsLeafOP},
{"TCPOpenResponseTime",		eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tUploadDiagnosticsLeafOP},
};
enum eUploadDiagnosticsLeaf
{
	eUD_DiagnosticsState,
	eUD_Interface,
	eUD_UploadURL,
	//eUD_UploadTransports,
	eUD_DSCP,
	eUD_EthernetPriority,
	eUD_TestFileLength,
	eUD_ROMTime,
	eUD_BOMTime,
	eUD_EOMTime,
	eUD_TotalBytesSent,
	eUD_TCPOpenRequestTime,
	eUD_TCPOpenResponseTime
};
struct CWMP_LEAF tUploadDiagnosticsLeaf[] =
{
{ &tUploadDiagnosticsLeafInfo[eUD_DiagnosticsState] },
{ &tUploadDiagnosticsLeafInfo[eUD_Interface] },
{ &tUploadDiagnosticsLeafInfo[eUD_UploadURL] },
//{ &tUploadDiagnosticsLeafInfo[eUD_UploadTransports] },
{ &tUploadDiagnosticsLeafInfo[eUD_DSCP] },
{ &tUploadDiagnosticsLeafInfo[eUD_EthernetPriority] },
{ &tUploadDiagnosticsLeafInfo[eUD_TestFileLength] },
{ &tUploadDiagnosticsLeafInfo[eUD_ROMTime] },
{ &tUploadDiagnosticsLeafInfo[eUD_BOMTime] },
{ &tUploadDiagnosticsLeafInfo[eUD_EOMTime] },
{ &tUploadDiagnosticsLeafInfo[eUD_TotalBytesSent] },
{ &tUploadDiagnosticsLeafInfo[eUD_TCPOpenRequestTime] },
{ &tUploadDiagnosticsLeafInfo[eUD_TCPOpenResponseTime] },
{ NULL }
};

/******DownloadDiagnostics***************************************************************************/
struct CWMP_OP tDownloadDiagnosticsLeafOP = { getDownloadDiagnostics, setDownloadDiagnostics };
struct CWMP_PRMT tDownloadDiagnosticsLeafInfo[] =
{
/*(name,			type,		flag,					op)*/
{"DiagnosticsState",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ|CWMP_DENY_ACT,	&tDownloadDiagnosticsLeafOP},
{"Interface",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,			&tDownloadDiagnosticsLeafOP},
{"DownloadURL",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,			&tDownloadDiagnosticsLeafOP},
//{"DownloadTransports",	eCWMP_tSTRING,	CWMP_READ,			&tDownloadDiagnosticsLeafOP},
{"DSCP",			eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,			&tDownloadDiagnosticsLeafOP},
{"EthernetPriority",		eCWMP_tUINT,	CWMP_WRITE|CWMP_READ,			&tDownloadDiagnosticsLeafOP},
{"ROMTime",			eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tDownloadDiagnosticsLeafOP},
{"BOMTime",			eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tDownloadDiagnosticsLeafOP},
{"EOMTime",			eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tDownloadDiagnosticsLeafOP},
{"TestBytesReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,		&tDownloadDiagnosticsLeafOP},
{"TotalBytesReceived",		eCWMP_tUINT,	CWMP_READ|CWMP_DENY_ACT,		&tDownloadDiagnosticsLeafOP},
{"TCPOpenRequestTime",		eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tDownloadDiagnosticsLeafOP},
{"TCPOpenResponseTime",		eCWMP_tDATETIME,CWMP_READ|CWMP_DENY_ACT,		&tDownloadDiagnosticsLeafOP},
};
enum eDownloadDiagnosticsLeaf
{
	eDD_DiagnosticsState,
	eDD_Interface,
	eDD_DownloadURL,
//    eDD_DownloadTransports,
	eDD_DSCP,
	eDD_EthernetPriority,
	eDD_ROMTime,
	eDD_BOMTime,
	eDD_EOMTime,
	eDD_TestBytesReceived,
	eDD_TotalBytesReceived,
	eDD_TCPOpenRequestTime,
	eDD_TCPOpenResponseTime
};
struct CWMP_LEAF tDownloadDiagnosticsLeaf[] =
{
{ &tDownloadDiagnosticsLeafInfo[eDD_DiagnosticsState] },
{ &tDownloadDiagnosticsLeafInfo[eDD_Interface] },
{ &tDownloadDiagnosticsLeafInfo[eDD_DownloadURL] },
//{ &tDownloadDiagnosticsLeafInfo[eDD_DownloadTransports] },
{ &tDownloadDiagnosticsLeafInfo[eDD_DSCP] },
{ &tDownloadDiagnosticsLeafInfo[eDD_EthernetPriority] },
{ &tDownloadDiagnosticsLeafInfo[eDD_ROMTime] },
{ &tDownloadDiagnosticsLeafInfo[eDD_BOMTime] },
{ &tDownloadDiagnosticsLeafInfo[eDD_EOMTime] },
{ &tDownloadDiagnosticsLeafInfo[eDD_TestBytesReceived] },
{ &tDownloadDiagnosticsLeafInfo[eDD_TotalBytesReceived] },
{ &tDownloadDiagnosticsLeafInfo[eDD_TCPOpenRequestTime] },
{ &tDownloadDiagnosticsLeafInfo[eDD_TCPOpenResponseTime] },
{ NULL }
};

/******PerformanceDiagnostic***************************************************************************/
struct CWMP_OP tPerformanceDiagnosticLeafOP = { getPerformanceDiagnostic, NULL };
struct CWMP_PRMT tPerformanceDiagnosticLeafInfo[] =
{
/*(name,			type,		flag,		op)*/
{"DownloadTransports",		eCWMP_tSTRING,	CWMP_READ,	&tPerformanceDiagnosticLeafOP},
{"UploadTransports",		eCWMP_tSTRING,	CWMP_READ,	&tPerformanceDiagnosticLeafOP},
};
enum ePerformanceDiagnosticLeaf
{
	eDownloadTransports,
	eUploadTransports
};
struct CWMP_LEAF tPerformanceDiagnosticLeaf[] =
{
{ &tPerformanceDiagnosticLeafInfo[eDownloadTransports] },
{ &tPerformanceDiagnosticLeafInfo[eUploadTransports] },
{ NULL }
};
/*****************************************************************************************************/
int getShmem( void **t, int s, char *name )
{
	key_t key;
	int shm_id;

	if(t==NULL || s<=0 || name==NULL)
		return -1;

	key = ftok(name, SHM_PROJ_ID);
	if(key==-1)
	{
		//perror("ftok");
		return -1;
	}
	
	shm_id=shmget(key, s , 0);
	if(shm_id==-1)
	{
		//perror("shmget");
		return -1;		
	}
	
	*t=shmat(shm_id,NULL,0);
	if( *t==(void *)-1 )
	{
		//perror("shmget");
		return -1;		
	}
	
	return 0;
	
}

int detachShmem( void *t )
{
	if( shmdt(t)==-1 )
	{
		//perror( "shmdt" );
		return -1;
	}
	return 0;
}

/***********************************************************************/
/*************** TR143 HTTP Download/Upload Diagnostics ****************/
/***********************************************************************/
int findItfNamebyAddr(struct in_addr *a, char *n)
{
	#define MAX_ITF         48
	struct ifreq itf_ifreq[MAX_ITF];
	struct ifreq *itf_ifreq_end, *c;
	struct ifconf IoCtlReq;
	int s;

	if(!a || !n) return -1;
	s = socket( AF_INET, SOCK_DGRAM, 0 );
	if( s<0 ) return -1;
	IoCtlReq.ifc_buf = (void *)itf_ifreq;
	IoCtlReq.ifc_len = sizeof( itf_ifreq );
	if( ioctl( s, SIOCGIFCONF, &IoCtlReq ) < 0 )
	{
		close(s);
		return -1;
	}
	itf_ifreq_end = (void *)((char *)itf_ifreq + IoCtlReq.ifc_len);

	for( c=itf_ifreq; c<itf_ifreq_end; c++ )
	{
		struct sockaddr_in *p;
		
		fprintf( stderr, "findItfNamebyAddr>got name=%s\n", c->ifr_name );
		p=(struct sockaddr_in *)&c->ifr_addr;
		fprintf( stderr, "findItfNamebyAddr>got p->sin_family=%d, %s\n", p->sin_family, inet_ntoa(p->sin_addr) );
		if( (p->sin_family==AF_INET) && (p->sin_addr.s_addr==a->s_addr) )
		{
			strcpy( n, c->ifr_name );
			fprintf( stderr, "findItfNamebyAddr>got match\n" );
			break;
		}
	}
	close(s);	
	return 0;
}


void TR143HttpDiagClientEnd(struct soap *soap)
{
	CWMPDBG( 1, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__) );

	if(soap)
	{
		soap_destroy(soap);
		soap_end(soap);
		soap_done(soap);
	}
}

int TR143HttpDiagClientInit( struct soap *soap, void* data )
{
	CWMPDBG( 1, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__) );

	if( soap==NULL ) return -1;
	soap_init(soap);
	soap->connect_timeout=10;
	soap->user=data;
	return 0;

http_client_init_err:
	soap_print_fault(soap, stderr);
	http_client_end(soap);
	return -1;
}

int TR143HttpDiagClientSend( struct soap *soap, struct TR143_Diagnostics *p, struct netdevstate *s )
{
	int action;
	
	CWMPDBG( 1, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__) );

	if( soap_begin_count(soap) || soap_end_count(soap) )
		return soap->error;

	switch( p->Way )
	{	
	case DLWAY_UP:
		{
		// struct stat st;
		// if( stat( filename, &st)==0 ) soap->count = st.st_size;
		 soap->count = p->TestFileLength;
		 soap->http_content = "text/plain";
		 action = SOAP_PUT;
		}
		break;
	case DLWAY_DOWN:
		action = SOAP_GET;
		break;
	default:
		return -1;
	}

	if( soap_connect_command(soap, action, p->pURL, NULL) )
	{
		soap_print_fault(soap, stderr);
		return -1;
	}

	if( soap_flush(soap) )
	{
		soap_print_fault(soap, stderr);
		return -1;
	}

	p->TCPOpenRequestTime = soap->tcp_open_request_time;
	p->TCPOpenResponseTime = soap->tcp_open_response_time;
	p->ROMTime = soap->start_send_time;
	if( strlen(p->IfName)==0 )
	{
		struct sockaddr_in myaddr;
		int myaddrlen=sizeof(struct sockaddr_in);
		if( getsockname( soap->socket, (struct sockaddr *)&myaddr, &myaddrlen )< 0 )
			perror( "getsockname" );
		else{
			fprintf( stderr, "my out addr=%s\n", inet_ntoa(myaddr.sin_addr) );
			findItfNamebyAddr( &myaddr.sin_addr, p->IfName );
		}
	}
	if( strlen(p->IfName)>0 ) getInterfaceStat( p->IfName, &s->bytes_sent, &s->bytes_recv, &s->packets_sent, &s->packets_recv );
	fprintf( stderr, "%s> int=%s, %d,%d,%d,%d\n", __FUNCTION__, p->IfName, 
				s->bytes_sent, s->bytes_recv, s->packets_sent, s->packets_recv );

	switch(p->Way)
	{
	case DLWAY_UP:
		{
			char rbuf[64];
			unsigned int  rsize=p->TestFileLength;

			memset( rbuf, 'A', sizeof(rbuf) );
			gettimeofday( &p->BOMTime, NULL );
			
			while(rsize)
			{
				if( rsize>64 )
				{
					soap_send_raw( soap,rbuf,64 );
					rsize-=64;
				}else
				{
					soap_send_raw( soap,rbuf,rsize );
					rsize-=rsize;
				}
			}

			if( soap_end_send(soap) )
			{
				soap_print_fault(soap, stderr);
				return -1;
			}
		}
		break;
	case DLWAY_DOWN:
		break;
	}

	return 0;
}

int TR143HttpDiagClientRecv( struct soap *soap, struct TR143_Diagnostics *p, struct netdevstate *s )
{
	int	ret=-1;

	CWMPDBG( 1, ( stderr, "<%s:%d>\n", __FUNCTION__, __LINE__) );

	if( soap_begin_recv(soap) )
	{
		p->DiagnosticsState=eTR143_Error_NoResponse;
		return soap->error;
	}

	switch(p->Way)
	{
	case DLWAY_DOWN:
		{
			unsigned int c, buf_index=0;
			
			//fprintf( stderr, "content-length:%d <SOAP_BUFLEN:%d>\n", c_soap.length,SOAP_BUFLEN );
			//fprintf( stderr, "%s", & c_soap.buf[c_soap.bufidx] );
#if 0
			while( (c=soap_get1(soap))!=EOF )
			{  
				buf_index++;
			}
#else
			while(1)
			{
				//soap_get1() refered
				if (soap->bufidx >= soap->buflen && soap_recv(soap)) break;
				buf_index=buf_index+(soap->buflen-soap->bufidx);
				soap->bufidx=soap->buflen;
			}
#endif

			gettimeofday( &p->EOMTime, NULL );
			p->BOMTime = soap->start_recv_time;
			p->TestBytesReceived = soap->test_bytes_rx;
			
			if(soap->length!=buf_index) 
				p->DiagnosticsState=eTR143_Error_TransferFailed;				
			else
				p->DiagnosticsState=eTR143_Completed;

			ret=0;
		}
		break;
	case DLWAY_UP:
		{
			p->EOMTime = soap->start_recv_time;
			p->DiagnosticsState=eTR143_Completed;
			ret=0;
		}
		break;
	default:
		p->DiagnosticsState=eTR143_Error_NoResponse;
		break;
	}

	soap_end_recv(soap);
	if( strlen(p->IfName)>0 ) getInterfaceStat( p->IfName, &s->bytes_sent, &s->bytes_recv, &s->packets_sent, &s->packets_recv );		
	fprintf( stderr, "%s> int=%s, %d,%d,%d,%d\n", __FUNCTION__, p->IfName, 
				s->bytes_sent, s->bytes_recv, s->packets_sent, s->packets_recv );

	return ret;
}

int TR143HttpDiagClient( struct TR143_Diagnostics *p )
{
	struct soap *c_soap = NULL;
	int ret=-1;
	struct netdevstate start_state, end_state;

	if(!p)
	{ 
		p->DiagnosticsState=eTR143_Error_InitConnectionFailed;
		return -1;
	}
	CWMPDBG( 1, ( stderr, "<%s:%d>start, Way=%d, pURL=%s, IfName=%s, DSCP=%u, EthernetPriority=%u, TestFileLength=%u\n",
				__FUNCTION__, __LINE__, p->Way, p->pURL?p->pURL:"", p->IfName, p->DSCP, p->EthernetPriority, p->TestFileLength ) );

	c_soap = malloc(sizeof(struct soap));
	if(c_soap == NULL)
	{
		printf("<%s:%d> malloc fail!\n", __FUNCTION__, __LINE__);
		return ERR_9002;
	}

	//init
	if( TR143HttpDiagClientInit( c_soap, p ) )
	{
		free(c_soap);
		p->DiagnosticsState=eTR143_Error_InitConnectionFailed;
		 return -1;
	}

	soap_begin(c_soap);
	c_soap->userid = NULL;
	c_soap->passwd = NULL;
	strcpy( c_soap->itfname, p->IfName );
	c_soap->dscp = p->DSCP;
	c_soap->get_state_for_tr143 = 1;
	memset( &start_state, 0, sizeof(start_state) );
	memset( &end_state, 0, sizeof(end_state) );
	if( TR143HttpDiagClientSend( c_soap, p, &start_state )==0 ) //success
	{
		c_soap->get_state_for_tr143 = 1;
		TR143HttpDiagClientRecv( c_soap, p, &end_state );
		if(end_state.bytes_sent>start_state.bytes_sent) 
			p->TotalBytesSent=end_state.bytes_sent-start_state.bytes_sent;
		if(end_state.bytes_recv>start_state.bytes_recv) 
			p->TotalBytesReceived=end_state.bytes_recv-start_state.bytes_recv;
		
		/*clear the connection*/
		c_soap->keep_alive = 0;
		soap_closesock( c_soap );
	}else
	{
		p->DiagnosticsState=eTR143_Error_InitConnectionFailed;	
	}
	TR143HttpDiagClientEnd( c_soap );	
	
	fprintf( stderr, "<%s:%d> done.\n",__FUNCTION__,__LINE__ );

	if(c_soap != NULL)
		free(c_soap);

	return ret;
}

void *TR143HttpDiagThread(void *data)
{
	struct TR143_Diagnostics *p=data;
	if(!data) return NULL;

	TR143HttpDiagClient(data);
	cwmpDiagnosticDone();
	p->http_pid=0;
	return NULL;
}

int TR143StartHttpDiag(struct TR143_Diagnostics *p)
{
	pthread_t pid;
	
	if(!p) return -1;

	if( (p->pURL==NULL)||(strncmp( p->pURL, "http://", 7 )!=0) )
	{
		p->DiagnosticsState=eTR143_Error_InitConnectionFailed;
		cwmpDiagnosticDone();
		return 0;
	}

	if( pthread_create( &pid, NULL, TR143HttpDiagThread, p ) != 0 )
	{
		p->DiagnosticsState=eTR143_Error_InitConnectionFailed;
		cwmpDiagnosticDone();
		return 0;		
	}
	p->http_pid=pid;
	pthread_detach(pid);
	return 0;
}

int TR143StopHttpDiag(struct TR143_Diagnostics *p)
{
	if(!p) return -1;
	if(p->http_pid)
	{ 
		if( pthread_kill( p->http_pid, SIGUSR2 )!=0 )
			perror( "pthread_kill" );
	}
	p->http_pid=0;
	return 0;
}
/***********************************************************************/
/*************** End TR143 HTTP Download/Upload Diagnostics ************/
/***********************************************************************/

int getPerformanceDiagnostic(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
#ifdef CONFIG_USER_FTP_FTP_FTP
	char	*protocol_name="HTTP,FTP";
#else
	char	*protocol_name="HTTP";
#endif
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "DownloadTransports" )==0 )
	{
		 *data=strdup( protocol_name );
	}else if( strcmp( lastname, "UploadTransports" )==0 )
	{
		*data = strdup( protocol_name );
	}else{
		return ERR_9005;
	}
	
	return 0;
}

/*****************************************************************************************************/
int getDownloadDiagnostics(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "DiagnosticsState" )==0 )
	{
		 *data=strdup( strTR143State[gDownloadDiagnostics.DiagnosticsState] );
	}else if( strcmp( lastname, "Interface" )==0 )
	{
		if( gDownloadDiagnostics.pInterface )
			*data = strdup( gDownloadDiagnostics.pInterface );
		else
			*data = strdup( "" );
	}else if( strcmp( lastname, "DownloadURL" )==0 )
	{
		if( gDownloadDiagnostics.pURL )
			*data = strdup( gDownloadDiagnostics.pURL );
		else
			*data = strdup( "" );
	}else if( strcmp( lastname, "DownloadTransports" )==0 )
    {
        *data = strdup("HTTP");
    }else if( strcmp( lastname, "DSCP" )==0 )
	{
		*data=uintdup( gDownloadDiagnostics.DSCP );
	}else if( strcmp( lastname, "EthernetPriority" )==0 )
	{
		*data=uintdup( gDownloadDiagnostics.EthernetPriority );
	}else if( strcmp( lastname, "ROMTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gDownloadDiagnostics.ROMTime );
	}else if( strcmp( lastname, "BOMTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gDownloadDiagnostics.BOMTime );
	}else if( strcmp( lastname, "EOMTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gDownloadDiagnostics.EOMTime );
	}else if( strcmp( lastname, "TestBytesReceived" )==0 )
	{
		*data=uintdup( gDownloadDiagnostics.TestBytesReceived );
	}else if( strcmp( lastname, "TotalBytesReceived" )==0 )
	{
		*data=uintdup( gDownloadDiagnostics.TotalBytesReceived );
	}else if( strcmp( lastname, "TCPOpenRequestTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gDownloadDiagnostics.TCPOpenRequestTime );
	}else if( strcmp( lastname, "TCPOpenResponseTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gDownloadDiagnostics.TCPOpenResponseTime );
	}else{
		return ERR_9005;
	}
		
	return 0;
}

#define StopDownloadAndResetState() \
	do{ \
		StopTR143DownloadDiag(); \
		if(gStartTR143DownloadDiag==0) gDownloadDiagnostics.DiagnosticsState=eTR143_None; \
	}while(0)
int setDownloadDiagnostics(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned int *vUInt=data;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	if( strcmp( lastname, "DiagnosticsState" )==0 )
	{
		if( buf && strcmp( buf, "Requested" )==0 )
		{
			StopTR143DownloadDiag();
			gDownloadDiagnostics.DiagnosticsState=eTR143_Requested;
			gStartTR143DownloadDiag=1;
			return 0;
		}
		return ERR_9007;
	}else if( strcmp( lastname, "Interface" )==0 )
	{
		char ifname[32];
		if( (buf==NULL) || strlen(buf)==0 )
		{
			if( gDownloadDiagnostics.pInterface )
			{
				free( gDownloadDiagnostics.pInterface );
				gDownloadDiagnostics.pInterface = NULL;
			}
			strcpy( gDownloadDiagnostics.IfName, "" );
			fprintf( stderr, "( set Interface:%s,%s)\n", "", buf?buf:"" );
			StopDownloadAndResetState();
			return 0;
		}else
		{
			if( transfer2IfName( buf, ifname )==0 )
			{
				LANDEVNAME2BR0(ifname);
				if( gDownloadDiagnostics.pInterface )
				{
					free( gDownloadDiagnostics.pInterface );
					gDownloadDiagnostics.pInterface = NULL;
				}
				gDownloadDiagnostics.pInterface = strdup( buf );
				strcpy( gDownloadDiagnostics.IfName, ifname );
				fprintf( stderr, "( set Interface:%s,%s)\n", ifname, buf );
				StopDownloadAndResetState();
				return 0;
			}
		}
		return ERR_9007;
	}else if( strcmp( lastname, "DownloadURL" )==0 )
	{
		if( gDownloadDiagnostics.pURL )
		{
			free( gDownloadDiagnostics.pURL );
			gDownloadDiagnostics.pURL = NULL;
		}
		if(buf) gDownloadDiagnostics.pURL = strdup( buf );
		StopDownloadAndResetState();
		return 0;
	}else if( strcmp( lastname, "DSCP" )==0 )
	{
		if( vUInt==NULL ) return ERR_9007;
		if( *vUInt>63 ) return ERR_9007;
		gDownloadDiagnostics.DSCP = *vUInt;
		StopDownloadAndResetState();
		return 0;
	}else if( strcmp( lastname, "EthernetPriority" )==0 )
	{
		if( vUInt==NULL ) return ERR_9007;
		if( *vUInt>7 ) return ERR_9007;
		gDownloadDiagnostics.EthernetPriority = *vUInt;
		StopDownloadAndResetState();
		return 0;
	}else{
		return ERR_9005;
	}
	
	return 0;
}

/*****************************************************************************************************/
int getUploadDiagnostics(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "DiagnosticsState" )==0 )
	{
		 *data=strdup( strTR143State[gUploadDiagnostics.DiagnosticsState] );
	}else if( strcmp( lastname, "Interface" )==0 )
	{
		if( gUploadDiagnostics.pInterface )
			*data = strdup( gUploadDiagnostics.pInterface );
		else
			*data = strdup( "" );
	}else if( strcmp( lastname, "UploadURL" )==0 )
	{
		if( gUploadDiagnostics.pURL )
			*data = strdup( gUploadDiagnostics.pURL );
		else
			*data = strdup( "" );
	}else if( strcmp( lastname, "UploadTransports" )==0 )
    {
            *data = strdup( "HTTP" );
    }else if( strcmp( lastname, "DSCP" )==0 )
	{
		*data=uintdup( gUploadDiagnostics.DSCP );
	}else if( strcmp( lastname, "EthernetPriority" )==0 )
	{
		*data=uintdup( gUploadDiagnostics.EthernetPriority );
	}else if( strcmp( lastname, "TestFileLength" )==0 )
	{
		*data=uintdup( gUploadDiagnostics.TestFileLength );
	}else if( strcmp( lastname, "ROMTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gUploadDiagnostics.ROMTime );
	}else if( strcmp( lastname, "BOMTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gUploadDiagnostics.BOMTime );
	}else if( strcmp( lastname, "EOMTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gUploadDiagnostics.EOMTime );
	}else if( strcmp( lastname, "TotalBytesSent" )==0 )
	{
		*data=uintdup( gUploadDiagnostics.TotalBytesSent );
	}else if( strcmp( lastname, "TCPOpenRequestTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gUploadDiagnostics.TCPOpenRequestTime );
	}else if( strcmp( lastname, "TCPOpenResponseTime" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		*data=timevaldup( gUploadDiagnostics.TCPOpenResponseTime );
	}else{
		return ERR_9005;
	}	

	return 0;
}

#define StopUploadAndResetState() \
	do{ \
		StopTR143UploadDiag(); \
		if(gStartTR143UploadDiag==0) gUploadDiagnostics.DiagnosticsState=eTR143_None; \
	}while(0)
int setUploadDiagnostics(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	char	*buf=data;
	unsigned int *vUInt=data;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	if( strcmp( lastname, "DiagnosticsState" )==0 )
	{
		if( buf && strcmp( buf, "Requested" )==0 )
		{
			StopTR143UploadDiag();
			gUploadDiagnostics.DiagnosticsState=eTR143_Requested;
			gStartTR143UploadDiag=1;
			return 0;
		}
		return ERR_9007;
	}else if( strcmp( lastname, "Interface" )==0 )
	{
		char ifname[32];
		if( (buf==NULL) || strlen(buf)==0 )
		{
			if( gUploadDiagnostics.pInterface )
			{
				free( gUploadDiagnostics.pInterface );
				gUploadDiagnostics.pInterface = NULL;
			}
			strcpy( gUploadDiagnostics.IfName, "" );
			fprintf( stderr, "( set Interface:%s,%s)\n", "", buf?buf:"" );
			StopUploadAndResetState();
			return 0;
		}else
		{
			if( transfer2IfName( buf, ifname )==0 )
			{
				LANDEVNAME2BR0(ifname);
				if( gUploadDiagnostics.pInterface )
				{
					free( gUploadDiagnostics.pInterface );
					gUploadDiagnostics.pInterface = NULL;
				}
				gUploadDiagnostics.pInterface = strdup( buf );
				strcpy( gUploadDiagnostics.IfName, ifname );
				fprintf( stderr, "( set Interface:%s,%s)\n", ifname, buf );
				StopUploadAndResetState();
				return 0;
			}
		}
		return ERR_9007;
	}else if( strcmp( lastname, "UploadURL" )==0 )
	{
		if( gUploadDiagnostics.pURL )
		{
			free( gUploadDiagnostics.pURL );
			gUploadDiagnostics.pURL = NULL;
		}
		if(buf) gUploadDiagnostics.pURL = strdup( buf );
		StopUploadAndResetState();
		return 0;
	}else if( strcmp( lastname, "DSCP" )==0 )
	{
		if( vUInt==NULL ) return ERR_9007;
		if( *vUInt>63 ) return ERR_9007;
		gUploadDiagnostics.DSCP = *vUInt;
		StopUploadAndResetState();
		return 0;
	}else if( strcmp( lastname, "EthernetPriority" )==0 )
	{
		if( vUInt==NULL ) return ERR_9007;
		if( *vUInt>7 ) return ERR_9007;
		gUploadDiagnostics.EthernetPriority = *vUInt;
		StopUploadAndResetState();
		return 0;
	}else if( strcmp( lastname, "TestFileLength" )==0 )
	{
		if( vUInt==NULL ) return ERR_9007;
		gUploadDiagnostics.TestFileLength = *vUInt;
		StopUploadAndResetState();
		return 0;
	}else{
		return ERR_9005;
	}
	
	return 0;
}

/*****************************************************************************************************/
int getUDPEchoConfig(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned int vChar=0;
	unsigned int vShort=0;
	unsigned char buff[256]={0};
	struct ECHORESULT *result;
	struct timeval t={0,0};
	
	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL)) 
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, "Enable" )==0 )
	{
		mib_get( MIB_TR143_UDPECHO_ENABLE, (void *)&vChar );
		*data=booldup( vChar!=0 );
	}else if( strcmp( lastname, "Interface" )==0 )
	{
		unsigned char intfName[8]= {0};
		mib_get( MIB_TR143_UDPECHO_ITFTYPE, (void *)intfName );
		transfer2PathNamefromItf(intfName, buff);
		*data = strdup( buff );
	}else if( strcmp( lastname, "SourceIPAddress" )==0 )
	{
		mib_get( MIB_TR143_UDPECHO_SRCIP, (void *)buff );
		if( buff[0]==0 && buff[1]==0 && buff[2]==0 && buff[3]==0 )
		{
			*data = strdup( "" );
		}else{
			struct in_addr *pSIP = (struct in_addr *)buff;
			*data = strdup( inet_ntoa(*pSIP) );
		}
	}else if( strcmp( lastname, "UDPPort" )==0 )
	{
		mib_get( MIB_TR143_UDPECHO_PORT, (void *)&vShort );
		*data=uintdup( vShort );
	}else if( strcmp( lastname, "EchoPlusEnabled" )==0 )
	{
		mib_get( MIB_TR143_UDPECHO_PLUS, (void *)&vChar );
		*data=booldup( vChar!=0 );
	}else if( strcmp( lastname, "EchoPlusSupported" )==0 )
	{
		*data=booldup( 1 );
	}else if( strcmp( lastname, "PacketsReceived" )==0 )
	{
		if( getShmem( (void**)&result, sizeof(struct ECHORESULT), ECHOTOK )<0 )
			*data=uintdup( 0 );
		else{
			*data=uintdup( result->PacketsReceived );
			detachShmem( result );
		}
	}else if( strcmp( lastname, "PacketsResponded" )==0 )
	{
		if( getShmem( (void**)&result, sizeof(struct ECHORESULT), ECHOTOK )<0 )
			*data=uintdup( 0 );
		else{
			*data=uintdup( result->PacketsResponded );
			detachShmem( result );
		}
	}else if( strcmp( lastname, "BytesReceived" )==0 )
	{
		if( getShmem( (void**)&result, sizeof(struct ECHORESULT), ECHOTOK )<0 )
			*data=uintdup( 0 );
		else{
			*data=uintdup( result->BytesReceived );
			detachShmem( result );
		}
	}else if( strcmp( lastname, "BytesResponded" )==0 )
	{
		if( getShmem( (void**)&result, sizeof(struct ECHORESULT), ECHOTOK )<0 )
			*data=uintdup( 0 );
		else{
			*data=uintdup( result->BytesResponded );
			detachShmem( result );
		}
	}else if( strcmp( lastname, "TimeFirstPacketReceived" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		if( getShmem( (void**)&result, sizeof(struct ECHORESULT), ECHOTOK )<0 )
			*data=timevaldup( t );
		else{
			*data=timevaldup( result->TimeFirstPacketReceived );
			detachShmem( result );
		}
	}else if( strcmp( lastname, "TimeLastPacketReceived" )==0 )
	{
		*type=eCWMP_tMICROSECTIME;
		if( getShmem( (void**)&result, sizeof(struct ECHORESULT), ECHOTOK )<0 )
			*data=timevaldup( t );
		else{
			*data=timevaldup( result->TimeLastPacketReceived );
			detachShmem( result );
		}
	}else{
		return ERR_9005;
	}

	return 0;
}

int setUDPEchoConfig(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char		*lastname = entity->info->name;
	char		*buf=data;
	int		*vInt=data;
	unsigned int	*vUInt=data;
	unsigned int	vChar;
	struct TR143_UDPEchoConfig olddata;
	
	if( (name==NULL) || (data==NULL) || (entity==NULL)) return -1;
	if( entity->info->type!=type ) return ERR_9006;

	UDPEchoConfigSave( &olddata );
	if( strcmp( lastname, "Enable" )==0 )
	{
		if(vInt)
		{
			vChar=(*vInt)?1:0;
			mib_set( MIB_TR143_UDPECHO_ENABLE, (void *)&vChar );
		}
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_UDPEchoConfig, CWMP_RESTART, 0, &olddata, sizeof(struct TR143_UDPEchoConfig ) );
		return 0;
#else
		//return 1;	
		apply_UDPEchoConfig( CWMP_RESTART, 0, &olddata );
		return 0;	
#endif
	}else if( strcmp( lastname, "Interface" )==0 )
	{
		char ifname[32] = {0};
		if(buf)
		{
			if( transfer2IfName( buf, ifname )==0 )
			{
				mib_set( MIB_TR143_UDPECHO_ITFTYPE, (void *)ifname );
			}else
				return ERR_9007;
		}
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_UDPEchoConfig, CWMP_RESTART, 0, &olddata, sizeof(struct TR143_UDPEchoConfig ) );
		return 0;
#else
		//return 1;		
		apply_UDPEchoConfig( CWMP_RESTART, 0, &olddata );
		return 0;
#endif
	}else if( strcmp( lastname, "SourceIPAddress" )==0 )
	{
		struct in_addr vInAddr;
		if(!buf) return ERR_9007;
		if(strlen(buf)==0)
			memset( &vInAddr, 0, sizeof(vInAddr) );
		else if(inet_aton(buf, &vInAddr)==0) //error
			return ERR_9007;
		mib_set( MIB_TR143_UDPECHO_SRCIP, (void *)&vInAddr );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_UDPEchoConfig, CWMP_RESTART, 0, &olddata, sizeof(struct TR143_UDPEchoConfig ) );
		return 0;
#else
		//return 1;		
		apply_UDPEchoConfig( CWMP_RESTART, 0, &olddata );
		return 0;
#endif		
	}else if( strcmp( lastname, "UDPPort" )==0 )
	{
		unsigned int vShort=0;
		
		if( vUInt==NULL ) return ERR_9007;
		if( *vUInt==0 || *vUInt>65535 ) return ERR_9007;
		vShort = *vUInt;
		mib_set( MIB_TR143_UDPECHO_PORT, (void *)&vShort );
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_UDPEchoConfig, CWMP_RESTART, 0, &olddata, sizeof(struct TR143_UDPEchoConfig ) );
		return 0;
#else
		//return 1;		
		apply_UDPEchoConfig( CWMP_RESTART, 0, &olddata );
		return 0;
#endif
	}else if( strcmp( lastname, "EchoPlusEnabled" )==0 )
	{
		if(vInt)
		{
			vChar = (*vInt)?1:0;
			mib_set( MIB_TR143_UDPECHO_PLUS, (void *)&vChar );
		}
#ifdef _CWMP_APPLY_
		apply_add( CWMP_PRI_N, apply_UDPEchoConfig, CWMP_RESTART, 0, &olddata, sizeof(struct TR143_UDPEchoConfig ) );
		return 0;
#else
		//return 1;		
		apply_UDPEchoConfig( CWMP_RESTART, 0, &olddata );
		return 0;
#endif
	}else{
		return ERR_9005;
	}
	
	return 0;
}

/*****************************************************************************************************/
int clearWanUDPEchoItf( void )
{
#if 0 //No ATM PVC		
	int total,i;
	MIB_CE_ATM_VC_T *pEntry, vc_entity;
	
	total = mib_chain_total(MIB_ATM_VC_TBL);
	for( i=0; i<total; i++ )
	{
		pEntry = &vc_entity;
		if( !mib_chain_get(MIB_ATM_VC_TBL, i, (void*)pEntry ) )
			continue;

		if(pEntry->TR143UDPEchoItf)
		{
			pEntry->TR143UDPEchoItf=0;
			mib_chain_update( MIB_ATM_VC_TBL, (unsigned char*)pEntry, i );
		}
	}
#endif	
	return 0;
}

int setWanUDPEchoItf( unsigned char ifindex )
{
#if 0 //No ATM PVC	
	int total,i;
	MIB_CE_ATM_VC_T *pEntry, vc_entity;
	
	total = mib_chain_total(MIB_ATM_VC_TBL);
	for( i=0; i<total; i++ )
	{
		pEntry = &vc_entity;
		if( !mib_chain_get(MIB_ATM_VC_TBL, i, (void*)pEntry ) )
			continue;
		
		if(pEntry->ifIndex==ifindex)
		{
			pEntry->TR143UDPEchoItf=1;
			mib_chain_update( MIB_ATM_VC_TBL, (unsigned char*)pEntry, i );
			return 0;
		}
	}
#endif		
	return -1;
}

int getWanUDPEchoItf( unsigned char *pifindex)
{
#if 0 //No ATM PVC	
	int total,i;
	MIB_CE_ATM_VC_T *pEntry, vc_entity;
	
	if(pifindex==NULL) return -1;
	*pifindex=0xff;
	
	total = mib_chain_total(MIB_ATM_VC_TBL);
	for( i=0; i<total; i++ )
	{
		pEntry = &vc_entity;
		if( !mib_chain_get(MIB_ATM_VC_TBL, i, (void*)pEntry ) )
			continue;
		if(pEntry->TR143UDPEchoItf)
		{
			*pifindex=pEntry->ifIndex;
			return 0;
		}
	}
#endif	
	return -1;
}

#ifdef CONFIG_USER_FTP_FTP_FTP
#define FTPDIAG_DOWNLOADFILE		"/tmp/ftpdiagdown.txt"
#define FTPDIAG_DOWNLOADFILE_RESULT	"/tmp/ftpdiagdown.txt.result"
#define FTPDIAG_UPLOADFILE		"/tmp/ftpdiagup.txt"
#define FTPDIAG_UPLOADFILE_RESULT	"/tmp/ftpdiagup.txt.result"
static int getFtpDiagResut( char *filename, struct TR143_Diagnostics *p )
{
	FILE *fd;
	int	ret;
	int	DiagnosticsState;
	unsigned int	TestBytesReceived;
	unsigned int	TotalBytesReceived;
	unsigned int	TotalBytesSent;
	struct timeval	ROMTime;
	struct timeval	BOMTime;
	struct timeval	EOMTime;
	struct timeval	TCPOpenRequestTime;
	struct timeval	TCPOpenResponseTime;
			
	if( filename==NULL || p==NULL ) return -1;
	fd=fopen( filename, "r" );
	if(fd==NULL) goto resulterror ;
	
	ret=fscanf( fd, "%d %u %u %u %u.%u %u.%u %u.%u %u.%u %u.%u", 
			&DiagnosticsState, 
			&TestBytesReceived, 
			&TotalBytesReceived, 
			&TotalBytesSent,
			&ROMTime.tv_sec,&ROMTime.tv_usec,
			&BOMTime.tv_sec,&BOMTime.tv_usec,
			&EOMTime.tv_sec,&EOMTime.tv_usec,
			&TCPOpenRequestTime.tv_sec,&TCPOpenRequestTime.tv_usec,
			&TCPOpenResponseTime.tv_sec,&TCPOpenResponseTime.tv_usec );
	fclose(fd);
	if(ret!=14)  goto resulterror ;
	if(DiagnosticsState<=eTR143_None || DiagnosticsState>=eTR143_End) goto resulterror;
	
	p->DiagnosticsState=DiagnosticsState;
	p->TestBytesReceived=TestBytesReceived;
	p->TotalBytesReceived=TotalBytesReceived;
	p->TotalBytesSent=TotalBytesSent;
	p->ROMTime.tv_sec=ROMTime.tv_sec;
	p->ROMTime.tv_usec=ROMTime.tv_usec;
	p->BOMTime.tv_sec=BOMTime.tv_sec;
	p->BOMTime.tv_usec=BOMTime.tv_usec;
	p->EOMTime.tv_sec=EOMTime.tv_sec;
	p->EOMTime.tv_usec=EOMTime.tv_usec;
	p->TCPOpenRequestTime.tv_sec=TCPOpenRequestTime.tv_sec;
	p->TCPOpenRequestTime.tv_usec=TCPOpenRequestTime.tv_usec;
	p->TCPOpenResponseTime.tv_sec=TCPOpenResponseTime.tv_sec;
	p->TCPOpenResponseTime.tv_usec=TCPOpenResponseTime.tv_usec;	
	return 0;

resulterror:
	p->DiagnosticsState=eTR143_Error_InitConnectionFailed;
	return -1;
}

void checkPidforFTPDiag( pid_t  pid )
{
	//if(pid!=-1)
	{
		if(pid==gDownloadDiagnostics.ftp_pid)
		{
			gDownloadDiagnostics.ftp_pid=0;
			getFtpDiagResut( FTPDIAG_DOWNLOADFILE_RESULT, &gDownloadDiagnostics );
			cwmpDiagnosticDone();
		}
		
		if(pid==gUploadDiagnostics.ftp_pid)
		{
			gUploadDiagnostics.ftp_pid=0;
			getFtpDiagResut( FTPDIAG_UPLOADFILE_RESULT, &gUploadDiagnostics );
			cwmpDiagnosticDone();
		}
	}
}

static int ExecFTPbyScript( char *scriptname, struct TR143_Diagnostics *p)
{
	pid_t pid;

	if( scriptname==NULL || p==NULL ) return -1;
	pid = vfork();
	if(pid==0)
	{
		/* the child */
		char *env[2];
		char *argv[16];
		char *filename="/bin/ftp";
		int i=0;
		char strDSCP[32],strFileLen[32];
		
		sprintf( strDSCP, "%u", p->DSCP );
		sprintf( strFileLen, "%u", p->TestFileLength );
		p->ftp_pid = getpid();//
		signal(SIGINT, SIG_IGN);
		argv[i++] = filename;
		argv[i++] = "-inv";
		argv[i++] = "-TR143TestMode";
		argv[i++] = "-f";
		argv[i++] = scriptname;
		argv[i++] = "-DSCP";
		argv[i++] = strDSCP;
		if( p->IfName && strlen(p->IfName)>0 )
		{
			argv[i++] = "-Interface";
			argv[i++] = p->IfName;
		}
		if(p->Way==DLWAY_UP)
		{
			argv[i++] = "-TestFileLength";
			argv[i++] = strFileLen;
		}
		argv[i++] = NULL;
	
		
		env[0] = "PATH=/bin:/usr/bin:/etc:/sbin:/usr/sbin";
		env[1] = NULL;

		execve(filename, argv, env);
		perror( "execve" );
		printf("exec %s failed\n", filename);
		_exit(2);
	}else if(pid < 0)
	{
		perror( "vfork" );
		return -1;
	}

	//p->ftp_pid=pid;
	return pid;
}

static int TR143StartFtpDiag(struct TR143_Diagnostics *p)
{
	char ftpdiagname[32]="";
	FILE *fd;
	int pid;
	char *pUrl=NULL, *pHost, *pPort, *pPath, *pFile, *pTmp;

	{//parser url
		if(p->pURL==NULL) goto ftperror;
		pHost=NULL; pPort=NULL; pPath=NULL; pFile=NULL;
		pUrl=strdup( p->pURL );
		if(pUrl==NULL) goto ftperror;
		if(strncmp(pUrl, "ftp://", 6)!=0) goto ftperror;
		pHost=pUrl+6;
		pPath=strchr( pHost, '/');
		if(pPath==NULL) goto ftperror;
		pPath[0]=0;
		pPath++;
		pFile=strrchr( pPath, '/');
		if(pFile==NULL)
		{
			pFile=pPath;
			pPath=NULL;
		}else{
			pFile[0]=0;
			pFile++;
		}
		
		pTmp=strchr( pHost, '@');
		if(pTmp)
		{
			pTmp[0]=0;
			pTmp++;
			//skip user&pwd???(tr143 uses anonymous to login)
			pHost=pTmp;
		}
		pPort=strchr( pHost, ':');
		if(pPort)
		{
			pPort[0]=0;
			pPort++;
			if( strlen(pPort)==0 ) pPort=NULL;
		}
		fprintf( stderr, "parser url=%s\n", p->pURL );
		fprintf( stderr, "\t pHost=%s\n", pHost?pHost:"" );
		fprintf( stderr, "\t pPort=%s\n", pPort?pPort:"" );
		fprintf( stderr, "\t pPath=%s\n", pPath?pPath:"" );
		fprintf( stderr, "\t pFile=%s\n", pFile?pFile:"" );
		
		if( pHost==NULL || strlen(pHost)==0 ||
			pFile==NULL || strlen(pFile)==0 )
			goto ftperror;
	}

	{//write script
		if( p->Way==DLWAY_UP )
			strcpy( ftpdiagname, FTPDIAG_UPLOADFILE );
		else
			strcpy( ftpdiagname, FTPDIAG_DOWNLOADFILE );
		
		fd=fopen( ftpdiagname, "w" );
		if(fd==NULL)
		{
			perror( "fopen" );
			goto ftperror;
		}
		
		if(pPort)
			fprintf( fd, "open %s %s\n", pHost, pPort );
		else
			fprintf( fd, "open %s\n", pHost);
		fprintf( fd, "user anonymous dummypwd@\n" );
		fprintf( fd, "binary\n" );
		fprintf( fd, "passive\n" );
		if( p->Way==DLWAY_UP )
		{
			if(pPath) fprintf( fd, "cd %s/\n", pPath );
			fprintf( fd, "put /bin/ftp %s\n", pFile );
		}else{
			if(pPath)
			{
				fprintf( fd, "size %s/%s\n", pPath, pFile );
				fprintf( fd, "get %s/%s /dev/null\n", pPath, pFile );
			}else{
				fprintf( fd, "size %s\n", pFile );
				fprintf( fd, "get %s /dev/null\n", pFile );
			}
		}
		fprintf( fd, "quit\n" );
		fclose(fd);
	}
	
	pid=ExecFTPbyScript( ftpdiagname, p );
	if(pid==-1) goto ftperror;
	if(pUrl) free(pUrl);
	return 0;	

ftperror:
	p->DiagnosticsState=eTR143_Error_InitConnectionFailed;
	if(pUrl) free(pUrl);
	cwmpDiagnosticDone();
	return -1;
}

static int TR143StopFtpDiag(struct TR143_Diagnostics *p)
{
	pid_t pid;
	
	if(!p) return -1;
	pid=p->ftp_pid;
	p->ftp_pid=0;
	if(pid) kill(pid, SIGTERM);
	return 0;
}
#endif //CONFIG_USER_FTP_FTP_FTP

static void ResetTR143ResultValues(struct TR143_Diagnostics *p)
{
	if(p)
	{
		p->TestBytesReceived=0;
		p->TotalBytesReceived=0;
		p->TotalBytesSent=0;
		memset( &p->ROMTime, 0, sizeof(p->ROMTime) );
		memset( &p->BOMTime, 0, sizeof(p->BOMTime) );
		memset( &p->EOMTime, 0, sizeof(p->EOMTime) );
		memset( &p->TCPOpenRequestTime, 0, sizeof(p->TCPOpenRequestTime) );
		memset( &p->TCPOpenResponseTime, 0, sizeof(p->TCPOpenResponseTime) );
		p->http_pid=0;
		p->ftp_pid=0;
		
		if( p->pInterface && p->pInterface[0] && (transfer2IfName( p->pInterface, p->IfName )==0) )
		{
			LANDEVNAME2BR0(p->IfName);
		}else
			p->IfName[0]=0;
	}
}

void StopTR143DownloadDiag(void)
{
	TR143StopHttpDiag( &gDownloadDiagnostics );
#ifdef CONFIG_USER_FTP_FTP_FTP
	TR143StopFtpDiag( &gDownloadDiagnostics );
	unlink(FTPDIAG_DOWNLOADFILE);
	unlink(FTPDIAG_DOWNLOADFILE_RESULT);
#endif //CONFIG_USER_FTP_FTP_FTP
}

void StartTR143DownloadDiag(void)
{
	StopTR143DownloadDiag();
	ResetTR143ResultValues(&gDownloadDiagnostics);
#ifdef CONFIG_USER_FTP_FTP_FTP
	if( (gDownloadDiagnostics.pURL!=NULL) 
		&& (strncmp(gDownloadDiagnostics.pURL, "ftp://", 6)==0) )
		TR143StartFtpDiag( &gDownloadDiagnostics );
	else
#endif //CONFIG_USER_FTP_FTP_FTP
		TR143StartHttpDiag( &gDownloadDiagnostics );
}

void StopTR143UploadDiag(void)
{
	TR143StopHttpDiag( &gUploadDiagnostics );
#ifdef CONFIG_USER_FTP_FTP_FTP
	TR143StopFtpDiag( &gUploadDiagnostics );
	unlink(FTPDIAG_UPLOADFILE);
	unlink(FTPDIAG_UPLOADFILE_RESULT);
#endif //CONFIG_USER_FTP_FTP_FTP
}

void StartTR143UploadDiag(void)
{
	StopTR143UploadDiag();
	ResetTR143ResultValues(&gUploadDiagnostics);
#ifdef CONFIG_USER_FTP_FTP_FTP
	if( (gUploadDiagnostics.pURL!=NULL) 
		&& (strncmp(gUploadDiagnostics.pURL, "ftp://", 6)==0) )
		TR143StartFtpDiag( &gUploadDiagnostics );
	else
#endif //CONFIG_USER_FTP_FTP_FTP
		TR143StartHttpDiag( &gUploadDiagnostics );
}

#endif /*_PRMT_TR143_*/
