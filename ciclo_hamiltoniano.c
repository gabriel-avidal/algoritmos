// Algoritmo para verificar se um grafo possui um Ciclo Hamiltoniano e
// emite um certificado com um ciclo, caso haja um.
// Essa versao utiliza analise combinatoria para testar todas as opcoes possiveis.
// Caso encontre, imprime o ciclo encontrado.
// Caso esgote todas as opcoes possiveis, imprime uma mensagem informando que o grafo
// nao possui um Ciclo Hamiltoniano.
// Autor: Gabriel Vidal, 2026
// E-mail: gabriel@posgraduacao.uerj.br, gabriel@uerj.br

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define TOT_V 10

// funcao que imprime o ciclo contido no vetor ciclohc.
// entrada: g (grafo): matriz com as informacoes das arestas do grafo
// n: total de elementos no ciclo
// ciclohc: vetor que contem o ciclo
// rotulo: vetor que contem os rotulos de cada vertice
int imprime_ciclo(int g[TOT_V][TOT_V], int n, int ciclohc[TOT_V], char rotulo[TOT_V]) {
    int i = 0;
    int i_rotulo;
    printf("Ciclo HC = { ");
    while ((i<n)  ) {
        if (i>0)
            printf(",");
        i_rotulo = ciclohc[i]-1;
        printf("%c",rotulo[i_rotulo]);
        i++;
    }
    printf("}\n");
    return 1;
}

// funcao que verifica se o ciclo contido em ciclohc possui um Ciclo Hamiltoniano.
// entrada: g (grafo): matriz com as informacoes das arestas do grafo
// n: total de elementos no ciclo
// ciclohc: vetor que contem o ciclo
int verifica_ciclo_hamiltoniano(int g[TOT_V][TOT_V], int n, int ciclohc[TOT_V]) {
    int i = 0;
    int i_rotulo;
    int resultado;
    resultado = 1;
    while ((i<n)  ) {
        if (i>0) {
            if (g[ciclohc[i]-1][ciclohc[i-1]-1] == 0)
                resultado = 0;
        }
        i++;
    }
    return resultado;
}

// funcao que busca um Ciclo Hamiltoniano. Caso nao haja algum, retorna 0, ou 1 caso
// encontre, e imprime o certificado que contem o ciclo.
// entrada: g (grafo): matriz com as informacoes das arestas do grafo
// n: total de elementos no ciclo
// rotulo: vetor que contem os rotulos de cada vertice
int encontra_ciclo_hamiltoniano(int g[TOT_V][TOT_V], int n, char rotulo[TOT_V]) {
    int h = 0;
    int encontrado = 0;
    int finalizado = 0;
    int ciclohc[n+1];
    int direcao = 1;   // +1: segue adiante, -1: volta
    int i;
    for (i=0; i<n; i++) {
        ciclohc[i] = 0;
    }
    while ((!finalizado) & (!encontrado)) {
        ciclohc[h] = ciclohc[h] + 1;
        i = h-1;
        while (i>=0) {
            if (ciclohc[h] == ciclohc[i]) {
                ciclohc[h]++;
                i = h-1;
            }
            else
                i--;
        }
        if (ciclohc[h] > n) {
            h = h - 1;
            if (h < 0)
                finalizado = 1;
        }
        else {
            h++;
            if (h == n) {
                ciclohc[n] = ciclohc[0];
                if (verifica_ciclo_hamiltoniano(g, n+1, ciclohc)) {
                    encontrado = 1;
                    imprime_ciclo(g, n+1, ciclohc, rotulo);
                }
                else {
                    h--;
                }
            }
            else {
                ciclohc[h] = 0;
            }
        }
    }
    return encontrado;
}

// verifica se o grafo possui consistencia. Ou seja, toda aresta que começa em u e termina em
// v, deve também possuir uma aresta que comeca em v e termina em u, pois sao grafos
// nao-direcionados. Retorna 1 caso seja consistente, ou 0 caso nao seja.
// entrada: g (grafo): matriz com as informacoes das arestas do grafo
// n: total de elementos no ciclo
int verifica_consistencia_grafo(int g[TOT_V][TOT_V], int n) {
    int i;
    int j;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            if (g[i][j] != g[j][i])
                return 0;
        }
    }
    return 1;
}

int main(void) {
    //int g[TOT_V][TOT_V];
    char rotulo[TOT_V] = {'a','b','c','d','e','f','g','h', 'i'};
    double start;
    double end;
    int i;
    int j;
    // este grafo possui um ciclo hamiltoniano
    // ajustar TOT_V para 10.(10 vertices)
    /*int g[TOT_V][TOT_V] = {
//   0 1 2 3 4 5 6 7 8 9
    {0,0,1,0,0,1,1,1,0,0},
    {0,0,1,1,0,1,1,1,0,0},
    {1,1,0,1,0,0,0,0,0,0},
    {0,1,1,0,0,1,1,0,0,0},
    {0,0,0,0,0,0,1,1,0,1},
    {1,1,0,1,0,0,0,0,0,0},
    {1,1,0,1,1,0,0,0,1,0},
    {1,1,0,0,1,0,0,0,1,1},
    {0,0,0,0,0,0,1,1,0,1},
    {0,0,0,0,1,0,0,1,1,0},
    };*/
    // este grafo nao possui um ciclo hamiltoniano
    // ajustar TOT_V para 9. (9 vertices)
    int g[TOT_V][TOT_V] = {
        {0,1,0,0,0,0,1,0,0},
        {1,0,1,0,0,0,0,0,0},
        {0,1,0,1,0,0,0,1,1},
        {0,0,1,0,1,0,0,0,0},
        {0,0,0,1,0,1,0,0,0},
        {0,0,0,0,1,0,1,1,1},
        {1,0,0,0,0,1,0,0,0},
        {0,0,1,0,0,1,0,0,0},
        {0,0,1,0,0,1,0,0,0}
    };


    struct timeval tstart, tend;

    gettimeofday(&tstart, NULL);

    if (!verifica_consistencia_grafo(g, TOT_V)) {
        printf("Grafo inconsistente. Encerrando programa...\n");
        return 1;
    }
    // procura um ciclo de Hamilton
    if (encontra_ciclo_hamiltoniano(g, TOT_V, rotulo))
        printf("Encontrado um Ciclo Hamiltoniano.\n");
    else
        printf("Nao foi encontrado um Ciclo Hamiltoniano.\n");
    gettimeofday(&tend, NULL);
    printf("%ld microseconds\n", ((tend.tv_sec * 1000000 + tend.tv_usec) - (tstart.tv_sec * 1000000 + tstart.tv_usec)));
    return 0;
}
