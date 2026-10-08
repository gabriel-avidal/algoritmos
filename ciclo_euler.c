// Algoritmo para verificar se um grafo possui um ciclo Euleriano e
// emite um certificado com um ciclo, caso haja um.
// Autor: Gabriel Vidal, 2026
// E-mail: gabriel@posgraduacao.uerj.br, gabriel@uerj.br

#include <stdio.h>
#include <stdlib.h>

// TOT_V representa o total de vertices do grafo
#define TOT_V 9
// constantes utilizadas para a busca em largura
#define BRANCO 0
#define CINZA 1
#define PRETO 2
#define D_ZERO 0
#define D_INFINITO 9999
#define PI_NULL 0
#define MAX 100

// Estrutura de dados usada para o Grafo - aresta
// contem o vertice 1 (v1) e o vertice 2 (v2) que compoe a aresta
struct s_aresta {
    int v1;
    int v2;
};
typedef struct s_aresta aresta;

// implementação da fila dinâmica, usado pela
// busca em largura
struct fila {
    int inicio, final, qtd;
    int v[MAX];
};
typedef struct fila Fila;

Fila* cria_Fila() {

    Fila *fi = (Fila*)malloc(sizeof(struct fila));
    if (fi != NULL) {
        fi->inicio = 0;
        fi->final = 0;
        fi->qtd = 0;
    }
    return fi;
}

void destroi_Fila(Fila* fi) {
    free(fi);
}

int tamanho_Fila(Fila *fi) {
    if (fi == NULL)
        return -1;
    return fi->qtd;
}

int Fila_cheia(Fila* fi) {
    if (fi == NULL)
        return -1;
    if (fi->qtd == MAX)
        return 1;
    else
        return 0;
}

int Fila_vazia(Fila* fi) {
    if (fi == NULL)
        return -1;
    if (fi->qtd == 0)
        return 1;
    else
        return 0;
}

int enfileira(Fila* fi, int pv) {
    if (fi == NULL)
        return 0;
    if (Fila_cheia(fi))
        return 0;
    fi->v[fi->final] = pv;
    fi->final = (fi->final + 1) % MAX;
    fi->qtd++;
    printf("Enfileirando %d ...\n", pv);
    return 1;
}

int desenfileira(Fila* fi) {
    if (fi == NULL || Fila_vazia(fi) )
        return 0;
    int iPos = (fi->inicio) % MAX;
    fi->inicio = (fi->inicio + 1) % MAX;
    fi->qtd--;
    printf("Desenfileirei.\n");
    return fi->v[iPos];
}

// realiza a Busca em Largura em um grafo, e retorna 1 caso
// o grafo seja conexo, ou 0, caso nao seja conexo
int BFS_Conexo(int g[TOT_V][TOT_V], int n, int v, char rotulo[TOT_V]) {
    int i;
    int cor[n];
    int d[n];
    int pi[n];
    for (i=0; i<n; i++) {
        cor[i] = BRANCO;
        d[i] = D_INFINITO;
        pi[i] = PI_NULL;
    }
    cor[v-1] = CINZA;
    d[v-1] = 0;
    pi[v-1] = PI_NULL;
    Fila *Q;
    Q = cria_Fila();
    enfileira(Q, v);
    int iv;
    while (!Fila_vazia(Q)) {
        iv = desenfileira(Q);
        i = 0;
        while (i < n) {
            if (g[iv-1][i] == 1) {
                if (cor[i] == BRANCO) {
                    d[i] = d[iv-1] + 1;
                    pi[i] = iv;
                    enfileira(Q, i+1 );
                    cor[i] = CINZA;
                }
            }
            i++;
        }
        cor[iv-1] = PRETO;
    }
    destroi_Fila(Q);
    printf("Execucao da busca em largura terminada.\n");
    for (int i = 0; i< n; i++) {
        printf("Distancia do vertice %c ao vertice %c: %d\n", rotulo[i], rotulo[v-1], d[i]);
        if (d[i] == D_INFINITO)
            return 0;
    }
    return 1;
}

// seleciona uma aresta de um grafo. O v1 indica de qual vertice a aresta parte.
// o v2 eh o segundo vertice onde a aresta incide.
// caso v1 seja igual a 0, seleciona a primeira aresta do grafo. (a que parte do primeiro vertice)
aresta seleciona_aresta(int g[TOT_V][TOT_V], int n, int v1, int *visitados) {
    int i;
    int j;
    aresta uma_aresta;
    // inicializa a aresta sem vertices nas duas pontas.
    // caso nenhuma aresta seja selecionada, a aresta sera 0-0.
    uma_aresta.v1 = 0;
    uma_aresta.v2 = 0;
    if (v1 == 0) {
        // encontra o primeiro vertice com aresta disponivel
        for (i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (g[i][j] == 1) {
                    uma_aresta.v1 = i+1;
                    uma_aresta.v2 = j+1;
                    return uma_aresta;
                }
            }
        }
    }
    else {
        i = 0;
        uma_aresta.v1 = v1;
        uma_aresta.v2 = 0;
        while (i < n) {
            if (g[v1-1][i] == 1) {
                if (uma_aresta.v2 == 0)
                    uma_aresta.v2 = i+1;
                if (visitados[i] == 0) {
                    uma_aresta.v2 = i+1;
                    return uma_aresta;
                }
            }
            i++;
        }
        return uma_aresta;
    }
}

// cria um novo ciclo
// retorna um ponteiro para um vetor que contenha o tamanho da entrada tamanho_ciclo
int* cria_novo_ciclo(int tamanho_ciclo) {
    int *c = (int*)malloc(sizeof(int)*tamanho_ciclo);
    for (int i = 0; i < tamanho_ciclo; i++)
        c[i] = 0;
    return c;
}

// junta dois ciclos
// o ciclo 1, ao fim da execucao, contera ambos os conjuntos, inserindo o segundo conjunto inteiro
// na primeira ocorrencia no primeiro ciclo do primeiro elemento do segundo ciclo.
// c1 = { 1, 2, 6, 7, 8, 9 }
// c2 = { 2, 3, 4, 5, 2 }
// o conjunto c1 passara a ser: { 1, 2, 3, 4, 5, 2, 6, 7, 8, 9 }
// primeiro elemento do segundo conjunto: 2
// esse elemento aparece na segunda posicao do c1. logo, eh nessa posicao que o c2 ira aparecer.
int junta_conjuntos( int *c1, int *c2, int n ) {
    int i = 0;
    int j = 0;
    int n1 = 0;
    int n2 = 0;
    int num;
    while ((n1 < n) & (c1[n1] > 0) )
        n1++;
    while ((n2 < n) & (c2[n2] > 0) )
        n2++;
    if (c2[0] > 0) {
        while ( (i < n) & (c1[i] != c2[0]) )
            i++;
        if (i < n) {
            j = n1 -1;
            while (j>i) {
                c1[j+n2-1] = c1[j];
                j--;
            }
            j = 0;
            while (j<n2) {
                c1[i+j] = c2[j];
                j++;
            }
        }
    }
}

// funcao que busca um ciclo de Euler. Caso nao haja algum, retorna 0, ou 1 caso
// encontre, e imprime o certificado que contem o ciclo.
int encontra_ciclo_euler(int g[TOT_V][TOT_V], int n, char rotulo[TOT_V]) {
    // checa os graus de cada vertice
    int i;
    int j;
    int d_v;
    int cor[n];
    int d[n];
    int pi[n];
    int v;
    int n_arestas = 0;
    int tam_ciclo = 0;
    int n_ciclos = 1;
    int *vet_ciclos[TOT_V];
    for (i = 0; i < n; i++) {
        d_v = 0;
        for (j = 0; j < n; j++) {
            if ( (i != j) & (g[i][j] == 1) ) {
                d_v++;
                n_arestas++;
            }
        }
        if (d_v % 2 == 1) {
            printf("Grafo possui vertice com grau impar.\n");
            return 0;
        }
    }
    // Numero de Arestas
    n_arestas = n_arestas / 2;
    tam_ciclo = n_arestas + 1;
    printf("Numero de Arestas: %d\n", n_arestas );
    // verifica distancia entre os vertices, usando a busca em largura
    if (!BFS_Conexo(g, n, 1, rotulo)) {
        printf("Grafo nao eh conexo.\n");
        return 0;
    }
    // tamanho do ciclo: n_arestas + 1
    int *c = cria_novo_ciclo(tam_ciclo);
    vet_ciclos[0] = c;
    // cria o grafo auxiliar
    int g_aux[TOT_V][TOT_V];
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            g_aux[i][j] = g[i][j];
        }
    }
    int visitado[n];
    for (i = 0; i < n; i++)
        visitado[i] = 0;
    aresta e = seleciona_aresta(g_aux, n, 0, visitado);
    printf("Primeira aresta selecionada = %c,%c\n", rotulo[e.v1-1], rotulo[e.v2-1]);
    int v0 = e.v1;    // v0 eh o primeiro vertice do ciclo.
    int v1 = e.v1;
    int v2 = e.v2;
    int pos_ciclo_atual = 0;
    while (n_arestas > 0) {
        printf("Numero de arestas = %d\n", n_arestas);
        c[pos_ciclo_atual] = v1;
        pos_ciclo_atual++;
        visitado[v1-1] = 1;
        v1 = v2;
        g_aux[e.v1-1][e.v2-1] = 0;
        g_aux[e.v2-1][e.v1-1] = 0;
        n_arestas--;
        if (n_arestas > 0) {
            e = seleciona_aresta(g_aux, n, v1, visitado);
            // verifica se nenhuma aresta foi selecionada. significa que houve um ciclo fechado e
            // deve-se criar um novo ciclo.
            if ( (e.v1 == 0) || (e.v2 == 0) ) {
                printf("Ciclo fechado. Vamos criar um novo ciclo.\n");
                c[pos_ciclo_atual] = v2;
                n_ciclos++;
                c = cria_novo_ciclo(tam_ciclo);
                vet_ciclos[n_ciclos-1] = c;
                pos_ciclo_atual = 0;
                e = seleciona_aresta(g_aux, n, 0, visitado);
                v1 = e.v1;
                v2 = e.v2;
                v0 = v1;
            }
            else {
                v2 = e.v2;
            }
            printf("aresta selecionada = %c,%c\n", rotulo[e.v1-1], rotulo[e.v2-1]);
        }
    }
    c[pos_ciclo_atual] = v2;
    printf("Total de Ciclos: %d\n", n_ciclos);
    for (int j = 0; j < n_ciclos; j++) {
        c = vet_ciclos[j];
        printf("Ciclo encontrado: {");
        for (i=0; i<=tam_ciclo; i++) {
            if (c[i] > 0) {
                if (i>0)
                    printf(",");
                printf("%c", rotulo[c[i]-1]);
            }
        }
        printf("}\n");
    }
    // juncao - finaliza todos os ciclos em um unico ciclo Euleriano.
    // o primeiro ciclo sera concatenado com os restantes.
    // portanto, caso tenha apenas um ciclo, entao nao sera necessario
    // concatenar
    c = vet_ciclos[0];
    for (i = 1; i < n_ciclos; i++) {
        int *c2 = vet_ciclos[i];
        junta_conjuntos( c, c2, tam_ciclo );
    }
    printf("Imprimindo o ciclo final.\n {");
    for (int i = 0; i < tam_ciclo; i++) {
        if (c[i] > 0) {
            if (i>0)
                printf(",");
            printf("%c", rotulo[c[i]-1]);
        }
    }
    printf("}\n");

    // limpa a memoria
    for (int i = 0; i < n_ciclos; i++) {
        c = vet_ciclos[i];
        free(c);
    }
    return 1;
}

// verifica se o grafo possui consistencia. Ou seja, toda aresta que começa em u e termina em
// v, deve também possuir uma aresta que comeca em v e termina em u, pois sao grafos
// nao-direcionados. Retorna 1 caso seja consistente, ou 0 caso nao seja.
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
    int n = 9; // numero de vertices
    // matriz de adjacencia, que representa o grafo

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
    // os rotulos dos vertices do grafo
    char rotulo[TOT_V] = {'a','b','c','d','e','f','g','h', 'i'};

    if (!verifica_consistencia_grafo(g, TOT_V)) {
        printf("Grafo inconsistente. Encerrando programa...\n");
        return 1;
    }

    // procura um ciclo de Euler
    if (encontra_ciclo_euler(g, TOT_V, rotulo))
        printf("Encontrado um Ciclo Euleriano.\n");
    else
        printf("Nao foi encontrado um Ciclo Euleriano.\n");
    return 0;
}
