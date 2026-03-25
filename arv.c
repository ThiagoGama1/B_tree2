#include <stdio.h>
#include <stdlib.h>
#include "arv.h"

Pagina *alocaPagina(int folha)
{
    Pagina *novaPagina = malloc(sizeof(*novaPagina));
    if (novaPagina == NULL)
    {
        return NULL;
    }
    novaPagina->n = 0;
    novaPagina->folha = folha;
    for (int i = 0; i < MAX_FILHOS; i++)
    {
        novaPagina->filhos[i] = NULL;
    }
    return novaPagina;
}
Registro *cadastrar(long matricula, char *nome, char *telefone)
{ // TO DO
}

ArvB *criaArvoreB()
{
    return alocaPagina(1);
}

Registro *buscaArvoreB(ArvB *arvore, long matricula)
{
    if (arvore == NULL)
    {
        return NULL;
    }
    int i = 0;
    while (i < arvore->n)
    {
        if (arvore->chaves[i].matricula == matricula)
        {
            return &(arvore->chaves[i]);
        }
        if (arvore->chaves[i].matricula > matricula)
        {
            return buscaArvoreB(arvore->filhos[i], matricula);
        }
        i++;
    }
    if (arvore->folha)
    {
        return NULL;
    }
    return buscaArvoreB(arvore->filhos[i], matricula);
}
int buscaArquivo(char *nomeArquivo, long offset)
{
    FILE *file = fopen(nomeArquivo, "r");
    char nome[100];
    char telefone[20];
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return 0;
    }
    fseek(file, offset, SEEK_SET);
    if (fscanf(file, "%s %s", nome, telefone) == 2)
    {
        printf("Nome: %s, Telefone: %s", nome, telefone);
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}
int chamaBusca(ArvB *arvore, char *nomeArquivo)
{
    long matricula;
    printf("Informe a matricula: ");
    scanf("%li", &matricula);

    Registro *r = buscaArvoreB(arvore, matricula);
    if (r == NULL)
    {
        return 0;
    }
    return buscaArquivo(nomeArquivo, r->offset);
}
void insereRaiz(ArvB *arvore, Registro r)
{
    if (arvore->folha == 1)
    {
        int i = arvore->n - 1;
        while (i >= 0 && arvore->chaves[i].matricula > r.matricula)
        {
            arvore->chaves[i + 1] = arvore->chaves[i];
            i--;
        }
        arvore->chaves[i + 1].matricula = r.matricula;
        arvore->chaves[i + 1].offset = r.offset;
        arvore->n++;
    }
    else
    {
        int i = 0;
        while (i < arvore->n && arvore->chaves[i].matricula < r.matricula)
        {
            i++;
        }
        if(arvore->filhos[i]->n > MAX_CHAVES){
            //split
        }
        else{
            insereRaiz(arvore->filhos[i], r);
        }
    }
}
void splitFilho(ArvB *pai, int i){
    ArvB *filhoCheio = pai->filhos[i];
    ArvB *novoFilho = alocaPagina(filhoCheio->folha);
    int meio = MAX_CHAVES / 2;
    int k = pai->n - 1;

    for(int j = 0; j < meio; j++){
        novoFilho->chaves[j] = filhoCheio->chaves[j + meio + 1];
    }
    novoFilho->n = meio;
    filhoCheio->n = meio;
    while(k >= i){
        pai->chaves[k+1] = pai->chaves[k];
        pai->filhos[k+2] = pai->filhos[k+1];
        k--;
    }
}
ArvB *insereArvore(ArvB *arvore, Registro r)
{
    if (arvore == NULL)
    {
        ArvB *novoRegistro = alocaPagina(1);
        novoRegistro->chaves[0].matricula = r.matricula;
        novoRegistro->chaves[0].offset = r.offset;
        novoRegistro->n = 1;
        return novoRegistro;
    }
    if (arvore->n >= MAX_CHAVES)
    {
        ArvB *novaRaiz = alocaPagina(0);
    }
}

int main(int argc, char *argv[])
{
    ArvB *arvore = criaArvoreB();
    return 0;
}