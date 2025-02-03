#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <raylib.h>

#define TOTAL_MEM_PAGES 8
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

#define TOTAL_FRAMES_PER_SECOND 1
#define TOTAL_TIME 60

struct Page {
    int pageNumber;
    int useIndex;
};

struct Memory {
    int numEntries;
    struct Page** entries;
};

Color generatePageColor(int pageNumber) {
    return (Color){ 
        (unsigned char)(pageNumber * 50 % 255),
        (unsigned char)(pageNumber * 100 % 255),
        (unsigned char)(pageNumber * 150 % 255),
        255 
    };
}

void drawMemoryBars(struct Memory* mem) {
    const int barWidth = SCREEN_WIDTH / TOTAL_MEM_PAGES - 20;
    const int maxBarHeight = 400;

    for (int i = 0; i < mem->numEntries; i++) {
        int barHeight = mem->entries[i]->useIndex * 10;
        if (barHeight > maxBarHeight) barHeight = maxBarHeight;
        
        int x = 20 + i * (barWidth + 20);
        int y = SCREEN_HEIGHT - 50 - barHeight;

        Color barColor = generatePageColor(mem->entries[i]->pageNumber);

        DrawRectangle(x, y, barWidth, barHeight, barColor);
        
        DrawText(TextFormat("Página %d", mem->entries[i]->pageNumber), x, SCREEN_HEIGHT - 30, 20, BLACK);
        DrawText(TextFormat("Tempo: %d", mem->entries[i]->useIndex), x, y - 30, 20, BLACK);
    }
}

int randInt(int end) {
    int num = rand() % end + 1;
    return num - 1;
}

struct Page** generatePages(int numPages) {
    struct Page** pages = malloc(numPages * sizeof(struct Page*));
    for(int i = 1; i <= numPages; i++) {
        struct Page* page = malloc(sizeof(struct Page));
        page->pageNumber = i;
        page->useIndex = 0;
        pages[i - 1] = page;
    }

    return pages;
}

void printPages(struct Page** pages, int n) {
    printf("[");
    for(int i = 0; i < n; i++) {
        printf("%d, ", pages[i]->pageNumber);
    }
    printf("];\n");
}

bool isPageInMemory(struct Page* page, struct Memory* mem) {
    for(int i = 0; i < mem->numEntries; i++) {
        if(mem->entries[i]->pageNumber == page->pageNumber) {
            return true;
        }
    }
    return false;
}

int main(void) {
    srand(time(NULL));

    struct Page** pages = generatePages(TOTAL_MEM_PAGES * 2);
    struct Page** memoryPages = malloc(TOTAL_MEM_PAGES * sizeof(struct Page*));

    struct Memory* memory = (struct Memory*)malloc(sizeof(struct Memory));
    memory->entries = memoryPages;
    memory->numEntries = 0;

    int totalPageFaults = 0;
    int simulationCounter = TOTAL_FRAMES_PER_SECOND;
    bool simulationRunning = true;

    // Variável para armazenar a mensagem de substituição
    char substitutionMessage[50] = "";
    int substitutionMessageTimer = 0;

    int currentTime = 0;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Trabalho SO LRU");
    SetTargetFPS(TOTAL_FRAMES_PER_SECOND);

    while (!WindowShouldClose()) {
        if (simulationRunning && simulationCounter % TOTAL_FRAMES_PER_SECOND == 0) {
            // Escolhe uma página aleatória
            int pageNum = rand() % (TOTAL_MEM_PAGES * 2);
            struct Page* page = pages[pageNum];
            printf("Página escolhida: %d\n", page->pageNumber);

            // Atualiza o tempo de uso das páginas na memória
            for (int j = 0; j < memory->numEntries; j++) {
                memory->entries[j]->useIndex++;
            }

            // Verifica se a página já está na memória
            page->useIndex = 0;
            if (isPageInMemory(page, memory)) {
                printf("Página estava na memória. Sem Swap.\n");
            } else {
                printf("Página não estava na memória...\n");

                // Se ainda há espaço na memória, adiciona a página
                if (memory->numEntries < TOTAL_MEM_PAGES) {
                    memory->entries[memory->numEntries] = page;
                    memory->numEntries++;
                } else {
                    // Acha a página LRU
                    struct Page* leastRecentlyUsedPage = memory->entries[0];
                    for (int j = 1; j < memory->numEntries; j++) {
                        if (memory->entries[j]->useIndex > leastRecentlyUsedPage->useIndex) {
                            leastRecentlyUsedPage = memory->entries[j];
                        }
                    }
                    printf("Página a mais tempo sem uso: %d\n", leastRecentlyUsedPage->pageNumber);

                    for (int j = 0; j < memory->numEntries; j++) {
                        if (memory->entries[j]->pageNumber == leastRecentlyUsedPage->pageNumber) {
                            snprintf(substitutionMessage, sizeof(substitutionMessage), "Substituição: %d->%d", leastRecentlyUsedPage->pageNumber, page->pageNumber);
                            // Exibe a mensagem por 1 segundo (60 frames)
                            substitutionMessageTimer = TOTAL_FRAMES_PER_SECOND;

                            memory->entries[j] = page;
                            break;
                        }
                    }
                }
                totalPageFaults++;
            }

            currentTime++;

            if (currentTime >= TOTAL_TIME) {
                simulationRunning = false;
            }
        }
        simulationCounter++;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawMemoryBars(memory);

        DrawText(TextFormat("Faltas de Pagina: %d", totalPageFaults), 10, 10, 20, DARKGRAY);
        DrawText(TextFormat("Tempo: %d/%d", currentTime, TOTAL_TIME), 10, 40, 20, DARKGRAY);

        // Exibe a mensagem de substituição, se houver
        if (substitutionMessageTimer > 0) {
            DrawText(substitutionMessage, SCREEN_WIDTH / 2 - MeasureText(substitutionMessage, 20) / 2, 50, 20, RED);
            substitutionMessageTimer--;
        }

        EndDrawing();
    }

    // Liberação de memória
    for (int i = 0; i < TOTAL_MEM_PAGES * 2; i++) {
        free(pages[i]);
    }
    free(memory);
    free(memoryPages);
    free(pages);

    CloseWindow();

    return 0;
}