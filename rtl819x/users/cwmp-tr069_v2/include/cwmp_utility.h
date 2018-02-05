#ifndef CWMPLIB_H_
#define CWMPLIB_H_

extern void cwmpinit_SendGetRPC( int flag );
extern void cwmpinit_SSLAuth( int flag );
extern void cwmpinit_SkipMReboot( int flag );
extern void cwmpinit_DelayStart( int flag );
extern void cwmpinit_NoDebugMsg( int flag );
extern void cwmpinit_DisConReqAuth( int flag );
extern void cwmpinit_OnlyDefaultWanIPinInform( int flag );
extern void cwmpinit_BringLanMacAddrInInform( int flag );
extern void cwmpinit_SslSetAllowSelfSignedCert( int flag );

extern void cwmp_closeDebugMsg(void);

#endif 
