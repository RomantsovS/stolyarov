#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

#define ARRAY_SIZE 10
#define WORKERS_COUNT 3
#define PAUSE_LENGTH 1

#ifndef PROTECTION
#define PROTECTION 1
#endif

struct thread_start_data {
    unsigned int randseed;
    int *array;
#if PROTECTION == 1
    pthread_mutex_t *arr_mutex;
    sem_t *data_sem;
#endif
};

static void *worker_thread(void *v_data)
{
    thread_start_data *data = (thread_start_data *)v_data;
    unsigned int randseed = data->randseed;
    int *array = data->array;
#if PROTECTION == 1
    pthread_mutex_t *arr_mutex = data->arr_mutex;
    sem_t *data_sem = data->data_sem;
    sem_post(data_sem);
#endif

    while (1)
    {
        int idx1, idx2;
        idx1 = rand_r(&randseed) % ARRAY_SIZE;
        idx2 = rand_r(&randseed) % ARRAY_SIZE;
        if (idx1 == idx2)
            continue;

#if PROTECTION == 1
        pthread_mutex_lock(arr_mutex);
#endif

        array[idx1]++;
        array[idx2]--;

#if PROTECTION == 1
        pthread_mutex_unlock(arr_mutex);
#endif
        sleep(1);
    }

    return NULL;
}

int main() {
    int i;
    int the_array[ARRAY_SIZE];
    thread_start_data tsdata;
    pthread_t thr;

#if PROTECTION == 1
    printf("launched protected\n");
    pthread_mutex_t arr_mutex = PTHREAD_MUTEX_INITIALIZER;
    sem_t tsd_sem;
#else
    printf("launched unprotected\n");
#endif

    for(i = 0; i < ARRAY_SIZE; ++i) {
        the_array[i] = 0;
    }

    tsdata.randseed = time(NULL);
    tsdata.array = the_array;

#if PROTECTION == 1
    tsdata.arr_mutex = &arr_mutex;
    tsdata.data_sem = &tsd_sem;
    sem_init(&tsd_sem, 0, 0);
#endif

    for(i = 0; i < WORKERS_COUNT; ++i) {
        tsdata.randseed++;
        pthread_create(&thr, NULL, worker_thread, &tsdata);
#if PROTECTION == 1
    sem_wait(&tsd_sem);
#endif
    }

    while(1) {
        int sum;
#if PROTECTION == 1
        pthread_mutex_lock(&arr_mutex);
#endif
        sum = 0;
        for(i = 0; i < ARRAY_SIZE; ++i) {
            sum += the_array[i];
        }
        printf("%d", sum);
        for(i = 0; i < ARRAY_SIZE; ++i) {
            printf("%c%4d", i ? ',' : '(', the_array[i]);
        }
        printf(")\n");
#if PROTECTION == 1
        pthread_mutex_unlock(&arr_mutex);
#endif

        sleep(PAUSE_LENGTH);
    }
    
    printf("done\n");

    return 0;
}