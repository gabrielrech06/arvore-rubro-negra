#ifndef _ARVORE_RN_
#define _ARVORE_RN_

// Árvore Rubro-Negra
//
// TAD que implementa uma árvore rubro-negra genérica.
//
// a árvore não sabe com que tipo de dados está lidando, ela apenas
// possui ponteiros void que ela mesmo pode remanejar. o usuário é o
// responsável pela alocação de memória do dado a ser inserido.
// além disso, ele deve fornecer uma função de comparação, para a 
// árvore saber como organizar esses dados, e uma função de liberação, 
// que a árvore utilizará para liberar o dado apontado pelo nó 
// completamente em casos de remoção ou liberação total da árvore.
//
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

// a função recebe um ponteiro para o dado a liberar
// ela é responsável por liberar toda a memória alocada
// pelo dado.
typedef void Liberador(void *dado);



//// --- criação / destruição ---

// cria e retorna uma árvore rubro-negra vazia.
// retorna um ponteiro para nova árvore ou NULL em caso de falha de alocação.
// se `libera` for NULL, a árvore não liberará a memória alocado pelos dados
// inseridos em uma futura remoção ou até mesmo na liberação de toda árvore.
// quem chamar deve liberar com arv_libera_arvore quando não for mais útil.
Arvore* arv_cria(Comparador *comp, Liberador *libera);

// libera toda a árvore rubro-negra. se a árvore possuir uma função de 
// liberação, libera toda a memória ocupada pelos dados também.
void arv_libera_arvore(Arvore *arv);



//// --- inserção/remoção ---

// insere na árvore rubro-negra o nó com o valor apontado por `v`.
// `v` deve apontar para uma região de memória alocada pelo usuário.
// retorna true se for bem sucedido ou false caso não.
bool arv_insere_no(Arvore *arv, void *v);

// remove da árvore rubro-negra o nó com o valor apontado por `v`.
// se a árvore possuir uma função de liberação, libera a memória ocupada 
// pelo dado também.
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

// retorna um ponteiro void para o valor armazenado pelo nó `no`.
// se `no` for vazio, é retornado NULL.
void* arv_busca_valor(No *no);

// retorna um ponteiro para o nó com o valor buscado.
// procura a partir do nó `raiz`.
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