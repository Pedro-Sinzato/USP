/*
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"
#include "item.h"

typedef struct no_ NO;
// 0 pro atomo, 1 pra listinha
struct no_ {
    int tipo;
    union {
        ITEM *atomo;
        NO *sublista;
    } info;
    NO *prox;
};

struct lista_ {
    NO *inicio;
    int profundidade;
};



int calcular_profundidade(NO *inicio) {
    if (inicio == NULL) return 0;

    int maior = 1;
    for (NO *p = inicio; p != NULL; p = p->prox) {
        if (p->tipo == 1) {
            int prof = 1 + calcular_profundidade(p->info.sublista);
            if (prof > maior) maior = prof;
        }
    }
    return maior;
}

void apagar_nos(NO *inicio) {
    while (inicio != NULL) {
        NO *proximo = inicio->prox;
        if (inicio->tipo == 0) {
            item_apagar(&inicio->info.atomo);
        } else {
            apagar_nos(inicio->info.sublista);
        }
        free(inicio);
        inicio = proximo;
    }
}

 ITEM *buscar_nos(NO *inicio, int chave) {
    for (NO *p = inicio; p != NULL; p = p->prox) {
        if (p->tipo == 0) {
            if (item_chave(p->info.atomo) == chave)
                return p->info.atomo;
        } else {
            ITEM *achado = buscar_nos(p->info.sublista, chave);
            if (achado != NULL) return achado;
        }
    }
    return NULL;
}

void imprimir_nos(const NO *inicio) {
    printf("(");
    for (const NO *p = inicio; p != NULL; p = p->prox) {
        if (p->tipo == 0) {
            item_imprimir(p->info.atomo);
        } else {
            imprimir_nos(p->info.sublista);
        }
        if (p->prox != NULL) printf(", ");
    }
    printf(")");
}

LISTA *lista_criar(void) {
    LISTA *lista = malloc(sizeof(LISTA));
    if (lista == NULL) return NULL;

    lista->inicio = NULL;
    lista->profundidade = 0;
    return lista;
}

void lista_apagar(LISTA **lista) {
    if (lista == NULL || *lista == NULL) return;

    apagar_nos((*lista)->inicio);
    free(*lista);
    *lista = NULL;
}

bool lista_inserir_atomo(LISTA *lista, ITEM *item) {
    if (lista == NULL || item == NULL) return false;

    NO *novo = malloc(sizeof(NO));
    if (novo == NULL) return false;

    novo->tipo = 0;
    novo->info.atomo = item;
    novo->prox = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        NO *p = lista->inicio;
        while (p->prox != NULL) p = p->prox;
        p->prox = novo;
    }

    lista->profundidade = calcular_profundidade(lista->inicio);
    return true;
}

bool lista_inserir_sublista(LISTA *lista, LISTA *sublista) {
    if (lista == NULL || sublista == NULL || lista == sublista) return false;

    NO *novo = malloc(sizeof(NO));
    if (novo == NULL) return false;

    novo->tipo = 1;
    novo->info.sublista = sublista->inicio;
    novo->prox = NULL;

    if (lista->inicio == NULL) {
        lista->inicio = novo;
    } else {
        NO *p = lista->inicio;
        while (p->prox != NULL) p = p->prox;
        p->prox = novo;
    }

    free(sublista);
    lista->profundidade = calcular_profundidade(lista->inicio);
    return true;
}

ITEM *lista_buscar(LISTA *lista, int chave) {
    if (lista == NULL) return NULL;
    return buscar_nos(lista->inicio, chave);
}

void lista_imprimir(const LISTA *lista) {
    if (lista == NULL) return;
    imprimir_nos(lista->inicio);
    printf("\n");
}

int lista_tamanho(const LISTA *lista) {
    if (lista == NULL) return 0;

    int tamanho = 0;
    for (NO *p = lista->inicio; p != NULL; p = p->prox) tamanho++;
    return tamanho;
}

int lista_profundidade(const LISTA *lista) {
    if (lista == NULL) return 0;
    return lista->profundidade;
}

bool lista_vazia(const LISTA *lista) {
    return lista != NULL && lista->inicio == NULL;
}
*/
