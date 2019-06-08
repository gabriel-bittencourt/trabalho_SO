#include <list>
#include <iostream>
using namespace std;

#include "Fila.cpp"
#include "CPU.cpp"
#include "MP.cpp"


#define N_FILAS_FEEDBACK 3

#define QUANTUM 2

#define TEMPO_USO_DISCO 2
#define TEMPO_USO_IMPRESSORA 2


class Escalonador{

    private:
        list<Processo*> entrada;             // Processos lidos do arquivo de entrada

        Fila* FTR;                           // Fila de Tempo Real
        Fila* feedback[N_FILAS_FEEDBACK];    // Filas de Usuário

        list<CPU*> cpus;                     // CPUs

        MP* mp;                              // MP

        int impressorasDisponiveis;          // Impressoras e Discos disponíveis no sistema
        int discosDisponiveis;


    public:

        Escalonador(list<Processo*> &_entrada, list<CPU*> &_cpus, MP* _mp, int _impressoras, int _discos){
            
            entrada = _entrada;
            
            Fila* FTR = new Fila();

            for(int i = 0; i < N_FILAS_FEEDBACK; i++)
                feedback[i] = new Fila();

            cpus = _cpus;

            mp = _mp;

            impressorasDisponiveis = _impressoras;
            discosDisponiveis = _discos;

        };


        // Encaminha processos da entrada paras as Filas, de acordo com o tipo
        void encaminharProcessos(){

            for(Processo* processo : entrada){

                // Checar impressoras e discos
                if( processo->getImpressoras() > impressorasDisponiveis || processo->getDiscos() > discosDisponiveis )
                    continue;

                // Checar MP
                //
                //


                if( processo->getPrioridade() == 0 )
                    FTR->inserir( processo );
                else{
                    processo->setUltimaFila(0);
                    feedback[0]->inserir( processo );

                    // Atualiza impressoras e discos disponíveis
                    impressorasDisponiveis -= processo->getImpressoras();
                    discosDisponiveis -= processo->getDiscos();
                }

                // retirar processo da lista
                //
                
            }

        }

        list<CPU*> cpusOciosas(){

            list<CPU*> ociosas;

            for(CPU* cpu : cpus)
                if( !cpu->getOcupado() )
                    ociosas.push_back(cpu);

            return ociosas;

        }


        list<CPU*> cpusOcupadas(){

            list<CPU*> ociosas;

            for(CPU* cpu : cpus)
                if( cpu->getOcupado() )
                    ociosas.push_back(cpu);

            return ociosas;

        }

        // Executar processos na CPUs ocupadas
        void executarProcessos(){
            
            for(CPU* cpu : cpusOcupadas() ){

                cpu->executar();

                Processo* processo = cpu->getProcesso();

                if( processo->getTempoProcessamento() == 0 ){ // Processo terminou
                    
                    // retirar processo da memória
                    // liberar impressora e discos

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
        void submeterProcessos(){

            for(CPU* cpu : cpusOciosas() ){                                     // Submete processos de tempo real

                if( !FTR->empty() )
                    cpu->receberProcesso( FTR->retirar() );

            }

            for(CPU* cpu : cpusOciosas() ){                                     // Submete processos de usuário

                for(int fila = 0; fila < N_FILAS_FEEDBACK; fila++){             // Percorre filas
                    
                    if( !feedback[fila]->empty() ){
                        cpu->receberProcesso( feedback[fila]->retirar() );      // Submete primeiro processo da fila
                        fila--;                                                 // Se processo foi submetido, continua na fila
                    }
                }
            }

        }

};
