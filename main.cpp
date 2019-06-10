#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "headers/Escalonador.h"
#include "CPU.cpp"
#include "MP.cpp"
#include "Fila.cpp"
#include "Escalonador.cpp"
#include "ListaEntrada.cpp"

using namespace std;


#define IMPRESSORAS 2
#define DISCOS 2

#define N_CPUS 4
#define TAM_MP 16384 // em MB


int main(void){
    novaLinha(2);
    imprimirLinhaFormatada("E S C A L O N A D O R    D E    P R O C E S S O S", (Alinhamento::CENTRO), true);
    imprimirLinhaFormatada("---------------------------------------------", (Alinhamento::CENTRO), true);
    imprimirLinhaFormatada("Iniciando leitura do arquivo de entrada...", (Alinhamento::ESQUERDA), true);
    novaLinha(1);
    // Lê arquivo de entrada
    list<Processo*> processosEntrada;
    processosEntrada = lerProcessosDeArquivo("entrada.txt");
    novaLinha(2);
    imprimirLinhaFormatada("* * * * *", (Alinhamento::CENTRO), true);
    novaLinha(2);
    
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
    escalonador->imprimirProcessosEntrada();
    novaLinha(2);
    imprimirLinhaFormatada("----------------INICIANDO ESCALONADOR----------------", (Alinhamento::CENTRO), true);
    novaLinha(1);
    char continuar = 's';
    int tempo_atual = 0;

    while(!processosEntrada.empty() && (continuar == 's' || continuar =='S')){
        imprimirLinhaFormatada("TEMPO ATUAL --> " + to_string(tempo_atual) + "s", (Alinhamento::ESQUERDA), true);
        novaLinha(2);
        escalonador->encaminharSuspensos();

        escalonador->encaminharProcessos(tempo_atual);

        escalonador->submeterProcessos();

        escalonador->imprimirProcessosCPUs();

        escalonador->executarProcessos();
        tempo_atual++;
        imprimirLinhaFormatada("continuar?", (Alinhamento::ESQUERDA), false);
        imprimirLinhaFormatada("   (s)  (n)", (Alinhamento::ESQUERDA), true);
        cin >> continuar;
    }
    novaLinha(1);
    imprimirLinhaFormatada("* * * TODOS OS PROCESSOS FORAM PROCESSADOS * * *", (Alinhamento::CENTRO),true);
    novaLinha(1);
    imprimirLinhaFormatada("-------ESCALONADOR DE PROCESSOS ENCERRADO-------", (Alinhamento::CENTRO), true);


    return 0;
}
