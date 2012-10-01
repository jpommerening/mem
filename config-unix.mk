# Copyright (c) 2012, Jonas Pommerening <jonas.pommerening@gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met: 
#
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer. 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution. 
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

E =
CSTDFLAG = --std=c99 -pedantic -Wall -Wextra
CFLAGS += -g
LINKFLAGS = 

CPPFLAGS += -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CPPFLAGS_JEMALLOC = --std=c99 -Wall -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable $(CPPFLAGS)

OBJS += src/buffer.o
OBJS += src/alloc.o
OBJS += src/jemalloc/jemalloc.o

ifeq (SunOS,$(uname_S))
CPPFLAGS +=
LINKFLAGS +=
OBJS +=
endif

ifeq (Darwin,$(uname_S))
CPPFLAGS +=
LINKFLAGS +=
OBJS +=
endif

ifeq (Linux,$(uname_S))
endif

ifeq (FreeBSD,$(uname_S))
endif

ifeq (DragonFly,$(uname_S))
endif

ifeq (NetBSD,$(uname_S))
endif

ifeq (OpenBSD,$(uname_S))
endif

ifneq (,$(findstring CYGWIN,$(uname_S)))
endif

mem.a: $(OBJS)
	$(AR) rcs mem.a $(OBJS)

src/jemalloc/jemalloc.o: src/jemalloc/jemalloc.c src/jemalloc/*.h include/mem-private/*.h
	$(CC) $(CPPFLAGS_JEMALLOC) -Iinclude/mem-private -Isrc/jemalloc $(CFLAGS) -c $< -o $@

src/%.o: src/%.c include/mem.h src/internal.h
	$(CC) $(CSTDFLAG) $(CPPFLAGS) -Isrc $(CFLAGS) -c $< -o $@

clean-platform:
	-#

distclean-platform:
	-#
