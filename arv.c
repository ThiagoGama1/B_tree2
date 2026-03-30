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
Registro *cadastrar(long matricula, char *nome, char *nomeArquivo, char *telefone)
{
     FILE *file = fopen(nomeArquivo, "a");
     if(file == NULL){
        perror("Erro ao abrir arquivo");
        return NULL;
     }
     long offsetAtual = ftell(file); //ftell retorna a posicao atual do ponteiro no arquivo
     fprintf(file, "%s %s\n", nome, telefone);

     fclose(file);
     Registro *r = malloc(sizeof(*r));
     r->matricula = matricula;
     r->offset = offsetAtual;
     return r;
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
        if (arvore->filhos[i]->n == MAX_CHAVES)
        {
            splitFilho(arvore, i);
            
            if(arvore->chaves[i].matricula < r.matricula){
                i++;
            }
        }
            insereRaiz(arvore->filhos[i], r);
    }
}
void splitFilho(ArvB *pai, int i)
{
    ArvB *filhoCheio = pai->filhos[i];
    ArvB *novoFilho = alocaPagina(filhoCheio->folha);
    int meio = MAX_CHAVES / 2;
    int k = pai->n - 1;

    for (int j = 0; j < meio; j++) 
    {
        //como a ordem é 3, nao copia nada, mas 
        //o codigo funciona caso a ordem seja alterada eventualmente
        novoFilho->chaves[j] = filhoCheio->chaves[meio + 1 + j];
    }
    novoFilho->n = MAX_CHAVES - meio - 1;
    filhoCheio->n = meio;
    while (k >= i)
    {
        pai->chaves[k + 1] = pai->chaves[k];
        pai->filhos[k + 2] = pai->filhos[k + 1];
        k--;
    }
    pai->chaves[i] = filhoCheio->chaves[meio];
    pai->filhos[i + 1] = novoFilho; // i+1 porque pai->chaves[i] é a chave do meio, e novofilho tem as chaves do meio para a direita (maiores que o meio)
    pai->n++;
    
    if(filhoCheio->folha == 0){
        int c = meio + 1;
        while(c <= MAX_CHAVES){
            novoFilho->filhos[c - meio - 1] = filhoCheio->filhos[c];
            c++;
        }
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
    if (arvore->n >= MAX_CHAVES) //se a raiz estiver cheia, cria nova raiz e split
    {
        ArvB *novaRaiz = alocaPagina(0);
        novaRaiz->filhos[0] = arvore;
        splitFilho(novaRaiz, 0);
        insereRaiz(novaRaiz, r);
        return novaRaiz;
    }
    else{ //se a raiz nao estiver cheia
        insereRaiz(arvore, r);
        return arvore;
    }
}
void destroiArv(ArvB *arvore){
    if(arvore == NULL){
        return;
    }
    for(int i = 0; i < arvore->n + 1; i++){
        destroiArv(arvore->filhos[i]);
    }
    free(arvore);
}
//falta o gravar e a main

int main(int argc, char *argv[])
{
    ArvB *arvore = criaArvoreB();
    return 0;
}