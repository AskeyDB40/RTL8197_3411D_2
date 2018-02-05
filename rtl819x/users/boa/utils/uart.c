#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/sysmacros.h>

#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <memory.h>
#include <utmp.h>
#include <mntent.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <getopt.h>

#define ok_error(num) ((num)==EIO)
#define fatal printf

#define QUERY_ON "AT+CFUN=1"
#define DIAL_ON "AT+CGDATA=\"M-0000\",1"
void setdtr (int tty_fd, int on);
static int translate_speed (int bps);
static int baud_rate_of (int speed);

int restore_term=0;
int modem=0;
int crtscts=1;
int baud_rate=0;
int inspeed=115200;
struct termios inittermios;
int default_device=0;
/*
 * List of valid speeds.
 */

struct speed {
    int speed_int, speed_val;
} speeds[] = {
#ifdef B50
    { 50, B50 },
#endif
#ifdef B75
    { 75, B75 },
#endif
#ifdef B110
    { 110, B110 },
#endif
#ifdef B134
    { 134, B134 },
#endif
#ifdef B150
    { 150, B150 },
#endif
#ifdef B200
    { 200, B200 },
#endif
#ifdef B300
    { 300, B300 },
#endif
#ifdef B600
    { 600, B600 },
#endif
#ifdef B1200
    { 1200, B1200 },
#endif
#ifdef B1800
    { 1800, B1800 },
#endif
#ifdef B2000
    { 2000, B2000 },
#endif
#ifdef B2400
    { 2400, B2400 },
#endif
#ifdef B3600
    { 3600, B3600 },
#endif
#ifdef B4800
    { 4800, B4800 },
#endif
#ifdef B7200
    { 7200, B7200 },
#endif
#ifdef B9600
    { 9600, B9600 },
#endif
#ifdef B19200
    { 19200, B19200 },
#endif
#ifdef B38400
    { 38400, B38400 },
#endif
#ifdef B57600
    { 57600, B57600 },
#endif
#ifdef B76800
    { 76800, B76800 },
#endif
#ifdef B115200
    { 115200, B115200 },
#endif
#ifdef EXTA
    { 19200, EXTA },
#endif
#ifdef EXTB
    { 38400, EXTB },
#endif
#ifdef B230400
    { 230400, B230400 },
#endif
#ifdef B460800
    { 460800, B460800 },
#endif
#ifdef B921600
    { 921600, B921600 },
#endif
    { 0, 0 }
};

/********************************************************************
 *
 * Translate from bits/second to a speed_t.
 */

static int translate_speed (int bps)
{
    struct speed *speedp;

    if (bps != 0) {
	for (speedp = speeds; speedp->speed_int; speedp++) {
	    if (bps == speedp->speed_int)
		return speedp->speed_val;
	}
	warn("speed %d not supported", bps);
    }
    return 0;
}

/********************************************************************
 *
 * Translate from a speed_t to bits/second.
 */

static int baud_rate_of (int speed)
{
    struct speed *speedp;

    if (speed != 0) {
	for (speedp = speeds; speedp->speed_int; speedp++) {
	    if (speed == speedp->speed_val)
		return speedp->speed_int;
	}
    }
    return 0;
}

/********************************************************************
 *
 * set_up_tty: Set up the serial port on `fd' for 8 bits, no parity,
 * at the requested speed, etc.  If `local' is true, set CLOCAL
 * regardless of whether the modem option was specified.
 */

void set_up_tty(int tty_fd, int local)
{
    int speed;
    struct termios tios;

    setdtr(tty_fd, 1);
    if (tcgetattr(tty_fd, &tios) < 0) {
	if (!ok_error(errno))
	    fatal("tcgetattr: %m (line %d)", __LINE__);
	return;
    }

    if (!restore_term)
	inittermios = tios;

    tios.c_cflag     &= ~(CSIZE | CSTOPB | PARENB | CLOCAL);
    tios.c_cflag     |= CS8 | CREAD | HUPCL;

    tios.c_iflag      = IGNBRK | IGNPAR;
    tios.c_oflag      = 0;
    tios.c_lflag      = 0;
    tios.c_cc[VMIN]   = 1;
    tios.c_cc[VTIME]  = 0;

    if (local || !modem)
	tios.c_cflag ^= (CLOCAL | HUPCL);

    switch (crtscts) {
    case 1:
	tios.c_cflag |= CRTSCTS;
	break;

    case -2:
	tios.c_iflag     |= IXON | IXOFF;
	tios.c_cc[VSTOP]  = 0x13;	/* DC3 = XOFF = ^S */
	tios.c_cc[VSTART] = 0x11;	/* DC1 = XON  = ^Q */
	break;

    case -1:
	tios.c_cflag &= ~CRTSCTS;
	break;

    default:
	break;
    }

    speed = translate_speed(inspeed);
    if (speed) {
	cfsetospeed (&tios, speed);
	cfsetispeed (&tios, speed);
    }
/*
 * We can't proceed if the serial port speed is B0,
 * since that implies that the serial port is disabled.
 */
    else {
	speed = cfgetospeed(&tios);
	if (speed == B0)
	    fatal("Baud rate for is 0; need explicit baud rate");
    }

    while (tcsetattr(tty_fd, TCSAFLUSH, &tios) < 0 && !ok_error(errno))
	if (errno != EINTR)
	    fatal("tcsetattr: %m (line %d)", __LINE__);

    baud_rate    = baud_rate_of(speed);
    restore_term = 1;
}

/********************************************************************
 *
 * setdtr - control the DTR line on the serial port.
 * This is called from die(), so it shouldn't call die().
 */

void setdtr (int tty_fd, int on)
{
    int modembits = TIOCM_DTR;

    ioctl(tty_fd, (on ? TIOCMBIS : TIOCMBIC), &modembits);
}

/********************************************************************
 *
 * restore_tty - restore the terminal to the saved settings.
 */

void restore_tty (int tty_fd)
{
    if (restore_term) {
	restore_term = 0;
/*
 * Turn off echoing, because otherwise we can get into
 * a loop with the tty and the modem echoing to each other.
 * We presume we are the sole user of this tty device, so
 * when we close it, it will revert to its defaults anyway.
 */
	if (!default_device)
	    inittermios.c_lflag &= ~(ECHO | ECHONL);

	if (tcsetattr(tty_fd, TCSAFLUSH, &inittermios) < 0) {
	    if (! ok_error (errno))
		warn("tcsetattr: %m (line %d)", __LINE__);
	}
    }
}

void print_usage(char *prg)
{
	fprintf(stderr, "\nUsage: %s [options] tty\n\n", prg);
	fprintf(stderr, "Options: -b <baud> (uart baud rate)\n");
	fprintf(stderr, "Options: -c <count> (ascii count)\n");
	fprintf(stderr, "\nExample:\n");
	fprintf(stderr, "uart -b 115200 /dev/ttyUSB0\n");
	fprintf(stderr, "\n");
	exit(1);
}

int main(int argc, char *argv[])
{
	char *btr = NULL;
	int opt,count;
 	char *tty,*cmd;
	char buff[4096];
	int fd;	
	int fdflags,nwrite,length;
    struct timeval tv = {0};
    fd_set rfds;
    int i;

#if 0
 	while ((opt = getopt(argc, argv, "b:c:s:?")) != -1) {
  		switch (opt) {
  		case 'b':
   			btr = optarg;
   			printf("baud=%s\n",btr);
			inspeed=atoi(btr);
   			break;
  		case 'c':
   			count = atoi(optarg);
   			printf("count=%d\n",count);
   			break;
		case 's':
			cmd = optarg;
			printf("%s\n",cmd);
			break;	
  		case '?':
  			default:
   			print_usage(argv[0]);
   			break;
		}
 	}

 	if (argc - optind != 1)
  		print_usage(argv[0]);
 
	tty = argv[optind];
 #endif
	
	tty = argv[1];
	cmd = argv[2];

	printf("tty=%s,cmd=%s\n",tty,cmd);
 
	if ((fd = open (tty, (O_NONBLOCK | O_RDWR | O_NOCTTY))) < 0) {
  		perror(tty);
  		exit(1);
 	}
    
	if ((fdflags = fcntl(fd, F_GETFL)) == -1
		|| fcntl(fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
			warn("Couldn't reset non-blocking mode on device: %m");

	set_up_tty(fd,1);

	tcflush(fd, TCIOFLUSH);

	strcpy(buff,cmd);
	strcat(buff,"\r\n");
	length=strlen(buff);
	nwrite=write(fd,buff,length);
	if(nwrite < length)
		printf("write error nwrite %d length %d\n",nwrite,length);

	/* need to check if ok */
	for(i = 0;i < 10;i++){
	    tv.tv_sec=1;
	    tv.tv_usec=0;
	    FD_ZERO(&rfds);
	    FD_SET(fd,&rfds);
	    if (select(1+fd,&rfds,NULL,NULL,&tv)>0)
	    {
	    	if(FD_ISSET(fd,&rfds))
	    	{
	    		int nread=read(fd, buff, 4095);
	    		printf("read(%d): %s\n", nread,buff);
				if(strstr(buff,"OK") != NULL
					|| strstr(buff,"CONNECT") != NULL
					|| strstr(buff,"DPPPI") != NULL
					|| strstr(buff,"ERROR") != NULL)
					break;
	    	}
		}
	}
	
	tcflush(fd, TCIOFLUSH);
	restore_tty(fd);
	close(fd);
}
