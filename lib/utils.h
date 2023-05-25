#ifndef UTILS_H
#define UTILS_H

#define BLOCK_SIZE 40

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

typedef struct block *block;

struct block
{
    size_t size;
    block prev;
    block next;
    int free;
    int padding;
    void *ptr;
    char data[1];
};

block find_block(block *last, size_t size, void *first_block);
void split_block(block b, size_t s);
block extend_heap(block last, size_t s);
size_t align8(size_t s);
block get_block(void *p);
int valid_addr(void *p, void *first_block);
block fusion(block b);
void copy_block(block src, block dst);


#endif /* UTILS_H */
