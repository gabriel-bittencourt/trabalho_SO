#include <list>
#include <iostream>
#include <string>

using namespace std;

#include "headers/Escalonador.h"
#include "Processo.cpp"

#define N_FILAS_FEEDBACK 3

#define QUANTUM 2

#define TEMPO_USO_DISCO 2
#define TEMPO_USO_IMPRESSORA 2


Escalonador::Escalonador(list<Processo*> &_entrada, list<CPU*> &_cpus, MP* _mp, int _impressoras, int _discos){
    
    entrada = _entrada;
    
    FTR = new Fila();

    for(int i = 0; i < N_FILAS_FEEDBACK; i++)
        feedback[i] = new Fila();

    suspensos = new Fila();

    cpus = _cpus;

    mp = _mp;

    impressorasDisponiveis = _impressoras;
    discosDisponiveis = _discos;

};

// Encaminha processos da entrada paras as Filas, de acordo com o tipo
void Escalonador::encaminharProcessos(int tempo_atual){

    list<Processo*> entradaTemp;

    for(Processo* processo : entrada){
        stringstream ss;
        ss << processo->getId();
        string str = ss.str();

        if( processo->getChegada() == tempo_atual ) {
            
            // Checar impressoras e discos
            if( processo->getImpressoras() > impressorasDisponiveis || processo->getDiscos() > discosDisponiveis ){
                entradaTemp.push_back(processo);
                continue;
            }

            // Checar MP
            if( !mp->first_fit(processo) ){     // Tenta alocar o processo
                
                mp->realocar();                 // Se não conseguir alocar

                if( !mp->first_fit(processo) && processo->getPrioridade() == 0 ){   // Se for de tempo real, tem que ser submetido
                    mp->suspender(feedback, N_FILAS_FEEDBACK, suspensos, processo->getTamanho(), &impressorasDisponiveis, &discosDisponiveis);
                    mp->realocar();
                    mp->first_fit(processo);
                }
                else{                           // Se não for de tempo real, continua na lista de entrada
                    entradaTemp.push_back(processo);
                    continue;
                }

            }

            if( processo->getPrioridade() == 0 ){
                FTR->inserir( processo );
                novaLinha(1);
                imprimirLinhaFormatada("PROCESSO "+ str+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
                novaLinha(1);
            }
            else{
                processo->setUltimaFila(0);
                feedback[0]->inserir( processo );
                novaLinha(1);
                imprimirLinhaFormatada("PROCESSO "+ str+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
                novaLinha(1);
                // Atualiza impressoras e discos disponíveis
                impressorasDisponiveis -= processo->getImpressoras();
                discosDisponiveis -= processo->getDiscos();
            }
            
        }
        else if(processo->getChegada() > tempo_atual)
            entradaTemp.push_back(processo);
        
        else{
            suspensos->inserir(processo);
            novaLinha(1);
            imprimirLinhaFormatada("PROCESSO "+ str+" FOI SUSPENSO", (Alinhamento::ESQUERDA), true);
            novaLinha(1);
        }
        ss.clear();
    }

    entrada.clear();
    for(Processo* processo : entradaTemp)
        entrada.push_back(processo);

}

void Escalonador::encaminharSuspensos(){
    
    list<Processo*> aux_susp;
    while( !suspensos->empty() ){

        Processo* processo = suspensos->retirar();

        // Checa recursos disponíveis
        if( processo->getImpressoras() > impressorasDisponiveis || processo->getDiscos() > discosDisponiveis )
            aux_susp.push_back(processo);

        // Checar MP
        if( !mp->first_fit(processo) ){     // Tenta alocar o processo
            
            mp->realocar();                 // Se não conseguir alocar

            if( mp->first_fit(processo) ){
                stringstream ss;
                ss << processo->getId();
                string str = ss.str();
                processo->setUltimaFila(0);
                feedback[0]->inserir( processo );
                novaLinha(1);
                imprimirLinhaFormatada("PROCESSO "+ str+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
                novaLinha(1);
                // Atualiza impressoras e discos disponíveis
                impressorasDisponiveis -= processo->getImpressoras();
                discosDisponiveis -= processo->getDiscos();
            }
            else
                aux_susp.push_back(processo);

        }

    }
    suspensos = new Fila();
    for(Processo* p : aux_susp){
        suspensos->inserir(p);
    }

}

list<CPU*> Escalonador::cpusOciosas(){

    list<CPU*> ociosas;

    for(CPU* cpu : cpus)
        if( !cpu->getOcupado() )
            ociosas.push_back(cpu);

    return ociosas;

}

list<CPU*> Escalonador::cpusOcupadas(){

    list<CPU*> ocupadas;

    for(CPU* cpu : cpus)
        if( cpu->getOcupado() )
            ocupadas.push_back(cpu);

    return ocupadas;

}

// Executar processos na CPUs ocupadas
void Escalonador::executarProcessos(){
    
    for(CPU* cpu : cpusOcupadas() ){

        cpu->executar();

        Processo* processo = cpu->getProcesso();
        stringstream ss;
        ss << processo->getId();
        string str = ss.str();

        if( processo->getTempoProcessamento() == 0 ){ // Processo terminou
            
            // retirar processo da memória
            //

            // liberar impressora e discos
            impressorasDisponiveis += processo->getImpressoras();
            discosDisponiveis += processo->getDiscos();
            novaLinha(1);
            imprimirLinhaFormatada("PROCESSO "+ str+" FOI TERMINADO", (Alinhamento::ESQUERDA), true);
            novaLinha(1);

            cpu->liberarProcesso();
        }

        if( 
            processo->getPrioridade() > 0 &&                       // Processo é de tipo Usuário
            cpu->getTempoProcessoAtual() == QUANTUM                // Fim da fatia de tempo
        ){
            novaLinha(1);
            imprimirLinhaFormatada(" FIM DA FATIA DE TEMPO DO PROCESSO "+ str, (Alinhamento::ESQUERDA), true);
            novaLinha(1);
            int ultima_fila = processo->getUltimaFila();

            if( ultima_fila == N_FILAS_FEEDBACK - 1 ){             // Processo estava na última fila
                processo->setUltimaFila(0);
                feedback[0]->inserir( processo );                  // vai pra primeira fila
            }
            else{
                processo->setUltimaFila(ultima_fila + 1);
                feedback[ultima_fila + 1]->inserir( processo );    // Processo vai pra próxima fila
            }
            
            cpu->liberarProcesso();

        }
        else {
            novaLinha(1);
            imprimirLinhaFormatada("PROCESSO "+ str+" CONTINUA EXECUTANDO", (Alinhamento::ESQUERDA), true);
            novaLinha(1);
        }
        

    }
    
}

// Submeter(?) processos para CPUs ociosas
void Escalonador::submeterProcessos(){

    for(CPU* cpu : cpusOciosas() ){                                     // Submete processos de tempo real

        if( !FTR->empty() )
            cpu->receberProcesso( FTR->retirar() );

    }

    for(CPU* cpu : cpusOciosas() ){                                     // Submete processos de usuário

        for(int fila = 0; fila < N_FILAS_FEEDBACK; fila++){             // Percorre filas
            
            if( !feedback[fila]->empty() ){
                cpu->receberProcesso( feedback[fila]->retirar() );      // Submete primeiro processo da fila
                break;
            }
        }
    }

}

void Escalonador::imprimirProcessosEntrada(){
    imprimirLinhaFormatada("-------------P R O C E S S O S-------------", (Alinhamento::CENTRO), true);
	imprimirLinhaFormatadaDef("Chegada", (Alinhamento::CENTRO), 8, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Processo", (Alinhamento::CENTRO), 9, false);
	cout << " | ";
	imprimirLinhaFormatadaDef("Prioridade", (Alinhamento::CENTRO), 11,  false);
	cout << " | ";
	imprimirLinhaFormatadaDef("Tempo de Processamento", (Alinhamento::CENTRO), 23, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Tamanho", (Alinhamento::CENTRO), 8, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Impressoras", (Alinhamento::CENTRO), 12, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Discos", (Alinhamento::CENTRO), 7, true);
    for(Processo* p : entrada){
        p->imprimir();
    }
    printCharRep('-', 100, true);
}

void Escalonador::imprimirProcessosCPUs(){
    int contCPU = 1;
    imprimirLinhaFormatada("-------------E S T A D O   C P Us-------------", (Alinhamento::CENTRO), true);
	imprimirLinhaFormatadaDef("CPU", (Alinhamento::CENTRO), 4, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Processo", (Alinhamento::CENTRO), 9, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Chegada", (Alinhamento::CENTRO), 8, false);
	cout << " | ";
	imprimirLinhaFormatadaDef("Prioridade", (Alinhamento::CENTRO), 11,  false);
	cout << " | ";
	imprimirLinhaFormatadaDef("Tempo de Processamento Atual", (Alinhamento::CENTRO), 28, true);
    for(CPU* cpu : cpus){
        if(cpu->getOcupado()){
            Processo* processo = cpu->getProcesso();
            printCharRep('-', 100, true);
            imprimirLinhaFormatadaDef(to_string(contCPU), (Alinhamento::CENTRO), 4, false);
            cout << " | ";
            imprimirLinhaFormatadaDef(to_string(processo->getId()), (Alinhamento::CENTRO), 9, false);
            cout << " | ";
            imprimirLinhaFormatadaDef(to_string(processo->getChegada()), (Alinhamento::CENTRO), 8, false);
            cout << " | ";
            imprimirLinhaFormatadaDef(to_string(processo->getPrioridade()), (Alinhamento::CENTRO), 11,  false);
            cout << " | ";
            imprimirLinhaFormatadaDef(to_string(cpu->getTempoProcessoAtual()), (Alinhamento::CENTRO), 28, false);
        }
        else{
            printCharRep('-', 100, true);
            imprimirLinhaFormatadaDef(to_string(contCPU), (Alinhamento::CENTRO), 4, false);
            cout << " | ";
            imprimirLinhaFormatadaDef("OCIOSA", (Alinhamento::CENTRO), 9, true);
        }
        contCPU++;
    }
    printCharRep('-', 100, true);
}

int Escalonador::getImpressorasDisponiveis(){
    return impressorasDisponiveis;
}

int Escalonador::getDiscosDisponiveis(){
    return discosDisponiveis;
}

Fila* Escalonador::getSuspensos(){
    return suspensos;
}