#include <list>
#include <iostream>
using namespace std;

#include "headers/Escalonador.h"
#include "Processo.cpp"

#define N_FILAS_FEEDBACK 3

#define QUANTUM 2

#define TEMPO_USO_DISCO 8
#define TEMPO_USO_IMPRESSORA 8


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

        if( processo->getChegada() == tempo_atual ) { //se o processo chega no tempo atual do sistema
            
            // Checar impressoras e discos
            if( processo->getImpressoras() > impressorasDisponiveis || processo->getDiscos() > discosDisponiveis ){
                //se o processo requer mais recursos do que lhe é disponivel
                suspensos->inserir(processo); //processo é suspenso
                novaLinha(1);
                imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUSPENSO", (Alinhamento::ESQUERDA), true);
                novaLinha(1);
                continue;
            }

            // Checar MP
            if( !mp->first_fit(processo) ){     // Tenta alocar o processo
                
                mp->realocar();                 // Se não conseguir alocar

                if( !mp->first_fit(processo) && processo->getPrioridade() == 0 ){   
                    // Se for de tempo real, tem que ser submetido
                    suspender(processo->getTamanho());
                    mp->realocar();
                    mp->first_fit(processo);
                }
                else if (mp->first_fit(processo) && processo->getPrioridade() == 0 ){  
                    //Se for de tempo real e conseguir ser alocado sem suspender outro processo, processo é submetido
                    FTR->inserir( processo );
                    novaLinha(1);
                    imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
                    novaLinha(1);                         
                    continue;
                }
                else if(mp->first_fit(processo) && processo->getPrioridade() > 0 ){ 
                    //Se for de usuario e conseguir ser alocado na segunda tentativa
                    processo->setUltimaFila(0);
                    feedback[0]->inserir( processo ); // processo é submetido 
                    novaLinha(1);
                    imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
                    novaLinha(1);
                    // Atualiza impressoras e discos disponíveis
                    impressorasDisponiveis -= processo->getImpressoras();
                    discosDisponiveis -= processo->getDiscos();
                    continue;
                }
                else{
                    //se for de usuario e nao conseguir ser alocado na segunda tentativa, o processo é suspenso
                    suspensos->inserir(processo); 
                    novaLinha(1);
                    imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUSPENSO", (Alinhamento::ESQUERDA), true);
                    novaLinha(1);
                    continue;
                }

            }

            if( processo->getPrioridade() == 0 ){ 
                //se consegue alocar o processo de tempo real na primeira tentativa, processo é submetido
                FTR->inserir( processo );
                novaLinha(1);
                imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
                novaLinha(1);
                }
            else{ //se consegue alocar o processo de usuario na primeira tentativa, processo é submetido
                processo->setUltimaFila(0);
                feedback[0]->inserir( processo );
                novaLinha(1);
                imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
                novaLinha(1);
                // Atualiza impressoras e discos disponíveis
                impressorasDisponiveis -= processo->getImpressoras();
                discosDisponiveis -= processo->getDiscos();
            }

        }
        else{
            entradaTemp.push_back(processo); //se o processo ainda nao chegou, volta pra entrada
        }

    }

    entrada.clear();
    for(Processo* processo : entradaTemp)
        entrada.push_back(processo);

}

void Escalonador::encaminharSuspensos(){

    Fila* aux = new Fila();
    while( !suspensos->empty()){
        Processo* processo = suspensos->retirar();

        // Checa recursos disponíveis
        if( processo->getImpressoras() > impressorasDisponiveis || processo->getDiscos() > discosDisponiveis ){
            // se nao há recursos disponiveis para o processo ser submetido, ele continua suspenso
            aux->inserir(processo);
            continue;
        }
        // Checar MP
        if( !mp->first_fit(processo) ){     // Tenta alocar o processo
            
            mp->realocar();                 // Se não conseguir alocar

            if( mp->first_fit(processo) ){ // se o processo consegue ser alocado na segunda tentativa
                // processo é submetido
                processo->setUltimaFila(0);
                feedback[0]->inserir( processo );
                novaLinha(1);
                imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
                novaLinha(1);
                // Atualiza impressoras e discos disponíveis
                impressorasDisponiveis -= processo->getImpressoras();
                discosDisponiveis -= processo->getDiscos();
            }
            else // senao, continua suspenso
                aux->inserir(processo);

        }

        if((mp->first_fit(processo))){ // se o processo consegue ser alocado de primeira
            // processo é submetido
            processo->setUltimaFila(0);
            feedback[0]->inserir( processo );
            novaLinha(1);
            imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUBMETIDO", (Alinhamento::ESQUERDA), true);
            novaLinha(1);
            // Atualiza impressoras e discos disponíveis
            impressorasDisponiveis -= processo->getImpressoras();
            discosDisponiveis -= processo->getDiscos();
        }
    }
    while(!aux->empty()){
        Processo* p = aux->retirar();
        suspensos->inserir(p);
    }
    aux->liberar(); 
}

list<CPU*> Escalonador::cpusOciosas(){ //identifica CPUs ociosas

    list<CPU*> ociosas;

    for(CPU* cpu : cpus)
        if( !cpu->getOcupado() )
            ociosas.push_back(cpu);

    return ociosas;

}

list<CPU*> Escalonador::cpusOcupadas(){ //identifica CPUs ocupadas

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

        if( processo->getTempoProcessamento() == 0 ){ // Processo terminou
            
            mp->remover(processo);
            // retirar processo da memória
            novaLinha(1);
            imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI TERMINADO", (Alinhamento::ESQUERDA), true);
            novaLinha(1);
            // liberar impressora e discos
            impressorasDisponiveis += processo->getImpressoras();
            discosDisponiveis += processo->getDiscos();

            cpu->liberarProcesso();
        }

        // Processo é suspenso para liberar recursos
        if(processo->getTempoUsoImp() == TEMPO_USO_IMPRESSORA || processo->getTempoUsoDisc() == TEMPO_USO_DISCO){ // Processo é de tipo Usuário
            if(cpu->getTempoProcessoAtual() == QUANTUM){ // Fim da fatia de tempo
                novaLinha(1);
                imprimirLinhaFormatada(" FIM DA FATIA DE TEMPO DO PROCESSO "+ to_string(processo->getId()), (Alinhamento::ESQUERDA), true);
                novaLinha(1);
            }
            // resetar tempo de uso dos recursos
            processo->setTempoUsoImp(0);
            processo->setTempoUsoDisc(0);
            // suspender processo
            suspensos->inserir(processo);
            novaLinha(1);
            imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" FOI SUSPENSO", (Alinhamento::ESQUERDA), true);
            novaLinha(1);

            mp->remover(processo);// retirar processo da memória
            // liberar impressora e discos
            impressorasDisponiveis += processo->getImpressoras();
            discosDisponiveis += processo->getDiscos();

            cpu->liberarProcesso();
            continue;
        }

        if( 
            processo->getPrioridade() > 0 &&                       // Processo é de tipo Usuário
            cpu->getTempoProcessoAtual() == QUANTUM                // Fim da fatia de tempo
        ){
            novaLinha(1);
            imprimirLinhaFormatada(" FIM DA FATIA DE TEMPO DO PROCESSO "+ to_string(processo->getId()), (Alinhamento::ESQUERDA), true);
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
            continue;

        }
        if((processo->getPrioridade() > 0 &&                       // Processo é de tipo Usuário
            cpu->getTempoProcessoAtual() < QUANTUM && processo->getTempoProcessamento() > 0) || (processo->getPrioridade() == 0 && processo->getTempoProcessamento() > 0)){ 
            //nao terminou o quantum ou é processo de tempo real
            novaLinha(1);
            imprimirLinhaFormatada("PROCESSO "+ to_string(processo->getId())+" CONTINUA EXECUTANDO", (Alinhamento::ESQUERDA), true);
            novaLinha(1);
        }

    }
    
}

// Submeter processos para CPUs ociosas para torná-las ocupadas
void Escalonador::submeterProcessos(){
    novaLinha(1);
    for(CPU* cpu : cpusOciosas() ){                                     // Submete processos de tempo real

        if( !FTR->empty() )
            cpu->receberProcesso( FTR->retirar() );

    }

    for(CPU* cpu : cpusOciosas() ){                                     // Submete processos de usuário

        for(int fila = 0; fila < N_FILAS_FEEDBACK; fila++){             // Percorre filas
            
            if( !feedback[fila]->empty() ){
                cpu->receberProcesso( feedback[fila]->retirar() );     // Submete primeiro processo da fila
                break;
            }
        }
    }

}
// imprimir tabela de todos os processos que estao no arquivo
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

// imprimir o estado de cada CPU
void Escalonador::imprimirProcessosCPUs(){
    int contCPU = 1;
    imprimirLinhaFormatada("-------------E S T A D O   C P Us-------------", (Alinhamento::CENTRO), true);
	imprimirLinhaFormatadaDef("CPU", (Alinhamento::CENTRO), 3, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Processo", (Alinhamento::CENTRO), 8, false);
	cout << " | ";
	imprimirLinhaFormatadaDef("Prioridade", (Alinhamento::CENTRO), 10,  false);
	cout << " | ";
	imprimirLinhaFormatadaDef("Tempo de Processamento Restante", (Alinhamento::CENTRO), 29, false);
    cout << " | ";
    imprimirLinhaFormatadaDef("Localizacao", (Alinhamento::CENTRO), 11, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Tamanho", (Alinhamento::CENTRO), 7, false);
    cout << " | ";
	imprimirLinhaFormatadaDef("Impressoras", (Alinhamento::CENTRO), 11, false);
	cout << " | ";
    imprimirLinhaFormatadaDef("Discos", (Alinhamento::CENTRO), 6, true);
    for(CPU* cpu : cpusOcupadas()){
        Processo* processo = cpu->getProcesso();
        printCharRep('-', 120, true);
        imprimirLinhaFormatadaDef(to_string(contCPU), (Alinhamento::CENTRO), 4, false);
        cout << " | ";
        imprimirLinhaFormatadaDef(to_string(processo->getId()), (Alinhamento::CENTRO), 8, false);
        cout << " | ";
        imprimirLinhaFormatadaDef(to_string(processo->getPrioridade()), (Alinhamento::CENTRO), 10,  false);
        cout << " | ";
        imprimirLinhaFormatadaDef(to_string(processo->getTempoProcessamento()), (Alinhamento::CENTRO), 32, false);
        cout << " | ";
        imprimirLinhaFormatadaDef(to_string(mp->buscar(processo)), (Alinhamento::CENTRO), 12, false);
        cout << " | ";
        imprimirLinhaFormatadaDef(to_string(processo->getTamanho()), (Alinhamento::CENTRO), 8, false);
        cout << " | ";
        imprimirLinhaFormatadaDef(to_string(processo->getImpressoras()), (Alinhamento::CENTRO), 12, false);
        cout << " | ";
        imprimirLinhaFormatadaDef(to_string(processo->getDiscos()), (Alinhamento::CENTRO), 6, true);
        contCPU++;
    }
    for(CPU* cpu : cpusOciosas()){
        printCharRep('-', 120, true);
        imprimirLinhaFormatadaDef(to_string(contCPU), (Alinhamento::CENTRO), 4, false);
        cout << " | ";
        imprimirLinhaFormatadaDef("OCIOSA", (Alinhamento::CENTRO), 8, true);
        contCPU++;
    }
    printCharRep('-', 120, true);
}

int Escalonador::getImpressorasDisponiveis(){
    return impressorasDisponiveis;
}

int Escalonador::getDiscosDisponiveis(){
    return discosDisponiveis;
}

list<Processo*> Escalonador::getEntrada(){
    return entrada;
}

void Escalonador::suspender(int tam_necessario){
    
    int tamanhoLivre = mp->getTamanho() - mp->memoria_usada();    

    // Percorre filas da de menor prioridade pra de maior prioridade
    for(int fila = N_FILAS_FEEDBACK; fila > 0 && tamanhoLivre < tam_necessario; fila--){
        
        if(!feedback[fila]->empty()){
            Processo* p = feedback[fila]->retirar(); //retira um processo da fila menos significativa do feedback
            mp->remover(p); // remove processo da MP
            novaLinha(1);
            imprimirLinhaFormatada("PROCESSO "+ to_string(p->getId())+" FOI SUSPENSO", (Alinhamento::ESQUERDA), true);
            novaLinha(1);
            // Atualiza impressoras e discos disponíveis
            impressorasDisponiveis += p->getImpressoras();
            discosDisponiveis += p->getDiscos();
            // Atualiza espaco disponivel
            tamanhoLivre += p->getTamanho();

            suspensos->inserir(p); // suspende o processo
        }
    }
}