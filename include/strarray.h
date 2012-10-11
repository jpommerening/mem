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
#ifndef _MEM_STRARRAY_H_
#define _MEM_STRARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "array.h"
#include "strstack.h"
  
#define STRARRAY_EXTERN extern
  
typedef struct strarray_s strarray_t;
  
struct strarray_s {
  strstack_t stack;
  struct array_s(const char*) arr;
};
  
#define STRARRAY_INIT { STRSTACK_INIT, ARRAY_INIT }

STRARRAY_EXTERN void strarray_init( strarray_t* strarr );
  
STRARRAY_EXTERN void strarray_destroy( strarray_t* strarr );
  
STRARRAY_EXTERN void strarray_push( strarray_t* strarr, const char* data, size_t len );
STRARRAY_EXTERN const char* strarray_pop( strarray_t* strarr, size_t* len );
  
STRARRAY_EXTERN void strarray_pushstr( strarray_t* strarr, const char* str );
STRARRAY_EXTERN const char* strarray_popstr( strarray_t* strarr );
  
__attribute__((format (printf,2,0)))
STRARRAY_EXTERN void strarray_pushvfmt( strarray_t* strarr, const char* fmt, va_list vargs );
__attribute__((format (printf,2,3)))
STRARRAY_EXTERN void strarray_pushfmt( strarray_t* strarr, const char* fmt, ... );
  
#ifdef __cplusplus
}
#endif

#endif
