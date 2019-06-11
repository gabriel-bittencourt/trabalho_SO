#include <iostream>
#include <vector>
#include <list>
#include <string>
using namespace std;


#include "headers/Escalonador.h"
#include "CPU.cpp"
#include "MP.cpp"
#include "Fila.cpp"
#include "Escalonador.cpp"
#include "ListaEntrada.cpp"


#define IMPRESSORAS 2
#define DISCOS 2

#define N_CPUS 4
#define TAM_MP 16384 // em MB


int main(void){
    novaLinha(1);
    imprimirLinhaFormatada("-------------------------------------------------", (Alinhamento::CENTRO), true);
    imprimirLinhaFormatada("E S C A L O N A D O R    D E    P R O C E S S O S", (Alinhamento::CENTRO), true);
    imprimirLinhaFormatada("-------------------------------------------------", (Alinhamento::CENTRO), true);
    imprimirLinhaFormatada("Iniciando leitura do arquivo de entrada...", (Alinhamento::ESQUERDA), true);
    novaLinha(1);
    // Lê arquivo de entrada
    list<Processo*> processosEntrada;
    processosEntrada = lerProcessosDeArquivo("entrada.txt");
    novaLinha(1);
    imprimirLinhaFormatadaDef("* * * * *", Alinhamento::CENTRO, 40, true);

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
    list<Processo*> entrada = escalonador->getEntrada();
    escalonador->imprimirProcessosEntrada();
    novaLinha(2);
    imprimirLinhaFormatada("----------------INICIANDO ESCALONADOR----------------", (Alinhamento::CENTRO), true);
    novaLinha(1);
    char continuar = 's';
    int tempo_atual = 0;

    while((!entrada.empty() || mp->memoria_usada() > 0) && (continuar == 's' || continuar =='S')){
        novaLinha(1);
        printCharRep('-', 100, true);
        imprimirLinhaFormatada("TEMPO ATUAL --> " + to_string(tempo_atual) + "s", (Alinhamento::ESQUERDA), true);
        printCharRep('-', 100, true);
        escalonador->encaminharProcessos(tempo_atual);
        
        escalonador->encaminharSuspensos();

        escalonador->submeterProcessos();

        escalonador->imprimirProcessosCPUs();

        escalonador->executarProcessos();
        tempo_atual++;
        imprimirLinhaFormatada("CONTINUAR?  (S)  (N)", (Alinhamento::CENTRO), true);
        cin >> continuar;
        entrada = escalonador->getEntrada();
    }
    novaLinha(1);
    if(continuar != 's'|| continuar != 's')
        imprimirLinhaFormatada("* * * * SAINDO DO ESCALONADOR DE PROCESSOS * * * *", (Alinhamento::CENTRO), true);
    else
        imprimirLinhaFormatada("* * * TODOS OS PROCESSOS FORAM PROCESSADOS * * *", (Alinhamento::CENTRO),true);
    novaLinha(1);
    imprimirLinhaFormatada("--------ESCALONADOR DE PROCESSOS ENCERRADO--------", (Alinhamento::CENTRO), true);

    return 0;
}
