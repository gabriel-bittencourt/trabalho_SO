#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "ListaEntrada.cpp"
#include "Processo.h"
using namespace std;

int main(void){
    list<Processo*> processosEntrada;
    processosEntrada = lerProcessosDeArquivo("entrada.txt");

    for(Processo* p : processosEntrada){
        p->imprimir();
    }

    return 0;
}