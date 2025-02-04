#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_BLOCKS 1000
#define QUEUE_SIZE 10
#define TOTAL_TIME 60

int randInt(int max) {
    int num = rand() % max + 1;
    return num - 1;
}

void printQueue(int* queue) {
    printf("[");
    for(int i = 0; i < QUEUE_SIZE; i++) {
        printf("%d, ", queue[i]);
    }
    printf("];\n");
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sortQueue(int* queue, int* finalArray) {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        finalArray[i] = queue[i];
    }

    for (int i = 0; i < QUEUE_SIZE - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < QUEUE_SIZE; j++) {
            if (finalArray[j] < finalArray[min_idx]) {
                min_idx = j;
            }
        }
        swap(&finalArray[min_idx], &finalArray[i]);
    }
}

void generateAnotherRequest(int* queue, int elementToSubstitute) {
    for(int i = 0; i < QUEUE_SIZE; i++) {
        if(queue[i] == elementToSubstitute) {
            queue[i] = randInt(NUM_BLOCKS);
        }
    }
}


int main(void) {
    srand(time(NULL));

    int queue[QUEUE_SIZE];
    for(int i = 0; i < QUEUE_SIZE; i++) {
        queue[i] = randInt(NUM_BLOCKS);
    }


    int currentPosition = (int)NUM_BLOCKS / 2; //Começa pela metade para ser melhot


    for(int i = 0; i < TOTAL_TIME; i++) {
        int sortedQueue[QUEUE_SIZE];
        sortQueue(queue, sortedQueue);
        
        int nextLocation = sortedQueue[0];
        for(int j = 0; j < QUEUE_SIZE; j++) {
            if(sortedQueue[j] > currentPosition) {
                nextLocation = sortedQueue[j];
                break;
            }
        }
        printQueue(sortedQueue);

        while(1) {
            currentPosition++;
            if(currentPosition == nextLocation) {
                printf("Elemento achado: %d\n", nextLocation);
                generateAnotherRequest(queue, nextLocation);
                break;
            }
            if(currentPosition == NUM_BLOCKS) {
                currentPosition = 0;
            }
        }
        sleep(1);
    }
    
    return 0;
}