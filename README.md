# B_tree2

Implementação de árvore B em C, feita pra uma disciplina de estruturas de dados. Segunda versão — a primeira tá no repositório [b_tree](https://github.com/ThiagoGama1/b_tree).

Árvore B é uma estrutura balanceada usada bastante em bancos de dados e sistemas de arquivo. A ideia é manter os nós com múltiplas chaves e dividir quando cheiam, o que mantém a árvore balanceada e as operações em O(log n).

## O que tem implementado

- Inserção com split de nós
- Busca por chave
- Exibição da estrutura

## Arquivos

- `arv.c` — implementação
- `arv.h` — definições e protótipos

## Como compilar

```bash
gcc arv.c -o arv
./arv
```
