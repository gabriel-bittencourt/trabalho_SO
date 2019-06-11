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

void MP::remover(Processo* processo){
    list<No>::iterator no = espacos.begin();
    list<No>::iterator prox, ant;

    while(no->processo != processo->getId()) no++; // Encontra o processo na lista

    if(no == espacos.begin()){  // Se for o primeiro da lista
        no->ocupado = false;
        prox = no;
        prox++;
        if(prox->ocupado)    return;    // Se o proximo estiver ocupado, só libera o espaco do no
        no->tamanho += prox->tamanho;   // Se não, adiciona o tamanho do buraco seguinte
        espacos.erase(prox);            // Remove o buraco seguinte
        return;
    }

    prox = no;
    prox++;
    if(prox == espacos.end()){  // Se for o último da lista
        no->ocupado = false;    // Só libera o espaco do nó
        return;
    }

    if(!prox->ocupado){ // Se estiver no meio da lista e o proximo for buraco
        no->ocupado = false;    // Libera o espaco do nó
        no->tamanho += prox->tamanho;   // Adiciona o tamanho do buraco seguinte
        espacos.erase(prox);    // Remove o buraco
    }
    ant = no;
    ant--;
    if(!ant->ocupado){  // Se estiver no meio da lista e o anterior for buraco
        ant->tamanho += no->tamanho;    // Adiciona o tamanho do no ao buraco anterior
        espacos.erase(no);  // Remove o nó
        return;
    }

    no->ocupado = false;    // Se o anterior e o próximo forem processos, só libera o espaço do nó
    
}

int MP::getTamanho(){
    return tamanho;
}
