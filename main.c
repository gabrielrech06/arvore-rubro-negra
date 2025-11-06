#include <stdio.h>
#include <stdlib.h>
#include "arvore-rn.h"

void imprime_arv_rec(No *no, int nivel) {
    if(arv_no_vazio(no)) return;

    // chama a recursão pro filho direito, incrementando `nivel`
    imprime_arv_rec(arv_busca_filho(no, true), nivel + 1);

    // indentação por nível
    for(int i = 0; i < nivel; i++) {
        printf("    ");
    }

    // busca o valor e a cor do nó
    int valor;
    arv_busca_valor(no, &valor);
    Cor cor = arv_busca_cor(no);

    // se o nó é vermelho, printa o valor de vermelho
    // \x1B[31m para vermelho,
    // \x1B[0m para voltar ao padrão (que é branco, mas nessa utilização
    // lemos como se fosse um nó preto)
    if(cor == VERMELHO) {
        printf("\x1B[31m(%d)\x1B[0m\n", valor);
    } 
    else {
        printf("(%d)\n", valor);
    }

    // chama a recursão pro filho esquerdo, incrementando `nivel`
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
    Arvore *arv = arv_cria();
    imprime_arvore(arv);

    printf("inserindo um nó 10\n");
    // como é o primeiro nó a ser inserido, é a raíz e deve ser preto
    arv_insere_no(arv, 10);
    imprime_arvore(arv);

    printf("inserindo um nó 20\n");
    // deve ser inserido a direita de 10 e deve ser vermelho
    arv_insere_no(arv, 20);
    imprime_arvore(arv);

    printf("inserindo um nó 5\n");
    // deve ser inserido a esquerda de 10 e deve ser vermelho
    arv_insere_no(arv, 5);
    imprime_arvore(arv);
    
    printf("inserindo um nó 3\n");
    // caso onde o pai e o tio são vermelhos:
    // caso 3 de inserção, deve repintar o pai e tio para preto,
    // e repintar o avô de preto (nesse caso já era pois é a raíz)
    arv_insere_no(arv, 3);
    imprime_arvore(arv);

    printf("inserindo um nó 4\n");
    // caso do pai vermelho e tio(NIL) preto:
    // cai no caso 4/5, repinta o 4 para preto, deve-se fazer uma rotação á esquerda no pai, 
    // tranformando agora no caso 5, onde deve-se rotacionar a direita no novo pai(5) 
    // e pinta-lo de vermelho
    arv_insere_no(arv, 4);
    imprime_arvore(arv);

    printf("removendo o nó 4\n");
    // caso de remoção de um nó com dois filhos:
    // caso 3 de remoção, que usa cópia. deve encontrar o seu sucessor (menor
    // nó do filho direito), que nesse caso é o nó 5, copia o valor dele para o nó
    // 4 (passa a ser 5), novo nó a remover é o sucessor(o que foi copiado), o nó
    // substituto é o antigo filho do 5 (nesse caso NIL), a cor do removido é vermelho,
    // apenas pinta o nó que recebeu a cópia de preto e não é necessário mais correções
    arv_remove_no(arv, 4);
    imprime_arvore(arv);

    printf("removendo o nó 3\n");
    // caso de remoção de um nó vermelho sem filhos e com pai preto:
    // apenas remove e o substitui por um NIL
    arv_remove_no(arv, 3);
    imprime_arvore(arv);

    printf("inserindo nó 15, 30 e 25\n");
    arv_insere_no(arv, 15);
    arv_insere_no(arv, 30);
    arv_insere_no(arv, 25);
    imprime_arvore(arv);

    printf("removendo o nó 10 (raiz com 2 filhos)\n");
    // caso que o nó tem dois filhos:
    // deve usar cópia, o nó 15 (menor a direita) é copiado para raíz (que é o no 10),
    // então deve remover o antigo nó 15, e como ele era um nó preto, deve-se
    // aplicar as correções de remoção, nesse caso, uma rotação a direita de 30, 
    // uma rotação a esquerda de 20 e repintar o 20 de preto
    // (caso 3(irmao com filho esquerdo vermelho) - rotação a direita no irmão do removido (30) 
    // -> (caso 4(irmao preto e filho direito vermelho) - rotação a esquerda no pai e recoloração)
    arv_remove_no(arv, 10);
    imprime_arvore(arv);

    if(arv_contem(arv, 10)) {
        printf("o nó 10 está contido na árvore.\n");
    }
    else {
        printf("o nó 10 não está contido na árvore.\n");
    }

    if(arv_contem(arv, 20)) {
        printf("o nó 20 está contido na árvore.\n");
    }
    else {
        printf("o nó 20 não está contido na árvore.\n");
    }

    printf("\naltura da árvore atual: %d.\n", arv_altura(arv));

    printf("número total de nós da árvore: %d.\n", arv_nnos(arv));

    No *raiz = arv_busca_raiz(arv);
    No *no_min = arv_busca_minimo(raiz);
    No *no_max = arv_busca_maximo(raiz);
    
    int valor_min, valor_max;
    if(arv_busca_valor(no_min, &valor_min) && arv_busca_valor(no_max, &valor_max)) {
        printf("valor mínimo da árvore: %d.\n", valor_min);
        printf("valor máximo da árvore: %d.\n", valor_max);
    } 
    else {
        printf("erro ao buscar min/max (árvore vazia?).\n");
    }

    No *no_20 = arv_busca_no(raiz, 20);
    if(!arv_no_vazio(no_20)) {
        No *pai_de_20 = arv_busca_pai(no_20);
        int valor_pai;
        
        if(arv_busca_valor(pai_de_20, &valor_pai)) {
            printf("pai do nó 20 é: %d.\n", valor_pai);
        } 
        else {
            printf("nó 20 é a raiz (não tem pai).\n");
        }
    } 
    else {
        printf("não foi possivel encontrar o nó 20.\n");
    }


    arv_libera_arvore(arv);
    return 0;
}