#include "alloc.h"
#include "jemalloc.h"

#define REALLOC_SIZE(s) (((s)+16)/8*13)

void * grow( void * mem, size_t len, size_t * alloc ) {
  if( len <= *alloc )
    return mem;
  
  *alloc = je_malloc_usable_size_in_advance(REALLOC_SIZE(len));
  return je_realloc( mem, *alloc );
}
