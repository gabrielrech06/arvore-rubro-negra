#include "arvore-rn.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// estrutura de um nó da árvore rubro-negra
struct no {
    void *dado;
    Cor cor;
    No *dir;
    No *esq;
    No *pai;
};

// estrutura de uma árvore rubro-negra
struct arvore {
    No *raiz;
    int num_nos;
    int tam_dado;
    Comparador *comp;
};

// nó sentinela para representar os nós NIL's da árvore
// todo nó NIL é preto por propriedade da árvore
static struct no NIL_SENTINELA;
static No *NIL = &NIL_SENTINELA;
static bool nil_inicializado = false;


//// --- criação / destruição ---

// função auxiliar para inicializar o nó
// sentinela NIL
static void arv_inicializa_nil() {
    if(!nil_inicializado) {
        NIL->cor = PRETO;
        NIL->dado = NULL;
        NIL->pai = NIL;
        NIL->esq = NIL;
        NIL->dir = NIL;
        nil_inicializado = true;
    }
}

Arvore* arv_cria(int tam_dado, Comparador *comp) {
    arv_inicializa_nil();

    Arvore *nova_arvore = (Arvore*)malloc(sizeof(Arvore));
    if(nova_arvore == NULL) {
        return NULL;
    }
    
    // raiz de uma árvore recém criada aponta para NIL
    nova_arvore->raiz = NIL; 
    nova_arvore->num_nos = 0;
    nova_arvore->tam_dado = tam_dado;
    nova_arvore->comp = comp;
    
    return nova_arvore;
}

// função auxiliar para liberar os nós a partir de `no`
// recursivamente
static void arv_libera_no(No *no) {
    if(arv_no_vazio(no)) return;

    arv_libera_no(no->esq);
    arv_libera_no(no->dir);

    free(no->dado);
    free(no);
}

void arv_libera_arvore(Arvore *arv) {
    if(arv == NULL) return;

    // libera todos os nós partindo da raiz da árvore
    arv_libera_no(arv->raiz);
    // libera o descritor da árvore
    free(arv);
}



//// --- inserção/remoção ---

// função auxiliar que busca o avô do nó `no`
// e retorna o ponteiro para seu avô
static No* arv_busca_avo(No *no) {
    // se o `no` não for NIL e tiver um pai, retorna 
    // seu avô
    if(!arv_no_vazio(no) && !arv_no_vazio(no->pai)) {
        return no->pai->pai;
    }

    // se chegou aqui, ou `no` é NIL ou não tem pai
    return NIL;
}

// função auxiliar que busca o tio do nó `no`
// (tio == irmão de seu pai)
// e retorna o ponteiro para seu tio
static No* arv_busca_tio(No *no) {
    No* avo = arv_busca_avo(no);

    // se não tem avô, é impossível ter tio
    if(arv_no_vazio(avo)) {
        return NIL;
    }
    // se o pai for o filho esquerdo de seu avô,
    // seu tio só pode ser o filho direito
    if(no->pai == avo->esq) {
        return avo->dir;
    }
    // senão o tio é o filho esquerdo de seu avô
    else {
        return avo->esq;
    }
}

// função auxiliar para fazer a rotação à esquerda do
// nó `no`
static void arv_rotacao_esquerda(Arvore *arv, No *no) {
    // `dir` é o filho a direita de `no`
    No *dir = no->dir;

    // se o filho a direita de `no` for NIL, é impossível
    // rotacionar à esquerda
    if(arv_no_vazio(dir)) return;

    // a subárvore esquerda do filho a direita de `no` é a nova
    // subárvore a direita de `no`
    no->dir = dir->esq;
    
    // se a nova subárvore a direita de `no` não for
    // NIL, atualiza o ponteiro do pai da subárvore para `no`
    if(!arv_no_vazio(no->dir)) {
        dir->esq->pai = no; 
    }

    // o novo pai de `dir` vira o antigo pai
    // de `no`
    dir->pai = no->pai;

    // atualizar para o antigo pai de `no` apontar para `dir`
    // se o antigo pai de `no` for NIL, significa que era a raíz
    // da árvore, então a nova raíz é `dir`.
    if(arv_no_vazio(no->pai)) {
        arv->raiz = dir;
    } 
    // se o `no` era um filho a esquerda de seu antigo pai,
    // preenche esse vazio com `dir`
    else if(no == no->pai->esq) {
        no->pai->esq = dir;
    }
    // se o `no` era um filho a direita de seu antigo pai,
    // preenche esse vazio com `dir`
    else {
        no->pai->dir = dir;
    }

    // novo filho a esquerda de `dir` é o
    // próprio `no`
    dir->esq = no;
    // e atualiza o pai de `no` para `dir`.
    no->pai = dir;
}

// função auxiliar para fazer a rotação à direita do
// nó `no`
static void arv_rotacao_direita(Arvore *arv, No *no) {
    // `esq` é o filho a esquerda de `no`
    No *esq = no->esq;

    // se o filho a esquerda de `no` for NIL, é impossível
    // rotacionar à direita
    if(arv_no_vazio(esq)) return;

    // a sub-árvore à direita de `esq` é o novo filho esquerdo de `no`
    no->esq = esq->dir;

    // se o novo filho esquerdo de `no` não for NIL,
    // atualiza o ponteiro pai da sub-árvore
    if(!arv_no_vazio(no->esq)) {
        esq->dir->pai = no;
    }

    // atualiza o pai de `esq` para o antigo pai de `no`
    esq->pai = no->pai;

    // se o antigo pai de `no` era NIL, significa que ele era
    // a raíz, então `esq` passa a ser a nova raíz.
    if(arv_no_vazio(no->pai)) {
        arv->raiz = esq;
    }
    // se `no` era o filho esquerdo de seu antigo pai,
    // preenche o vazio com `esq`
    else if(no == no->pai->esq) {
        no->pai->esq = esq;
    }
    // se `no` era o filho direito de seu antigo pai,
    // preenche o vazio com `esq`
    else {
        no->pai->dir = esq;
    }

    // `no` é atualizado para ser filho direito de `esq`
    esq->dir = no;
    // atualiza o ponteiro pai de `no` para ser o `esq`
    no->pai = esq;
}


// função auxiliar para corrigir a árvore,
// subindo a partir do novo nó inserido,
// rotacionando e repintando nós para não 
// quebrar nenhuma propriedade
static void arv_insere_fixup(Arvore *arv, No *no) {
    // é necessário corrigir apenas se o pai do novo nó for vermelho.
    // se for preto (incluindo NIL), a árvore não quebra nenhuma propriedade.
    while(no->pai->cor == VERMELHO) {
        No* avo = arv_busca_avo(no);
        No* tio = arv_busca_tio(no);

        // se o pai de `no` for o filho esquerdo do avô
        if (no->pai == avo->esq) {
            // caso 3a: tanto o pai quanto o tio são vermelhos.
            // deve-se repintar ambos de preto e pintar o avô de
            // vermelho. porém o avô vermelho pode ser a raíz ou quebrar
            // a propriedade de seus filhos serem pretos, deve-se continuar
            // o fixup com agora `no` sendo o avô.
            if(tio->cor == VERMELHO) {
                // repinta o pai, o tio e o avô.
                no->pai->cor = PRETO;
                tio->cor = PRETO;
                avo->cor = VERMELHO;
                
                // atualiza `no`, agora é o avô do nó atual
                no = avo;
            }
            // senão, o tio é preto
            else {
                // caso 4a: o pai é vermelho e o tio é preto, `no`
                // é o filho direito de seu pai e seu pai é
                // o filho esquerdo de seu avô.
                // deve-se realizar uma rotação a esquerda, onde o `no`
                // passa a ser o novo pai de seu pai.
                if(no == no->pai->dir) {
                    no = no->pai;
                    arv_rotacao_esquerda(arv, no);
                }

                // caso 5a: o pai é vermelho, o tio é preto, `no` 
                // é o filho esquerdo de seu pai e seu pai é 
                // o filho esquerdo de seu avô.
                // tem que realizar uma rotação a direita no avô e trocar
                // as cores do pai e do avô.
                no->pai->cor = PRETO;
                avo->cor = VERMELHO;
                arv_rotacao_direita(arv, avo);
            
                // chegou aqui, a sub-árvore está corrigida e o loop pode parar,
                // o que vai acontecer já que acabamos de pintar o pai de preto
            }
        }
        // senão, o pai de `no` é o filho direito do avô, apenas espelha as rotações
        else {
            // caso 3b: tanto o pai quanto o tio são vermelhos.
            // deve-se repintar ambos de preto e pintar o avô de
            // vermelho. porém o avô vermelho pode ser a raíz ou quebrar
            // a propriedade de seus filhos serem pretos, deve-se continuar
            // o fixup com agora `no` sendo o avô.
            if(tio->cor == VERMELHO) {
                // repinta o pai, o tio e o avô.
                no->pai->cor = PRETO;
                tio->cor = PRETO;
                avo->cor = VERMELHO;
                
                // atualiza `no`, agora é o avô do nó atual
                no = avo;
            }
            // senão, o tio é preto
            else {
                // caso 4b: o pai é vermelho e o tio é preto, `no`
                // é o filho esquerdo de seu pai e seu pai é
                // o filho direito de seu avô.
                // deve-se realizar uma rotação a direita, onde o `no`
                // passa a ser o novo pai de seu pai.
                if(no == no->pai->esq) {
                    no = no->pai;
                    arv_rotacao_direita(arv, no);
                }

                // caso 5b: o pai é vermelho, o tio é preto, `no` 
                // é o filho direito de seu pai e seu pai é 
                // o filho direito de seu avô.
                // tem que realizar uma rotação a esquerda no avô e trocar
                // as cores do pai e do avô.
                no->pai->cor = PRETO;
                avo->cor = VERMELHO;
                arv_rotacao_esquerda(arv, avo);
            
                // chegou aqui, a sub-árvore está corrigida e o loop pode parar,
                // o que vai acontecer já que acabamos de pintar o pai de preto
            }
        }
    }

    // caso 2: o pai do nó a ser inserido é preto.
    // nenhuma das 5 propriedades é ameaçada, basta inserir

    // caso 1: o nó a inserir será a raiz.
    // é o primeiro nó a ser inserido, então ele deve ser
    // repintado de preto já que é a nova raíz da árvore

    // (nesses casos só pinta a raiz de preto para garantir a propriedade)
    arv->raiz->cor = PRETO;
}

// função auxiliar para alocar o novo nó
static No* arv_cria_no(void *valor, Cor cor, int tam_bytes_dado) {
    No *novo_no = (No*)malloc(sizeof(No));
    if(novo_no == NULL) return NULL;

    novo_no->dado = (void*)malloc(tam_bytes_dado);
    if(novo_no->dado == NULL) {
        free(novo_no);
        return NULL;
    }

    memcpy(novo_no->dado, valor, tam_bytes_dado);
    novo_no->cor = cor;
    novo_no->pai = NIL;
    novo_no->dir = NIL;
    novo_no->esq = NIL;

    return novo_no;
}

bool arv_insere_no(Arvore *arv, void *v) {
    if(arv == NULL) return false;

    // aloca o novo nó com o dado `v`
    // todo nó a ser inserido é pintado de vermelho
    // inicialmente, com possibilidade de ser repintado
    // de preto para não quebrar nenhuma propriedade
    No *novo_no = arv_cria_no(v, VERMELHO, arv->tam_dado);
    if(novo_no == NULL) return false;

    No *pai = NIL;
    No *atual = arv->raiz;
    // procura pela posição de inserção do novo nó
    while(!arv_no_vazio(atual)) {
        pai = atual;
        if(arv->comp(novo_no->dado, atual->dado) < 0) {
            atual = atual->esq;
        } 
        else {
            atual = atual->dir;
        }
    }
    
    // correção dos ponteiros para inserção do novo nó
    novo_no->pai = pai;
    // se o pai for NIL, significa que a árvore estava
    // vazia, vira a nova raíz da árvore
    if(arv_no_vazio(pai)) {
        arv->raiz = novo_no;
    }
    else if(arv->comp(novo_no->dado, pai->dado) < 0) {
        pai->esq = novo_no;
    } 
    else {
        pai->dir = novo_no;
    }

    // chama a função auxiliar para corrigir a árvore
    // para não quebrar nenhuma propriedade
    arv_insere_fixup(arv, novo_no);

    arv->num_nos++;
    return true;
}


// função auxiliar para ajudar na correção da remoção,
// substitui a sub-árvore `sub1` pela sub-árvore `sub2`,
// fazendo com que o pai de `sub1` aponte para `sub2`
static void arv_subtitui_subarv(Arvore *arv, No *sub1, No *sub2) {
    // atualiza o pai de `sub1`
    // se o pai de `sub1` for NIL, significa que `sub1` era raíz
    if(arv_no_vazio(sub1->pai)) {
        // `sub2` vira a nova raíz
        arv->raiz = sub2;
    }
    // se `sub1` era um filho esquerdo
    else if(sub1 == sub1->pai->esq) {
        // `sub2` agora é o novo filho esquerdo
        sub1->pai->esq = sub2;
    } 
    // se `sub1` era um filho direito
    else {
        // `sub2` agora é o novo filho direito
        sub1->pai->dir = sub2;
    }

    // agora, atualiza o pai de `sub2`
    // o novo pai de `sub2` é o antigo pai de `sub1`

    // importante, mesmo que `sub2` seja um NIL, porque no fixup da remoção
    // podemos precisar navegar para cima de `sub2`
    // como o pai de um NIL só é utilizado pela fixup de uma remoção, não
    // precisamos nos preocupar em restaurar o pai de um NIL para NIL denovo
    sub2->pai = sub1->pai;
}


// função auxiliar para fazer a correção da árvore
// partindo do nó `no` (que tem um "preto extra", quebrando
// a altura-preta), e navegando para cima
static void arv_remove_fixup(Arvore *arv, No *no) {
    // enquanto `no` não for a raiz e ainda ter um "preto extra"
    // (se for raíz já está válida novamente a árvore também)
    while(no != arv->raiz && no->cor == PRETO) {
        // se `no` é um filho esquerdo
        if(no == no->pai->esq) {
            // busca o irmao
            No *irmao = no->pai->dir;

            // caso 1a: o irmão de `no` é vermelho
            // deve trocar as cores do pai e do irmão,
            // além de rotacionar o pai para esquerda
            if(irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                no->pai->cor = VERMELHO;
                arv_rotacao_esquerda(arv, no->pai);
                
                // atualiza o `irmao` para o novo irmão de `no`, que
                // agora deve ser preto
                irmao = no->pai->dir;
                
                // o problema continua em um dos casos abaixo: 2a, 3a ou 4a
            }

            // caso 2a: o irmão `irmao` é preto e seus 2 filhos são pretos
            // o irmão é pintado de vermelho, "empurrando" o problema
            // do "preto-extra" para cima
            if(irmao->esq->cor == PRETO && irmao->dir->cor == PRETO) {
                // pinta `irmao` de vermelho
                irmao->cor = VERMELHO;
                
                // atualiza o novo `no` do loop, que passa a ser o pai,
                // já que o "preto extra" foi para cima
                no = no->pai;
            }
            // caso 3a ou 4a: o irmão `irmao` é preto e tem tem pelo menos 1 filho vermelho
            else {
                // caso 3a: `irmao` é preto, seu filho esquerdo é vermelho e
                // seu filho direito é preto
                // deve repintar o filho esquerdo para preto, o `irmao` para
                // vermelho e rotacionar o `irmao` para direita
                if(irmao->dir->cor == PRETO) {
                    // pinta o filho esquerdo de preto
                    irmao->esq->cor = PRETO;
                    // pinta o `irmao` de vermelho
                    irmao->cor = VERMELHO;
                    // rotaciona para direita o `irmao`
                    arv_rotacao_direita(arv, irmao);
                    
                    // `irmao` agora é o novo irmão depois da rotação
                    irmao = no->pai->dir;
                }

                // caso 4a: `irmao` é preto e seu filho direito é vermelho
                // deve remover o "preto extra", dessa vez é definitivo
                // o `irmao` herda a cor de seu pai
                irmao->cor = no->pai->cor;
                // o pai é pintado de preto
                no->pai->cor = PRETO;
                // filho direito de `irmao` é pintado de preto
                irmao->dir->cor = PRETO;
                // rotaciona o pai de `no` para esquerda
                arv_rotacao_esquerda(arv, no->pai);
                

                // depois disso, o "preto extra" foi resolvido
                // para sair do loop atualiza `no` para a raíz da árvore
                no = arv->raiz;
            }
        }
        // senão, `no` é um filho direito (apenas espelha os casos a)
        else {
            // busca o irmao
            No *irmao = no->pai->esq;

            // caso 1b: o irmão de `no` é vermelho
            // deve trocar as cores do pai e do irmão,
            // além de rotacionar o pai para direita
            if(irmao->cor == VERMELHO) {
                irmao->cor = PRETO;
                no->pai->cor = VERMELHO;
                arv_rotacao_direita(arv, no->pai);

                // atualiza o `irmao` para o novo irmão de `no`, que
                // agora deve ser preto
                irmao = no->pai->esq;

                // o problema continua em um dos casos abaixo: 2b, 3b ou 4b
            }

            // caso 2b: o irmão `irmao` é preto e seus 2 filhos são pretos
            // o irmão é pintado de vermelho, "empurrando" o problema
            // do "preto-extra" para cima
            if(irmao->esq->cor == PRETO && irmao->dir->cor == PRETO) {
                irmao->cor = VERMELHO;
                no = no->pai;
            }
            
            // caso 3b ou 4b: o irmão `irmao` é preto e tem tem pelo menos 1 filho vermelho
            else {
                // caso 3a: `irmao` é preto, seu filho direito é vermelho e
                // seu filho esquerdo é preto
                // deve repintar o filho direito para preto, o `irmao` para
                // vermelho e rotacionar o `irmao` para esquerda
                if(irmao->esq->cor == PRETO) {
                    // pinta o filho direito de preto
                    irmao->dir->cor = PRETO;
                    // pinta o `irmao` de vermelho
                    irmao->cor = VERMELHO;
                    // rotaciona para esquerda o `irmao`
                    arv_rotacao_esquerda(arv, irmao);

                    // `irmao` agora é o novo irmão depois da rotação
                    irmao = no->pai->esq;
                }

                // caso 4b: `irmao` é preto e seu filho esquerdo é vermelho
                // deve remover o "preto extra", dessa vez é definitivo
                // o `irmao` herda a cor de seu pai
                irmao->cor = no->pai->cor;
                // o pai é pintado de preto
                no->pai->cor = PRETO;
                // filho esquerdo de `irmao` é pintado de preto
                irmao->esq->cor = PRETO;
                // rotaciona o pai de `no` para direita
                arv_rotacao_direita(arv, no->pai);
                
                // depois disso, o "preto extra" foi resolvido
                // para sair do loop atualiza `no` para a raíz da árvore
                no = arv->raiz;
            }
        }
    }

    // `no` pode ter chegado a raiz, deve ser preto, e garantindo
    // ser preto corrige qualquer "preto extra" que sobrou e vira preto.
    // há também o caso em que um nó vermelho absorveu o "preto extra",
    // que corrige também
    no->cor = PRETO;
}

bool arv_remove_no(Arvore *arv, void *v) {
    if(arv == NULL) return false;

    // busca o nó a remover
    No *no_buscado = arv_busca_no(arv->raiz, v, arv->comp);
    // se não encontrou, retorna false
    if(arv_no_vazio(no_buscado)) return false;

    // `no_remover` é o  ponteiro para o nó que realmente será removido,
    // vamos copiar o conteudo do sucessor para cima, facilitando
    // a remoção (se tiver 2 filhos)
    No *no_remover = no_buscado;
    
    // se `no_buscado` tiver 2 filhos, copiamos o conteúdo do sucessor para
    // `no_buscado` (que está acima) e removemos o sucessor dele
    if(!arv_no_vazio(no_buscado->esq) && !arv_no_vazio(no_buscado->dir)) {
        // pega o menor dos sucessores partindo do `no_buscado`
        No *sucessor = arv_busca_minimo(no_buscado->dir);
        // copia o dado para `no_buscado`
        memcpy(no_buscado->dado, sucessor->dado, arv->tam_dado);
        // agora, o nó realmente a remover é esse sucessor que teve seu
        // dado copiado
        no_remover = sucessor;
    }

    // agora, `no_remover` pode ter no máximo 1 filho
    // (ou é o `no_buscado`, ou é o menor sucessor)

    // acha o nó para substituir esse que será realmente removido,
    // que sera seu filho não NIL ou NIL se o `no_remover`
    // for um nó sem filhos
    No *no_substituto;
    if(arv_no_vazio(no_remover->esq)) {
        no_substituto = no_remover->dir;
    }
    else {
        no_substituto = no_remover->esq;
    }

    // salva a cor do `no_remover` antes de substitui a sub-árvore
    Cor cor_removido = no_remover->cor;

    // substitui a sub-árvore de `no_remover` pelo `no_substituto`
    // (o pai de `no_remover` passa a apontar para `no_substituto`)
    arv_subtitui_subarv(arv, no_remover, no_substituto);

    // se a cor do nó removido for preta, a propriedade 5 (altura-preta)
    // foi quebrada, devemos corrigir a partir de `no_substituto`
    if(cor_removido == PRETO) {
        arv_remove_fixup(arv, no_substituto);
    }

    // libera o nó realmente removido
    // mas primeiro libera o dado do nó
    free(no_remover->dado);
    free(no_remover);
    arv->num_nos--;
    return true;
}


//// --- consultas ---

bool arv_vazia(Arvore *arv) {
    if(arv == NULL) return true;
    return arv->num_nos == 0;
}

bool arv_no_vazio(No *no) {
    // checar o NULL também por mais segurança
    return no == NIL || no == NULL;
}

int arv_nnos(Arvore *arv) {
    if(arv == NULL) return 0;
    return arv->num_nos;
}

No* arv_busca_raiz(Arvore *arv) {
    if(arv == NULL) return NIL;
    return arv->raiz;
}

No* arv_busca_filho(No *no, bool dir) {
    if(arv_no_vazio(no)) return NIL;

    if(dir) {
        return no->dir;
    }
    return no->esq;
}

Cor arv_busca_cor(No *no) {
    if(arv_no_vazio(no)) return PRETO;

    return no->cor;
}

No* arv_busca_pai(No *no) {
    if(arv_no_vazio(no)) return NIL;

    return no->pai;
}

bool arv_busca_valor(No *no, void *retorno, int tam_bytes_dado) {
    if(arv_no_vazio(no)) return false;
    if(retorno == NULL) return false;

    memcpy(retorno, no->dado, tam_bytes_dado);
    return true;
}

No* arv_busca_no(No *raiz, void *v, Comparador *comp) {
    if(arv_no_vazio(raiz)) return NIL;

    No *atual = raiz;
    while(!arv_no_vazio(atual)) {
        int resultado_comp = comp(v, atual->dado);

        if(resultado_comp == 0) {
            return atual;
        }
        else if(resultado_comp < 0) {
            atual = atual->esq;
        }
        else {
            atual = atual->dir;
        }
    }

    // se chegou aqui, não achou
    return NIL;
}

bool arv_contem(Arvore *arv, void *v) {
    if(arv == NULL) return false;

    return arv_busca_no(arv->raiz, v, arv->comp) != NIL;
}

// função auxiliar para calcular a altura da árvore
// de forma recursiva
static int arv_altura_rec(No *no) {
    if(arv_no_vazio(no)) return 0;

    int altura_esq = arv_altura_rec(no->esq);
    int altura_dir = arv_altura_rec(no->dir);

    if(altura_esq > altura_dir) {
        return 1 + altura_esq;
    } 
    else {
        return 1 + altura_dir;
    }
}

int arv_altura(Arvore *arv) {
    if(arv_vazia(arv)) return 0;
    
    return arv_altura_rec(arv->raiz);
}

No* arv_busca_minimo(No *raiz) {
    if(arv_no_vazio(raiz)) return NIL;

    No *menor = raiz;

    while(!arv_no_vazio(menor->esq)) {
        menor = menor->esq;
    }

    return menor;
}

No* arv_busca_maximo(No *raiz) {
    if(arv_no_vazio(raiz)) return NIL;

    No *maior = raiz;

    while(!arv_no_vazio(maior->dir)) {
        maior = maior->dir;
    }

    return maior;
}
