#include <stdio.h> 
#include <stdlib.h>
#include "Processo.cpp"

typedef struct fila{
    
    Processo* processo;
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

        }

        Processo retira(){
            FILA* f = this->fila;
            FILA* aux;

            while(f->prox){
                aux = f;
                f = f->prox;
            }

            aux->prox = NULL;
            Processo p(
                f->processo.id,
                f->processo.chegada,
                f->processo.prioridade,
                f->processo.tempo_processamento,
                f->processo.tamanho,
                f->processo.impressoras,
                f->processo.discos
            );
            delete f->processo;
            free(f);
            return p;

        }

        void libera(){
            libera(this->fila);
        }

        void libera(FILA* f) {
            if( f ) {
                libera(f -> prox);
                free(f);
            }
        }

};

