#include "buffer.h"
#include "test.h"

TEST( test_buffer_init_destroy ) {
  buffer_t buf;
  buffer_init( &buf, 16 );
  ASSERTSTREQ(buf.data, "");
  
  buffer_destroy( &buf );
}

TEST( test_buffer_attach_detach ) {
  buffer_t buf = BUFFER_INIT;
  char     str1[16] = "test";
  char*    str2 = NULL;
  size_t   len = 0;
  
  buffer_attach( &buf, str1, 4, 0 /* because it is not alloc'd */ );
  
  ASSERTSTREQ( buf.data, "test" );
  ASSERTEQ( buf.data, str1 );
  ASSERTEQ( buf.length, 4 );
  ASSERTEQ( buf.alloc, 0 );
    
  str2 = buffer_detach( &buf, &len );
  ASSERTSTREQ( buf.data, "" );
  ASSERTSTREQ( str1, str2 );
  ASSERTEQ( len, 4 );
}