#ifndef MP_H
#define MP_H

#include "Processo.h"
#include "Fila.h"


typedef struct no{
    int processo;
    bool ocupado;
    int inicio; // em MB
    int tamanho; // em MB
} No;

using namespace std;

class MP {

    private:
        int tamanho;
        list<No> espacos;

    public:

        MP(int _tamanho);

        void realocar();
    
        bool first_fit(Processo* p);

        void suspender(Fila** feedback, int n_filas, Fila* suspensos, int tam_necessario, int* impressorasSistema, int* discosSistema);

        int buscar(Processo* processo);

        int memoria_usada();

};

#endif
