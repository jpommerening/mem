#include "buffer.h"
#include "alloc.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>

#define BUFFER_GUESS_FMTSIZE(fmt) (strlen(fmt)*1.2)

char buffer_initial[1] = { '\0' };

void buffer_init( buffer_t* buf, size_t len ) {
  buf->alloc  = 0;
  buf->length = 0;
  buf->data   = buffer_initial;
  
  if( len ) buffer_grow( buf, len );
}

void buffer_destroy( buffer_t* buf ) {
  array_destroy(buf);
}

char * buffer_detach( buffer_t* buf, size_t* len ) {
  size_t size = buf->length;
  char*  data = buf->data;
  
  buffer_init( buf, 0 );
  if( len != NULL ) {
    *len = size;
  }
  return data;
}

void buffer_attach( buffer_t* buf, char* data, size_t len, size_t alloc ) {
  buffer_destroy( buf );
  buf->alloc  = alloc;
  buf->length = len;
  buf->data   = data;
}

void buffer_grow( buffer_t* buf, size_t len ) {
  array_grow(buf, len);
}

void buffer_splice( buffer_t* buf, size_t pos, size_t rem, const char* data, size_t len ) {
  assert( pos <= buf->length && "buffer_splice() beyond buffer." );
  ssize_t diff = len - rem;
  
  if( diff > 0 ) {
    buffer_grow( buf, buf->length+diff );
    memmove( &(buf->data[pos+diff]), &(buf->data[pos]), buf->length - pos );
  } else if( diff < 0 ) {
    buffer_editable( buf );
    memmove( &(buf->data[pos]), &(buf->data[pos-diff]), buf->length - pos + diff );
  }
  
  if( len && data )
    memcpy( &(buf->data[pos]), data, len );
  
  buffer_setlen( buf, buf->length + diff );
}

void buffer_remove( buffer_t* buf, size_t pos, size_t len ) {
  buffer_splice( buf, pos, len, NULL, 0 );
}
void buffer_insert( buffer_t* buf, size_t pos, const char * data, size_t len ) {
  buffer_splice( buf, pos, 0, data, len );
}
void buffer_append( buffer_t* buf, const char* data, size_t len ) {
  buffer_splice( buf, buf->length, 0, data, len );
}

void buffer_insertbuf( buffer_t* buf, size_t pos, const buffer_t* b ) {
  buffer_splice( buf, pos, 0, b->data, b->length );
}
void buffer_appendbuf( buffer_t* buf, const buffer_t* b ) {
  buffer_splice( buf, buf->length, 0, b->data, b->length );
}

void buffer_str( buffer_t* buf, const char * str ) {
  size_t len = strlen(str);
  buffer_grow( buf, len );
  memcpy( buf->data, str, len );
  buffer_setlen( buf, len );
}
void buffer_insertstr( buffer_t* buf, size_t pos, const char* str ) {
  buffer_insert( buf, pos, (const char*)str, strlen(str) );
}
void buffer_appendstr( buffer_t* buf, const char* str ) {
  buffer_append( buf, (const char*)str, strlen(str) );
}

void buffer_c( buffer_t* buf, int c ) {
  buffer_grow( buf, 1 );
  buf->data[0] = c;
  buf->data[1] = '\0';
  buffer_setlen( buf, 1 );
}
void buffer_insertc( buffer_t* buf, size_t pos, int c ) {
  buffer_splice( buf, pos, 0, NULL, 1 );
  buf->data[pos] = c;
}
void buffer_appendc( buffer_t* buf, int c ) {
  buffer_grow( buf, buf->length + 1 );
  buf->data[buf->length] = c;
  buffer_setlen( buf, buf->length + 1 );
}

void buffer_vfmt( buffer_t* buf, const char* fmt, va_list vargs ) {
  size_t  len = BUFFER_GUESS_FMTSIZE(fmt);
  va_list cpy;
  va_copy(cpy, vargs);
  
  buffer_grow( buf, len );
  len = vsnprintf( buf->data, buf->alloc, fmt, vargs );
  
  if( len >= buf->alloc ) {
    buffer_grow( buf, len );
    len = vsnprintf( buf->data, buf->alloc, fmt, cpy );
  }
  va_end(cpy);
  buffer_setlen( buf, len );
}
void buffer_insertvfmt( buffer_t* buf, size_t pos, const char* fmt, va_list vargs ) {
  buffer_t tmp = BUFFER_INIT;
  buffer_vfmt( &tmp, fmt, vargs );
  buffer_insertbuf( buf, pos, &tmp );
  buffer_destroy( &tmp );
}
void buffer_appendvfmt( buffer_t* buf, const char* fmt, va_list vargs ) {
  buffer_t tmp = BUFFER_INIT;
  buffer_vfmt( &tmp, fmt, vargs );
  buffer_appendbuf( buf, &tmp );
  buffer_destroy( &tmp );
}

void buffer_fmt( buffer_t* buf, const char* fmt, ... ) {
  va_list vargs;
  va_start(vargs, fmt);
  buffer_vfmt( buf, fmt, vargs );
  va_end(vargs);
}
void buffer_insertfmt( buffer_t* buf, size_t pos, const char* fmt, ... ) {
  va_list vargs;
  va_start(vargs, fmt);
  buffer_insertvfmt( buf, pos, fmt, vargs );
  va_end(vargs);
}
void buffer_appendfmt( buffer_t* buf, const char* fmt, ... ) {
  va_list vargs;
  va_start(vargs, fmt);
  buffer_appendvfmt( buf, fmt, vargs );
  va_end(vargs);
}

static inline int char_in_set( char b, const char* set, size_t len ) {
  while( len > 0 )
    if( b == set[--len] ) return 1;
  return 0;
}

size_t buffer_read( buffer_t* buf,
                    const char* data,  size_t len,
                    const char* delim, size_t ndelim ) {
  size_t i;
  for( i=0; (i<len) && !char_in_set( data[i], delim, ndelim ); i++ );
  buffer_append( buf, data, i );
  return i;
}

size_t buffer_read_escaped( buffer_t* buf, char c,
                            const char* data,  size_t len,
                            const char* delim, size_t ndelim,
                            const char* esc,   size_t nesc ) {
  size_t i, j;
  for( i=j=0; (i<len) && !char_in_set( data[i], delim, ndelim ); i++ ) {
    if( data[i] == c && char_in_set(data[i+1], esc, nesc ) ) {
      buffer_append( buf, data+j, i-j );
      j = ++i;
    }
  }
  buffer_append( buf, data+j, i-j );
  return i;
}

size_t buffer_ltrim( buffer_t* buf, const char* trim, size_t ntrim ) {
  size_t i;
  for( i=0; i < buf->length && char_in_set( buf->data[i], trim, ntrim ); i++ );
  buffer_remove( buf, 0, i );
  return i;
}

size_t buffer_rtrim( buffer_t* buf, const char* trim, size_t ntrim ) {
  size_t i;
  for( i=0; i < buf->length && char_in_set( buf->data[buf->length-i-1], trim, ntrim ); i++ );
  buffer_remove( buf, buf->length-i, i );
  return i;
}

size_t buffer_trim( buffer_t* buf, const char* trim, size_t ntrim ) {
  return buffer_ltrim( buf, trim, ntrim ) + buffer_rtrim( buf, trim, ntrim );
}

int buffer_cmp( const buffer_t* a, const buffer_t* b ) {
  assert( a && b && "both buffers shoud be non-NULL");
  if( a->data == b->data ) return 0;
  if( a->length == b->length )
    return memcmp( a->data, b->data, a->length );
  return (int)a->length - (int)b->length;
}