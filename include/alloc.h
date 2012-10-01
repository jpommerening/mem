#ifndef _ALLOC_H_
#define _ALLOC_H_
#include <stddef.h>

#include <stdlib.h>

void * grow( void * mem, size_t len, size_t * alloc );
  
#endif
