#include <stdlib.h>
#include <iostream>
using namespace std;

#include "headers/CPU.h"


CPU::CPU(){
    ocupado = false;
    tempo_processo_atual = 0;
}

void CPU::receberProcesso(Processo* p){ //CPU recebe um processo e fica ocupada
    processo = p;
    ocupado = true;
    tempo_processo_atual = 0;
}

Processo* CPU::liberarProcesso(){ //tira processo da CPU e CPU fica ociosa
    tempo_processo_atual = 0;
    ocupado = false;
    return processo;
}

void CPU::executar(){

    processo->setTempoProcessamento(
        processo->getTempoProcessamento() - 1  // tempo de processamento restante do processo atualizado
    );
    if (processo->getImpressoras() > 0){
        processo->setTempoUsoImp(processo->getTempoUsoImp() + 1); // tempo de uso de impressoras atualizado
    }
    if (processo->getDiscos() > 0){
        processo->setTempoUsoDisc(processo->getTempoUsoDisc() + 1); //tempo de uso de discos atualizado
    }
    tempo_processo_atual++; // atualiza tempo de execucao do processo na CPU
}

void CPU::setProcesso(Processo* _processo){
    processo = _processo;
}

void CPU::setOcupado(bool _ocupado){
    ocupado = _ocupado;
}

void CPU::setTempoProcessoAtual(int _tempo_processo_atual){
    tempo_processo_atual = _tempo_processo_atual;
}

Processo* CPU::getProcesso(){
    return processo;
}

bool CPU::getOcupado(){
    return ocupado;
}

int CPU::getTempoProcessoAtual(){
    return tempo_processo_atual;
}
