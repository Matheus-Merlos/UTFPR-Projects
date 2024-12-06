#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NOS 100  // Define o tamanho máximo do grafo
#define SEM_ARESTA -1
#define INFINITO 100000  // Representa o infinito

// Estrutura para representar o grafo
typedef struct {
    int numNos;
    int matriz[MAX_NOS][MAX_NOS];
} Grafo;

// Função para inicializar o grafo
void inicializarGrafo(Grafo *g) {
    g->numNos = 0;
    for (int i = 0; i < MAX_NOS; i++) {
        for (int j = 0; j < MAX_NOS; j++) {
            g->matriz[i][j] = SEM_ARESTA;  // Inicializa todos os pesos como sem aresta
        }
    }
}

// Função para inserir um nó no grafo
void inserirNo(Grafo *g) {
    if (g->numNos < MAX_NOS) {
        g->numNos++;
        printf("Nó %d inserido.\n", g->numNos - 1);
    } else {
        printf("Número máximo de nós atingido.\n");
    }
}

// Função para inserir uma aresta entre dois nós
void inserirAresta(Grafo *g, int no1, int no2, int peso) {
    if (no1 < g->numNos && no2 < g->numNos) {
        g->matriz[no1][no2] = peso;  // Define o peso da aresta
        g->matriz[no2][no1] = peso;  // Como o grafo é não direcionado, espelha o valor
        printf("Aresta inserida entre os nós %d e %d com peso %d.\n", no1, no2, peso);
    } else {
        printf("Erro: nós inválidos.\n");
    }
}

// Função para exibir a matriz de adjacência
void exibirGrafo(Grafo *g) {
    printf("Matriz de Adjacência:\n");
    for (int i = 0; i < g->numNos; i++) {
        for (int j = 0; j < g->numNos; j++) {
            if (g->matriz[i][j] == SEM_ARESTA) {
                printf("  ");  // Deixa em branco se não há aresta
            } else {
                printf("%d ", g->matriz[i][j]);
            }
        }
        printf("\n");
    }
}

void atualizarCaminhos(Grafo *g, int origem, int destino, int distancias[], bool visitados[], int distancia) {
    if(visitados[origem] == true) return;
    if(origem == destino) return;

    visitados[origem] = true;

    for(int i = 0; i < g->numNos; i++) {
        if(g->matriz[origem][i] == SEM_ARESTA) continue;

        /*
        Vê se a distância atual é menor que a deste caminho, se for,atualiza.
        */
        if(distancias[i] > distancia + g->matriz[origem][i]) {
            distancias[i] = (distancia + g->matriz[origem][i]);
        }
        atualizarCaminhos(g, i, destino, distancias, visitados, distancia + g->matriz[origem][i]);
    }
}

// Função para encontrar a menor distância entre dois nós usando o algoritmo de Dijkstra
void encontrarMenorCaminho(Grafo *g, int origem, int destino) {
    /* 
    Cria um nó com o número de nós do grafo
    O índice da lista será utilizado como a chave para o nó, ou seja, o índice 0 representa a distância para o nó 0, o índice 1 para o nó 1...
    */
    int distancias[g->numNos];
    bool visitados[g->numNos];

    // Preenche os vetores com "INFINITO"
    //Afinal, no algoritmo de Dijkstra todos começam assim né kkk
    for(int i = 0; i < g->numNos; i++) {
        distancias[i] = INFINITO;
        visitados[i] = false;
    }

    distancias[origem] = 0;
    visitados[origem] = true;

    //1° problema:
    // Pegar os nós vizinhos do nó de origem
    //Irei fazer essa primeira função
    for(int i = 0; i < g->numNos; i++) {
        //Ignora os que não tem aresta
        if(g->matriz[origem][i] == SEM_ARESTA) continue;

        distancias[i] = g->matriz[origem][i];
        atualizarCaminhos(g, i, destino, distancias, visitados, g->matriz[origem][i]);
    }

    int distancia = distancias[destino];

    // Verifica a distância final
    if (distancia == INFINITO) {
        printf("Não há caminho do nó %d para o nó %d.\n", origem, destino);
    } else {
        printf("A menor distância do nó %d para o nó %d é %d.\n", origem, destino, distancia);
    }
}

int main(void) {
    Grafo g;
    inicializarGrafo(&g);

    // Inserir nós
    inserirNo(&g); // a 0
    inserirNo(&g); // b 1
    inserirNo(&g); // c 2
    inserirNo(&g); // d 3
    inserirNo(&g); // e 4
    inserirNo(&g); // f 5
    inserirNo(&g); // g 6
    inserirNo(&g); // h 7
    inserirNo(&g); // i 8

    // Inserir arestas
    inserirAresta(&g, 0, 1, 7);
    inserirAresta(&g, 0, 3, 5);
    inserirAresta(&g, 1, 2, 8);
    inserirAresta(&g, 1, 3, 9);
    inserirAresta(&g, 1, 4, 7);
    inserirAresta(&g, 2, 4, 5);
    inserirAresta(&g, 2, 7, 1);
    inserirAresta(&g, 3, 4, 15);
    inserirAresta(&g, 3, 5, 6);
    inserirAresta(&g, 4, 5, 8);
    inserirAresta(&g, 4, 6, 9);
    inserirAresta(&g, 5, 6, 11);
    inserirAresta(&g, 6, 8, 10);
    inserirAresta(&g, 7, 8, 2);


    // Encontrar menor caminho
    encontrarMenorCaminho(&g, 8, 3);
    encontrarMenorCaminho(&g, 3, 4);
    encontrarMenorCaminho(&g, 6, 3);
    encontrarMenorCaminho(&g, 2, 3);
    encontrarMenorCaminho(&g, 5, 8);
}