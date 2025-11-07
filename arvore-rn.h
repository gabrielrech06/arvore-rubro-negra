#ifndef _ARVORE_RN_
#define _ARVORE_RN_

// Árvore Rubro-Negra
//
// TAD que implementa uma árvore rubro-negra genérica.
//

#include <stdbool.h>

//// --- tipos exportados ---
typedef struct no No;
typedef struct arvore Arvore;
typedef enum { VERMELHO, PRETO } Cor;
// a função recebe ponteiros para dois dados, e retorna um inteiro
// com o resultado da comparação, que deve ser:
//   - positivo se o primeiro item tem "valor" maior que o segundo,
//   - negativo se o primeiro item tem "valor" menor que o segundo,
//   - zero se os itens têm o mesmo "valor".
typedef int Comparador(void *dado1, void *dado2);



//// --- criação / destruição ---

// cria e retorna uma árvore rubro-negra vazia.
// retorna um ponteiro para nova árvore ou NULL em caso de falha de alocação.
// quem chamar deve liberar com arv_libera_arvore quando não for mais útil.
Arvore* arv_cria(int tam_bytes_dado, Comparador *comp);

// libera toda a árvore rubro-negra.
void arv_libera_arvore(Arvore *arv);



//// --- inserção/remoção ---

// insere na árvore rubro-negra o nó com o valor passado como argumento.
// retorna true se for bem sucedido ou false caso não.
bool arv_insere_no(Arvore *arv, void *v);

// remove da árvore rubro-negra o nó com o valor passado como argumento.
// retorna true se for bem sucedido ou false caso não.
bool arv_remove_no(Arvore *arv, void *v);



//// --- consultas ---

// retorna true se a árvore estiver vazia ou false senão estiver vazia.
bool arv_vazia(Arvore *arv);

// retorna true se o nó for vazio.
// um nó vazio é um nó NIL.
bool arv_no_vazio(No *no);

// retorna o número de nós da árvore.
int arv_nnos(Arvore *arv);

// retorna um ponteiro para o nó raiz da árvore.
No* arv_busca_raiz(Arvore *arv);

// retorna um ponteiro para o filho do nó.
// `dir` == false -> filho esquerdo.
// `dir` == true -> filho direito.
No* arv_busca_filho(No *no, bool dir);

// retorna a cor do nó.
Cor arv_busca_cor(No *no);

// retorna um ponteiro para o nó pai.
No* arv_busca_pai(No *no);

// retorna true e grava em `*retorno` o valor do nó.
// retorna false se `no` == NULL/NIL ou `retorno` == NULL.
bool arv_busca_valor(No *no, void *retorno, int tam_bytes_dado);

// retorna um ponteiro para o nó com o valor buscado.
// procura a partir de `raiz`.
No* arv_busca_no(No *raiz, void *v, Comparador *comp);

// retorna true se a árvore conter o valor `v` ou false senão conter.
bool arv_contem(Arvore *arv, void *v);

// retorna a altura da árvore.
int arv_altura(Arvore *arv);

// retorna um ponteiro para o nó com menor valor a partir do nó passado como argumento.
No* arv_busca_minimo(No *raiz);

// retorna um ponteiro para o nó com maior valor a partir do nó passado como argumento.
No* arv_busca_maximo(No *raiz);



#endif