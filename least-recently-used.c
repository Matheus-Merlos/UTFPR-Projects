#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define TOTAL_MEM_PAGES 8

struct Page {
    int pageNumber;
    int useIndex;
};

struct Memory {
    int numEntries;
    struct Page** entries;
};

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

    for(int i = 0; i < 15; i++) {
        //Escolhe uma das páginas pré-definidas
        int pageNum = randInt(TOTAL_MEM_PAGES * 2);
        struct Page* page = pages[pageNum];
        printf("Página escolhida: %d\n", page->pageNumber);

        //Aumenta o useIndex de cada página na memória
        for(int j = 0; j < memory->numEntries; j++) {
            memory->entries[j]->useIndex++;
        }

        if(isPageInMemory(page, memory)) {
            page->useIndex = 0;
            printf("Página estava na memória. Sem Swap.\n");
        }
        else {
            printf("Página não estava na memória...\n");
            page->useIndex = 0;
            sleep(1);

            //Se tem algum espaço livre na memória ainda...
            if(memory->numEntries < TOTAL_MEM_PAGES) {
                memory->entries[memory->numEntries] = page;
                memory->numEntries++;
            }
            else {
                //Acha a página mais tempo sem uso
                struct Page* leastRecentlyUsedPage = memory->entries[0];
                for(int j = 0; j < memory->numEntries; j++) {
                    if(leastRecentlyUsedPage->useIndex < memory->entries[j]->useIndex) {
                        leastRecentlyUsedPage = memory->entries[j];
                    };
                }
                printf("Página a mais tempo sem uso: %d\n", leastRecentlyUsedPage->pageNumber);
                //Substitui essa página pela página que vai entrar
                for(int j = 0; j < memory->numEntries; j++) {
                    if(leastRecentlyUsedPage->pageNumber == memory->entries[j]->pageNumber) {
                        memory->entries[j] = page;
                        break;
                    };
                }
                printf("Substituição: %d->%d\n", leastRecentlyUsedPage->pageNumber, page->pageNumber);
            }
            totalPageFaults++;
        }

        printf("Páginas na memória atual: ");
        printPages(memory->entries, memory->numEntries);

        sleep(1);
    }

    for(int i = 0; i < TOTAL_MEM_PAGES * 2; i++) {
        free(pages[i]);
    }
    free(memory);
    free(memoryPages);
    free(pages);

    printf("Total de faltas de página: %d\n", totalPageFaults);
    return 0;
}