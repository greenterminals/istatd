/*
 *  Copyright 2012 William Yodlowsky. All rights reserved.
 *  Copyright 2010 William Tisäter. All rights reserved.
 *  Copyright 2010 Mo McRoberts.
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

#include <string.h>

#include "system.h"

#ifdef HAVE_LIBSTATGRAB

#include <statgrab.h>

# ifdef USE_CPU_LIBSTATGRAB
int get_cpu_data(struct cpu_data * _cpu)
{
	sg_cpu_stats *cpu_s;
	
	_cpu->u = _cpu->n = _cpu->i = _cpu->s = 0;
	if(NULL == (cpu_s = sg_get_cpu_stats())) return -1;
	_cpu->u += cpu_s->user;
	_cpu->s += cpu_s->kernel;
	_cpu->i += cpu_s->idle;
	return 0;
}
# endif /*USE_CPU_LIBSTATGRAB*/

# ifdef USE_MEM_LIBSTATGRAB
int get_mem_data(struct mem_data * _mem)
{
	sg_mem_stats *mem;
	sg_page_stats *page;
	sg_swap_stats *swap;
	
	memset(_mem, 0, sizeof(struct mem_data));

	if (NULL == (mem = sg_get_mem_stats())) return -1;

	_mem->t = mem->total / 1024;	/* convert from MB to KB */
	_mem->f = mem->free  / 1024;
	_mem->a = mem->used  / 1024;
	_mem->i = 0;	/* XXX */
	_mem->c = mem->cache / 1024;

	if (NULL == (page = sg_get_page_stats())) return -1;
        _mem->swi = page->pages_pagein;
        _mem->swo = page->pages_pageout;

	if (NULL == (swap = sg_get_swap_stats())) return -1;

        _mem->swt = swap->total;
        _mem->swu = swap->used;
	return 0;
}
# endif /*USE_MEM_LIBSTATGRAB*/

# ifdef USE_NET_LIBSTATGRAB
int get_net_data(const char * _dev, struct net_data * _data)
{
	int num_io, num_iface, i;
	sg_network_io_stats *io;

	if (NULL == (io = sg_get_network_io_stats(&num_io))) return -1;

	for (i = 0; i < num_io; i++) {
		if (!strcmp(io[i].interface_name, _dev)) {
			_data->s = io[i].tx;
			_data->r = io[i].rx;
			break;
		}
	}
	return 0;
}
# endif /*USE_NET_LIBSTATGRAB*/

# ifdef USE_UPTIME_LIBSTATGRAB
int get_uptime()
{
	sg_host_info *host;

	if (NULL == (host = sg_get_host_info())) return -1;

	return host->uptime;
}
# endif /*USE_UPTIME_LIBSTATGRAB*/

#endif /*HAVE_LIBSTATGRAB*/
