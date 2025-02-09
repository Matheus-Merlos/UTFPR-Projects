#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <signal.h>

#define BUFFER_SIZE 5

typedef struct {
    int buffer[BUFFER_SIZE];
    int itemsInBuffer;
} Buffer;

void insertItem(Buffer* buffer, int item) {
    if(buffer->itemsInBuffer + 1 > BUFFER_SIZE) {
        printf("Stack overflow.");
        exit(-1);
    }

    buffer->buffer[buffer->itemsInBuffer] = item;
    buffer->itemsInBuffer++;
}

int pop(Buffer* buffer) {
    int item = buffer->buffer[buffer->itemsInBuffer - 1];
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

int main(void) {
    for(int i = 0; i < BUFFER_SIZE; i++) {
        insertItem(buffer, rand() % 10);
    }
    
    for(int i = 0; i < BUFFER_SIZE; i++) {
        printBuffer(buffer);
        printf("%d\n", pop(buffer));
    }

    return 0;
}



void sigint_handler(int signum) {
    (void)signum;
    free(buffer);
    exit(0);
}

__attribute__((constructor))
void before_main() {
    srand(time(NULL));

    signal(SIGINT, sigint_handler);
    signal(0, sigint_handler);

    buffer = (Buffer*)malloc(sizeof(Buffer));
    if(buffer == NULL) {
        printf("Erro ao alocar memoria para o buffer");
        exit(-1);
    }

    buffer->itemsInBuffer = 0;
}