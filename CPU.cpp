#include <stdlib.h>
#include <iostream>
using namespace std;

#include "Processo.h"

class CPU{

    private:
        bool ocupado;
        int tempo_processo_atual; // Para política de feedback
        Processo* processo;

    public:

        CPU(){
            ocupado = false;
            tempo_processo_atual = 0;
        }

        void receberProcesso(Processo* p){
            processo = p;
            tempo_processo_atual = 0;
        }

        Processo* liberarProcesso(){
            tempo_processo_atual = 0;
            ocupado = false;
            return processo;
        }

        void executar(){

            processo->setTempoProcessamento(
                processo->getTempoProcessamento() - 1
            );
            tempo_processo_atual++;
        }

        void imprimirEstado(){

            if( ocupado ){
                cout << "Processo atual:" << endl;
                processo->imprimir();
                cout << "Tempo de processamento atual: " << tempo_processo_atual << " s" << endl;
            } else
                cout << "CPU ociosa!" << endl;

        }

        void setProcesso(Processo* _processo){
            processo = _processo;
        }

        void setOcupado(bool _ocupado){
            ocupado = _ocupado;
        }

        void setTempoProcessoAtual(int _tempo_processo_atual){
            tempo_processo_atual = _tempo_processo_atual;
        }

        Processo* getProcesso(){
            return processo;
        }

        bool getOcupado(){
            return ocupado;
        }

        int getTempoProcessoAtual(){
            return tempo_processo_atual;
        }
};
