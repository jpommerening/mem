#include "strstack.h"
#include "test.h"

struct test_strstack_s {
  size_t items;
  const char* seq[10];
};

static const struct test_strstack_s _data[] = {
  { 1, { "test" } },
  { 2, { "one", "two" } },
  { 3, { "", "two", "three" } },
  { 3, { "one", "", "three" } },
  { 3, { "one", "two", "" } },
};

TEST( test_strstack_linear, _data, const struct test_strstack_s* data ) {
  strstack_t  stack = STRSTACK_INIT;
  const char* str   = "";
  int i;
  
  for( i = 0; i < data->items; i++ ) {
    ASSERTNE( stack.top, NULL, "before push %i: %s, top: %s", i, data->seq[i], stack.top );
    ASSERTSTREQ( stack.top, str, "str: %s, top: %s", str, stack.top );

    strstack_pushstr( &stack, data->seq[i] );
    ASSERTSTREQ( stack.top, data->seq[i], "after push %i: %s", i, data->seq[i] );
    str = data->seq[i];
  }
  
  for( i -= 1 ; i >= 0; i-- ) {
    ASSERTSTREQ( stack.top, data->seq[i], "before pop n-%i: %s, top: %s", i, data->seq[i], stack.top );
    str = strstack_popstr( &stack );
    ASSERTSTREQ( str, data->seq[i], "after pop n-%i: %s, str: %s", i, data->seq[i], str );
  }
  
  ASSERTNE( stack.top, NULL, "after test" );
  ASSERTSTREQ( stack.top, "", "after test" );

  strstack_destroy( &stack );
}

TEST( test_strstack_random ) {
  strstack_t  stack = STRSTACK_INIT;
  const char* str;
  
  /* already tested by test_strstack_linear */
  strstack_pushstr( &stack, "one" );
  strstack_pushstr( &stack, "two" );
  strstack_pushstr( &stack, "three" );
  str = strstack_popstr( &stack );
  
  ASSERTSTREQ( str, "three" );
  ASSERTSTREQ( stack.top, "two" );
  
  strstack_pushstr( &stack, "test" );
  ASSERTSTREQ( stack.top, "test" );
  
  str = strstack_popstr( &stack );
  ASSERTSTREQ( str, "test" );
  ASSERTSTREQ( stack.top, "two" );
  
  str = strstack_popstr( &stack );
  ASSERTSTREQ( str, "two" );
  ASSERTSTREQ( stack.top, "one" );
  
  strstack_destroy( &stack );
}

TEST( test_strstack_fmt ) {
  strstack_t  stack = STRSTACK_INIT;
  const char* str;
  size_t len;
  
  strstack_pushfmt( &stack, "%s %i, %i", "Test", 1, 2 );
  ASSERTSTREQ( stack.top, "Test 1, 2" );
  
  strstack_push( &stack, "Test 3, 4", 6 );
  ASSERTSTREQ( stack.top, "Test 3" );
  
  str = strstack_pop( &stack, &len );
  ASSERTSTREQ( str, "Test 3" );
  ASSERTEQ( len, 6 );
  
  str = strstack_pop( &stack, &len );
  ASSERTSTREQ( str, "Test 1, 2" );
  ASSERTEQ( len, 9 );
  
  strstack_destroy( &stack );
}