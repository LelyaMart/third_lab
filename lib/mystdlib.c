#include "mystdlib.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *first_block = NULL;

void *malloc(size_t size)
{

    block b, last;
    size_t s;

    pthread_mutex_lock(&mutex);

    s = align8(size);

    if (first_block)
    {
        last = first_block;
        b = find_block(&last, s, first_block);

        if (b)
        {
            if ((b->size - s) >= (BLOCK_SIZE + 8))
            {
                split_block(b, s);
            }
            b->free = 0;
        }
        else
        {
            b = extend_heap(last, s);
            if (!b)
            {
                pthread_mutex_unlock(&mutex);
                return NULL;
            }
        }
    }
    else
    {
        b = extend_heap(NULL, s);
        if (!b)
        {
            pthread_mutex_unlock(&mutex);
            return NULL;
        }
        first_block = b;
    }

    pthread_mutex_unlock(&mutex);
    b->ptr = &(b->data);
    return b->data;
}

void free(void *p)
{
    block b;

    pthread_mutex_lock(&mutex);

    if (valid_addr(p, first_block))
    {
        b = get_block(p);
        b->free = 1;
        if (b->prev && b->prev->free)
        {
            b = fusion(b->prev);
        }
        if (b->next)
        {
            fusion(b);
        }
        else
        {
            if (b->prev)
            {
                b->prev->next = NULL;
            }
            else
            {
                first_block = NULL;
            }
            brk(b);
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *calloc(size_t number, size_t size)
{
    size_t *new;
    int i, j;
    new = malloc(number * size);
    if (new)
    {
        j = align8(number * size) >> 3;

        for (i = 0; i < j; i++)
        {
            new[i] = 0;
        }
    }

    return new;
}

void *realloc(void *p, size_t size)
{
    size_t s;
    block b, new;
    void *newp;

    if (!p)
    {
        return malloc(size);
    }
    if (valid_addr(p, first_block))
    {
        s = align8(size);
        b = get_block(p);
        if (b->size >= s)
        {
            if (b->size - s >= (BLOCK_SIZE + 8))
            {
                split_block(b, s);
            }
        }
        else
        {
            if (b->next && b->next->free && (b->size + BLOCK_SIZE + b->next->size) >= s)
            {
                fusion(b);
                if (b->size - s >= (BLOCK_SIZE + 8))
                {
                    split_block(b, s);
                }
            }
            else
            {
                newp = malloc(s);
                if (!newp)
                {
                    return NULL;
                }

                new = get_block(newp);
                copy_block(b, new);
                free(p);

                return (newp);
            }
        }
        return (p);
    }
    return NULL;
}

int main() {
    int *numbers = calloc(sizeof(int), 10);
    int count = 10;
    for (int i = 0; i < count; i++)
    {
        numbers[i] = i+1;
    }
    for (int i = 0; i < count; i++)
    {
        printf("%d\n", numbers[i]);
    }
    free(numbers);
}