#include <stdio.h>
#include <pthread.h>

pthread_mutex_t A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t B = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg)
{
    int id = *(int *)arg;

    printf("Thread %d is waiting for Resource A\n", id);

    pthread_mutex_lock(&A);
    printf("Thread %d locked Resource A\n", id);

    pthread_mutex_lock(&B);
    printf("Thread %d locked Resource B\n", id);

    printf("Thread %d is using both resources\n", id);

    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);

    printf("Thread %d released both resources\n", id);

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    pthread_create(&t1, NULL, worker, &id1);
    pthread_create(&t2, NULL, worker, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&A);
    pthread_mutex_destroy(&B);

    printf("Program completed without deadlock.\n");

    return 0;
}
