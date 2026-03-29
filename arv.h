#ifndef ARVORE_H
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
Registro *cadastrar(long matricula, char *nome, char *telefone);

#endif