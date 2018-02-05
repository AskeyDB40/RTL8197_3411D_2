/*
 * trace-tcp.c - TCP support for IPv6 traceroute tool
 */

/*************************************************************************
 *  Copyright © 2005-2006 Rémi Denis-Courmont.                           *
 *  This program is free software: you can redistribute and/or modify    *
 *  it under the terms of the GNU General Public License as published by *
 *  the Free Software Foundation, versions 2 or 3 of the license.        *
 *                                                                       *
 *  This program is distributed in the hope that it will be useful,      *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 *  GNU General Public License for more details.                         *
 *                                                                       *
 *  You should have received a copy of the GNU General Public License    *
 *  along with this program. If not, see <http://www.gnu.org/licenses/>. *
 *************************************************************************/

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#undef _GNU_SOURCE
#define _BSD_SOURCE 1

#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>

#include <sys/types.h>
#include <unistd.h> // getpid()
#include <sys/socket.h> // SOCK_STREAM
#include <netinet/in.h>
//#include <netinet/tcp.h>

#include "traceroute.h"

#define TCP_WINDOW 4096

#ifndef TH_ECE
# define TH_ECE 0x40
# define TH_CWR 0x80
#endif

typedef u_int32_t tcp_seq;
/*
 * TCP header.
 * Per RFC 793, September, 1981.
 */
struct tcphdr
  {
    u_int16_t th_sport;         /* source port */
    u_int16_t th_dport;         /* destination port */
    tcp_seq th_seq;             /* sequence number */
    tcp_seq th_ack;             /* acknowledgement number */
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int8_t th_x2:4;           /* (unused) */
    u_int8_t th_off:4;          /* data offset */
#  endif
#  if __BYTE_ORDER == __BIG_ENDIAN
    u_int8_t th_off:4;          /* data offset */
    u_int8_t th_x2:4;           /* (unused) */
#  endif
    u_int8_t th_flags;
#  define TH_FIN        0x01
#  define TH_SYN        0x02
#  define TH_RST        0x04
#  define TH_PUSH       0x08
#  define TH_ACK        0x10
#  define TH_URG        0x20
    u_int16_t th_win;           /* window */
    u_int16_t th_sum;           /* checksum */
    u_int16_t th_urp;           /* urgent pointer */
};


/* TCP/SYN probes */
static ssize_t
send_syn_probe (int fd, unsigned ttl, unsigned n, size_t plen, uint16_t port)
{
	if (plen < sizeof (struct tcphdr))
		plen = sizeof (struct tcphdr);

	struct
	{
		struct tcphdr th;
		uint8_t payload[plen - sizeof (struct tcphdr)];
	} packet;

	memset (&packet, 0, sizeof (packet));
	packet.th.th_sport = sport;
	packet.th.th_dport = port;
	packet.th.th_seq = htonl ((ttl << 24) | (n << 16) | getpid ());
	packet.th.th_off = sizeof (packet.th) / 4;
	packet.th.th_flags = TH_SYN | (ecn ? (TH_ECE | TH_CWR) : 0);
	packet.th.th_win = htons (TCP_WINDOW);

	return send_payload (fd, &packet, plen, ttl);
}


static ssize_t
parse_syn_resp (const void *data, size_t len, int *ttl, unsigned *n,
                uint16_t port)
{
	const struct tcphdr *pth = (const struct tcphdr *)data;
	uint32_t seq;

	if ((len < sizeof (*pth))
	 || (pth->th_dport != sport)
	 || (pth->th_sport != port)
	 || ((pth->th_flags & TH_ACK) == 0)
	 || (((pth->th_flags & TH_SYN) != 0) == ((pth->th_flags & TH_RST) != 0))
	 || (pth->th_off < (sizeof (*pth) / 4)))
		return -1;

	seq = ntohl (pth->th_ack) - 1;
	if ((seq & 0xffff) != (unsigned)getpid ())
		return -1;

	*ttl = seq >> 24;
	*n = (seq >> 16) & 0xff;
	return 1 + ((pth->th_flags & TH_SYN) == TH_SYN);
}


static ssize_t
parse_syn_error (const void *data, size_t len, int *ttl, unsigned *n,
                 uint16_t port)
{
	const struct tcphdr *pth = (const struct tcphdr *)data;
	uint32_t seq;

	if ((len < 8)
	 || (pth->th_sport != sport)
	 || (pth->th_dport != port))
		return -1;

	seq = ntohl (pth->th_seq);
	if ((seq & 0xffff) != (unsigned)getpid ())
		return -1;

	*ttl = seq >> 24;
	*n = (seq >> 16) & 0xff;
	return 0;
}


const tracetype syn_type =
	{ SOCK_STREAM, IPPROTO_TCP, 16,
	  send_syn_probe, parse_syn_resp, parse_syn_error };


/* TCP/ACK probes */
static ssize_t
send_ack_probe (int fd, unsigned ttl, unsigned n, size_t plen, uint16_t port)
{
	if (plen < sizeof (struct tcphdr))
		plen = sizeof (struct tcphdr);

	struct
	{
		struct tcphdr th;
		uint8_t payload[plen - sizeof (struct tcphdr)];
	} packet;

	memset (&packet, 0, sizeof (packet));
	packet.th.th_sport = sport;
	packet.th.th_dport = port;
	packet.th.th_ack = htonl ((ttl << 24) | (n << 16) | getpid ());
	packet.th.th_off = sizeof (packet.th) / 4;
	packet.th.th_flags = TH_ACK;
	packet.th.th_win = htons (TCP_WINDOW);

	return send_payload (fd, &packet, plen, ttl);
}


static ssize_t
parse_ack_resp (const void *data, size_t len, int *ttl, unsigned *n,
                uint16_t port)
{
	const struct tcphdr *pth = (const struct tcphdr *)data;
	uint32_t seq;

	if ((len < sizeof (*pth))
	 || (pth->th_dport != sport)
	 || (pth->th_sport != port)
	 || (pth->th_flags & TH_SYN)
	 || (pth->th_flags & TH_ACK)
	 || ((pth->th_flags & TH_RST) == 0)
	 || (pth->th_off < (sizeof (*pth) / 4)))
		return -1;

	seq = ntohl (pth->th_seq);
	if ((seq & 0xffff) != (unsigned)getpid ())
		return -1;

	*ttl = seq >> 24;
	*n = (seq >> 16) & 0xff;
	return 0;
}


static ssize_t
parse_ack_error (const void *data, size_t len, int *ttl, unsigned *n,
                 uint16_t port)
{
	const struct tcphdr *pth = (const struct tcphdr *)data;
	uint32_t seq;

	if ((len < 8)
	 || (pth->th_sport != sport)
	 || (pth->th_dport != port))
		return -1;

	seq = ntohl (pth->th_ack);
	if ((seq & 0xffff) != (unsigned)getpid ())
		return -1;

	*ttl = seq >> 24;
	*n = (seq >> 16) & 0xff;
	return 0;
}


const tracetype ack_type =
	{ SOCK_STREAM, IPPROTO_TCP, 16,
	  send_ack_probe, parse_ack_resp, parse_ack_error };

