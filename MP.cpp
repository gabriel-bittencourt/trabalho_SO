#include <stdlib.h>
#include <iostream>

#include "Processo.cpp"
#include "Fila.cpp"

typedef struct lista{
    int id_processo;
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
            
            while( l->prox && ( l->ocupado || (l->tamanho < p.getTamanho()) ) )
                l = l->prox;

            if(!l){
                this->realocar();
                this->first_fit(p);
            }

            if(p.getTamanho() > l->tamanho){
                Lista* novo = (Lista *) malloc(sizeof(Lista));

                novo->inicio = l->inicio + p.getTamanho();
                novo->tamanho = p.getTamanho() - l->tamanho;
                novo->ocupado = false;
                novo->prox = l->prox;
                l->prox = novo;
            }

            l->ocupado = true;
            l->tamanho = p.getTamanho();

        }

        Lista* busca(Processo p){
            Lista* l = this->espacos;

            while(l->prox){
                if( l->id_processo == p.getId() ){
                    return l;
                }
                l = l->prox;
            }

            return NULL;
        }

        void remover(Processo p){
            Lista* l = this->busca(p);
            l->ocupado = false;
        }

        Processo suspender(Processo p, Fila suspensos){
            suspensos.inserir(p);
            this->remover(p);
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
