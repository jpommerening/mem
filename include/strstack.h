/* -*- Mode: C; tab-width: 2; c-basic-offset: 2 -*- */
/* vim:set softtabstop=2 shiftwidth=2: */
/*
 * Copyright (c) 2012, Jonas Pommerening <jonas.pommerening@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _MEM_STRSTACK_H_
#define _MEM_STRSTACK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdarg.h>

#include "buffer.h"
  
#define STRSTACK_EXTERN extern

typedef struct strstack_s strstack_t;

struct strstack_s {
  buffer_t buf;
  const char* top;
};

#define STRSTACK_INIT { BUFFER_INITSTR("\0"), "" }

STRSTACK_EXTERN void strstack_init( strstack_t* stack );

STRSTACK_EXTERN void strstack_destroy( strstack_t* stack );

STRSTACK_EXTERN void strstack_push( strstack_t* stack, const char* data, size_t len );
STRSTACK_EXTERN const char* strstack_pop( strstack_t* stack, size_t* len );
  
STRSTACK_EXTERN void strstack_pushstr( strstack_t* stack, const char* str );
STRSTACK_EXTERN const char* strstack_popstr( strstack_t* stack );
  
__attribute__((format (printf,2,0)))
STRSTACK_EXTERN void strstack_pushvfmt( strstack_t* stack, const char* fmt, va_list vargs );
__attribute__((format (printf,2,3)))
STRSTACK_EXTERN void strstack_pushfmt( strstack_t* stack, const char* fmt, ... );

#ifdef __cplusplus
}
#endif

#endif
