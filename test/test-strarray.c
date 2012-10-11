#include "test.h"
#include "strarray.h"

TEST( test_strarray_init_destroy ) {
  strarray_t strarr;
  strarray_init( &strarr );
  strarray_destroy( &strarr );
  /* todo: mock free & assure it was called */
}

TEST( test_strarray_init_static ) {
  strarray_t strarr = STRARRAY_INIT;
  strarray_destroy( &strarr );
  /* todo: mock free & assure it was called */
}

TEST( test_strarray_push_pop ) {
  strarray_t strarr;
  const char* str;
  
  strarray_init( &strarr );
  
  strarray_pushstr( &strarr, "Test" );
  ASSERTEQ( strarr.stack.top, strarr.arr.data[0] );
  ASSERTSTREQ( strarr.arr.data[0], "Test" );
  ASSERTEQ( strarr.arr.length, 1 );
  
  str = strarray_popstr( &strarr );
  ASSERTSTREQ( str, "Test" );
  ASSERTEQ( strarr.arr.length, 0 );
  
  strarray_destroy( &strarr );
}

TEST( test_strarray_realloc ) {
  strarray_t strarr = STRARRAY_INIT;
  int i, max = 32;
  const char* str;
  static const char* strings[32] = {
    "Test 00", "Test 01", "Test 02", "Test 03",
    "Test 04", "Test 05", "Test 06", "Test 07",
    "Test 08", "Test 09", "Test 0a", "Test 0b",
    "Test 0c", "Test 0d", "Test 0e", "Test 0f",
    
    "Test 10", "Test 11", "Test 12", "Test 13",
    "Test 14", "Test 15", "Test 16", "Test 17",
    "Test 18", "Test 19", "Test 1a", "Test 1b",
    "Test 1c", "Test 1d", "Test 1e", "Test 1f",
  };

  /* Trigger reallocation of the internal buffer .. */
  for( i=0; i<max; i++ ) {
    strarray_pushstr( &strarr, strings[i] );
  }
  
  ASSERTEQ( strarr.arr.length, 32 );
  
  /* Check if the array pointers survived reallocation. */
  for( i=0; i<max; i++ ) {
    ASSERTSTREQ( strarr.arr.data[i], strings[i] );
  }
  
  for( i=max-1; i>=0; i-- ) {
    str = strarray_popstr( &strarr );
    ASSERTSTREQ( str, strings[i] );
    ASSERTEQ( strarr.arr.length, i );
  }
  
  strarray_destroy( &strarr );
}

TEST( test_strarray_fmt ) {
  strarray_t strarr = STRARRAY_INIT;
  size_t len;
  const char* str;
  
  strarray_pushfmt( &strarr, "%s %i", "Test", 0 );
  ASSERTEQ( strarr.arr.length, 1 );
  ASSERTSTREQ( strarr.arr.data[0], "Test 0" );
  
  str = strarray_pop( &strarr, &len );
  ASSERTSTREQ( str, "Test 0" );
  ASSERTEQ( len, 6 );
  ASSERTEQ( strarr.arr.length, 0 );
  
  strarray_destroy( &strarr );
}