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

[colocar aqui um trecho da main.c que vai ser usada para exemplificar o uso do TAD]
