// Algoritmo para verificar se um grafo possui um ciclo Hamiltoniano e
// emite um certificado com um ciclo, caso haja um.
// Essa versao utiliza uma arvore de decisao para otimizar o tempo de execucao.
// Autor: Gabriel Vidal, 2026
// E-mail: gabriel@posgraduacao.uerj.br, gabriel@uerj.br

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define TOT_V 9

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

// funcao que busca um ciclo de Euler. Caso nao haja algum, retorna 0, ou 1, caso
// encontre, e imprime o certificado que contem o ciclo.
// entrada: g (grafo): matriz com as informacoes das arestas do grafo
// n: total de vertices no grafo
// rotulo: vetor que contem os rotulos de cada vertice
// saida: retorna 0, caso nao tenha encontrado um ciclo hamiltoniano, e 1 caso tenha encontrado
int encontra_ciclo_hamiltoniano(int g[TOT_V][TOT_V], int n, char rotulo[TOT_V]) {
    int h = 1;
    int encontrado = 0;
    int encontradohc = 0;
    int finalizado = 0;
    int ciclohc[n+1];
    int direcao = 1;   // +1: segue adiante, -1: volta
    int i;
    int v1;
    int v2;
    for (i=0; i<n; i++) {
        ciclohc[i] = 0;
    }
    while ((!finalizado) & (!encontradohc)) {
        ciclohc[h-1] = ciclohc[h-1] + 1;
        v1 = ciclohc[h-1];
        // se o vertice atual na posicao h for maior que n, entao eh porque
        // nao ha mais vertices disponiveis para as selecoes anteriores.
        // por isso, deve-se regredir a posicao h para tentar a proxima opcao de ciclo disponivel.
        // no entanto, se h = 1, entao eh porque nao ha mais opcoes disponiveis, provando
        // que o grafo nao possui ciclo hamiltoniano.
        if (v1 > n) {
            if (h == 1)
                finalizado = 1;
            else
                h--;
        }
        else {
            // verifica se este vertice jah aparece no ciclo.
            // se aparecer, devemos mover para o proximo vertice na posicao h.
            i = h-2;
            encontrado = 0;
            while ( (i >= 0) & (!encontrado) ) {
                if (v1 == ciclohc[i])
                    encontrado = 1;
                i--;
            }
            if (!encontrado) {
                // verifica se ha uma aresta conectando o vertice ao seu imediato anterior
                if (h > 1) {
                    v2 = ciclohc[h-2];
                    if (g[v1-1][v2-1] == 1) {
                        encontrado = 1;
                    }
                }
                if ( ( h ==1 ) || (encontrado) ) {
                    // possui aresta com o imediato anterior (ou h=1). movemos o h para a proxima posicao.
                    h++;
                    // se h alcancou a posicao n + 1, entao, basta verificar se ha uma aresta
                    // da posicao n com o mesmo vertice que inicia o ciclo.
                    // caso haja, entao, um ciclo hamiltoniano foi encontrado.
                    // caso contrario, voltamos h para a posicao n para tentarmos com o proximo vertice.
                    if (h == n+1) {
                        v2 = ciclohc[0];
                        if (g[v1-1][v2-1] == 1) {
                            encontradohc = 1;
                            ciclohc[h-1] = v2;
                        }
                        else
                            h--;
                    }
                    else
                        ciclohc[h-1] = 0;
                }
            }
        }
    }
    if (encontradohc) {
        printf("Ciclo hc = {");
        for (i = 0; i <= n; i++) {
            v1 = ciclohc[i];
            if (i>0)
                printf(",");
            printf("%c", rotulo[v1-1]);
        }
        printf("}\n");
    }
    return encontradohc;
}

// verifica se o grafo possui consistencia. Ou seja, toda aresta que começa em u e termina em
// v, deve também possuir uma aresta que comeca em v e termina em u, pois sao grafos
// nao-direcionados. Retorna 1 caso seja consistente, ou 0 caso nao seja.
// entrada: g (grafo): matriz com as informacoes das arestas do grafo
// n: total de vertices no grafo
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
