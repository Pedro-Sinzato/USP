/* Autores: Pedro Sinzato e Mauricio Pires
 Revisao: Eric Azuma
 Objetivo: Simular o sistema de inventario de uma loja com funcoes para manipular o estoque,
 executar vendas e salvar as informacoes em um arquivo txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SALDO_INICIAL 100 /* saldo base estabelecido quando o arquivo estoque.txt nao existia */
#define FIM_DIA -1        /* Valor retornado pela funcao ProcessarComando quando recebe o comando FinalizarDia \
           para terminar o programa*/

struct item_estoque
{
    char *nome;
    int quantidade;
    float valor;
};

char *LerNome();
char *LerNomeArquivo(FILE *arquivo);
void LerArquivo(struct item_estoque **Estoque, int *num_itens, int *tamanho_estoque, float *saldo);
void ImprimirDivisoria();
void RealocacaoEstoque(struct item_estoque **Estoque, int *tamanho_estoque);
void InsereProduto(struct item_estoque **Estoque, int *num_itens, int *tamanho_estoque);
void AumentarEstoque(struct item_estoque *Estoque, int num_itens, float *saldo);
void VenderItem(struct item_estoque *Estoque, int num_itens, float *saldo);
void ModificarPreco(struct item_estoque *Estoque, int num_itens);
void ConsultaEstoque(struct item_estoque *Estoque, int num_itens);
void ConsultaSaldo(float saldo);
int ProcessarComando(char *escolha, struct item_estoque **Estoque, int *num_itens, int *tamanho_estoque, float *saldo);
void FinalizarDia(struct item_estoque **Estoque, int num_itens, int tamanho_estoque, float saldo);

/**
 @brief Funcao principal que le o arquivo de estoque (estoque.txt) e comeca a ler os comandos
 colocados no STDIN ate encontrar o comando de finalizar o dia.
 */
int main(void)
{
    int tamanho_estoque; /* tamanho do vetor de structs Estoque */
    int num_itens;       /* numero de itens que estao no vetor */
    float saldo;
    struct item_estoque *Estoque; /* Vetor de structs */

    LerArquivo(&Estoque, &num_itens, &tamanho_estoque, &saldo);

    char escolha[3];
    int retorno = 0;

    while (retorno != FIM_DIA) /* continuamente requisita comandos ate a funcao ProcessarComando rodar a funcao FinalizarDia (que retorna -1) */
    {
        scanf(" %2s", escolha); /* le o comando dado pelo usuario que corresponde a uma das funcoes */
        retorno = ProcessarComando(escolha, &Estoque, &num_itens, &tamanho_estoque, &saldo);
    }

    return 0;
}
/* fim da funcao principal */

/**
 @brief Le o nome dos produtos no arquivo

 le o nome dos produtos no arquivo contendo as informacoes do estoque de iteracoes passadas do programa.

 @param arquivo - ponteiro para o arquivo que contem informacoes do estoque.

 @return retorna uma string (com espaco exato alocado) contendo o nome do produto
 */
char *LerNomeArquivo(FILE *arquivo)
{
    char string_auxiliar[100];
    fscanf(arquivo, " %99s", string_auxiliar);

    char *nome;
    nome = (char *)malloc(sizeof(char) * (strlen(string_auxiliar) + 1));
    strcpy(nome, string_auxiliar);

    return (nome);
}

/**
 @brief Le o nome dos produtos no arquivo

 le o nome dos produtos no arquivo contendo as informacoes do estoque de iteracoes passadas do programa.

 @param arquivo ponteiro para o arquivo que contem informacoes do estoque.

 @return retorna uma string (com espaco exato alocado) contendo o nome do produto
 */
char *LerNome()
{
    char string_auxiliar[100];
    scanf(" %99s", string_auxiliar);

    char *nome;
    nome = (char *)malloc(sizeof(char) * (strlen(string_auxiliar) + 1));
    strcpy(nome, string_auxiliar);

    return (nome);
}

/**
 @brief Le (caso exista) o arquivo de info do estoque

 abre o arquivo estoque.txt no modo READ (armazenando seu endereco na variavel arquivo), aloca a matriz de itens Estoque,
 coloca os itens salvos no arquivo estoque.txt na matriz alocada, le o valor do saldo salvo no e fecha o arquivo.

 Caso nao exista (ou nao encontre) o arquivo estoque.txt, aloca o vetor com o valor dado pelo usuario e estabelece o saldo
 inicial com base na constante definida no inicio do codigo

 @param Estoque - ponteiro para o vetor Estoque.
 @param num_itens - ponteiro para a variavel num_itens
 @param tamanho_estoque - ponteiro para a variavel tamanho_estoque
 @param saldo - ponteiro para a variavel saldo
 */
void LerArquivo(struct item_estoque **Estoque, int *num_itens, int *tamanho_estoque, float *saldo)
{

    FILE *arquivo = fopen("estoque.txt", "r");

    if (arquivo == NULL)
    {
        *num_itens = 0;
        *saldo = SALDO_INICIAL;

        scanf("%d", tamanho_estoque);

        *Estoque = (struct item_estoque *)malloc(sizeof(struct item_estoque) * (*tamanho_estoque));
        return;
    }

    fscanf(arquivo, "%d %d %f", num_itens, tamanho_estoque, saldo);

    *Estoque = (struct item_estoque *)malloc(sizeof(struct item_estoque) * (*tamanho_estoque));

    for (int i = 0; i < *num_itens; i++)
    {
        (*Estoque)[i].nome = LerNomeArquivo(arquivo);
        fscanf(arquivo, "%d %f", &(*Estoque)[i].quantidade, &(*Estoque)[i].valor);
    }

    fclose(arquivo);
    return;
}

/**
 @brief Imprime a divisoria entre comandos (os 50 hifens)
 */
void ImprimirDivisoria()
{
    for (int i = 0; i < 50; i++)
        printf("-");

    printf("\n");
}

/**
 @brief Realoca o vetor de itens, Estoque, e dobra o seu tamanho

 @param Estoque - ponteiro para o vetor de itens, Estoque
 @param tamanho_estoque - ponteiro para a variavel tamanho_estoque
 */
void RealocacaoEstoque(struct item_estoque **Estoque, int *tamanho_estoque)
{
    *tamanho_estoque *= 2;
    *Estoque = (struct item_estoque *)realloc(*Estoque, sizeof(struct item_estoque) * (*tamanho_estoque));
}

/**
 @brief Funcao que coloca um produto novo no vetor Estoque

 Checa tambem se colocar um item novo superaria a capacidade do vetor, e se sim, realoca-o

 @param Estoque - ponteiro para o vetor Estoque
 @param num_itens - ponteiro para a variavel num_itens
 @param tamanho_estoque - ponteiro para a variavel tamanho_estoque
 */
void InsereProduto(struct item_estoque **Estoque, int *num_itens, int *tamanho_estoque)
{
    if (*num_itens == *tamanho_estoque)
    {
        RealocacaoEstoque(Estoque, tamanho_estoque);
    }
    (*Estoque)[*num_itens].nome = LerNome();
    scanf("%d %f", &(*Estoque)[*num_itens].quantidade, &(*Estoque)[*num_itens].valor);
    (*num_itens)++;
}

/**
 @brief Aumenta o estoque de um item ja no vetor

 le o codigo de um item ja no vetor e aumenda o estoque dele em uma quantidade dada pelo Usuario,
 tambem subtrai o valor necessario do saldo para reestocar o item

 @param Estoque - vetor Estoque
 @param saldo - ponteiro para a variavel saldo
 */
void AumentarEstoque(struct item_estoque *Estoque, int num_itens, float *saldo)
{
    int codigo;
    int adicionar_quantidade;
    scanf("%d %d", &codigo, &adicionar_quantidade);

    Estoque[codigo].quantidade += adicionar_quantidade;
    *saldo -= adicionar_quantidade * Estoque[codigo].valor;
}

/**
 @brief  Faz o programa entrar em um "estado de venda", vendendo os itens dos codigos providenciados ate o usuario digitar -1

 le o codigo de um item e vende ele, diminuindo o seu estoque em 1 e aumentando o saldo no valor de venda do item.
 Alem disso, utiliza uma variavel acumuladora (valor_total) para, apos a venda do ultimo item, imprimir o total do saldo ganho nas vendas

 @param Estoque - vetor Estoque
 @param num_itens - variavel num_itens
 @param saldo - ponteiro para a variavel saldo
 */
void VenderItem(struct item_estoque *Estoque, int num_itens, float *saldo)
{
    float valor_total = 0;
    int codigo;
    scanf("%d", &codigo);

    while (codigo != -1)
    {
        Estoque[codigo].quantidade--;
        *saldo += Estoque[codigo].valor;
        valor_total += Estoque[codigo].valor;
        printf("%s %.2f\n", Estoque[codigo].nome, Estoque[codigo].valor);
        scanf("%d", &codigo);
    }

    printf("Total: %.2f\n", valor_total);
    ImprimirDivisoria();
}

/**
 @brief Permite o usuario mudar o preco de um item ja no vetor

 @param Estoque - vetor Estoque
 @param num_itens - variavel num_itens
 */
void ModificarPreco(struct item_estoque *Estoque, int num_itens)
{
    int codigo;
    float novo_preco;
    scanf("%d %f", &codigo, &novo_preco);
    Estoque[codigo].valor = novo_preco;
}

/**
 @brief Imprime o estoque de todos os itens no vetor

 @param Estoque - ponteiro para o vetor Estoque
 @param num_itens - variavel num_itens
 */
void ConsultaEstoque(struct item_estoque *Estoque, int num_itens)
{
    for (int i = 0; i < num_itens; i++)
        printf("%d %s %d\n", i, Estoque[i].nome, Estoque[i].quantidade);

    ImprimirDivisoria();
}

/**
 @brief Imprime o saldo

 @param saldo - variavel saldo
 */
void ConsultaSaldo(float saldo)
{
    printf("Saldo: %.2f\n", saldo);
    ImprimirDivisoria();
}

/**
 @brief Comando central que, baseado na string lida do STDIN pela main, chama a funcao correspondente ao comando dado pelo usuario

 @param escolha - string contendo o comando dado pelo usuario
 @param Estoque - ponteiro para o vetor Estoque
 @param num_itens - ponteiro para a variavel num_itens
 @param tamanho_estoque - ponteiro para a variavel tamanho_estoque
 @param saldo - ponteiro para a variavel saldo

 @return quando chamando qualquer comando exceto FinalizarDia, retorna 0; com o comando FinalizarDia, retorna FIM_DIA (-1) para sinalizar na main
 que o programa deve teminar
 */
int ProcessarComando(char *escolha, struct item_estoque **Estoque, int *num_itens, int *tamanho_estoque, float *saldo)
{
    if (strcmp(escolha, "IP") == 0)
        InsereProduto(Estoque, num_itens, tamanho_estoque);

    else if (strcmp(escolha, "AE") == 0)
        AumentarEstoque(*Estoque, *num_itens, saldo);

    else if (strcmp(escolha, "MP") == 0)
        ModificarPreco(*Estoque, *num_itens);

    else if (strcmp(escolha, "VE") == 0)
        VenderItem(*Estoque, *num_itens, saldo);

    else if (strcmp(escolha, "CE") == 0)
        ConsultaEstoque(*Estoque, *num_itens);

    else if (strcmp(escolha, "CS") == 0)
        ConsultaSaldo(*saldo);

    else if (strcmp(escolha, "FE") == 0)
    {
        FinalizarDia(Estoque, *num_itens, *tamanho_estoque, *saldo);
        return FIM_DIA;
    }

    return (0);
}

/**
 @brief Comando que cria (ou recria e substitui) o arquivo estoque.txt e fecha o arquivo. Apos isso, libera a memoria alocada pelo vetor Estoque

 Criar o arquivo estoque intui criar o arquivo em si e preenche-lo com o numero de itens no vetor (num_itens),
 o tamanho do vetor Estoque (tamanho_estoque), o saldo remanescente (saldo) e os itens do vetor em si (o nome, quantidade e valor das structs)

 @param Estoque - ponteiro para o vetor Estoque
 @param num_itens - variavel num_itens
 @param tamanho_estoque - variavel tamanho_estoque
 @param saldo - variavel saldo
 */
void FinalizarDia(struct item_estoque **Estoque, int num_itens, int tamanho_estoque, float saldo)
{
    FILE *arquivo = fopen("estoque.txt", "w");

    fprintf(arquivo, "%d %d %.2f\n", num_itens, tamanho_estoque, saldo);

    for (int i = 0; i < num_itens; i++)
    {
        fprintf(arquivo, "%s %d %.2f", (*Estoque)[i].nome, (*Estoque)[i].quantidade, (*Estoque)[i].valor);
        if (i != num_itens - 1)
            fprintf(arquivo, "\n");
    }

    fclose(arquivo);

    for (int i = 0; i < num_itens; i++)
        free((*Estoque)[i].nome); /* Desaloca a string contendo o nome de cada item */

    free(*Estoque);
    *Estoque = NULL;
}