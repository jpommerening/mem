#include "buffer.h"
#include "test.h"

TEST( test_buffer_init_destroy ) {
  buffer_t buf;
  buffer_init( &buf, 16 );
  ASSERTSTREQ( buf.data, "" );
  
  buffer_destroy( &buf );
  /* todo: mock free & assure it was called */
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
  
  buffer_destroy( &buf );
}

TEST( test_buffer_append ) {
  buffer_t buf = BUFFER_INITSTR( "Hello" );
  buffer_t bob = BUFFER_INITSTR( "Hello Bob!");

  buffer_append( &buf, " world!", 7 );
  ASSERTSTREQ( buf.data, "Hello world!" );
  
  buffer_appendstr( &buf, "\nHello buffer!\n" );
  ASSERTSTREQ( buf.data, "Hello world!\nHello buffer!\n" );

  buffer_appendbuf( &buf, &bob );
  ASSERTSTREQ( buf.data, "Hello world!\nHello buffer!\nHello Bob!" );
  
  /* Don't care for self assignment for now.
     We would have to track ANY pointer given to buffer_splice(),
     check if it is completely inside the buffers bounds AND whether it
     would be spliced during the operation. */
  
  buffer_destroy( &buf );
  buffer_destroy( &bob );
}

TEST( test_buffer_insert ) {
  buffer_t buf = BUFFER_INITSTR( "Horld!\n" );
  buffer_t bob = BUFFER_INITSTR( "Hello Bob!");
  
  buffer_insert( &buf, 1, "ello w", 6 );
  ASSERTSTREQ( buf.data, "Hello world!\n" );

  buffer_insertstr( &buf, 0, "Hello buffer!\n");
  ASSERTSTREQ( buf.data, "Hello buffer!\nHello world!\n" );
  
  buffer_insertbuf( &buf, buf.length, &bob );
  ASSERTSTREQ( buf.data, "Hello buffer!\nHello world!\nHello Bob!" );

  buffer_destroy( &buf );
  buffer_destroy( &bob );
}

TEST( test_buffer_remove ) {
  buffer_t buf = BUFFER_INITSTR("Hello world!");
  
  buffer_remove( &buf, 5, 6 );
  ASSERTSTREQ( buf.data, "Hello!" );
  
  buffer_destroy( &buf );
}