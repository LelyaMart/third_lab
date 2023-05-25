#include <stdio.h>
#include <pthread.h>
#include "lib/mystdlib.h"

#define NUM_BLOCKS 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct memory_block {
    size_t size;
    int* ptr;
} memory_block;

typedef struct some_args {
    int id;
    memory_block* blocks;
    int num_threads;
} some_args;


void* allocate_memory(void* args)
{
    some_args* arg = (some_args*) args;
    memory_block *blocks = arg->blocks;
    int id = arg->id;
    size_t sizes[NUM_BLOCKS] = {16, 1024, 1024*1024};
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        blocks[i + NUM_BLOCKS * id].size = sizes[i];
        blocks[i + NUM_BLOCKS * id].ptr = malloc(sizes[i]);
        printf("%p\n", blocks[i + NUM_BLOCKS * id].ptr);
    }
    return NULL;
}

void* fill_blocks(void *args)
{
    some_args* arg = (some_args*) args;
    memory_block *blocks = arg->blocks;
    int id = arg->id;
    int num_threads = arg->num_threads;

    int n = id % (num_threads / NUM_BLOCKS);
    for (int i = 0; i < NUM_BLOCKS; i++) {
        int* ptr = blocks[i + NUM_BLOCKS * n].ptr;
        *ptr = id + 1;
    }
    return NULL;
}


void* save_blocks(void *args)
{
    some_args* arg = (some_args*) args;
    memory_block *blocks = arg->blocks;
    int id = arg->id;
    int num_threads = arg->num_threads;

    int n = id % (num_threads / NUM_BLOCKS);
    FILE *fp = fopen("output.txt", "a");
    if (fp == NULL) {
        printf("Opening error");
    }
   
    for (int i = 0; i < NUM_BLOCKS; i++) {
        pthread_mutex_lock(&mutex);
        fprintf(fp, "%d\n", *blocks[i + NUM_BLOCKS * n].ptr);
        pthread_mutex_unlock(&mutex);
        free(blocks[i + NUM_BLOCKS * n].ptr);
    }
    
    fclose(fp);
    return 0;
}


int main()
{
    int m, num_threads;
    printf("Введите количество потоков: ");
    scanf("%d", &num_threads);
    printf("Введите количество повторений: ");
    scanf("%d", &m);

    memory_block* blocks = (memory_block*) malloc(sizeof(memory_block) * num_threads);    
    
    some_args args[num_threads];

    for (int i = 0; i < num_threads; i++)
    {
        args[i].blocks = blocks;
        args[i].num_threads = num_threads;
        args[i].id = i;
    }

    pthread_t threads[num_threads];
    int status;
    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Opening error");
    }
    fclose(file);


    for (int j = 0; j < m; j++)
    {
        for (int i = 0; i < num_threads / 3; i++) {
            status = pthread_create(&threads[i], NULL, (void *) allocate_memory, (void *) &args[i]);

            if (status != 0) {
                printf("main error: can't create thread, status = %d\n", status);
                return -1;
            }
        }

        for (int i = 0; i < num_threads / 3; i++) {
            status = pthread_join(threads[i], NULL);

            if (status != 0) {
                printf("main error: can't join thread, status = %d\n", status);
                return -1;
            }
        }

        for (int i = num_threads / 3; i < num_threads / 3 * 2; i++) {
            status = pthread_create(&threads[i], NULL, (void *) fill_blocks, (void *) &args[i]);

            if (status != 0) {
                printf("main error: can't create thread, status = %d\n", status);
                return -1;
            }
        }

        for (int i = num_threads / 3; i < num_threads / 3 * 2; i++) {
            status = pthread_join(threads[i], NULL);

            if (status != 0) {
                printf("main error: can't join thread, status = %d\n", status);
                return -1;
            }
        }

        for (int i = num_threads / 3 * 2; i < (num_threads / 3) * 3; i++) {
            status = pthread_create(&threads[i], NULL, (void *) save_blocks, (void *) &args[i]);

            if (status != 0) {
                printf("main error: can't create thread, status = %d\n", status);
                return -1;
            }
        }

        for (int i = num_threads / 3 * 2; i < num_threads / 3 * 3; i++) {
            status = pthread_join(threads[i], NULL);

            if (status != 0) {
                printf("main error: can't join thread, status = %d\n", status);
                return -1;
            }
        }
    }

    free(blocks);

    return 0;
}