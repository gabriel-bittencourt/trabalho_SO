#include <stdlib.h>
#include <iostream>
using namespace std;

#include "headers/Fila.h"

#include "headers/Processo.h"


Fila::Fila(){
    fila = {};
};

int Fila::tamanho(){
    return fila.size();
}

void Fila::inserir(Processo* p){
    fila.push_back(p);
}

Processo* Fila::retirar(){
    Processo* p = fila.front();
    fila.pop_front();
    return p;
}

bool Fila::empty(){
    return fila.empty();
}

void Fila::liberar(){
    fila.clear();
}


list<Processo*> Fila::getFila(){
    return this->fila;
}
