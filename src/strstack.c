#include "strstack.h"

#include <string.h>

#define assert_inside_buffer(b,p) assert( (p) >= (b).data && (p) <= (b).data + (b).length )


void strstack_init( strstack_t* stack ) {
  buffer_init( &(stack->buf), 8 );
  buffer_setlen( &(stack->buf), 1 );
  stack->top = &(stack->buf.data[0]);
}

void strstack_destroy( strstack_t* stack ) {
  buffer_destroy( &(stack->buf) );
}

void strstack_push( strstack_t* stack, const char* data, size_t len ) {
  size_t offset = stack->buf.length;

  /* buffer_append() may realloc; set stack->top after that. */
  if( len )
    buffer_append( &(stack->buf), data, len );
  buffer_appendc( &(stack->buf), '\0' );
  stack->top = &(stack->buf.data[offset]);

  assert_inside_buffer( stack->buf, stack->top );
}

const char* strstack_pop( strstack_t* stack, size_t* len ) {
  size_t offset   = stack->top - stack->buf.data;
  size_t strlen   = stack->buf.length - offset - 1;
  const char* top = stack->top;
  
  assert_inside_buffer( stack->buf, stack->top );
  assert( offset > 0 );
  
  /* Avoiding buffer_setlen() we can return the previous top of the stack. */
  stack->buf.length = offset;
  
  /* Now, seek back to the previous entry. */
  do {
    offset -= 1;
  } while( offset > 0 && stack->buf.data[offset-1] != '\0' );
  stack->top = &(stack->buf.data[offset]);
  
  assert_inside_buffer( stack->buf, stack->top );

  if( len )
    *len = strlen;
  return top;
}

void strstack_pushstr( strstack_t* stack, const char* str ) {
  strstack_push( stack, str, strlen(str) );
}

const char* strstack_popstr( strstack_t* stack ) {
  return strstack_pop( stack, NULL );
}

void strstack_pushvfmt( strstack_t* stack, const char* fmt, va_list vargs ) {
  buffer_t tmp = BUFFER_INIT;
  buffer_vfmt( &tmp, fmt, vargs );
  strstack_push( stack, tmp.data, tmp.length );
  buffer_destroy( &tmp );
}

void strstack_pushfmt( strstack_t* stack, const char* fmt, ... ) {
  va_list vargs;
  va_start(vargs, fmt);
  strstack_pushvfmt( stack, fmt, vargs );
  va_end(vargs);
}
