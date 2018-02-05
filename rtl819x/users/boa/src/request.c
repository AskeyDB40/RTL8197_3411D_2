/*
 *  Boa, an http server
 *  Copyright (C) 1995 Paul Phillips <paulp@go2net.com>
 *  Copyright (C) 1996-2005 Larry Doolittle <ldoolitt@boa.org>
 *  Copyright (C) 1996-2004 Jon Nelson <jnelson@boa.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 1, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/* $Id: request.c,v 1.112.2.51 2005/02/22 14:11:29 jnelson Exp $*/

#include "boa.h"
#ifdef BOA_WITH_OPENSSL
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#include <stddef.h>             /* for offsetof */
#ifdef SUPPORT_ASP
#include "asp_page.h"
#endif

#ifdef SUPER_NAME_SUPPORT
#include "auth.h"
#endif

#ifdef DIGEST_AUTHENTICATION
#include "md5.h"
#define TEMP_DIGEST_LENGTH 16
#define FINAL_DIGEST_LENGTH 32
#endif

#include "apform.h"
#define TUNE_SNDBUF
/*
#define USE_TCPNODELAY
#define NO_RATE_LIMIT
#define DIE_ON_ERROR_TUNING_SNDBUF
*/

unsigned total_connections = 0;
unsigned int system_bufsize = 0; /* Default size of SNDBUF given by system */
struct status status;

#ifdef BOA_WITH_OPENSSL
extern int server_ssl;                  /*the socket to listen for ssl requests*/
extern SSL_CTX *ctx;                    /*The global connection context*/
extern int do_ssl;                              /*do ssl sockets??*/
#endif /*BOA_WITH_OPENSSL*/

static unsigned int sockbufsize = SOCKETBUF_SIZE;

/* function prototypes located in this file only */
static void free_request(request * req);
static void sanitize_request(request * req, int make_new_request);
// davidhsu --------------------------------------------------------------
char last_url[100];
//int check_auth_flag=0;
#ifdef WEB_ONLY_ONE_CLIENT
web_client_t webClientStatus[CLINET_MAX_NUM]={0};
int current_mac_index=-1;


int auth_get_clone_mac_by_ip(char *ip, char *clone_mac)
{
	FILE *fp;
	char line_buffer[128], tmp_mac_str[18], ip_str[16], if_name[16];		
	int i, j, retval=-1;	
	
	if(ip==NULL || clone_mac==NULL)
		return retval; 
	
	if((fp= fopen("/proc/net/arp", "r"))==NULL)
		return retval;
	
	while(fgets(line_buffer, sizeof(line_buffer), fp))
	{			
		line_buffer[strlen(line_buffer)-1]='\0';		

		sscanf(line_buffer,"%s %*s %*s %s %*s %s",ip_str,tmp_mac_str,if_name);
		
		if(strcmp(if_name, "br0")!=0 || strcmp(ip, ip_str)!=0)
			continue;
		
		for(i=0, j=0; i<17 && j<12; i++)
		{
			if(tmp_mac_str[i]!=':')
				clone_mac[j++]=tmp_mac_str[i];
		}
		clone_mac[12]=0;
		retval=0;
		break;
	}
	fclose(fp);
	return retval;		
}

int auth_get_mac_idx(char *ip)
{
	char remove_mac[6]={0};
	int i=0,find=0;
	auth_get_clone_mac_by_ip(ip,remove_mac);
	for(i=0;i<CLINET_MAX_NUM;i++)
	{
		if(memcmp(remove_mac,webClientStatus[i].remove_mac,6)==0)
		{
			return i;
		}
		
	}
//not in the table when run  here
	do{
		i=rand()%CLINET_MAX_NUM;
	}while(webClientStatus[i].status==AUTHED);
	memcpy(webClientStatus[i].remove_mac,remove_mac,6);
	webClientStatus[i].status=NOT_LOGIN;
	return i;
}
void auth_setAuthed(int idx)
{
	int i=0;
	for(i=0;i<CLINET_MAX_NUM;i++)
	{
		if(webClientStatus[i].status==AUTHED)
			webClientStatus[i].status=NOT_LOGIN;
		
		if(i==idx)
		{
			webClientStatus[i].status=AUTHED;
		}			
	}
}

#endif

//--------------------------------------------------------------------
#ifdef HTTP_FILE_SERVER_SUPPORTED
extern void http_file_server_req_init(request *wp);
extern void http_file_server_req_free(request *wp);
extern void CheckUA(request *wp);
#endif

#ifdef USE_AUTH
/*
 * Name: base64decode
 *
 * Description: Decodes BASE-64 encoded string
 */
static int base64decode(void *dst,char *src,int maxlen)
{
	int bitval,bits;
	int val;
	int len,x,y;

	len = strlen(src);
	bitval=0;
	bits=0;
	y=0;

	for(x=0;x<len;x++)
	{
		if ((src[x]>='A')&&(src[x]<='Z')) val=src[x]-'A'; else
			if ((src[x]>='a')&&(src[x]<='z')) val=src[x]-'a'+26; else
				if ((src[x]>='0')&&(src[x]<='9')) val=src[x]-'0'+52; else
					if (src[x]=='+') val=62; else
						if (src[x]=='-') val=63; else
							val=-1;
		if (val>=0)
		{
			bitval=bitval<<6;
			bitval+=val;
			bits+=6;
			while (bits>=8)
			{
				if (y<maxlen)
					((char *)dst)[y++]=(bitval>>(bits-8))&0xFF;
				bits-=8;
				bitval &= (1<<bits)-1;
			}
		}
	}
	if (y<maxlen)
		((char *)dst)[y++]=0;
	return y;
}
#endif // USE_AUTH

/*
 * Name: new_request
 * Description: Obtains a request struct off the free list, or if the
 * free list is empty, allocates memory
 *
 * Return value: pointer to initialized request
 */

request *new_request(void)
{
	//printf("%s\n", __FUNCTION__);
	request *req;

	if (request_free) {
		req = request_free;     /* first on free list */
		dequeue(&request_free, request_free); /* dequeue the head */
	} else {
		//req = (request *) malloc(sizeof (request));
		req = (request *) calloc(sizeof(request),1);//init memory,make debug happy
		if (!req) {
			log_error_time();
			perror("malloc for new request");
			return NULL;
		}
	}

#ifdef BOA_WITH_OPENSSL
	req->ssl = NULL;
#endif

#ifdef SUPPORT_ASP
	req->max_buffer_size = CLIENT_STREAM_SIZE;
	//req->buffer = (char *)malloc(req->max_buffer_size+1);
	req->buffer = (char *)calloc(req->max_buffer_size+1,1);//init memory,make debug happy
	if (!req->buffer)
		DIE("out of memory");	
#endif
	req->multipart_boundary = NULL;
#ifdef HTTP_FILE_SERVER_SUPPORTED
	http_file_server_req_init(req);
#endif

#ifdef BOA_WITH_MBEDTLS
    mbedtls_net_init( &(req->mbedtls_client_fd) );
    mbedtls_ssl_init( &(req->mbedtls_ssl_ctx) );
#endif

	sanitize_request(req, 1);

	return req;
}

/*
 * Name: get_request
 *
 * Description: Polls the server socket for a request.  If one exists,
 * does some basic initialization and adds it to the ready queue;.
 */

#ifdef BOA_WITH_OPENSSL
request* get_request(int server_sock)
#else
void get_request(int server_sock)
#endif
{
	//    printf("%s\n", __FUNCTION__);
	int fd;                     /* socket */
	struct SOCKADDR remote_addr; /* address */
	struct SOCKADDR salocal;
	unsigned int remote_addrlen = sizeof (struct SOCKADDR);
	request *conn=NULL;              /* connection */
	socklen_t len;

#ifdef BOA_WITH_MBEDTLS
    conn = new_request();
    if (!conn) {
        return NULL;
    }

    if(server_sock==((mbedtls_net_context *) &mbedtls_listen_fd)->fd)
    {
        if(get_mbedtls_request(conn)!=0)
		{
			//printf("[%s:%d] get mbedtls request fail!\n", __FUNCTION__, __LINE__);
            enqueue(&request_free, conn);
            return ;
		}
        fd = (mbedtls_net_context *)(&(conn->mbedtls_client_fd))->fd;
        //printf("[%s:%d] accept mbedtls client fd %d\n", __FUNCTION__, __LINE__, fd);
    }else
#endif
    {
#ifndef INET6
	remote_addr.S_FAMILY = (sa_family_t) 0xdead;
#endif
	fd = accept(server_sock, (struct sockaddr *) &remote_addr,
			&remote_addrlen);
    }

	if (fd == -1) {
		if (errno != EAGAIN && errno != EWOULDBLOCK) {
			/* abnormal error */
			WARN("accept");
		} else {
			/* no requests */
		}
		pending_requests = 0;
#ifdef BOA_WITH_MBEDTLS
        enqueue(&request_free, conn);
#endif
		return NULL;
	}
	if (fd >= FD_SETSIZE) {
		log_error("Got fd >= FD_SETSIZE.");
		close(fd);
#ifdef BOA_WITH_MBEDTLS
        enqueue(&request_free, conn);
#endif
		return NULL;
	}
#ifdef DEBUGNONINET
	/* This shows up due to race conditions in some Linux kernels
	   when the client closes the socket sometime between
	   the select() and accept() syscalls.
	   Code and description by Larry Doolittle <ldoolitt@boa.org>
	   */
	if (remote_addr.sin_family != PF_INET) {
		struct sockaddr *bogus = (struct sockaddr *) &remote_addr;
		char *ap, ablock[44];
		int i;
		close(fd);
		log_error_time();
		for (ap = ablock, i = 0; i < remote_addrlen && i < 14; i++) {
			*ap++ = ' ';
			*ap++ = INT_TO_HEX((bogus->sa_data[i] >> 4) & 0x0f);
			*ap++ = INT_TO_HEX(bogus->sa_data[i] & 0x0f);
		}
		*ap = '\0';
		fprintf(stderr, "non-INET connection attempt: socket %d, "
				"sa_family = %hu, sa_data[%d] = %s\n",
				fd, bogus->sa_family, remote_addrlen, ablock);
#ifdef BOA_WITH_MBEDTLS
        enqueue(&request_free, conn);
#endif
		return NULL;
	}
#endif

	/* XXX Either delete this, or document why it's needed */
	/* Pointed out 3-Oct-1999 by Paul Saab <paul@mu.org> */
#ifdef REUSE_EACH_CLIENT_CONNECTION_SOCKET
	if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *) &sock_opt,
					sizeof (sock_opt))) == -1) {
		DIE("setsockopt: unable to set SO_REUSEADDR");
	}
#endif

	len = sizeof (salocal);

	if (getsockname(fd, (struct sockaddr *) &salocal, &len) != 0) {
		WARN("getsockname");
		close(fd);
#ifdef BOA_WITH_MBEDTLS
        enqueue(&request_free, conn);
#endif
		return NULL;
	}

#ifndef BOA_WITH_MBEDTLS
    {
	conn = new_request();
	if (!conn) {
		close(fd);
		return NULL;
	}
    }
#endif
	conn->fd = fd;
	conn->status = READ_HEADER;
	conn->header_line = conn->client_stream;
	conn->time_last = current_time;
	conn->kacount = ka_max;

	if (ascii_sockaddr
			(&salocal, conn->local_ip_addr,
			 sizeof (conn->local_ip_addr)) == NULL) {
		WARN("ascii_sockaddr failed");
		close(fd);
		enqueue(&request_free, conn);
		return conn;
	}

	/* nonblocking socket */
	if (set_nonblock_fd(conn->fd) == -1) {
		WARN("fcntl: unable to set new socket to non-block");
		close(fd);
		enqueue(&request_free, conn);
		return conn;
	}

	/* set close on exec to true */
	if (fcntl(conn->fd, F_SETFD, 1) == -1) {
		WARN("fctnl: unable to set close-on-exec for new socket");
		close(fd);
		enqueue(&request_free, conn);
		return;
	}

#ifdef TUNE_SNDBUF
	/* Increase buffer size if we have to.
	 * Only ask the system the buffer size on the first request,
	 * and assume all subsequent sockets have the same size.
	 */
	if (system_bufsize == 0) {
		len = sizeof (system_bufsize);
		if (getsockopt
				(conn->fd, SOL_SOCKET, SO_SNDBUF, &system_bufsize, &len) == 0
				&& len == sizeof (system_bufsize)) {
			;
		} else {
			WARN("getsockopt(SNDBUF)");
			system_bufsize = 1;
		}
	}
	if (system_bufsize < sockbufsize) {
		if (setsockopt
				(conn->fd, SOL_SOCKET, SO_SNDBUF, (void *) &sockbufsize,
				 sizeof (sockbufsize)) == -1) {
			WARN("setsockopt: unable to set socket buffer size");
#ifdef DIE_ON_ERROR_TUNING_SNDBUF
			exit(errno);
#endif /* DIE_ON_ERROR_TUNING_SNDBUF */
		}
	}
#endif                          /* TUNE_SNDBUF */

	/* for log file and possible use by CGI programs */
	if (ascii_sockaddr
			(&remote_addr, conn->remote_ip_addr,
			 sizeof (conn->remote_ip_addr)) == NULL) {
		WARN("ascii_sockaddr failed");
		close(fd);
		enqueue(&request_free, conn);
		return conn;
	}

	/* for possible use by CGI programs */
	conn->remote_port = net_port(&remote_addr);

	status.requests++;

#ifdef USE_TCPNODELAY
	/* Thanks to Jef Poskanzer <jef@acme.com> for this tweak */
	{
		int one = 1;
		if (setsockopt(conn->fd, IPPROTO_TCP, TCP_NODELAY,
					(void *) &one, sizeof (one)) == -1) {
			DIE("setsockopt: unable to set TCP_NODELAY");
		}

	}
#endif

	total_connections++;
	/* gotta have some breathing room */
	if (total_connections > max_connections) {
		pending_requests = 0;
#ifndef NO_RATE_LIMIT
		/* have to fake an http version */
		conn->http_version = HTTP10;
		conn->method = M_GET;
		send_r_service_unavailable(conn);
		conn->status = DONE;
#endif                          /* NO_RATE_LIMIT */
	}

	enqueue(&request_ready, conn);

	return conn;
}//end get_request

#ifdef BOA_WITH_OPENSSL
void get_ssl_request(void)
{
	//printf("%s\n",__FUNCTION__);
	request *conn;
	conn = get_request(server_ssl);
	if(!conn)
		return;
	conn->ssl = SSL_new(ctx);
	if(conn->ssl == NULL){
		printf("Couldn't create ssl connection stuff\n");
		return ;
	}
	SSL_set_fd(conn->ssl, conn->fd);
	if(SSL_accept(conn->ssl) <= 0){ 
		ERR_print_errors_fp(stderr);
		return;
	}   
	else{printf("SSL_accepted\n");}
}
#endif

#ifdef BOA_WITH_MBEDTLS
int get_mbedtls_request(request *req)
{
    int ret=0, len;
    unsigned char buf[1024];
	
	/*
     * 4b. Setup stuff
     */	
	mbedtls_printf( "  . Setting up the SSL data....stage 2" );
    fflush( stdout );
	
	if( ( ret = mbedtls_ssl_setup( &(req->mbedtls_ssl_ctx), &mbedtls_conf ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_ssl_setup returned %d\n\n", ret );
        return ret;
    }
	
    mbedtls_printf( " ok\n" );
	
reset:
#ifdef MBEDTLS_ERROR_C
    if( ret != 0 )
    {
        char error_buf[100];
        mbedtls_strerror( ret, error_buf, 100 );
        mbedtls_printf("Last error was: %d - %s\n\n", ret, error_buf );
    }
#endif

    mbedtls_net_free( &(req->mbedtls_client_fd) );

    mbedtls_ssl_session_reset( &(req->mbedtls_ssl_ctx) );
    /*
     * 3. Wait until a client connects
     */
    mbedtls_printf( "  . Waiting for a remote connection ..." );
    fflush( stdout );

    if( ( ret = mbedtls_net_accept( &mbedtls_listen_fd, &(req->mbedtls_client_fd),
                    NULL, 0, NULL ) ) != 0 )
    {
        mbedtls_printf( " failed\n  ! mbedtls_net_accept returned %d\n\n", ret );
        mbedtls_net_free( &(req->mbedtls_client_fd) );
        mbedtls_ssl_free( &(req->mbedtls_ssl_ctx) );
        return ret; 
    }

    mbedtls_ssl_set_bio( &(req->mbedtls_ssl_ctx), &(req->mbedtls_client_fd), mbedtls_net_send, mbedtls_net_recv, NULL );

    mbedtls_printf( " ok... client_fd %d\n", ((mbedtls_net_context *) &(req->mbedtls_client_fd))->fd  );

    /*
     * 5. Handshake
     */
    mbedtls_printf( "  . Performing the SSL/TLS handshake..." );
    fflush( stdout );

    while( ( ret = mbedtls_ssl_handshake( &(req->mbedtls_ssl_ctx) ) ) != 0 )
    {
        if( ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE )
        {
            mbedtls_printf( " failed\n  ! mbedtls_ssl_handshake returned %d\n\n", ret );
            goto reset;
        }
    }

    mbedtls_printf( " ok! Now can r/w data...\n" );

    return ret; 
}
#endif

static void sanitize_request(request * req, int new_req)
{
	static unsigned int bytes_to_zero = offsetof(request, fd);

	if (new_req) {
		req->kacount = ka_max;
		req->time_last = current_time;
		req->client_stream_pos = 0;
	} else {
		unsigned int bytes_to_move =
			req->client_stream_pos - req->parse_pos;

		if (bytes_to_move) {
			memmove(req->client_stream,
					req->client_stream + req->parse_pos, bytes_to_move);
		}
		req->client_stream_pos = bytes_to_move;
	}

	/* bzero */
	/* we want to clear a middle part of the request:
	*/

	DEBUG(DEBUG_REQUEST) {
		log_error_time();
		fprintf(stderr, "req: %p, offset: %u\n", (void *) req,
				bytes_to_zero);
	}

	memset(req, 0, bytes_to_zero);

	req->status = READ_HEADER;
	req->header_line = req->client_stream;
}

/*
 * Name: free_request
 *
 * Description: Deallocates memory for a finished request and closes
 * down socket.
 */

static void free_request(request * req)
{
	//printf("%s\n",__FUNCTION__);
	int i;
	/* free_request should *never* get called by anything but
	   process_requests */

	if (req->buffer_end && req->status < TIMED_OUT) {
		/*
		   WARN("request sent to free_request before DONE.");
		   */
		req->status = DONE;

		/* THIS IS THE SAME CODE EXECUTED BY THE 'DONE' SECTION
		 * of process_requests. It must be exactly the same!
		 */
		i = req_flush(req);
		/*
		 * retval can be -2=error, -1=blocked, or bytes left
		 */
		if (i == -2) {          /* error */
			req->status = DEAD;
		} else if (i > 0) {
			return;
		}
	}
	/* put request on the free list */
	dequeue(&request_ready, req); /* dequeue from ready or block list */

	/* set response status to 408 if the client has timed out */
	if (req->status == TIMED_OUT && req->response_status == 0)
		req->response_status = 408;

	if (req->kacount < ka_max &&
			!req->logline &&
			req->client_stream_pos == 0) {
		/* A keepalive request wherein we've read
		 * nothing.
		 * Ignore.
		 */
		;
	} else {
		log_access(req);
	}

	if (req->mmap_entry_var)
		release_mmap(req->mmap_entry_var);
	else if (req->data_mem)
		munmap(req->data_mem, req->filesize);

	if (req->data_fd) {
		close(req->data_fd);
		BOA_FD_CLR(req, req->data_fd, BOA_READ);
	}

	// davidhsu ----------------------
#ifndef NEW_POST
	if (req->post_data_fd) {
		close(req->post_data_fd);
		BOA_FD_CLR(req, req->post_data_fd, BOA_WRITE);
	}
#else
#if defined(BOA_CGI_SUPPORT)
	if(req->cgi_type==CGI){
		if (req->post_data_fd) {
			close(req->post_data_fd);
			BOA_FD_CLR(req, req->post_data_fd, BOA_WRITE);
		}
	}else
#endif
	if (req->post_data) {  	
		free(req->post_data);
		req->post_data = NULL;   	
		req->post_data_len = req->post_data_idx = 0;	
	}
#endif   
	//--------------------------------

	if (req->response_status >= 400)
		status.errors++;

	for (i = common_cgi_env_count; i < req->cgi_env_index; ++i) {
		if (req->cgi_env[i]) {
			free(req->cgi_env[i]);
		} else {
			log_error_time();
			fprintf(stderr, "Warning: CGI Environment contains NULL value"
					"(index %d of %d).\n", i, req->cgi_env_index);
		}
	}

	if (req->pathname)
		free(req->pathname);
	if (req->path_info)
		free(req->path_info);
	if (req->path_translated)
		free(req->path_translated);
	if (req->script_name)
		free(req->script_name);
	if (req->host)
		free(req->host);
	if (req->ranges)
		ranges_reset(req);

#ifdef USE_AUTH
	if (req->userName)
		free(req->userName);			
	if (req->password)
		free(req->password);	
#endif
#ifdef DIGEST_AUTHENTICATION
	if(req->authorization_nonce != NULL)//free nonce
	{
		free(req->authorization_nonce); //free nonce
		req->authorization_nonce = NULL;
	}
	if(req->authorization_uri != NULL)//free uri
	{
		free(req->authorization_uri);
		req->authorization_uri = NULL;
	}
	if(req->authorization_response != NULL)//free response
	{
		free(req->authorization_response);
		req->authorization_response = NULL;
	}
#endif

#ifdef SUPPORT_ASP
	if (req->buffer)
	{
		free(req->buffer);
		req->buffer=NULL;
	}

	// davidhsu -------------	
	if (req->upload_data)
	{
#if defined(CONFIG_APP_FWD)		
		fprintf(stderr, "free upload data..............................\n");
		/*upload_data points to share memory*/
		req->upload_data=NULL;
#else
		fprintf(stderr, "free upload data..............................\n");	
		free(req->upload_data);	
		req->upload_data = NULL;
#endif
	}
	//----------------------	


#endif	

	if (req->multipart_boundary) {
		free(req->multipart_boundary);
	}

#ifdef HTTP_FILE_SERVER_SUPPORTED
	http_file_server_req_free(req);
	http_file_server_req_init(req);
#endif

	if (req->status < TIMED_OUT && (req->keepalive == KA_ACTIVE) &&
			(req->response_status < 500 && req->response_status != 0) && req->kacount > 0) {
		sanitize_request(req, 0);

		--(req->kacount);

		status.requests++;
		enqueue(&request_block, req);
		BOA_FD_SET(req, req->fd, BOA_READ);
		BOA_FD_CLR(req, req->fd, BOA_WRITE);
		return;
	}

#ifdef BOA_WITH_OPENSSL
	SSL_free(req->ssl);
#elif defined(BOA_WITH_MBEDTLS)
    if(req->mbedtls_client_fd.fd!=-1)
    {
        int ret;
        mbedtls_printf( "  . Closing the connection %d...", req->mbedtls_client_fd.fd );

        while( ( ret = mbedtls_ssl_close_notify( &req->mbedtls_ssl_ctx ) ) < 0 )
        {
            if( ret != MBEDTLS_ERR_SSL_WANT_READ &&
                    ret != MBEDTLS_ERR_SSL_WANT_WRITE )
            {
                mbedtls_printf( " failed\n  ! mbedtls_ssl_close_notify returned %d\n\n", ret );
                break;
            }
        }

        mbedtls_printf( " ok\n" );

        mbedtls_net_free( &(req->mbedtls_client_fd) );
        mbedtls_ssl_session_reset( &(req->mbedtls_ssl_ctx) );
        mbedtls_ssl_free( &(req->mbedtls_ssl_ctx) );
    }
#endif
	/*
	   While debugging some weird errors, Jon Nelson learned that
	   some versions of Netscape Navigator break the
	   HTTP specification.

	   Some research on the issue brought up:

http://www.apache.org/docs/misc/known_client_problems.html

As quoted here:

"
Trailing CRLF on POSTs

This is a legacy issue. The CERN webserver required POST
data to have an extra CRLF following it. Thus many
clients send an extra CRLF that is not included in the
Content-Length of the request. Apache works around this
problem by eating any empty lines which appear before a
request.
"

Boa will (for now) hack around this stupid bug in Netscape
(and Internet Exploder)
by reading up to 32k after the connection is all but closed.
This should eliminate any remaining spurious crlf sent
by the client.

Building bugs *into* software to be compatible is
just plain wrong
*/

	if (req->method == M_POST) {
		char buf[32768];
		read(req->fd, buf, sizeof(buf));
	}
	close(req->fd);
	BOA_FD_CLR(req, req->fd, BOA_READ);
	BOA_FD_CLR(req, req->fd, BOA_WRITE);
	total_connections--;

	enqueue(&request_free, req);

	return;
}

/*
 * Name: process_requests
 *
 * Description: Iterates through the ready queue, passing each request
 * to the appropriate handler for processing.  It monitors the
 * return value from handler functions, all of which return -1
 * to indicate a block, 0 on completion and 1 to remain on the
 * ready list for more processing.
 */

extern int isFWUPGRADE;
#ifdef BOA_WITH_SSL
extern int fwInProgress;
#endif
int firmware_len=0;
char *firmware_data;
void process_requests(int server_sock)
{
	int retval = 0;
	request *current, *trailer;

	if (pending_requests) {
		get_request(server_sock);
#ifdef ORIGINAL_BEHAVIOR
		pending_requests = 0;
#endif
	}

	current = request_ready;

	while (current) {
		time(&current_time);
		retval = 1;             /* emulate "success" in case we don't have to flush */

		if (current->buffer_end && /* there is data in the buffer */
				current->status < TIMED_OUT) {
			retval = req_flush(current);
			/*
			 * retval can be -2=error, -1=blocked, or bytes left
			 */
			if (retval == -2) { /* error */
				current->status = DEAD;
				retval = 0;
			} else if (retval >= 0) {
				/* notice the >= which is different from below?
				   Here, we may just be flushing headers.
				   We don't want to return 0 because we are not DONE
				   or DEAD */
				retval = 1;
			}
		}
		//fprintf(stderr, "###current=%p status=%d###\n", current, current->status);
		if (retval == 1) {
			switch (current->status) {
				case READ_HEADER:
				case ONE_CR:
				case ONE_LF:
				case TWO_CR:
					retval = read_header(current);
#if defined(_CWMP_WITH_SSL_)
					/*tr069 cert,not killdaemon*/
					if(strstr(current->request_uri,"/boafrm/formTR069CACert"))
						current->daemon_killed = 1;
#endif
					break;
				case BODY_READ:
					retval = read_body(current);
					break;
				case BODY_WRITE:
					retval = write_body(current);
					break;
				case WRITE:
					retval = process_get(current);
					break;
				case PIPE_READ:
					retval = read_from_pipe(current);
					break;
				case PIPE_WRITE:
					retval = write_from_pipe(current);
					break;
				case IOSHUFFLE:
#ifdef HAVE_SENDFILE
					retval = io_shuffle_sendfile(current);
#else
					retval = io_shuffle(current);
#endif
					break;
				case DONE:
					/* a non-status that will terminate the request */
					retval = req_flush(current);
					/*
					 * retval can be -2=error, -1=blocked, or bytes left
					 */
					if (retval == -2) { /* error */
						current->status = DEAD;
						retval = 0;
					} else if (retval > 0) {
						retval = 1;
					}
					break;
				case TIMED_OUT:
				case DEAD:
					retval = 0;
					current->buffer_end = 0;
					SQUASH_KA(current);
					break;
				default:
					retval = 0;
					fprintf(stderr, "Unknown status (%d), "
							"closing!\n", current->status);
					current->status = DEAD;
					break;
			}

		}

		if (sigterm_flag)
			SQUASH_KA(current);

		/* we put this here instead of after the switch so that
		 * if we are on the last request, and get_request is successful,
		 * current->next is valid!
		 */
		if (pending_requests)
			get_request(server_sock);

		//printf("[%s:%d]retval=%d, isFWUPGRADE=%d\n", __FUNCTION__,__LINE__,retval, isFWUPGRADE);
		switch (retval) {
			case -1:               /* request blocked */
				trailer = current;
				current = current->next;
				block_request(trailer);
				break;
			case 0:                /* request complete */
				current->time_last = current_time;
				//brad add
                //printf("[%s:%d]isFWUPGRADE=%d, current=%x, current->request_uri=%s\n" ,__FUNCTION__, __LINE__, isFWUPGRADE, current, current->request_uri);
#ifdef BOA_WITH_SSL
                if(isFWUPGRADE == 1 && (!strcmp(current->request_uri,"/boafrm/formUpload")) && fwInProgress == 0) 
#else
                if(isFWUPGRADE == 1 && (!strcmp(current->request_uri,"/boafrm/formUpload"))) 
#endif
                {
					firmware_len= current->upload_len;   //assign upload length
					firmware_data = (char *)current->upload_data; //assign upload data pointer
#ifdef BOA_WITH_SSL
                    fwInProgress = 1;
#endif
				}
				trailer = current;
				current = current->next;
				free_request(trailer);
				break;
			case 1:                /* more to do */
				current->time_last = current_time;
				current = current->next;
				break;
			default:
				log_error_doc(current);
				fprintf(stderr, "Unknown retval in process.c - "
						"Status: %d, retval: %d\n", current->status, retval);
				current->status = DEAD;
				current = current->next;
				break;
		}
	}
}

/*
 * Name: process_logline
 *
 * Description: This is called with the first req->header_line received
 * by a request, called "logline" because it is logged to a file.
 * It is parsed to determine request type and method, then passed to
 * translate_uri for further parsing.  Also sets up CGI environment if
 * needed.
 */

int process_logline(request * req)
{
	char *stop, *stop2;

	req->logline = req->client_stream;

	if (strlen(req->logline) < 5) {
		/* minimum length req'd. */
		log_error_doc(req);
		DEBUG(DEBUG_BOA) {
			fprintf(stderr, "Request too short: \"%s\"\n", req->logline);
		}
		send_r_bad_request(req);
		return 0;
	}

	if (!memcmp(req->logline, "GET ", 4))
		req->method = M_GET;
	else if (!memcmp(req->logline, "HEAD ", 5))
		/* head is just get w/no body */
		req->method = M_HEAD;
	else if (!memcmp(req->logline, "POST ", 5))
		req->method = M_POST;
	else {
		log_error_doc(req);
		DEBUG(DEBUG_BOA) {
			fprintf(stderr, "malformed request: \"%s\"\n", req->logline);
		}
		send_r_not_implemented(req);
		return 0;
	}

	req->http_version = HTTP10;

	/* Guaranteed to find ' ' since we matched a method above */
	stop = req->logline + 3;
	if (*stop != ' ')
		++stop;

	/* scan to start of non-whitespace */
	while (*(++stop) == ' ');

	stop2 = stop;

	/* scan to end of non-whitespace */
	while (*stop2 != '\0' && *stop2 != ' ')
		++stop2;

	if (stop2 - stop > MAX_HEADER_LENGTH) {
		log_error_doc(req);
		DEBUG(DEBUG_BOA) {
			fprintf(stderr, "URI too long %d: \"%s\"\n", MAX_HEADER_LENGTH,
					req->logline);
		}
		send_r_bad_request(req);
		return 0;
	}

	/* check for absolute URL */
	if (!memcmp(stop, SERVER_METHOD,
				strlen(SERVER_METHOD)) &&
			!memcmp(stop + strlen(SERVER_METHOD), "://", 3)) {
		char *host;

		/* we have an absolute URL */
		/* advance STOP until first '/' after host */
		stop += strlen(SERVER_METHOD) + 3;
		host = stop;
		/* if *host is '/' there is no host in the URI
		 * if *host is ' ' there is corruption in the URI
		 * if *host is '\0' there is nothing after http://
		 */
		if (*host == '/' || *host == ' ' || *host == '\0') {
			/* nothing *at all* after http:// */
			/* no host in absolute URI */
			log_error_doc(req);
			/* we don't need to log req->logline, because log_error_doc does */
			DEBUG(DEBUG_BOA) {
				fprintf(stderr, "bogus absolute URI\n");
			}
			send_r_bad_request(req);
			return 0;
		}

		/* OK.  The 'host' is at least 1 char long.
		 * advance to '/', or end of host+url (' ' or ''\0')
		 */
		while(*stop != '\0' && *stop != '/' && *stop != ' ')
			++stop;

		if (*stop != '/') { /* *stop is '\0' or ' ' */
			/* host is valid, but there is no URL. */
			log_error_doc(req);
			DEBUG(DEBUG_BOA) {
				fprintf(stderr, "no URL in absolute URI: \"%s\"\n",
						req->logline);
			}
			send_r_bad_request(req);
			return 0;
		}

		/* we have http://X/ where X is not ' ' or '/' (or '\0') */
		/* since stop2 stops on '\0' and ' ', it *must* be after stop */
		/* still, a safety check (belts and suspenders) */
		if (stop2 < stop) {
			/* Corruption in absolute URI */
			/* This prevents a DoS attack from format string attacks */
			log_error_doc(req);
			DEBUG(DEBUG_BOA) {
				fprintf(stderr, "Error: corruption in absolute URI:"
						"\"%s\".  This should not happen.\n", req->logline);
			}
			send_r_bad_request(req);
			return 0;
		}

		/* copy the URI */
		memcpy(req->request_uri, stop, stop2 - stop);
		/* place a NIL in the file spot to terminate host */
		*stop = '\0';
		/* place host */
		/* according to RFC2616 --

		   1. If Request-URI is an absoluteURI, the host is part of the
		   Request-URI. Any Host header field value in the request MUST
		   be ignored.

		   Since we ignore any Host header if req->host is already set,
		   well, we rock!

*/
		req->header_host = host; /* this includes the port! (if any) */
	} else {
		/* copy the URI */
		memcpy(req->request_uri, stop, stop2 - stop);
	}

	req->request_uri[stop2 - stop] = '\0';

	/* METHOD URL\0 */
	if (*stop2 == '\0')
		req->http_version = HTTP09;
	else if (*stop2 == ' ') {
		/* if found, we should get an HTTP/x.x */
		unsigned int p1, p2;

		/* scan to end of whitespace */
		++stop2;
		while (*stop2 == ' ' && *stop2 != '\0')
			++stop2;

		if (*stop2 == '\0') {
			req->http_version = HTTP09;
		} else {
			/* scan in HTTP/major.minor */
			if (sscanf(stop2, "HTTP/%u.%u", &p1, &p2) == 2) {
				/* HTTP/{0.9,1.0,1.1} */
				if (p1 == 0 && p2 == 9) {
					req->http_version = HTTP09;
				} else if (p1 == 1 && p2 == 0) {
					req->http_version = HTTP10;
				} else if (p1 == 1 && p2 == 1) {
					req->http_version = HTTP11;
					//req->keepalive = KA_ACTIVE; /* enable keepalive */
					req->keepalive = KA_STOPPED; /* disable it for some smart phone access DUT web UI fail */
					/* Disable send_r_continue because some clients
					 * *still* don't work with it, Python 2.2 being one
					 * see bug 227361 at the sourceforge web site.
					 * fixed in revision 1.52 of httplib.py, dated
					 * 2002-06-28 (perhaps Python 2.3 will
					 * contain the fix.)
					 *
					 * Also, send_r_continue should *only* be
					 * used if the expect header was sent.
					 */
					/* send_r_continue(req); */
				} else {
					goto BAD_VERSION;
				}
			} else {
				goto BAD_VERSION;
			}
		}
	}

	if (req->method == M_HEAD && req->http_version == HTTP09) {
		log_error("method is HEAD but version is HTTP/0.9");
		send_r_bad_request(req);
		return 0;
	}
	req->cgi_env_index = common_cgi_env_count;

	return 1;

BAD_VERSION:
	log_error_doc(req);
	DEBUG(DEBUG_BOA) {
		fprintf(stderr, "bogus HTTP version: \"%s\"\n", stop2);
	}
	send_r_bad_request(req);
	return 0;
}

#ifdef DIGEST_AUTHENTICATION     //define MD5_encryption method
char * MD5_encryption(char *input_string)
{
	if(input_string == NULL)
	{
		perror("boa: argument of MD5_encryption is NULL.");
		return NULL;
	}

	unsigned char temp_encryption[TEMP_DIGEST_LENGTH];
	MD5_CTX md5;
	
	MD5Init(&md5);
	MD5Update(&md5,input_string,strlen(input_string));
	MD5Final(&md5,temp_encryption);

	char * encryption=malloc(FINAL_DIGEST_LENGTH+1);
	if(encryption == NULL)
	{
		perror("boa: malloc encryption out of memory in function MD5_encryption.");
		return NULL;
	}
	memset(encryption,0,FINAL_DIGEST_LENGTH+1);
	char str[2];

	int i;
	for(i=0;i<16;i++)
	{
		//sprintf(str,"%02x",temp_encryption[i]&0x0ff);
		sprintf(str,"%02x",temp_encryption[i]);
		strcat(encryption,str);
	}
	return encryption;
}

char * generate_digest(request * req,char user_name[],char user_password[])
{
	if(req == NULL)
	{
		perror("boa: req is NULL in function process_header_end.");
		exit(1);
	}
	
	char server_realm[]=":Realtek:";       //===generate HA1===
	int A1_length=strlen(user_name)+strlen(server_realm)+strlen(user_password);
	unsigned char * A1=malloc(A1_length+1);
	if(A1 == NULL)
	{
		perror("boa: malloc A1 out of memory in function process_header_end.");
		exit(1);
	}
	memset(A1,0,A1_length+1);
	strcat(A1,user_name);
	strcat(A1,server_realm);
	strcat(A1,user_password);

	char * HA1 = MD5_encryption(A1);
	free(A1);
	if(HA1 == NULL)
	{
		perror("boa: generate HA1 error in function process_header_end.");
		exit(1);
	}	

	char * request_method=NULL;             //===generate HA2===
	switch(req->method)
	{
		case M_GET:
			request_method="GET";
			break;
		case M_HEAD:
			request_method="HEAD";
			break;
		case M_PUT:
			request_method="PUT";
			break;
		case M_POST:
			request_method="POST";
			break;
		case M_DELETE:
			request_method="DELETE";
			break;
		case M_LINK:
			request_method="LINK";
			break;
		case M_UNLINK:
			request_method="UNLINK";
			break;
		case M_MOVE:
			request_method="MOVE";
			break;
		case M_TRACE:
			request_method="TRACE";
			break;
		default:
			log_error("request_method is NULL.\n");
			send_r_bad_request(req);
			return 0;
	}
	//if(request_method == NULL)
	//{
	//	log_error("request_method is NULL.\n");
	//	send_r_bad_request(req);
	//	return 0;
	//}
	int A2_length;
	if(req->authorization_uri != NULL)
	{
		A2_length=strlen(request_method)+1+strlen(req->authorization_uri);
	}
	else
	{
		A2_length=strlen(request_method)+1;
	}
	
	unsigned char * A2=malloc(A2_length+1);
	if(A2 == NULL)
	{
		perror("boa: malloc A2 out of memory in function process_header_end.");
		exit(1);
	}
	
	memset(A2,0,A2_length+1);
	strcat(A2,request_method);
	strcat(A2,":");
	if(req->authorization_uri != NULL)
	{
		strcat(A2,req->authorization_uri);
	}
	char * HA2=MD5_encryption(A2);
	free(A2);
	if(HA2 == NULL)
	{
		perror("boa: generate HA2 error in function process_header_end.");
		exit(1);
	}	

	int MD5_input_length;                     //===generate digest===
	if(req->authorization_nonce != NULL)
	{
		MD5_input_length=strlen(HA1)+1+strlen(req->authorization_nonce)+1+strlen(HA2);
	}
	else
	{
		MD5_input_length=strlen(HA1)+1+1+strlen(HA2);
	}
	
	unsigned char * MD5_input=malloc(MD5_input_length+1);
	if(MD5_input == NULL)
	{
		perror("boa: malloc MD5_input out of memory in function process_header_end.");
		exit(1);
	}
	
	memset(MD5_input,0,MD5_input_length+1);
	strcat(MD5_input,HA1);
	strcat(MD5_input,":");
	if(req->authorization_nonce != NULL)
	{
		strcat(MD5_input,req->authorization_nonce);
	}
	strcat(MD5_input,":");
	strcat(MD5_input,HA2);
	
	free(HA1);
	free(HA2);
	char * digest=MD5_encryption(MD5_input);
	free(MD5_input);
	if(digest == NULL)
	{
		perror("boa: generate digest error in function process_header_end.");
		exit(1);
	}
	
	return digest;
}
#endif
/*
 * Name: process_header_end
 *
 * Description: takes a request and performs some final checking before
 * init_cgi or init_get
 * Returns 0 for error or NPH, or 1 for success
 */

int process_header_end(request * req)
{
	if (!req->logline) {
		log_error_doc(req);
		fputs("No logline in process_header_end\n", stderr);
		send_r_error(req);
		return 0;
	}

	/* Percent-decode request */
	if (unescape_uri(req->request_uri, &(req->query_string)) == 0) {
		log_error_doc(req);
		DEBUG(DEBUG_BOA) {
			fputs("URI contains bogus characters\n", stderr);
		}
		send_r_bad_request(req);
		return 0;
	}

	/* clean pathname */
	clean_pathname(req->request_uri);

	if (req->request_uri[0] != '/') {
		log_error("URI does not begin with '/'\n");
		send_r_bad_request(req);
		return 0;
	}

	if (vhost_root) {
		char *c;
		if (!req->header_host) {
			req->host = strdup(default_vhost);
		} else {
			req->host = strdup(req->header_host);
		}
		if (!req->host) {
			log_error_doc(req);
			fputs("unable to strdup default_vhost/req->header_host\n", stderr);
			send_r_error(req);
			return 0;
		}
		strlower(req->host);
		/* check for port, and remove
		 * we essentially ignore the port, because we cannot
		 * as yet report a different port than the one we are
		 * listening on
		 */
		c = strchr(req->host, ':');
		if (c)
			*c = '\0';

		if (check_host(req->host) < 1) {
			log_error_doc(req);
			fputs("host invalid!\n", stderr);
			send_r_bad_request(req);
			return 0;
		}
	}

// davidhsu ----------------
#ifdef USE_AUTH
{
#ifdef WEB_ONLY_ONE_CLIENT
	int idx=-1;						
#endif

#ifdef SUPER_NAME_SUPPORT
	char super_name[MAX_NAME_LEN], super_password[MAX_NAME_LEN];
#endif
	char user_name[MAX_NAME_LEN], user_password[MAX_NAME_LEN];
	char host_name[MAX_NAME_LEN]={0};
	int check_auth_flag=0;
	
#ifdef SUPER_NAME_SUPPORT
	apmib_get(MIB_SUPER_NAME, super_name);
	apmib_get(MIB_SUPER_PASSWORD, super_password);
	if( !strcmp(super_name, "") && !strcmp(super_password, ""))//no super user,login without check
	{
		req->auth_flag = NO_LOGIN;
		check_auth_flag = 1;
	}
#endif
	apmib_get(MIB_USER_NAME, user_name);
	apmib_get(MIB_USER_PASSWORD, user_password);
#ifdef WEB_ONLY_ONE_CLIENT
	idx=auth_get_mac_idx(req->remote_ip_addr);			
#endif

#ifdef SUPER_NAME_SUPPORT
	if(check_auth_flag == 0 && req->auth_flag == 0){//super user exits
	#ifdef DIGEST_AUTHENTICATION
		if (req->userName != NULL) 
		{
			char * digest = NULL;
			if(strcmp(req->userName, super_name) == 0)//if  req->userName == super_name
			{					
				digest = generate_digest(req, super_name, super_password);
				if((digest != NULL) && (req->authorization_response != NULL))
				{
					if(strncmp(digest,req->authorization_response,FINAL_DIGEST_LENGTH) == 0)
					{
						req->auth_flag = SUPER_FLAG;
					}
				}
				free(digest);
			}
			else if(strcmp(req->userName, user_name) == 0)//if  req->userName == user_name
			{
				digest = generate_digest(req,user_name,user_password);
				if((digest != NULL) && (req->authorization_response != NULL))
				{
					if(strncmp(digest,req->authorization_response,FINAL_DIGEST_LENGTH) == 0)
					{
						req->auth_flag = SECOND_FLAG;
					}
				}
				free(digest);
			}
		}
	#else
		if (req->userName) {
			if (!strcmp(req->userName, super_name)){					
				if (req->password==NULL || req->password[0]==0){
					if (super_password[0]==0)
						req->auth_flag = SUPER_FLAG;
				}
				else {
					if (!strcmp(req->password, super_password))
						req->auth_flag = SUPER_FLAG;
				}
			}
			else{
				if (!strcmp(req->userName, user_name)){
					if (req->password==NULL || req->password[0]==0) {
						if (user_password[0]==0)
							req->auth_flag = SECOND_FLAG;							
					}
					else  {
						if (!strcmp(req->password, user_password))
							req->auth_flag = SECOND_FLAG;							
					}
				}
						
			}
					
		}//end if (req->userName)
//	}//end if(check_auth_flag == 0 && req->auth_flag == 0)
	#endif
		
#else
	
	if (strcmp(user_name, "") || strcmp(user_password, "") ) {		
		if (req->auth_flag == 0) {
		#ifdef DIGEST_AUTHENTICATION
			char * digest = generate_digest(req,user_name,user_password);
			if((digest != NULL) && (req->authorization_response != NULL))
			{
				if(strncmp(digest,req->authorization_response,FINAL_DIGEST_LENGTH) == 0)
				{
					req->auth_flag = 1;
				}
			}
			free(digest);
		#else
			if (req->userName) {
				if (!strcmp(req->userName, user_name)){
					if (req->password==NULL || req->password[0]==0) {
						if (user_password[0]==0)
							req->auth_flag = 1;					
					}
					else  {
						if (!strcmp(req->password, user_password))
							req->auth_flag = 1;					
					}//end else
				}				//end if (!strcmp(req->userName, user_name))		
			}		//end if (req->userName)
		#endif
		}//end if (req->auth_flag == 0)
#endif
		
#ifdef WEB_ONLY_ONE_CLIENT
		if (req->auth_flag == 1)
		{
			if(webClientStatus[idx].status==NOT_LOGIN)
			{
				req->auth_flag=0;
			}else
			if(webClientStatus[idx].status==TOBE_LOGIN)
			{
				webClientStatus[idx].status=AUTHED;
				auth_setAuthed(idx);
			}
		}
#endif
		if (req->auth_flag == 0 && check_auth_flag == 0) {
#ifdef HOME_GATEWAY
			apmib_get(MIB_HOST_NAME, host_name);
#else
			strcpy(host_name, "");
#endif
#ifdef WEB_ONLY_ONE_CLIENT
			webClientStatus[idx].status=TOBE_LOGIN;
#endif
			send_r_unauthorized(req, host_name);
			return 0;
		}
	}//end strcmp(user_name, "") || strcmp(user_password, "")
	
}
#endif
	//-------------------------

	if (translate_uri(req) == 0) { /* unescape, parse uri */
		/* errors already logged */
		SQUASH_KA(req);
		return 0;               /* failure, close down */
	}

	if (req->method == M_POST) {
		// davidhsu ------------------------
#ifndef NEW_POST
		req->post_data_fd = create_temporary_file(1, NULL, 0);
		if (req->post_data_fd == 0) {
			/* errors already logged */
			send_r_error(req);
			return 0;
		}
		if (fcntl(req->post_data_fd, F_SETFD, 1) == -1) {
			boa_perror(req, "unable to set close-on-exec for req->post_data_fd!");
			close(req->post_data_fd);
			req->post_data_fd = 0;
			return 0;
		}
#else
#if defined(BOA_CGI_SUPPORT)
		if(strstr(req->request_uri, ".cgi")){
			req->post_data_fd = create_temporary_file(1, NULL, 0);
			if (req->post_data_fd == 0) {
				/* errors already logged */
				send_r_error(req);
				return 0;
			}
			if (fcntl(req->post_data_fd, F_SETFD, 1) == -1) {
				boa_perror(req, "unable to set close-on-exec for req->post_data_fd!");
				close(req->post_data_fd);
				req->post_data_fd = 0;
				return 0;
			}
		}else
#endif
		{
		req->post_data =  calloc(1, BUFFER_SIZE);
		if (req->post_data == NULL) {
			send_r_error(req);
			return 0;
		}
		req->post_data_len = req->post_data_idx = 0;
		}
#endif
		//------------------------------------------

		// davidhsu --------------------
#ifdef SUPPORT_ASP
#if defined(CONFIG_APP_TR069) && defined(_CWMP_WITH_SSL_)
#ifdef CONFIG_RTL_BT_CLIENT
		if (strstr(req->logline, FORM_FW_UPLOAD) ||
				strstr(req->logline, FORM_CFG_UPLOAD) ||
				strstr(req->logline, FORMTR069CACERT) ||
				strstr(req->logline, FORMTR069CPECERT) ||
				strstr(req->logline, FORM_BT_NEW_TORRENT)) {
#else
			if (strstr(req->logline, FORM_FW_UPLOAD) || strstr(req->logline, FORM_CFG_UPLOAD) || strstr(req->logline, FORMTR069CACERT) || strstr(req->logline, FORMTR069CPECERT)) {
#endif
#else
#ifdef CONFIG_RTL_BT_CLIENT
				if (strstr(req->logline, FORM_FW_UPLOAD) ||
						strstr(req->logline, FORM_CFG_UPLOAD) ||
						strstr(req->logline, FORM_BT_NEW_TORRENT)) {
#else
					if (strstr(req->logline, FORM_FW_UPLOAD) || strstr(req->logline, FORM_CFG_UPLOAD)) {
#endif
#endif

#if defined(CONFIG_APP_FWD)
						{
							extern int get_shm_id();
							extern int clear_fwupload_shm();
							int shm_id = get_shm_id();
							/* free upload share memory, if it is existed */
							if (shm_id != 0) {
								clear_fwupload_shm(shm_id);
							}
						}
#endif

						req->upload_data = malloc(MAX_UPLOAD_SIZE);
						if (req->upload_data == NULL) {
							boa_perror(req, "allocate upload buffer failed!");
							return 0;
						}
						req->upload_len = 0;
					}
#endif
					//-----------------------------
					return 1;             /* success */
				}

				//fprintf(stderr,"####%s:%d req->request_uri=%s req->cgi_type=%d###\n",  __FILE__, __LINE__ , req->request_uri, req->cgi_type);
				//fprintf(stderr,"####%s:%d req->host=%s req->header_host=%s default_vhost=%s###\n",  __FILE__, __LINE__ , req->host, req->header_host, default_vhost);
#ifdef SUPPORT_ASP
				if (strstr(req->request_uri, ".htm") ||	strstr(req->request_uri, ".asp") ||	strstr(req->request_uri, "navigation.js")) {
					req->cgi_type = ASP;
				}else
#endif
				if(strstr(req->request_uri, ".cgi")){
					req->cgi_type = CGI;
				}

				//fprintf(stderr,"####%s:%d req->request_uri=%s req->cgi_type=%d###\n",  __FILE__, __LINE__ , req->request_uri, req->cgi_type);
				if (req->cgi_type) {
#ifdef SUPPORT_ASP
					if (req->cgi_type == ASP)
						return init_get2(req);
					else
#endif
						return init_cgi(req);

				}

				req->status = WRITE;

				return init_get(req);       /* get and head */
			}

#ifdef DIGEST_AUTHENTICATION
char * pick_up_field_value(char * value, char * field)
{
	if(value == NULL || field == NULL)
	{
		printf("argument is NULL in pick_up_field_value function.\n");
		return NULL;
	}
	char * field_pointer=strstr(value,field);
	if(field_pointer == NULL)
	{
		printf("No %s field in Authorizaition line.\n",field);
		return NULL;
	}
	char * field_start_pointer = field_pointer+strlen(field)+strlen("=\"");
	char * field_end_pointer = strstr(field_start_pointer,"\",");
	int field_length = field_end_pointer - field_start_pointer;
	char * field_value = malloc(field_length+1);
	if(field_value == NULL)
	{
		perror("boa: malloc username out of memory in function process_option_line");
		exit(1);
	}
	memset(field_value,0,field_length+1);
	strncpy(field_value,field_start_pointer,field_length);
	field_value[field_length]='\0';
	return field_value;
}
#endif

			/*
			 * Name: process_option_line
			 *
			 * Description: Parses the contents of req->header_line and takes
			 * appropriate action.
			 */

			int process_option_line(request * req)
			{
				char c, *value, *line = req->header_line;

				/* Start by aggressively hacking the in-place copy of the header line */

#ifdef FASCIST_LOGGING
				log_error_time();
				fprintf(stderr, "%s:%d - Parsing \"%s\"\n", __FILE__, __LINE__, line);
#endif

				value = strchr(line, ':');
				if (value == NULL) {
					log_error_doc(req);
					fprintf(stderr, "header \"%s\" does not contain ':'\n", line);
					return 0;
				}
				*value++ = '\0';            /* overwrite the : */
				to_upper(line);             /* header types are case-insensitive */

				/* the code below *does* catch '\0' due to the c = *value test */
				while ((c = *value) && (c == ' ' || c == '\t'))
					value++;

				/* if c == '\0' there was no 'value' for the key */
				if (c == '\0') {
					/* return now to bypass any parsing or assignment */
					return 1;
				}

				switch (line[0]) {
					case 'A':
						if (!memcmp(line, "ACCEPT", 7)) {
#ifdef ACCEPT_ON
							add_accept_header(req, value);
#endif
							return 1;
						}

#ifdef USE_AUTH
	#ifdef DIGEST_AUTHENTICATION
			if (!memcmp(line,"AUTHORIZATION",14))
			{
				if (strncasecmp(value, "Digest ", 7))
				{
					printf("Can only handle Digest auth\n");
					send_r_bad_request(req);
					return 0;
				}

				//Authorization: Digest username="111", realm="Realtek", nonce="VGh1IE9jdCAxNSAxMDowNjoxMSAyMDE1", uri="/", response="07f9580e67fc71ad24301541f9a52d8f"

				//pick up the value of 'username' from 'value' string
				req->userName=pick_up_field_value(value,"username");
				if(req->userName == NULL)
				{
					send_r_bad_request(req);
					return 0;
				}

				//pick up the value of 'nonce' from 'value' string
				req->authorization_nonce = pick_up_field_value(value,"nonce");
				if(req->authorization_nonce == NULL)
				{
					send_r_bad_request(req);
					return 0;
				}

				//pick up the value of 'uri' from 'value' string
				req->authorization_uri = pick_up_field_value(value,"uri");
				if(req->authorization_uri == NULL)
				{
					send_r_bad_request(req);
					return 0;
				}

				//pick up the value of 'response' from 'value' string
				char * response_pointer=strstr(value,"response");
				if(response_pointer != NULL)
				{
					req->authorization_response=malloc(FINAL_DIGEST_LENGTH+1);//magic number
					if(req->authorization_response != NULL)
					{
						memset(req->authorization_response,0,FINAL_DIGEST_LENGTH+1);
						strncpy(req->authorization_response,response_pointer+10,FINAL_DIGEST_LENGTH);//magic number
						req->authorization_response[FINAL_DIGEST_LENGTH]='\0';
					}
					else
					{
						perror("boa: malloc response out of memory in function process_option_line");
						exit(1);
					}
				}
				else
				{
					printf("No response field in Authorizaition line.\n");
					send_r_bad_request(req);
					return 0;
				}
				return 1;
			}
	#else
												 //get username and password inputed by user from browser,
												 //after decode, assign them to 'req->userName' and 'req->password'
						if (!memcmp(line,"AUTHORIZATION",14)) {
							char userAuth[0x80];
							char *cp;
							if (strncasecmp(value, "Basic ", 6)) {
								printf("Can only handle Basic auth\n");
								send_r_bad_request(req);
								return 0;
							}
							base64decode(userAuth, value+6, sizeof(userAuth));
							if ( (cp = strchr(userAuth,':')) == 0 ) {
								printf("No user:pass in Basic auth\n");
								send_r_bad_request(req);
								return 0;
							}
							*cp++=0;
							req->userName = strdup(userAuth);
							req->password = strdup(cp);
							return 1;
						}
	#endif
#endif

						break;
					case 'C':
						//printf("line=%s value=%s\n", line, value);
						if (!memcmp(line, "CONTENT_TYPE", 13) && !req->content_type) {
							req->content_type = value;
							req->multipart_boundary = NULL;
							if (strlen(value) > strlen("multipart/form-data")) {
								if (strncmp(value, "multipart/form-data", strlen("multipart/form-data")) == 0) {
									char *ptr;
									if ((ptr = strstr(value, "boundary=")) != NULL) {
										ptr += strlen("boundary=");
										req->multipart_boundary = malloc(strlen(ptr)+4);
										if (req->multipart_boundary)
											sprintf(req->multipart_boundary , "--%s", ptr);
									}
								}
							}
							//printf("req->content_type=%s\n", req->content_type);
							//if (req->multipart_boundary)
							//	printf("req->multipart_boundary=%s\n", req->multipart_boundary);
							return 1;
						} else if (!memcmp(line, "CONTENT_LENGTH", 15)
								&& !req->content_length) {
							req->content_length = value;
#ifdef HTTP_FILE_SERVER_SUPPORTED
							req->clen = boa_atoi(req->content_length);
							req->TotalContentLen = req->clen;
#endif
							return 1;
						} else if (!memcmp(line, "CONNECTION", 11) &&
								ka_max && req->keepalive != KA_STOPPED) {
#ifdef SUPPORT_ASP
							req->keepalive = KA_STOPPED;
#else
							req->keepalive = (!strncasecmp(value, "Keep-Alive", 10) ?
									KA_ACTIVE : KA_STOPPED);
#endif            
							return 1;
						}
						break;
					case 'H':
						if (!memcmp(line, "HOST", 5) && !req->header_host) {
							req->header_host = value; /* may be complete garbage! */
							return 1;
						}
						break;
					case 'I':
						if (!memcmp(line, "IF_MODIFIED_SINCE", 18)
								&& !req->if_modified_since) {
							req->if_modified_since = value;
							return 1;
						}
						break;
					case 'R':
						/* Need agent and referer for logs */
						if (!memcmp(line, "REFERER", 8)) {
							req->header_referer = value;
							if (!add_cgi_env(req, "REFERER", value, 1)) {
								/* errors already logged */
								return 0;
							}
						} else if (!memcmp(line, "RANGE", 6)) {
							if (req->ranges && req->ranges->stop == INT_MAX) {
								/* there was an error parsing, ignore */
								return 1;
							} else if (!range_parse(req, value)) {
								/* unable to parse range */
								send_r_invalid_range(req);
								return 0;
							}                   /* req->ranges */
						}
						break;
					case 'U':
						if (!memcmp(line, "USER_AGENT", 11)) {
							req->header_user_agent = value;
#ifdef HTTP_FILE_SERVER_SUPPORTED
							CheckUA(req);
#endif            

							if (!add_cgi_env(req, "USER_AGENT", value, 1)) {
								/* errors already logged */
								return 0;
							}
							return 1;
						}
						break;
					default:                   /* no default */
						break;
				}                           /* switch */

				return add_cgi_env(req, line, value, 1);
			}

#ifdef ACCEPT_ON
			/*
			 * Name: add_accept_header
			 * Description: Adds a mime_type to a requests accept char buffer
			 *   silently ignore any that don't fit -
			 *   shouldn't happen because of relative buffer sizes
			 */

			void add_accept_header(request * req, const char *mime_type)
			{
				int l = strlen(req->accept);
				int l2 = strlen(mime_type);

				if ((l + l2 + 2) >= MAX_HEADER_LENGTH)
					return;

				if (req->accept[0] == '\0') {
					memcpy(req->accept, mime_type, l2 + 1);
				} else {
					req->accept[l] = ',';
					req->accept[l + 1] = ' ';
					memcpy(req->accept + l + 2, mime_type, l2 + 1);
					/* the +1 is for the '\0' */
				}
			}
#endif

			void free_requests(void) {
				request *ptr, *next;

				ptr = request_free;
				while (ptr != NULL) {
					next = ptr->next;

#ifdef SUPPORT_ASP
					if (ptr->buffer)
					{
						free(ptr->buffer);
						ptr->buffer=NULL;
					}
					// davidhsu -------------
					if (ptr->upload_data){
						free(ptr->upload_data);
						ptr->upload_data=NULL;
					}
					//----------------------
#endif

					free(ptr);
					ptr = next;
				}
				request_free = NULL;
			}
