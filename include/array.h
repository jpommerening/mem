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
#ifndef _MEM_ARRAY_H_
#define _MEM_ARRAY_H_

#include <string.h>

#include "alloc.h"

#define array_s(T) \
  { \
    size_t alloc; \
    size_t length; \
    T* data; \
  }

#define ARRAY_INIT { 0, 0, NULL }
#define ARRAY_INITARR(x) { 0, sizeof(x) / sizeof((x)[0]), x }

#define array_init(arr, len) \
  do { \
    (arr)->alloc  = 0; \
    (arr)->length = 0; \
    (arr)->data   = NULL; \
    array_grow( arr, len ); \
  } while( 0 )

#define array_destroy(arr) \
  do { \
    if( (arr)->alloc && (arr)->data ) \
      free( (arr)->data ); \
  } while( 0 )

#define array_grow(arr, len) \
  do { \
    size_t arrsize ## __LINE__ = ( ( (len) > ((arr)->length) ) ? len + 1 : (arr)->length + 1 ) * sizeof((arr)->data[0]); \
    void*  arrdata ## __LINE__ = (arr)->data; \
    if( (arr)->alloc == 0 ) { \
      (arr)->data = grow( NULL, arrsize ## __LINE__, &((arr)->alloc) ); \
      if( arrdata ## __LINE__ ) \
        memcpy( (arr)->data, arrdata ## __LINE__, sizeof((arr)->data[0]) * ((arr)->length + 1) ); \
    } else if( (arr)->alloc < arrsize ## __LINE__ ) { \
      (arr)->data = grow( (arr)->data, arrsize ## __LINE__, &((arr)->alloc) ); \
    } \
  } while( 0 )

#define array_avail(arr) \
  ( (arr)->alloc ? ( (arr)->alloc / sizeof((arr)->data[0]) ) - (arr)->length : 0 )

#define array_push(arr, x) \
  do { \
    array_grow(arr, (arr)->length + 1); \
    (arr)->data[(arr)->length] = x; \
    (arr)->length += 1; \
  } while( 0 )


#define array_pop(arr) \
  ( ( (arr)->length > 0 && (arr)->data != NULL ) ? (arr)->data[--((arr)->length)] : 0 )


#endif