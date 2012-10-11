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
#ifndef _MEM_BUFFER_H_
#define _MEM_BUFFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdarg.h>
#include <assert.h>

#include "array.h"

#define BUFFER_EXTERN extern

BUFFER_EXTERN char buffer_initial[];

typedef struct buffer_s buffer_t;

struct buffer_s array_s(char);

#define BUFFER_INIT { 0, 0, buffer_initial }
#define BUFFER_INITSTR(s) { 0, sizeof(s "")-1, s "" } 

#define BUFFER_WHITESPACE  " \t\n\r"
#define BUFFER_NWHITESPACE (sizeof(BUFFER_WHITESPACE "")-1)

BUFFER_EXTERN void buffer_init( buffer_t* buf, size_t len );
BUFFER_EXTERN void buffer_destroy( buffer_t* buf );
BUFFER_EXTERN char * buffer_detach( buffer_t* buf, size_t* len );
BUFFER_EXTERN void buffer_attach( buffer_t* buf, char*, size_t len, size_t alloc );
static inline void buffer_swap( buffer_t* a, buffer_t* b ) {
  buffer_t tmp = *a;
  *a = *b;
  *b = tmp;
}

BUFFER_EXTERN void buffer_grow( buffer_t*, size_t );

static inline size_t buffer_avail( const buffer_t* buf ) {
  return buf->alloc ? buf->alloc - buf->length - 1 : 0;
}

static inline int buffer_empty( const buffer_t* buf ) {
  return (buf->length == 0);
}

static inline void buffer_editable( buffer_t* buf ) {
  if( buf->alloc == 0 ) buffer_grow( buf, buf->length );
}
  
static inline void buffer_setlen( buffer_t* buf, size_t len ) {
  assert( len <= (buf->alloc ? buf->alloc - 1 : 0) && "buffer_setlen() beyond buffer" );
  buf->length    = len;
  buf->data[len] = '\0';
}
#define buffer_reset(sb) buffer_setlen(sb, 0)

BUFFER_EXTERN void buffer_splice( buffer_t* buf, size_t pos, size_t rem, const char* data, size_t len );
BUFFER_EXTERN void buffer_remove( buffer_t* buf, size_t pos, size_t len );
BUFFER_EXTERN void buffer_insert( buffer_t* buf, size_t pos, const char* data, size_t len );
BUFFER_EXTERN void buffer_append( buffer_t* buf, const char* data, size_t len );

BUFFER_EXTERN void buffer_insertbuf( buffer_t* buf, size_t pos, const buffer_t* b );
BUFFER_EXTERN void buffer_appendbuf( buffer_t* buf, const buffer_t* b );

BUFFER_EXTERN void buffer_str( buffer_t* buf, const char* str );
BUFFER_EXTERN void buffer_insertstr( buffer_t* buf, size_t pos, const char* str );
BUFFER_EXTERN void buffer_appendstr( buffer_t* buf, const char* str );

BUFFER_EXTERN void buffer_c( buffer_t* buf, int c );
BUFFER_EXTERN void buffer_insertc( buffer_t* buf, size_t pos, int c );
BUFFER_EXTERN void buffer_appendc( buffer_t* buf, int c );

__attribute__((format (printf,2,0)))
BUFFER_EXTERN void buffer_vfmt( buffer_t* buf, const char* fmt, va_list vargs );
__attribute__((format (printf,3,0)))
BUFFER_EXTERN void buffer_insertvfmt( buffer_t* buf, size_t pos, const char* fmt, va_list vargs );
__attribute__((format (printf,2,0)))
BUFFER_EXTERN void buffer_appendvfmt( buffer_t* buf, const char* fmt, va_list vargs );

__attribute__((format (printf,2,3)))
BUFFER_EXTERN void buffer_fmt( buffer_t* buf, const char* fmt, ... );
__attribute__((format (printf,3,4)))
BUFFER_EXTERN void buffer_insertfmt( buffer_t* buf, size_t pos, const char* fmt, ... );
__attribute__((format (printf,2,3)))
BUFFER_EXTERN void buffer_appendfmt( buffer_t* buf, const char* fmt, ... );

BUFFER_EXTERN size_t buffer_read( buffer_t* buf,
                                  const char* data,  size_t len,
                                  const char* delim, size_t ndelim );

BUFFER_EXTERN size_t buffer_read_escaped( buffer_t* buf, char c,
                                          const char* data,  size_t len,
                                          const char* delim, size_t ndelim,
                                          const char* esc,   size_t nesc );

BUFFER_EXTERN size_t buffer_rtrim( buffer_t* buf, const char* trim, size_t ntrim );
BUFFER_EXTERN size_t buffer_ltrim( buffer_t* buf, const char* trim, size_t ntrim );
BUFFER_EXTERN size_t buffer_trim( buffer_t* buf, const char* trim, size_t ntrim );

static inline size_t buffer_rtrimw( buffer_t* buf ) {
  return buffer_rtrim( buf, BUFFER_WHITESPACE, BUFFER_NWHITESPACE );
}

static inline size_t buffer_ltrimw( buffer_t* buf ) {
  return buffer_ltrim( buf, BUFFER_WHITESPACE, BUFFER_NWHITESPACE );
}

static inline size_t buffer_trimw( buffer_t* buf ) {
  return buffer_trim( buf, BUFFER_WHITESPACE, BUFFER_NWHITESPACE );
}

BUFFER_EXTERN int buffer_cmp( const buffer_t*, const buffer_t* );

#ifdef __cplusplus
}
#endif

#endif
