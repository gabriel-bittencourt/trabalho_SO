#include <stdio.h>
#include <stdlib.h>

typedef struct lista{
    int info;
    struct lista * prox;
} LISTA;

LISTA * inicializa_lista(void){
    return NULL;
}

LISTA * insere_inicio(LISTA * l, int elem){
    LISTA * novo = (LISTA*) malloc(sizeof(LISTA));
    novo -> info = elem;

    novo -> prox = l;
    return novo;
}

LISTA * insere_final(LISTA * l, int elem){
    if( !l ){
        return insere_inicio(l, elem);
    }

    LISTA * p = l;
    LISTA * novo = (LISTA *) malloc(sizeof(LISTA));
    novo -> info = elem;

    while( p -> prox){
        p = p -> prox;
    }

    novo -> prox = NULL;
    p -> prox = novo;
    return l;
}

LISTA * insere_final_rec(LISTA * l, int elem){
    if( !l ){
        return insere_inicio(l, elem);
    }

    l -> prox = insere_final_rec(l -> prox, elem);
    return l;
}

LISTA * insere_ordem(LISTA * l, int elem){
    
    LISTA * p = l, * ant = NULL;

    while( (p) && ((p -> info) < elem) ){
        ant = p;
        p = p -> prox;
    }

    LISTA * novo = (LISTA *) malloc(sizeof(LISTA));
    novo -> info = elem;

    if ( !ant ){
        return insere_inicio(l, elem);
    }

    ant -> prox = novo;
    novo -> prox = p;
    return l;
}

LISTA * insere_ordem_rec(LISTA * l, int elem){

    if( (!l) && (l -> info >= elem ) ){
        return insere_inicio(l, elem);
    }

    l -> prox = insere_ordem_rec(l -> prox, elem);
    return l;
}

void imprime(LISTA * l){

    LISTA * p = l;

    while( p ){
        printf("%d ", p -> info);
        p = p -> prox;
    }

}

void imprime_rec(LISTA * l){

    if( l ){
        printf("%d ", l -> info);
        imprime_rec(l -> prox);
    }

}

void libera(LISTA * l){

    while( l ){
        LISTA * p = l;
        l = l -> prox;
        free(p);
    }
}

void libera_rec(LISTA * l){

    if( l ){
        libera_rec(l -> prox);
        free(l);
    }

}

LISTA * busca(LISTA * l, int elem){

    LISTA * p = l;

    while( ( p ) && (p -> info != elem) ){
        p = p -> prox;
    }

    return p;
}

LISTA * busca_rec(LISTA * l, int elem){

    if( (!l) || (l -> info == elem) ){
        return l;
    }

    return busca_rec(l -> prox, elem);

}

LISTA * retira_prim(LISTA * l){

    LISTA * p = l;

    l = l-> prox;

    free(p);
    return l;
}

LISTA * retira_ult(LISTA * l){

    if( !l ) return NULL;

    LISTA * p = l, * ant = NULL;

    while( p -> prox ){
        ant = p;
        p = p -> prox;
    }

    ant -> prox = NULL;
    free(p);
    return l;
}

LISTA * retira(LISTA * l, int elem){

    LISTA * p = l, * ant = NULL;

    while( (p -> prox) && (p -> info != elem)){
        ant = p;
        p = p -> prox;
    }

    if ( !p ) return NULL;

    if( !ant ) l = l-> prox;

    else ant -> prox = p -> prox;

    free(p);
    return l;
}

LISTA * retira_rec(LISTA * l, int elem){

    if( !l ) return l;

    if(l -> info == elem){
        LISTA * p = l;
        l = l -> prox;
        free(p);
    } else{
        l -> prox = retira_rec(l -> prox, elem);
    }
    return l;
}
