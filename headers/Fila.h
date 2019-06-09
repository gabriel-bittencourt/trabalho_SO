#ifndef FILA_H
#define FILA_H

#include <list>
using namespace std;

#include "Processo.h"

typedef struct fila{
    
    Processo* processo;
    struct fila* prox;

} FILA;

class Fila {

    private:
        list<Processo*> fila;

    public:

        Fila();

        int tamanho();

        void inserir(Processo* p);

        Processo* retirar();

        bool empty();

        void liberar();

        list<Processo*> getFila();

};

#endif
