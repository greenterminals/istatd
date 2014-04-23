/*
 *  Copyright 2012 William Yodlowsky. All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    1.  Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *
 *    2.  Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *
 *    3.  The name of the copyright holder may not be used to endorse or promote
 *        products derived from this software without specific prior written
 *        permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER ``AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL WILLIAM TISÄTER BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "system.h"

# ifdef HAVE_GETIFADDRS

get_net_ip(const char * _dev, char *)
{
	struct ifaddrs *ifap, *ifa;
	struct ifreq *ifrp;
	size_t nlen = strlen(_dev);
	char hbuf[NI_MAXHOST];

	if (getifaddrs(&ifap) != 0)
		return -1;

	namep = NULL;
	for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
		if (nlen && isdigit(_dev[nlen - 1])) {
			if (strncmp(_dev, ifa->ifa_name, nlen) != 0)
				    continue;;

			if (ifa->ifa_addr->sa_family == AF_INET) {
				sa = ifa->ifa_addr;
				if (inet_ntop(ifa->ifa_addr->sa_family, &(((struct sockaddr_in *)sa)->sin_addr), hbuf, sizeof(hbuf)) == NULL) {
					freeifaddrs(ifap);
					return(NULL);
				}

				return(&hbuf);
			}
		}
	}
}

# endif		/* HAVE_GETIFADDRS */
