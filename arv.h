#ifndef ARVORE_H
#include <stdio.h>
#define ARVORE_H

#define ORDEM 3
#define MAX_CHAVES (ORDEM - 1)
#define MAX_FILHOS (ORDEM)

typedef struct _registro
{
    long matricula;
    long offset;
} Registro;

typedef struct _pagina
{
    int n;
    Registro chaves[MAX_CHAVES];
    struct _pagina *filhos[MAX_FILHOS];
    int folha;
} Pagina;

typedef Pagina ArvB;

Pagina *alocaPagina(int folha);
ArvB *criaArvoreB();
Registro *buscaArvoreB(ArvB *arvore, long matricula);
int buscaArquivo(char *nomeArquivo, long offset);
Registro *cadastrar(long matricula, char *nome, char *nomeArquivo, char *telefone);
void destroiArv(ArvB *arvore);
void gravaArvore(ArvB *arvore, char *nomeArquivo);
void gravaArvoreRec(ArvB *arvore, FILE *file);
void insereRaiz(ArvB *arvore, Registro r);
void splitFilho(ArvB *pai, int i);
ArvB *insereArvore(ArvB *arvore, Registro r);
int chamaBusca(ArvB *arvore, char *nomeArquivo);
#endif