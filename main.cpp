#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;


#include "Escalonador.cpp"
#include "ListaEntrada.cpp"
#include "CPU.cpp"
#include "MP.cpp"
#include "Processo.h"


#define IMPRESSORAS 2
#define DISCOS 2

#define N_CPUS 4
#define TAM_MP 16384 // em MB


int main(void){

    // Lê arquivo de entrada
    list<Processo*> processosEntrada;
    processosEntrada = lerProcessosDeArquivo("entrada.txt");


    // Inicializa CPUs
    list<CPU*> cpus;
    for(int i = 0; i < N_CPUS; i++){
        cpus.push_back(new CPU());
    }
    
    // Inicializa MP
    MP* mp = new MP(TAM_MP);

    // Inicializa Impressoras e Discos
    int impressoras = IMPRESSORAS;
    int discos = DISCOS;


    // Inicializa escalonador
    Escalonador* escalonador = new Escalonador(processosEntrada, cpus, mp, impressoras, discos);


    return 0;
}
