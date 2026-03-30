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
     fprintf(file, "%ld %s %s\n", matricula, nome, telefone);

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
    long mat;
    fseek(file, offset, SEEK_SET);
    if (fscanf(file, "%ld %s %s", &mat, nome, telefone) == 3)
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

    for (int j = 0; j < (MAX_CHAVES - meio - 1); j++)
    {
        //copia as chaves apos o meio para o novo filho
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
void gravaArvoreRec(ArvB *arvore, FILE *file){
    if(arvore == NULL){
        return;
    }
    fprintf(file, "%p %d", arvore, arvore->n);

    for(int i = 0; i < arvore->n; i++){
        fprintf(file, " %ld %ld", arvore->chaves[i].matricula, arvore->chaves[i].offset);
    }

    for(int i = 0; i < arvore->n + 1; i++){
        fprintf(file, " %p", arvore->filhos[i]);
    }
    fprintf(file, "\n");
    
    for(int i = 0; i < arvore->n + 1; i++){
        gravaArvoreRec(arvore->filhos[i], file);
    }
}
void gravaArvore(ArvB *arvore, char *nomeArquivo){
    if(arvore == NULL){
        return;
    }
    FILE *file = fopen(nomeArquivo, "w");
    if(file == NULL){
        return;
    }
    fprintf(file, "%p\n", arvore);
    gravaArvoreRec(arvore, file);

    fclose(file);
}
int main(int argc, char *argv[])
{
    if(argc < 3){
        printf("Uso: %s <arquivo_registros> <arquivo_arvore>\n", argv[0]);
        return 1;
    }

    ArvB *arvore = criaArvoreB();
    
    FILE *file = fopen(argv[1], "r");
    if(file != NULL){
        long matricula;
        char nome[100], telefone[20];
        while(1){
            long offset = ftell(file);
            if(fscanf(file, "%ld %s %s", &matricula, nome, telefone) != 3) break;
            Registro r;
            r.matricula = matricula;
            r.offset = offset;
            arvore = insereArvore(arvore, r);
        }
        fclose(file);
    }

    int opcao;
    do {
        printf("\n1. Cadastrar\n2. Pesquisar\n3. Gravar\n4. Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        switch(opcao){
            case 1: {
                long matricula;
                char nome[100], telefone[20];
                printf("Matricula: "); scanf("%ld", &matricula);
                printf("Nome: ");     scanf("%s", nome);
                printf("Telefone: "); scanf("%s", telefone);
                Registro *r = cadastrar(matricula, nome, argv[1], telefone);
                if(r != NULL){
                    arvore = insereArvore(arvore, *r);
                    free(r);
                }
                break;
            }
            case 2:
                chamaBusca(arvore, argv[1]);
                break;
            case 3:
                gravaArvore(arvore, argv[2]);
                break;
            case 4:
                destroiArv(arvore);
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 4);

    return 0;
}