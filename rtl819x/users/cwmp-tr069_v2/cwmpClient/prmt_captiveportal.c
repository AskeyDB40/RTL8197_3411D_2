#include "prmt_captiveportal.h"

#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
struct CWMP_OP tCaptivePortalLeafOP = { getCaptivePortal,	setCaptivePortal };
struct CWMP_PRMT tCaptivePortalLeafInfo[] =
{
/*(name,			type,		flag,			op)*/
{"Enable",			eCWMP_tBOOLEAN,	CWMP_WRITE|CWMP_READ,	&tCaptivePortalLeafOP },
{"Status",			eCWMP_tSTRING,	CWMP_READ,		&tCaptivePortalLeafOP },
{"AllowedList",			eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tCaptivePortalLeafOP },
{"CaptivePortalURL",		eCWMP_tSTRING,	CWMP_WRITE|CWMP_READ,	&tCaptivePortalLeafOP },
};
enum eCaptivePortalLeaf
{
	eCP_Enable,
	eCP_Status,
	eCP_AllowedList,
	eCP_CaptivePortalURL,
};
struct CWMP_LEAF tCaptivePortalLeaf[] =
{
{ &tCaptivePortalLeafInfo[eCP_Enable] },
{ &tCaptivePortalLeafInfo[eCP_Status] },
{ &tCaptivePortalLeafInfo[eCP_AllowedList] },
{ &tCaptivePortalLeafInfo[eCP_CaptivePortalURL] },
{ NULL	}
};
/*************************************************************************************/
int getCaptivePortal(char *name, struct CWMP_LEAF *entity, int *type, void **data)
{
	char	*lastname = entity->info->name;
	unsigned int enable;
	unsigned char buf[256+1]={0};

	if( (name==NULL) || (type==NULL) || (data==NULL) || (entity==NULL))
		return -1;

	*type = entity->info->type;
	*data = NULL;
	if( strcmp( lastname, tCaptivePortalLeafInfo[eCP_Enable].name )==0 )
	{
		mib_get( MIB_CAPTIVEPORTAL_ENABLE, (void *)&enable);
		*data = booldup( enable!=0 );
	}else if( strcmp( lastname, tCaptivePortalLeafInfo[eCP_Status].name )==0 )
	{
		mib_get( MIB_CAPTIVEPORTAL_ENABLE, (void *)&enable);
		if(enable!=0)
		{
			mib_get( MIB_CAPTIVEPORTAL_URL, (void *)buf);
			if( strlen(buf)==0 )
				*data = strdup( "Error_URLEmpty" );
			else
				*data = strdup( "Enabled" );
		}else
			*data = strdup( "Disabled" );
	}
	else if( strcmp( lastname, tCaptivePortalLeafInfo[eCP_AllowedList].name )==0 )
	{
		getCaptivePortalAllowedList( FILE4CaptivePortal );
		*type = eCWMP_tFILE;
		*data = strdup( FILE4CaptivePortal );
	}else if( strcmp( lastname, tCaptivePortalLeafInfo[eCP_CaptivePortalURL].name )==0 )
	{
		mib_get( MIB_CAPTIVEPORTAL_URL, (void *)buf);
		*data = strdup( buf );
	}else{
		return ERR_9005;
	}

	return 0;
}
int setCaptivePortal(char *name, struct CWMP_LEAF *entity, int type, void *data)
{
	char	*lastname = entity->info->name;
	int *pINT=data;
	char *pbuf = data;
	unsigned int enable;
	char cmd[256] = {0};
	if( (name==NULL) || (entity==NULL)) return -1;
	if( data==NULL ) return ERR_9007;
	if( entity->info->type!=type ) return ERR_9006;
	if( strcmp( lastname, tCaptivePortalLeafInfo[eCP_Enable].name )==0 )
	{
		unsigned int last_enable;
		if(pINT==NULL) return ERR_9007;
		//fprintf(stderr, "set %s=%d\n", name, (*pINT==0)?0:1 );
		enable=(*pINT==0)?0:1;
		mib_get(MIB_CAPTIVEPORTAL_ENABLE, (void *)&last_enable);
		mib_set(MIB_CAPTIVEPORTAL_ENABLE, &enable);
		sprintf(cmd,"echo %d > /proc/http_redirect/enable",enable);
		system(cmd);
#ifdef _CWMP_APPLY_
		if(!last_enable && enable)
			apply_add(CWMP_PRI_L, Apply_CaptivePortal, CWMP_START, 0, NULL, 0);
		else if(last_enable && !enable)
			apply_add(CWMP_PRI_L, Apply_CaptivePortal, CWMP_STOP, 0, NULL, 0);

		return 0;
#else
		return 0;
#endif
	}else if( strcmp( lastname, tCaptivePortalLeafInfo[eCP_AllowedList].name )==0 )
	{
		//fprintf(stderr, "set %s=%s\n", name, pbuf?pbuf:""  );
		if( setCaptivePortalAllowedList( pbuf )<0 ) return ERR_9007;
#ifdef _CWMP_APPLY_
		mib_get(MIB_CAPTIVEPORTAL_ENABLE, (void *)&enable);
		if(enable)
			apply_add(CWMP_PRI_L, Apply_CaptivePortal, CWMP_RESTART, 0, NULL, 0);

		return 0;
#else
		return 0;
#endif
	}else if( strcmp( lastname, tCaptivePortalLeafInfo[eCP_CaptivePortalURL].name )==0 )
	{
		//fprintf(stderr, "set %s=%s\n", name, pbuf?pbuf:"" );
		if(pbuf==NULL || strlen(pbuf)==0 )
			mib_set( MIB_CAPTIVEPORTAL_URL, (void *)"");
		else{
			if( (strlen(pbuf)>=MAX_URL_LEN) || strncmp( pbuf, "http://", 7 ) ) return ERR_9007;
			mib_set( MIB_CAPTIVEPORTAL_URL, (void *)pbuf);
			sprintf(cmd,"echo %s > /proc/http_redirect/CaptivePortalURL",pbuf);
			system(cmd);	
		}
		return 0;
	}else{
		return ERR_9005;
	}
	return 0;

}

/***********************************************************************/
/*API*/
/***********************************************************************/
int getCaptivePortalAllowedList( char *pfilename )
{
	FILE *fp;
	int num,i;

	if(!pfilename) return -1;
	fp=fopen( pfilename, "w" );
	if(!fp) return -1;
	apmib_get(MIB_CAP_PORTAL_ALLOW_TBL_NUM,&num);
	for( i=1;i<=num;i++ )
	{
		CAP_PORTAL_T cpal_entry;
		*((char *)&cpal_entry) = (char)i;
		if ( !apmib_get(MIB_CAP_PORTAL_ALLOW_TBL, (void *)&cpal_entry))
			break;
		if(i>1) fprintf( fp, "," );
		fprintf( fp, "%s",  cpal_entry.ipAddr);
	}
	fclose(fp);
	return 0;
}
#define EQ_DEBUG
int setCaptivePortalAllowedList( char *list )
{
	FILE *fp;
	int num,i, count=0;
	char orig[32] = {0};
	char buf[32];
	char cmd[256] = {0};
	struct in_addr	inaddr;
	fp=fopen( FILE4CaptivePortal, "w" );
	if(!fp) return -1;
	if( list && strlen(list) )
	{
		char *tok;
		tok=strtok( list, ", " );
		while(tok)
		{
			char *p, *pmask=NULL;
			int paser_error=0;
			strcpy(orig,tok);
			p=strchr( tok, '/' );
			if(p)
			{
				*p=0;
				p++;
				pmask=p;
				if(strlen(p)>0)
				{
					while(*p!=0)
					{
						if(isdigit(*p)==0)
						{
							paser_error=1;
							break;
						}
						p++;
					}
					if( paser_error==0 && (atoi(pmask)<0 || atoi(pmask)>32) )
						paser_error=1;
				}else
					paser_error=1;
			}

			if(paser_error==0 && (inet_aton(tok, &inaddr)==0) )
				paser_error=1;

			if(paser_error)
			{
				fclose(fp);
				unlink(FILE4CaptivePortal);
				return -1;
			}
			fprintf(fp,"%s\n",orig);
			//next
			count++;
			tok=strtok( NULL, ", " );
		}
	}
	fclose(fp);
	if(count>MAX_CAP_PORTAL_ALLOW_NUM)
	{
		unlink(FILE4CaptivePortal);
		return -1;
	}
	//destroy allowlist
	apmib_get( MIB_CAP_PORTAL_ALLOW_TBL_NUM,&num);
	for( i=num;i>=1;i-- ) 
	{
		CAP_PORTAL_T cpal_entry;
		*((char *)&cpal_entry) = (char)i;
		if ( !apmib_get(MIB_CAP_PORTAL_ALLOW_TBL, (void *)&cpal_entry))
			break;
		apmib_set(MIB_CAP_PORTAL_ALLOW_TBL_DEL, (void*)(&cpal_entry));
	}
	system("echo flush > /proc/http_redirect/AllowedList");
	//save the new allowlist
	fp=fopen( FILE4CaptivePortal, "r" );
	if(!fp) return -1;
	while(fgets(buf, 32, fp))
	{
		char ipbuf[32];
		int ipmask;
		CAP_PORTAL_T cpal_entry;
		if(buf[strlen(buf)-1]=='\n')
		{
			buf[strlen(buf)-1] = '\0';//del '\n' at end
		}
		strcpy(cpal_entry.ipAddr,buf);
		apmib_set(MIB_CAP_PORTAL_ALLOW_TBL_ADD,(void*)(&cpal_entry));
		sprintf(cmd,"echo add %s > /proc/http_redirect/AllowedList",buf);
		system(cmd);
	}
	fclose(fp);

	unlink(FILE4CaptivePortal);
	return 0;
}

#endif //_SUPPORT_CAPTIVEPORTAL_PROFILE_

