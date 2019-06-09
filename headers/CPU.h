#ifndef CPU_H
#define CPU_H

#include "Processo.h"

class CPU{

    private:
        bool ocupado;
        int tempo_processo_atual; // Para política de feedback
        Processo* processo;

    public:

        CPU();

        void receberProcesso(Processo* p);

        Processo* liberarProcesso();

        void executar();

        void imprimirEstado();

        void setProcesso(Processo* _processo);

        void setOcupado(bool _ocupado);

        void setTempoProcessoAtual(int _tempo_processo_atual);

        Processo* getProcesso();

        bool getOcupado();

        int getTempoProcessoAtual();

};

#endif
