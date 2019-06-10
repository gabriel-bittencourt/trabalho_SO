#include <stdlib.h>
#include <iostream>
using namespace std;

#include "headers/CPU.h"


CPU::CPU(){
    ocupado = false;
    tempo_processo_atual = 0;
}

void CPU::receberProcesso(Processo* p){
    processo = p;
    ocupado = true;
    tempo_processo_atual = 0;
}

Processo* CPU::liberarProcesso(){
    tempo_processo_atual = 0;
    ocupado = false;
    return processo;
}

void CPU::executar(){

    processo->setTempoProcessamento(
        processo->getTempoProcessamento() - 1
    );
    tempo_processo_atual++;
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
