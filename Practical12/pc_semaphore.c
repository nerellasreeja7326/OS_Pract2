#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2
#define ITEMS_PER_PRODUCER 500000
#define STOP_ITEM -1

int buffer[BUFFER_SIZE];
int in = 0, out = 0;

long long produced = 0;
long long consumed = 0;

pthread_mutex_t mutex;
sem_t empty_slots;
sem_t full_slots;

void insert_item(int item)
{
    sem_wait(&empty_slots);
    pthread_mutex_lock(&mutex);

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;

    if (item != STOP_ITEM)
        produced++;

    pthread_mutex_unlock(&mutex);
    sem_post(&full_slots);
}

int remove_item(void)
{
    int item;

    sem_wait(&full_slots);
    pthread_mutex_lock(&mutex);

    item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;

    if (item != STOP_ITEM)
        consumed++;

    pthread_mutex_unlock(&mutex);
    sem_post(&empty_slots);

    return item;
}

void *producer(void *arg)
{
    long id = *(long *)arg;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++)
        insert_item((int)(id * ITEMS_PER_PRODUCER + i + 1));

    return NULL;
}

void *consumer(void *arg)
{
    (void)arg;

    while (1) {
        int item = remove_item();

        if (item == STOP_ITEM)
            break;
    }

    return NULL;
}

int main(void)
{
    pthread_t producers[NUM_PRODUCERS];
    pthread_t consumers[NUM_CONSUMERS];
    long ids[NUM_PRODUCERS];

    if (pthread_mutex_init(&mutex, NULL) != 0 ||
        sem_init(&empty_slots, 0, BUFFER_SIZE) != 0 ||
        sem_init(&full_slots, 0, 0) != 0) {
        fprintf(stderr, "Synchronization initialization failed.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, NULL) != 0) {
            fprintf(stderr, "Could not create consumer thread.\n");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        ids[i] = i;
        if (pthread_create(&producers[i], NULL, producer, &ids[i]) != 0) {
            fprintf(stderr, "Could not create producer thread.\n");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < NUM_PRODUCERS; i++)
        pthread_join(producers[i], NULL);

    for (int i = 0; i < NUM_CONSUMERS; i++)
        insert_item(STOP_ITEM);

    for (int i = 0; i < NUM_CONSUMERS; i++)
        pthread_join(consumers[i], NULL);

    printf("Producer-Consumer Problem using Semaphores\n");
    printf("Items produced: %lld\n", produced);
    printf("Items consumed: %lld\n", consumed);
    printf("Synchronization: %s\n",
           (produced == consumed) ? "CORRECT" : "ERROR");

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}
