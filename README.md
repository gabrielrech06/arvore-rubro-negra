# Implementação de Árvore Rubro-Negra (Red-Black Tree) em C

## 1. Descrição

Este projeto é uma implementação de uma Árvore Rubro-Negra (Red-Black Tree), desenvolvida em linguagem C para a disciplina de Estrutura de Dados da Universidade Federal de Santa Maria (UFSM).

Uma Árvore Rubro-Negra é uma árvore de busca binária auto-balanceada que garante que as operações básicas de inserção, remoção e busca mantenham um desempenho eficiente no pior caso. Isso é alcançado através de cinco propriedades fundamentais que são mantidas por meio de rotações e recolorações sempre que a árvore é modificada.

As propriedades da Árvore Rubro-Negra são:
  1. Todo nó é vermelho ou preto.
  2. A raiz é sempre preta.
  3. Toda folha (nó `NIL`) é preta.
  4. Se um nó é vermelho, então ambos os seus filhos são pretos.
  5. Todo caminho de um dado nó para qualquer de seus nós folhas descendentes contem o mesmo número de nós pretos.

<img width="960" height="471" alt="image" src="https://github.com/user-attachments/assets/71100f82-5784-4d3e-8f35-a137f36e0741" />

## 2. Funcionalidades Implementadas

O programa implementa as seguintes operações essenciais para a manipulação de uma Árvore Rubro-Negra:
  1. Inserção: Adiciona um novo elemento à árvore, seguindo as regras de uma árvore de busca binária e, em seguida, executa um procedimento de correção para restaurar as propriedades da árvore caso alguma tenha sido violada.
  2. Remoção: Exclui um elemento da árvore, tratando todos os casos possíveis e aplicando as devidas correções para garantir que o balanceamento e as propriedades da árvore sejam mantidos.
  3. Busca: Procura por um valor específico, aproveitando a ordenação da árvore para encontrar o elemento de forma eficiente.

## 3. Complexidade

Esta seção detalha os requisitos de tempo (quão rápido as operações são executadas) e de espaço (quanta memória a estrutura utiliza) da Árvore Rubro-Negra.

### Complexidade de Tempo (Time Complexity)

A principal vantagem da Árvore Rubro-Negra é a sua garantia de desempenho logarítmico no pior caso para as operações dinâmicas. A altura da árvore, h, é mantida em *O(logn)*, onde n é o número de nós.

A tabela abaixo detalha a complexidade de tempo para as operações fundamentais:

| Operação  | Caso Médio | Pior Caso | Justificativa |
| ------------- | ------------- | ------------- | ------------- |
| **Busca**  | *O(logn)*  | *O(logn)*  | A busca percorre um caminho da raiz até uma folha. Como a altura é garantida como logarítmica, a busca também é.  |
| **Inserção**  | *O(logn)*  | *O(logn)*  | A inserção consiste em uma busca (*O(logn)*) seguida por um número constante de rotações e recolorações para corrigir a árvore.  |
| **Remoção**  | *O(logn)*  | *O(logn)*  | Similar à inserção, a remoção envolve encontrar o nó e depois aplicar um número limitado de correções para manter as propriedades.  |

### Complexidade de Espaço (Space Complexity)

A complexidade de espaço refere-se à quantidade total de memória necessária para armazenar a estrutura de dados.
  * Complexidade: *O(n)*
  * Justificativa: A árvore precisa armazenar n nós. Cada nó ocupa uma quantidade constante de espaço (*O(1)*) para guardar seu valor, sua cor, e os ponteiros para o pai, filho esquerdo e filho direito. Portanto, o espaço total utilizado pela árvore é diretamente proporcional ao número de nós.


## 4. Exemplo de Uso

O exemplo abaixo demonstra o ciclo de vida básico e as principais operações do TAD da Árvore Rubro-Negra. Para mais detalhes e exemplos, consultar o arquivo `main.c`.

```
#include <stdio.h>
#include <stdlib.h>
#include "arvore-rn.h"

int main() {
    // 1. criação
    Arvore *arv = arv_cria();
    printf("árvore criada. nós: %d.\n", arv_nnos(arv));
    printf("----------------------------------\n");
    
    // 2. inserção
    printf("inserindo os nós 10, 20, 5 e 30.\n");
    arv_insere_no(arv, 10);
    arv_insere_no(arv, 20);
    arv_insere_no(arv, 5);
    arv_insere_no(arv, 30);

    // 3. consultas sobre a árvore
    printf("número de nós: %d.\n", arv_nnos(arv));
    printf("altura da árvore: %d.\n", arv_altura(arv));

    if(arv_contem(arv, 20)) {
        printf("o nó 20 está contido na árvore.\n");
    }

    if(!arv_contem(arv, 99)) {
        printf("o nó 99 não está contido na árvore.\n");
    }
    printf("----------------------------------\n");

    // 4. remoção
    printf("removendo o nó 10.\n");
    arv_remove_no(arv, 10);
        
    if(!arv_contem(arv, 10)) {
        printf("o nó 10 não está mais contido.\n");
    }

    // 5. consulta de min/max e pai
    No *raiz = arv_busca_raiz(arv);
    No *no_min = arv_busca_minimo(raiz);
    No *no_30 = arv_busca_no(raiz, 30);
    No *pai_do_30 = arv_busca_pai(no_30);

    int valor_min, valor_pai;
    arv_busca_valor(no_min, &valor_min);
    arv_busca_valor(pai_do_30, &valor_pai);
    
    printf("valor mínimo da árvore: %d.\n", valor_min);
    printf("o pai do nó 30 é: %d.\n", valor_pai);
    printf("----------------------------------\n");

    // 6. liberação da árvore
    arv_libera_arvore(arv);

    return 0;
}
```

## 5. Referências

  * https://pt.wikipedia.org/wiki/%C3%81rvore_rubro-negra
  * https://www.ime.usp.br/~song/mac5710/slides/08rb.pdf
  * https://www.ic.unicamp.br/~zanoni/teaching/mo637/2007-2s/aulas/arvoresRubroNegras.pdf
