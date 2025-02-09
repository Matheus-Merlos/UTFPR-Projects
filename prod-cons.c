#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 16

#define NUM_CONSUMERS 7
#define NUM_PRODUCERS 4


/*
Funções e declarações do Buffer
*/
//-----------------------------------------------------------
typedef struct {
    int buffer[BUFFER_SIZE];
    int itemsInBuffer;
} Buffer;

void insertItem(Buffer* buffer, int item) {
    if(buffer->itemsInBuffer + 1 > BUFFER_SIZE) {
        printf("Stack overflow.");
        exit(1);
    }

    buffer->buffer[buffer->itemsInBuffer] = item;
    buffer->itemsInBuffer++;
}
 
int pop(Buffer* buffer) {
    if (buffer->itemsInBuffer == 0) {
        printf("Underflow.\n");
        exit(1);
    }
    int item = buffer->buffer[0];
    for(int i = 1; i < buffer->itemsInBuffer; i++) {
        buffer->buffer[i - 1] = buffer->buffer[i];
    }
    buffer->itemsInBuffer--;
    return item;
}

void printBuffer(Buffer* buffer) {
    printf("[");
    for(int i = 0; i < buffer->itemsInBuffer; i++) {
        printf("%d, ", buffer->buffer[i]);
    }
    printf("]\n");
}

Buffer* buffer;

/*
Semáforos
*/
//-----------------------------------------------------------
sem_t full;
sem_t empty;
pthread_mutex_t mutex;

/*
Produtores e consumidores
*/
//-----------------------------------------------------------

void *producer(void *arg) {
    int numProducer = *(int*)arg;
    while(1) {
        int item = rand() % 10;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        printf("Produtor %d produziu o item %d\n", numProducer, item);

        insertItem(buffer, item);

        printBuffer(buffer);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}

void *consumer(void *arg) {
    int numConsumer = *(int*)arg;
    while(1) {
        sleep(1);
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = pop(buffer);

        printf("Consumidor %d consumiu o item %d\n", numConsumer, item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

/*
MAIN
*/
//-----------------------------------------------------------
int main(void) {
    pthread_t consumers[NUM_CONSUMERS];
    pthread_t producers[NUM_PRODUCERS];

    int numProducers[NUM_PRODUCERS];
    int numConsumers[NUM_CONSUMERS];

    for(int i = 0; i < NUM_CONSUMERS; i++) {
        numConsumers[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &numConsumers[i]);
    }

    for(int i = 0; i < NUM_PRODUCERS; i++) {
        numProducers[i] = i;
        pthread_create(&producers[i], NULL, producer, &numProducers[i]);
    }

    for(int i = 0; i < NUM_CONSUMERS; i++) pthread_join(consumers[i], NULL);
    for(int i = 0; i < NUM_PRODUCERS; i++) pthread_join(producers[i], NULL);

    return 0;
}

/*
Funções de gerenciamento
A before_main serve para incializar o buffer e o mutex corretamente antes da main executar
E a sigint_handler serve para dar free no buffer e no mutex alocado anteriormente quando o programa recebe um sinal de parada (SIGINT)
ou o programa conclui sua execução normal (sinal 0)
*/
//-----------------------------------------------------------
void cleanup() {
    free(buffer);
    pthread_mutex_destroy(&mutex);

    sem_destroy(&full);
    sem_destroy(&empty);
    printf("\nRecursos liberados...\n");
}

__attribute__((constructor))
void before_main() {
    srand(time(NULL));

    atexit(cleanup);

    buffer = (Buffer*)malloc(sizeof(Buffer));
    if(buffer == NULL) {
        printf("Erro ao alocar memoria para o buffer");
        exit(1);
    }

    buffer->itemsInBuffer = 0;

    pthread_mutex_init(&mutex, NULL);

    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
}