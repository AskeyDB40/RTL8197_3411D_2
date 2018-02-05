#include "prmt_services.h"

#ifdef CONFIG_APP_TR104
#include "prmt_voiceservice.h"
#endif
#ifdef _PRMT_SERVICES_

#ifdef CONFIG_APP_TR104
struct CWMP_OP tSRV_VoiceService_OP = { NULL, objVoiceService };
#endif

struct CWMP_PRMT tServicesObjectInfo[] =
{
/*(name,			type,		flag,		op)*/
#ifdef CONFIG_APP_TR104
{"VoiceService",		eCWMP_tOBJECT,	CWMP_READ,	&tSRV_VoiceService_OP},
#endif
};
enum eServicesObject
{
#ifdef CONFIG_APP_TR104
	eSX_VoiceService,
#endif
	eSX_CTCOM_DUMMY,
};
struct CWMP_NODE tServicesObject[] =
{
/*info,  				leaf,			next)*/
#ifdef CONFIG_APP_TR104
{&tServicesObjectInfo[eSX_VoiceService],	NULL,		NULL},
#endif
{NULL,					NULL,			NULL}	
};
#endif /*_PRMT_SERVICES_*/
