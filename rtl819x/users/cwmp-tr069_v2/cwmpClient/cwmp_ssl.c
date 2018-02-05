#include <sys/types.h>

#include <sys/stat.h>

#include <unistd.h>

#include "cwmp_ssl.h"

#include "cwmp_core.h"


extern int gTR069_SSLAllowSelfSignedCert;

extern int gTR069_SSLRejectErrCert;

#ifdef CWMP_ENABLE_SSL

/******************************************************************************\

 *

 *	OpenSSL

 *

\******************************************************************************/

#ifdef WITH_OPENSSL

int certificate_verify_cb(int ok, X509_STORE_CTX *store)

{

  int ret = ok;



  if (!ok)

  {

	int err;

	char data[256];

	X509 *cert = X509_STORE_CTX_get_current_cert(store);

    	CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ( "SSL verify error or warning with certificate at depth %d: %s\n", 

    		X509_STORE_CTX_get_error_depth(store), X509_verify_cert_error_string(X509_STORE_CTX_get_error(store))));

	err = X509_STORE_CTX_get_error(store);

	X509_NAME_oneline(X509_get_subject_name(cert), data, sizeof(data));

	CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ( "<%s:%d>Verifying certificate(%s) occurs error(%d)\n", __FUNCTION__, __LINE__, data, err ));



	switch(err)

	{

	case X509_V_ERR_CERT_NOT_YET_VALID:/*certificate is not yet valid*/

	case X509_V_ERR_CERT_HAS_EXPIRED:/*certificate has expired*/

	case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:/*skip self_signed*/

	case X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN:/*skip self_signed*/

		if( (gTR069_SSLAllowSelfSignedCert==0) &&
		    ((err==X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT)||(err==X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN)) )
		{
			CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ( "<%s:%d>gTR069_SSLAllowSelfSignedCert==0, donot ignore this error(%d)\n", __FUNCTION__, __LINE__, err ));
			break;
		}

		ret = 1;

		X509_STORE_CTX_set_error(store,X509_V_OK);

		CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ( "<%s:%d>ignore this error(%d)\n", __FUNCTION__, __LINE__, err ));

		break;

	}

	if(gTR069_SSLRejectErrCert && err!=X509_V_ERR_SELF_SIGNED_CERT_IN_CHAIN && err!=X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT){
		
		ret = 1;

		X509_STORE_CTX_set_error(store,err);

		CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ( "<%s:%d> error(%d)\n", __FUNCTION__, __LINE__, err ));		
	}

  }

  /* Note: return 1 to continue, but unsafe progress will be terminated by SSL */

  return ret;

}

#elif defined(_WITH_MATRIXSSL_)

extern void matrixSslSetAllowSelfSignedCert( unsigned char p);
int certificate_verify_cb(sslCertInfo_t *cert, void *arg)

{

	sslCertInfo_t	*next;

	struct soap 	*soap=arg;

	sslKeys_t	*keys;



	if( soap && soap->require_server_auth )

	{

		if(	(cert==NULL) ||

			(cert->subject.commonName==NULL) ||

			strcmp( cert->subject.commonName, soap->host ) )

		{

			CWMPDBG_FUNC(MODULE_CORE, LEVEL_WARNING, ( "error: commonName does NOT match the server host name\n" ));

			return -1;

		}

		//CWMPDBG_FUNC(MODULE_CORE, LEVEL_ERROR, ( "in certificate_verify_cb: <commonName:%s>, <ServerHostName%s>\n", cert->subject.commonName, cpe_client.cpe_soap.host ));

	}



/*

	Make sure we are checking the last cert in the chain

*/

	next = cert;

	while (next->next != NULL) {

		next = next->next;

	}

/*

	This case passes the true RSA authentication status through

*/

	return next->verified;

}

#endif



int certificate_setup( struct soap *soap, int use_cert )

{

	int ret= -1;

	char *cert_filename=NULL;

	char *ca_filename=NULL;

	char *pcertpw=NULL;

	int ssl_auth_mode=SOAP_SSL_NO_AUTHENTICATION;

	struct cwmp_userdata *data=soap->user;


#ifdef _WITH_MATRIXSSL_
	if(gTR069_SSLAllowSelfSignedCert)
		matrixSslSetAllowSelfSignedCert(1);
	else
		matrixSslSetAllowSelfSignedCert(0);
#endif //_WITH_MATRIXSSL_


	/*re-write the default callback function for verify certificate*/

	soap->fsslverify = certificate_verify_cb;

	soap->require_server_auth = gTR069_SSLRejectErrCert;


	if(use_cert)

		ssl_auth_mode=SOAP_SSL_REQUIRE_SERVER_AUTHENTICATION;

	

	if(data)

	{

		pcertpw = data->cert_passwd;			

		cert_filename = data->cert_path;

		ca_filename = data->ca_cert;		



		if (soap_ssl_client_context( soap,

			ssl_auth_mode,	/* use SOAP_SSL_DEFAULT in production code */

			cert_filename, 	/* keyfile: required only when client must authenticate to server (see SSL docs on how to obtain this file) */

			pcertpw, 	/* password to read the keyfile */

			ca_filename,	/* optional cacert file to store trusted certificates, use cacerts.pem for all public certificates issued by common CAs */

			NULL,		/* optional capath to directory with trusted certificates */

			NULL		/* if randfile!=NULL: use a file with random data to seed randomness */ 

			))

		{

			soap_print_fault(soap, stderr);

		}else{

			return 0;

		}

	}

	

	if (soap_ssl_client_context( soap,

		ssl_auth_mode,	/* use SOAP_SSL_DEFAULT in production code */

		NULL, 		/* keyfile: required only when client must authenticate to server (see SSL docs on how to obtain this file) */

		NULL, 		/* password to read the keyfile */

		NULL,		/* optional cacert file to store trusted certificates, use cacerts.pem for all public certificates issued by common CAs */

		NULL,		/* optional capath to directory with trusted certificates */

		NULL		/* if randfile!=NULL: use a file with random data to seed randomness */ 

		))

	{

		soap_print_fault(soap, stderr);

	}else{

		ret=0;

	}

				

	return ret;

}















#ifdef WITH_OPENSSL



#if defined(WIN32)

# define MUTEX_TYPE		HANDLE

# define MUTEX_SETUP(x)		(x) = CreateMutex(NULL, FALSE, NULL)

# define MUTEX_CLEANUP(x)	CloseHandle(x)

# define MUTEX_LOCK(x)		WaitForSingleObject((x), INFINITE)

# define MUTEX_UNLOCK(x)	ReleaseMutex(x)

# define THREAD_ID		GetCurrentThreadId()

#elif defined(_POSIX_THREADS)

# define MUTEX_TYPE		pthread_mutex_t

# define MUTEX_SETUP(x)		pthread_mutex_init(&(x), NULL)

# define MUTEX_CLEANUP(x)	pthread_mutex_destroy(&(x))

# define MUTEX_LOCK(x)		pthread_mutex_lock(&(x))

# define MUTEX_UNLOCK(x)	pthread_mutex_unlock(&(x))

# define THREAD_ID		pthread_self()

#else

# error "You must define mutex operations appropriate for your platform"

# error	"See OpenSSL /threads/th-lock.c on how to implement mutex on your platform"

#endif



struct CRYPTO_dynlock_value

{ MUTEX_TYPE mutex;

};



static MUTEX_TYPE *mutex_buf;



static struct CRYPTO_dynlock_value *dyn_create_function(const char *file, int line)

{ struct CRYPTO_dynlock_value *value;

  value = (struct CRYPTO_dynlock_value*)malloc(sizeof(struct CRYPTO_dynlock_value));

  if (value)

    MUTEX_SETUP(value->mutex);

  return value;

}



static void dyn_lock_function(int mode, struct CRYPTO_dynlock_value *l, const char *file, int line)

{ if (mode & CRYPTO_LOCK)

    MUTEX_LOCK(l->mutex);

  else

    MUTEX_UNLOCK(l->mutex);

}



static void dyn_destroy_function(struct CRYPTO_dynlock_value *l, const char *file, int line)

{ MUTEX_CLEANUP(l->mutex);

  free(l);

}



void locking_function(int mode, int n, const char *file, int line)

{ if (mode & CRYPTO_LOCK)

    MUTEX_LOCK(mutex_buf[n]);

  else

    MUTEX_UNLOCK(mutex_buf[n]);

}



unsigned long id_function()

{ return (unsigned long)THREAD_ID;

}



int CRYPTO_thread_setup()

{ int i;

  mutex_buf = (MUTEX_TYPE*)malloc(CRYPTO_num_locks() * sizeof(pthread_mutex_t));

  if (!mutex_buf)

    return SOAP_EOM;

  for (i = 0; i < CRYPTO_num_locks(); i++)

    MUTEX_SETUP(mutex_buf[i]);

  CRYPTO_set_id_callback(id_function);

  CRYPTO_set_locking_callback(locking_function);

  CRYPTO_set_dynlock_create_callback(dyn_create_function);

  CRYPTO_set_dynlock_lock_callback(dyn_lock_function);

  CRYPTO_set_dynlock_destroy_callback(dyn_destroy_function);

  return SOAP_OK;

}



void CRYPTO_thread_cleanup()

{ int i;

  if (!mutex_buf)

    return;

  CRYPTO_set_id_callback(NULL);

  CRYPTO_set_locking_callback(NULL);

  CRYPTO_set_dynlock_create_callback(NULL);

  CRYPTO_set_dynlock_lock_callback(NULL);

  CRYPTO_set_dynlock_destroy_callback(NULL);

  for (i = 0; i < CRYPTO_num_locks(); i++)

    MUTEX_CLEANUP(mutex_buf[i]);

  free(mutex_buf);

  mutex_buf = NULL;

}

#else
void CRYPTO_thread_cleanup() {return ;}

#endif



/******************************************************************************\

 *

 *	SIGPIPE

 *

\******************************************************************************/



void sigpipe_handle(int x){}



#else
void CRYPTO_thread_cleanup() {return ;}
#endif /*#ifdef CWMP_ENABLE_SSL*/
