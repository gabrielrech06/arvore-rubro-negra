#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore-rn.h"


// função que a árvore utilizará para comparar os
// dois dados strings para ordenar os dados
int comparador_str(void *p1, void *p2) {
    // converte os ponteiros void* de volta para char*
    char *str1 = (char*)p1;
    char *str2 = (char*)p2;
    // nesse caso, a comparação usa o próprio strcmp
    return strcmp(str1, str2);
}

void imprime_arv_rec(No *no, int nivel) {
    if(arv_no_vazio(no)) return;

    imprime_arv_rec(arv_busca_filho(no, true), nivel + 1);

    for(int i = 0; i < nivel; i++) {
        printf("    ");
    }

    char *valor = (char*)arv_busca_valor(no);
    Cor cor = arv_busca_cor(no);

    // se o nó é vermelho, printa o valor de vermelho
    // \x1B[31m para vermelho,
    // \x1B[0m para voltar ao padrão (que é branco, mas nessa utilização
    // vamos interpretar como se fosse um nó preto)
    if(cor == VERMELHO) {
        printf("\x1B[31m(%s)\x1B[0m\n", valor);
    } 
    else {
        printf("(%s)\n", valor);
    }

    imprime_arv_rec(arv_busca_filho(no, false), nivel + 1);
}

void imprime_arvore(Arvore *arv) {
    // se a árvore é NULL ou está vazia
    if(arv == NULL || arv_vazia(arv)) {
        printf("árvore vazia.\n");
        printf("-------------------------------------------\n\n");
        return;
    }

    No *raiz = arv_busca_raiz(arv);
    imprime_arv_rec(raiz, 0);
    printf("\n-------------------------------------------\n\n");
}


int main() {
    // para o liberador dessa implementação, que é uma string, é 
    // possível libera-la apenas com free, mas se fosse uma struct mais
    // complexa deveria se passar uma função que libera todos os campos e
    // o próprio descritor da struct, por exemplo
    Arvore *arv = arv_cria(comparador_str, free);
    printf("árvore de strings criada! nós: %d.\n", arv_nnos(arv));
    imprime_arvore(arv);

    // ao inserir, o usuário tem que alocar o espaço em memória para os dados
    // a serem inseridos, nesse caso strings, o strdup já faz isso para nós, 
    // aloca o espaço para a string do argumento e a copia para a região
    // de memória alocada
    printf("inserindo 'Abacaxi', 'Manga', 'Bergamota', 'Morango' e 'Kiwi'\n");
    arv_insere_no(arv, strdup("Abacaxi"));
    arv_insere_no(arv, strdup("Manga"));
    arv_insere_no(arv, strdup("Bergamota"));
    arv_insere_no(arv, strdup("Morango"));
    arv_insere_no(arv, strdup("Kiwi"));
    imprime_arvore(arv);

    printf("número de nós: %d.\n", arv_nnos(arv));
    printf("altura da árvore: %d.\n", arv_altura(arv));

    if(arv_contem(arv, "Kiwi")) {
        printf("a árvore contém 'Kiwi'.\n");
    }
    if(!arv_contem(arv, "Uva")) {
        printf("a árvore não contém 'Uva'.\n");
    }

    // remove o nó e libera ele também, já que passamos a função
    // de liberador para o dado
    printf("removendo 'Morango' da árvore.\n");
    arv_remove_no(arv, "Morango"); 
    imprime_arvore(arv);

    printf("número de nós: %d.\n", arv_nnos(arv)); 

    if(!arv_contem(arv, "Morango")) {
        printf("a árvore não contém 'Morango'.\n");
    }

    No *raiz = arv_busca_raiz(arv);
    No *no_min = arv_busca_minimo(raiz);
    No *no_max = arv_busca_maximo(raiz);

    // arv_busca_valor retorna um void*, devemos fazer o cast
    // para char* já que é uma string
    char *valor_min = (char*)arv_busca_valor(no_min);
    char *valor_max = (char*)arv_busca_valor(no_max);
    printf("valor mínimo da árvore: %s.\n", valor_min);
    printf("valor máximo da árvore: %s.\n", valor_max);


    arv_libera_arvore(arv);
    return 0;
}