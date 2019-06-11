#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "Fila.h"
#include "CPU.h"
#include "MP.h"

#define N_FILAS_FEEDBACK 3

#define QUANTUM 2

#define TEMPO_USO_DISCO 8
#define TEMPO_USO_IMPRESSORA 8


class Escalonador{

    private:
        list<Processo*> entrada;             // Processos lidos do arquivo de entrada

        Fila* FTR;                           // Fila de Tempo Real
        Fila* feedback[N_FILAS_FEEDBACK];    // Filas de Usuário

        Fila* suspensos;

        list<CPU*> cpus;                     // CPUs

        MP* mp;                              // MP

        int impressorasDisponiveis;          // Impressoras e Discos disponíveis no sistema
        int discosDisponiveis;
        int cpusociosas;


    public:

        Escalonador(list<Processo*> &_entrada, list<CPU*> &_cpus, MP* _mp, int _impressoras, int _discos);

        void encaminharProcessos(int tempo_atual);

        void encaminharSuspensos();

        list<CPU*> cpusOciosas();

        list<CPU*> cpusOcupadas();

        void executarProcessos();

        void submeterProcessos();

        void imprimirProcessosEntrada();

        void imprimirProcessosCPUs();

        int getImpressorasDisponiveis();

        int getDiscosDisponiveis();

        list<Processo*> getEntrada();

        void suspender(int tam_necessario);

};

#endif
