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

void strstack_push( strstack_t* stack, const char* str ) {
  size_t offset = stack->buf.length;

  /* buffer_append() may realloc; set stack->top after that. */
  buffer_append( &(stack->buf), str, strlen( str ) + 1 );
  stack->top = &(stack->buf.data[offset]);

  assert_inside_buffer( stack->buf, stack->top );
}

const char* strstack_pop( strstack_t* stack ) {
  size_t offset   = stack->top - stack->buf.data;
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

  return top;
}