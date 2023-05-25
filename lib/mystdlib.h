#ifndef MYSTDLIB_H
#define MYSTDLIB_H

#define BLOCK_SIZE 40

#include "utils.h"
#include <pthread.h>


void *malloc(size_t size);
void free(void *p);
void *calloc(size_t number, size_t size);
void *realloc(void *p, size_t size);



#endif /* MYSTDLIB_H */
