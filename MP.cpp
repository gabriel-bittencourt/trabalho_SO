#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "headers/MP.h"

#include "headers/Processo.h"
#include "headers/Fila.h"


MP::MP(int _tamanho){
    tamanho = _tamanho;

    No primeiroNo;
    
    primeiroNo.ocupado = false;
    primeiroNo.inicio = 0;
    primeiroNo.tamanho = _tamanho;

    espacos.push_back( primeiroNo );
}

void MP::realocar(){
    
    list<No>::iterator anterior = espacos.begin();
    list<No>::iterator atual = ++espacos.begin();

    while(anterior != espacos.end() && atual != espacos.end())
    {   
        // Se o anterior for buraco,
        // o próximo processo pega o lugar do buraco
        if(!anterior->ocupado){
            atual->inicio = anterior->inicio;
            espacos.erase(anterior++);
        }

        // Se o anterior for processo,
        // o proximo é ajustado
        else{
            atual->inicio = anterior->inicio + anterior->tamanho;
            anterior++;
        }
        atual++;
    }

    // Se o último nó for um buraco,
    // ajusta ao novo tamanho
    if(!anterior->ocupado){
        anterior->tamanho = tamanho - anterior->inicio;
    }

    // Se for um processo,
    // cria um buraco depois dele
    else{
        No aux;
        aux.ocupado = false;
        aux.inicio = anterior->inicio + anterior->tamanho;
        aux.tamanho = tamanho - aux.inicio;
        espacos.push_back(aux);
    }
}

bool MP::first_fit(Processo* p){

    No novo;
    novo.processo = p->getId();
    novo.ocupado = true;
    novo.tamanho = p->getTamanho();

    for(list<No>::iterator it = espacos.begin(); it != espacos.end(); it++)
        // Se achar um buraco em que caiba
        if(!it->ocupado && it->tamanho >= novo.tamanho){
            novo.inicio = it->inicio;
            it->inicio = it->inicio + novo.tamanho;
            it->tamanho = it->tamanho - novo.tamanho;
            espacos.insert(it, novo);
            if (it->tamanho == 0)    espacos.erase(it);
            return true;
        }
    // Caso não caiba em nenhum buraco
    return false;
}

void MP::suspender(Fila** feedback, int n_filas, Fila* suspensos, int tam_necessario, int* impressorasSistema, int* discosSistema){
    
    int tamanhoLivre = tamanho - memoria_usada();

    while( tamanhoLivre < tam_necessario ){
        
        for(int fila = n_filas; fila > 0; fila--){             // Percorre filas da de menor prioridade pra de maior prioridade
            
            if( !feedback[fila]->empty() ){
                Processo* p = feedback[fila]->retirar();

                // Atualiza impressoras e discos disponíveis
                *impressorasSistema += p->getImpressoras();
                *discosSistema += p->getDiscos();

                tamanhoLivre += p->getTamanho();

                suspensos->inserir(p);
                break;
            }

        }
    }

}

int MP::buscar(Processo* processo){
    for(No no : espacos){
        if( no.processo == processo->getId() )
            return no.inicio;
    }
    return -1;
}

int MP::memoria_usada(){
    int m = 0;

    for(list<No>::iterator it = espacos.begin(); it != espacos.end(); it++)
        if(it->ocupado)  m+= it->tamanho;

    return m;
}

