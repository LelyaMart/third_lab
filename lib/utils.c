#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "utils.h"

// void *first_block = NULL;

block find_block(block *last, size_t size, void *first_block)
{

    block b = (block)first_block;

    while (b && !(b->free && b->size >= size))
    {
        *last = b;
        b = b->next;
    }
    return b;
}

void split_block(block b, size_t s)
{

    block new;

    new->data[1] = b->data[1] + s;
    new->size = b->size - s - BLOCK_SIZE;
    new->next = b->next;
    new->free = 1;

    b->size = s;
    b->next = new;
}

block extend_heap(block last, size_t s)
{

    block b;
    b = sbrk(0);
    if (sbrk(BLOCK_SIZE + s) == (void *)-1)
    {
        return NULL;
    }

    b->size = s;
    b->next = NULL;

    if (last)
    {
        last->next = b;
    }

    b->free = 0;
    return b;
}

size_t align8(size_t s)
{

    if (s & 0x7 == 0)
    {
        return s;
    }

    return ((s >> 3) + 1) << 3;
}

block get_block(void *p)
{
    char *tmp;
    tmp = p;
    p = tmp -= BLOCK_SIZE;
    return p;
}

int valid_addr(void *p, void *first_block)
{
    // block first = (block)first_block;

    if (first_block)
    {
        if (p > first_block && p < sbrk(0))
        {
            return p == get_block(p)->ptr;
        }
    }
    return 0;
}

block fusion(block b)
{
    if (b->next && b->next->free)
    {
        b->size += BLOCK_SIZE + b->next->size;
        b->next = b->next->next;
        if (b->next)
        {
            b->next->prev = b;
        }
    }
    return b;
}

void copy_block(block src, block dst)
{
    size_t *sdata, *ddata;
    size_t i;
    sdata = src->ptr;
    ddata = dst->ptr;
    for (i = 0; (i * 8) < src->size && (i * 8) < dst->size; i++)
    {
        ddata[i] = sdata[i];
    }
}
