#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

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

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Trabalho SO LRU");
    SetTargetFPS(2);

    int queue[QUEUE_SIZE];
    for(int i = 0; i < QUEUE_SIZE; i++) {
        queue[i] = randInt(NUM_BLOCKS);
    }

    int currentPosition = NUM_BLOCKS / 2;
    int timeSteps[TOTAL_TIME] = {0};
    int currentTimeStep = 0;
    double lastUpdateTime = GetTime();

    while (!WindowShouldClose()) {
        if ((GetTime() - lastUpdateTime) >= 1.0 && currentTimeStep < TOTAL_TIME) {
            int sortedQueue[QUEUE_SIZE];
            sortQueue(queue, sortedQueue);
            
            int nextLocation = sortedQueue[0];
            for(int j = 0; j < QUEUE_SIZE; j++) {
                if(sortedQueue[j] > currentPosition) {
                    nextLocation = sortedQueue[j];
                    break;
                }
            }

            while(1) {
                currentPosition++;
                if(currentPosition == nextLocation) {
                    generateAnotherRequest(queue, nextLocation);
                    break;
                }
                if(currentPosition >= NUM_BLOCKS) {
                    currentPosition = 0;
                }
            }

            timeSteps[currentTimeStep] = nextLocation;
            currentTimeStep++;
            lastUpdateTime = GetTime();
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawLine(50, 50, 50, WINDOW_HEIGHT - 50, BLACK);   // Eixo Y
        DrawLine(50, WINDOW_HEIGHT - 50, WINDOW_WIDTH - 50, WINDOW_HEIGHT - 50, BLACK); // Eixo X

        DrawText("Tempo", 10, WINDOW_HEIGHT - 30, 10, DARKGRAY);
        DrawText("Posicao do Disco", WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT - 30, 10, DARKGRAY);

        for (int i = 0; i < TOTAL_TIME; i += 5) {
            int y = WINDOW_HEIGHT - 50 - (i * (WINDOW_HEIGHT - 100)) / TOTAL_TIME;
            DrawText(TextFormat("%d", i), 20, y - 10, 10, DARKGRAY);
        }

        for (int i = 0; i < NUM_BLOCKS; i += 100) {
            int x = 50 + (i * (WINDOW_WIDTH - 100)) / NUM_BLOCKS;
            DrawText(TextFormat("%d", i), x, WINDOW_HEIGHT - 40, 10, DARKGRAY);
        }

        for (int i = 0; i < currentTimeStep; i++) {
            int xPos = 50 + (timeSteps[i] * (WINDOW_WIDTH - 100)) / NUM_BLOCKS;
            
            int yPos = WINDOW_HEIGHT - 50 - (i * (WINDOW_HEIGHT - 100)) / TOTAL_TIME;
            
            DrawCircle(xPos, yPos, 10, BLUE);
        }

        DrawText(TextFormat("Passo de tempo atual: %d/%d", currentTimeStep, TOTAL_TIME), 10, 10, 20, DARKBLUE);
        DrawText(TextFormat("Posicao atual: %d", currentPosition), 10, 35, 20, DARKBLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}