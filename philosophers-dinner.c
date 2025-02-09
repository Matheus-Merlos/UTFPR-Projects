#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include <stdlib.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t forks[NUM_PHILOSOPHERS];

void think(int philosopher) {
    printf("Philosopher %d is thinking...\n", philosopher + 1);
    sleep(2);
}

void eat(int philosopher) {
    printf("Philosopher %d is eating!\n", philosopher + 1);
    sleep(2);
}

void take_forks(int philosopher) {
    int left = philosopher;
    int right = (philosopher + 1) % NUM_PHILOSOPHERS;

    if(philosopher == 0) {
        pthread_mutex_lock(&forks[right]);
        pthread_mutex_lock(&forks[left]);
        return;
    }
    pthread_mutex_lock(&forks[left]);
    pthread_mutex_lock(&forks[right]);
}

void put_forks(int philosopher) {
    int left = philosopher;
    int right = (philosopher + 1) % NUM_PHILOSOPHERS;

    pthread_mutex_unlock(&forks[left]);
    pthread_mutex_unlock(&forks[right]);
}

void *philosopher(void *arg) {
    int num = *(int *)arg;
    while(1) {
        think(num);
        take_forks(num);
        eat(num);
        put_forks(num);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_PHILOSOPHERS];
    int philosopher_numbers[NUM_PHILOSOPHERS];

    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_numbers[i] = i;
        pthread_create(&threads[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void cleanup() {
    printf("\nExcluindo mutexes antes de sair...\n");
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
        printf("Mutex %d destruído.\n", i);
    }
    exit(0);
}

__attribute__((constructor))
void init_forks() {
    atexit(cleanup);
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
}