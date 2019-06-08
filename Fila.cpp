#include <stdlib.h>
#include <iostream>
using namespace std;

#include "Processo.h"

typedef struct fila{
    
    Processo* processo;
    struct fila* prox;

} FILA;

class Fila {

    private:
        FILA* fila;

    public:

        Fila(){
            fila = NULL;
        };

        int tamanho(){
            int x = 0;
            FILA* f = this->getFila();
            while( f ){
                x++;
                f = f->prox;
            }
            return x;
        }

        void inserir(Processo* p){

            FILA* novo = (FILA*) malloc(sizeof(FILA));
            novo->processo = p;
            novo->prox = this->fila;
            this->fila = novo;

        }

        Processo* retirar(){
            FILA* f = this->fila;
            FILA* aux;

            while(f->prox){
                aux = f;
                f = f->prox;
            }

            aux->prox = NULL;

            Processo* p = f->processo;
            free(f);
            return p;
        }

        bool empty(){
            return this->getFila() == NULL;
        }

        void liberar(){
            liberar(this->fila);
        }

        void liberar(FILA* f) {
            if( f ) {
                liberar(f -> prox);
                free(f);
            }
        }

        FILA* getFila(){
            return this->fila;
        }

};
