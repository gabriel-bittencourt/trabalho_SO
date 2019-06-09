#include <list>
#include <iostream>
using namespace std;

#include "headers/Escalonador.h"

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

            if( processo->getPrioridade() == 0 )
                FTR->inserir( processo );
            else{
                processo->setUltimaFila(0);
                feedback[0]->inserir( processo );

                // Atualiza impressoras e discos disponíveis
                impressorasDisponiveis -= processo->getImpressoras();
                discosDisponiveis -= processo->getDiscos();
            }

        }
        else
            entradaTemp.push_back(processo);

    }

    entrada.clear();
    for(Processo* processo : entradaTemp)
        entrada.push_back(processo);

}

void Escalonador::encaminharSuspensos(){

    while( !suspensos->empty() ){

        Processo* processo = suspensos->retirar();

        // Checa recursos disponíveis
        if( processo->getImpressoras() > impressorasDisponiveis || processo->getDiscos() > discosDisponiveis )
            suspensos->inserir(processo);

        // Checar MP
        if( !mp->first_fit(processo) ){     // Tenta alocar o processo
            
            mp->realocar();                 // Se não conseguir alocar

            if( mp->first_fit(processo) ){
                processo->setUltimaFila(0);
                feedback[0]->inserir( processo );

                // Atualiza impressoras e discos disponíveis
                impressorasDisponiveis -= processo->getImpressoras();
                discosDisponiveis -= processo->getDiscos();
            }
            else
                suspensos->inserir(processo);

        }

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

    list<CPU*> ociosas;

    for(CPU* cpu : cpus)
        if( cpu->getOcupado() )
            ociosas.push_back(cpu);

    return ociosas;

}

// Executar processos na CPUs ocupadas
void Escalonador::executarProcessos(){
    
    for(CPU* cpu : cpusOcupadas() ){

        cpu->executar();

        Processo* processo = cpu->getProcesso();

        if( processo->getTempoProcessamento() == 0 ){ // Processo terminou
            
            // retirar processo da memória
            //

            // liberar impressora e discos
            impressorasDisponiveis += processo->getImpressoras();
            discosDisponiveis += processo->getDiscos();

            cpu->liberarProcesso();
        }

        if( 
            processo->getPrioridade() > 0 &&                       // Processo é de tipo Usuário
            cpu->getTempoProcessoAtual() == QUANTUM                // Fim da fatia de tempo
        ){
            
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

void Escalonador::teste(){
    for(Processo* p : entrada){
        cout << endl;
        p->imprimir();
        cout << endl;
    }
}

int Escalonador::getImpressorasDisponiveis(){
    return impressorasDisponiveis;
}

int Escalonador::getDiscosDisponiveis(){
    return discosDisponiveis;
}