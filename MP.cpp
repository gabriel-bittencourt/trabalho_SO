#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Processo.cpp"

typedef struct lista{
    Processo processo;
    bool ocupado;
    int inicio; // em MB
    int tamanho; // em MB
    struct lista * prox;
} Lista;


class MP {

    private:
        int tamanho;
        Lista* espacos;

    public:
        void realocar(){

            Lista* l = this->espacos;
            Lista* anterior;
            Lista* ultimo_ocupado;

            Lista* vazio = (Lista*) malloc(sizeof(Lista));
            vazio->tamanho = 0;

            while( l->prox ){
                if( l->ocupado )
                    ultimo_ocupado = l;

                if( !l->ocupado ){
                    vazio->tamanho += l->tamanho;

                    anterior->prox = l->prox;
                    Lista* p = l;
                    anterior = l;
                    l = l->prox;   
                    free(p);
                    continue;
                }

                anterior = l;
                l = l->prox;
            }

        }
    
        void first_fit(Processo p){

            Lista* l = this->espacos;
            
            while( l->prox && ( l->ocupado || (l->tamanho < p.tamanho) ) )
                l = l->prox;

            if(!l){
                realocar(mp);
                this->first_fit(p);
            }

            if(p.tamanho > l->tamanho){
                Lista* novo = (Lista *) malloc(sizeof(Lista));
                novo->inicio = l->inicio + p.tamanho;
                novo->tamanho = p.tamanho - l->tamanho;
                novo->ocupado = false;
                novo->prox = l->prox;
                l->prox = novo;
            }

            l->ocupado = true;
            l->tamanho = p.tamanho;

        }

        LISTA* busca(Processo p){
            LISTA* l = this->espacos;

            while(l->prox){
                if( l->processo.id == p.id ){
                    return l;
                }
                l = l->prox;
            }

            return NULL;
        }

        void remover(Processo p){
            LISTA* f = this->busca(p);
            l->ocupado = false;
            
        }

        Processo suspender(Processo p){

        }

        int memoria_usada(){
            int m = 0;

            Lista* l = this->espacos;

            while(l->prox){
                if(l->ocupado)
                    m+= l->tamanho;

                l = l->prox;
            }

            return m;
        }

};
