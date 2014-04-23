/*
 *  Copyright 2010 William Tisäter. All rights reserved
 *  Copyright 2010 Mo McRoberts.
 *  Copyright 2010 Guntram Blohm.
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

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifdef HAVE_SYS_SWAP_H
# include <sys/param.h>
# include <sys/swap.h>
#endif

#include "system.h"

#ifdef USE_SWAP_SWAPCTL
int get_swp_data(struct mem_data * _mem)
{
	struct swaptable *st;
	struct swapent *ent;
	char *pbuf, *obuf;
	int bpp, num, c;
    long long swapfree = 0;

	if (0 == _mem->swt && -1 != (num = swapctl(SC_GETNSWP, NULL)))
	{
		bpp = getpagesize() >> DEV_BSHIFT;
		st = (struct swaptable *) malloc(num = sizeof(swapent_t) + sizeof(int));
		pbuf = obuf = (char *) malloc(num * MAXPATHLEN);
		ent = st->swt_ent;

		/* Provide buffers for the swap device names */
		for (c = 0; c < num; c++, ent++)
		{
			ent->ste_path = pbuf;
			pbuf += MAXPATHLEN;
		}

		/* Retrieve the devices */
		if(-1 != (num = swapctl(SC_LIST, st)))
		{
			ent = st->swt_ent;

			for (c = 0; c < num; c++, ent++)
			{
				_mem->swt += (unsigned long long) ent->ste_pages * bpp * DEV_BSIZE / 1024;
				swapfree += (unsigned long long) ent->ste_free * bpp * DEV_BSIZE / 1024;
			}

            _mem->swu = _mem->swt - swapfree;
		}

		free(obuf);
		free(st);
	}
	
	return 0;
}
#endif /* USE_SWAP_SWAPCTL */
