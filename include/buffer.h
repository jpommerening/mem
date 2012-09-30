#ifndef cake_buffer_h
#define cake_buffer_h
#include <mem.h>
#include <stdarg.h>
#include <assert.h>

extern byte buffer_initial[];

typedef struct buffer * buffer_t;

struct buffer {
  size_t alloc;
  size_t length;
  byte * data;
};

#define BUFFER_INIT { 0, 0, buffer_initial }
#define BUFFER_INITSTR(s) { 0, sizeof(s "")-1, s "" } 

#define BUFFER_WHITESPACE  " \t\n\r"
#define BUFFER_NWHITESPACE (sizeof(BUFFER_WHITESPACE "")-1)

extern void buffer_init( buffer_t, size_t );
extern void buffer_destroy( buffer_t );
extern byte * buffer_detach( buffer_t, size_t * );
extern void buffer_attach( buffer_t, byte *, size_t, size_t );
static inline void buffer_swap( buffer_t a, buffer_t b ) {
  struct buffer tmp = *a;
  *a = *b;
  *b = tmp;
}

static inline size_t buffer_avail( const buffer_t buf ) {
  return buf->alloc ? buf->alloc - buf->length - 1 : 0;
}

static inline bool buffer_empty( const buffer_t buf ) {
  return (buf->length == 0);
}

extern void buffer_grow( buffer_t, size_t );

static inline void buffer_setlen( buffer_t buf, size_t len ) {
  assert( len <= (buf->alloc ? buf->alloc - 1 : 0) && "buffer_setlen() beyond buffer" );
  buf->length    = len;
  buf->data[len] = '\0';
}
#define buffer_reset(sb) buffer_setlen(sb, 0)

void buffer_splice( buffer_t buf, size_t pos, size_t rem, const byte * data, size_t len );
void buffer_remove( buffer_t buf, size_t pos, size_t len );
void buffer_insert( buffer_t buf, size_t pos, const byte * data, size_t len );
void buffer_append( buffer_t buf, const byte * data, size_t len );

void buffer_insertbuf( buffer_t buf, size_t pos, const buffer_t b );
void buffer_appendbuf( buffer_t buf, const buffer_t b );

void buffer_str( buffer_t buf, const char * str );
void buffer_insertstr( buffer_t buf, size_t pos, const char * str );
void buffer_appendstr( buffer_t buf, const char * str );

void buffer_vfmt( buffer_t buf, const char * fmt, va_list vargs );
void buffer_appendvfmt( buffer_t buf, const char * fmt, va_list vargs );
void buffer_insertvfmt( buffer_t buf, size_t pos, const char * fmt, va_list vargs );

void buffer_fmt( buffer_t buf, const char * fmt, ... );
void buffer_insertfmt( buffer_t buf, size_t pos, const char * fmt, ... );
void buffer_appendfmt( buffer_t buf, const char * fmt, ... );

size_t buffer_read( buffer_t buf,
                    const byte * data,  size_t len,
                    const byte * delim, size_t ndelim );

size_t buffer_read_escaped( buffer_t buf, byte c,
                            const byte * data,  size_t len,
                            const byte * delim, size_t ndelim,
                            const byte * esc,   size_t nesc );

size_t buffer_rtrim( buffer_t buf, const byte * trim, size_t ntrim );
size_t buffer_ltrim( buffer_t buf, const byte * trim, size_t ntrim );
size_t buffer_trim( buffer_t buf, const byte * trim, size_t ntrim );

static inline size_t buffer_rtrimw( buffer_t buf ) {
  return buffer_rtrim( buf, BUFFER_WHITESPACE, BUFFER_NWHITESPACE );
}

static inline size_t buffer_ltrimw( buffer_t buf ) {
  return buffer_ltrim( buf, BUFFER_WHITESPACE, BUFFER_NWHITESPACE );
}

static inline size_t buffer_trimw( buffer_t buf ) {
  return buffer_trim( buf, BUFFER_WHITESPACE, BUFFER_NWHITESPACE );
}
int buffer_cmp( const buffer_t, const buffer_t );

#endif
