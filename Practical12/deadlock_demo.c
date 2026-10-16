#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t B = PTHREAD_MUTEX_INITIALIZER;
pthread_barrier_t barrier;

void *thread1(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&A);
    printf("Thread 1 locked Resource A\n");
    fflush(stdout);

    pthread_barrier_wait(&barrier);

    printf("Thread 1 waiting for Resource B\n");
    fflush(stdout);
    pthread_mutex_lock(&B);

    printf("Thread 1 acquired both resources\n");

    pthread_mutex_unlock(&B);
    pthread_mutex_unlock(&A);

    return NULL;
}

void *thread2(void *arg)
{
    (void)arg;

    pthread_mutex_lock(&B);
    printf("Thread 2 locked Resource B\n");
    fflush(stdout);

    pthread_barrier_wait(&barrier);

    printf("Thread 2 waiting for Resource A\n");
    fflush(stdout);
    pthread_mutex_lock(&A);

    printf("Thread 2 acquired both resources\n");

    pthread_mutex_unlock(&A);
    pthread_mutex_unlock(&B);

    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    setvbuf(stdout, NULL, _IONBF, 0);

    pthread_barrier_init(&barrier, NULL, 2);

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_barrier_destroy(&barrier);
    pthread_mutex_destroy(&A);
    pthread_mutex_destroy(&B);

    return 0;
}
