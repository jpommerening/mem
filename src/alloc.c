#include "alloc.h"
#include "jemalloc.h"

#define REALLOC_SIZE(s) (((s)+16)/8*13)

void * grow( void * mem, size_t len, size_t * alloc ) {
  if( len <= *alloc )
    return mem;
  
  len = REALLOC_SIZE(len);
  if( *alloc == 0 )
    mem = je_malloc( len );
  else
    mem = je_realloc( mem, len );

  if( mem )
    *alloc = je_malloc_usable_size(mem);
  else
    *alloc = 0;
  
  return mem;
}
