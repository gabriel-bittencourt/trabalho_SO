#include <stdio.h> 
#include <stdlib.h>
#include "Processo.cpp"

typedef struct fila{
    
    Processo processo;
    struct fila* prox;

} FILA;

class Fila {

    private:
        FILA* fila;

    public:
        void insere(Processo p){

            FILA* novo = (FILA*) malloc(sizeof(FILA));
            novo->processo = p;
            novo->prox = this->fila;
            this->fila = novo;
            // return novo;
        }

        void retira(Processo p){
            //
        }

        void libera(){
            libera(this->fila);
            // this->fila = NULL;
        }

        void libera(FILA* f) {
            if( f ) {
                libera(f -> prox);
                free(f);
            }
        }

};
