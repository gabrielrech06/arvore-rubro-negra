#ifndef __ARVORE_RN__
#define __ARVORE_RN__

// Árvore Rubro-Negra
//
// TAD que implementa uma árvore rubro-negra (com dados do tipo int).
//

#include <stdbool.h>

//// --- tipos exportados ---
typedef struct no No;
typedef struct arvore Arvore;


//// --- criação / destruição ---

// cria e retorna uma árvore rubro-negra vazia.
// retorna um ponteiro para nova árvore ou NULL em caso de falha de alocação.
// quem chamar deve liberar com arv_libera_arvore quando não for mais útil.
Arvore* arv_cria();

// cria e retorna um nó com o valor de `v`.
// retorna um ponteiro para novo nó ou NULL em caso de falha de alocação.
// quem chamar deve liberar com `arv_libera_arvore` ou `arv_libera_no` quando não for mais útil.
No* arv_cria_no(int v);

// libera toda a árvore rubro-negra.
void arv_libera_arvore(Arvore *arv);

// libera o nó e suas subárvores.
void arv_libera_no(No *no);


//// --- consultas ---

// retorna true se a árvore estiver vazia ou false senão estiver vazia.
bool arv_vazia(Arvore *arvore);

// retorna true se o nó for vazio.
// um nó vazio é aquele que aponta para NULL.
bool arv_no_vazio(No *no);

// retorna um ponteiro para o nó raiz da árvore
No* arv_busca_raiz(Arvore *arvore);

// retorna um ponteiro para o filho do nó.
// `dir` == false -> filho esquerdo.
// `dir` == true -> filho direito.
No* arv_busca_filho(No *no, bool dir);

// retorna um ponteiro para o nó pai.
No* arv_busca_pai(No *no);

// retorna o valor do dado do nó.
// retorna true e grava em `*retorno` o valor se no != NULL.
// retorna false se no == NULL.
bool arv_busca_valor(No *no, int *retorno);

// retorna um ponteiro para o nó com o valor buscado.
// procura a partir de `raiz`.
No* arv_busca_no(No *raiz, int v);

// retorna true se a árvore conter o valor ou false senão conter.
bool arv_contem(Arvore *arv, int v);

// retorna a altura da árvore.
int arv_altura(Arvore *arv);

// retorna um ponteiro para o nó com menor valor a partir do nó passado como argumento.
No* arv_busca_minimo(No *raiz);

// retorna um ponteiro para o nó com maior valor a partir do nó passado como argumento.
No* arv_busca_maximo(No *raiz);


//// --- inserção/remoção ---

// insere na árvore rubro-negra o nó com o valor passado como argumento.
// retorna true se for bem sucedido ou false caso não.
bool arv_insere_no(Arvore *arv, int v);

// remove da árvore rubro-negra o nó com o valor passado como argumento.
// retorna true se for bem sucedido ou false caso não.
bool arv_remove_no(Arvore *arv, int v);



#endif