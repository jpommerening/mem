#include "array.h"
#include "test.h"

struct int_array_s array_s(int);
typedef struct int_array_s int_array_t;

TEST( test_array_init_destroy ) {
  int_array_t arr;
  array_init( &arr, 16 );
  ASSERTGE( array_avail(&arr), 16 );
  array_destroy( &arr );
  /* todo: mock free & assure it was called */
}

TEST( test_array_static ) {
  static int data[] = { 1, 2, 3 };
  int_array_t arr = ARRAY_INITARR( data );
  
  ASSERTEQ( arr.length, 3 );
  ASSERTEQ( arr.data[0], 1 );
  ASSERTEQ( arr.data[1], 2 );
  ASSERTEQ( arr.data[2], 3 );
  ASSERTEQ( array_avail(&arr), 0 );
  
  array_grow(&arr, 5);
  
  ASSERTEQ( arr.length, 3 );
  ASSERTEQ( arr.data[0], 1 );
  ASSERTEQ( arr.data[1], 2 );
  ASSERTEQ( arr.data[2], 3 );
  ASSERTGE( array_avail(&arr), 2 );
  
  arr.data[3] = 4;
  arr.data[4] = 5;
  arr.length  = 5;
  ASSERTGE( array_avail(&arr), 0 );
  
  array_destroy(&arr);
}
