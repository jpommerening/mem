#include "strarray.h"

void strarray_init( strarray_t* strarr ) {
  strstack_init( &(strarr->stack) );
  array_init( &(strarr->arr), 1 );
}

void strarray_destroy( strarray_t* strarr ) {
  array_destroy( &(strarr->arr) );
  strstack_destroy( &(strarr->stack) );
}

static void strarray__rebuild( strarray_t* strarr ) {
  ptrdiff_t offset;
  size_t    i;
  
  if( strarr->arr.data == NULL ) return;
  
  /* strstack data starts with '\0', the first entry comes after that. */
  offset = strarr->arr.data[0] - (strarr->stack.buf.data + 1);
  if( offset == 0 ) return;
  
  for( i = 0; i < strarr->arr.length; i++ ) {
    strarr->arr.data[i] -= offset;
    assert( strarr->arr.data[i] > strarr->stack.buf.data );
    assert( strarr->arr.data[i] < strarr->stack.buf.data + strarr->stack.buf.length );
  }
}

void strarray_push( strarray_t* strarr, const char* str ) {
  strstack_pushstr( &(strarr->stack), str );
  strarray__rebuild( strarr ); /* realloc may have moved stack */
  
  array_push( &(strarr->arr), strarr->stack.top );
}

const char* strarray_pop( strarray_t* strarr ) {
  const char* str = strstack_popstr( &(strarr->stack) );
  array_pop( &(strarr->arr) );
  return str;
}
