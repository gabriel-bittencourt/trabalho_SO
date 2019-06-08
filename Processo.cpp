#include <stdlib.h>
#include <iostream> 
using namespace std;

#include "Processo.h"

Processo::Processo(){};

Processo::Processo(
	int _chegada,
	int _prioridade,
	int _tempo_processamento, 
	int _tamanho,
	int _impressoras,
	int _discos
) {
	id = id_atual++;
	chegada = _chegada;
	prioridade = _prioridade;
	tempo_processamento = _tempo_processamento;
	tamanho = _tamanho;
	impressoras = _impressoras;
	discos = _discos;
}

// Copy constructor
Processo::Processo(const Processo &p) {
	id = p.id;
	chegada = p.chegada;
	prioridade = p.prioridade;
	tempo_processamento = p.tempo_processamento;
	tamanho = p.tamanho;
	impressoras = p.impressoras;
	discos = p.discos;
} 

int Processo::id_atual = 0;

void Processo::imprimir(){
    cout << "Processo: " << this->getId() << endl
         << "Chegada: " << this->getChegada() << endl
         << "Prioridade: " << this->getPrioridade() << endl
		 << "Tempo de Processamento: " << this->getTempoProcessamento() << " s"<< endl
         << "Tamanho: " << this->getTamanho() << " MB" << endl
         << "Impressoras: " << this->getImpressoras() << endl
         << "Discos: " << this->getDiscos() << endl << endl;
}


int Processo::getId(){
	return id;
}

int Processo::getChegada(){
	return chegada;
}

int Processo::getPrioridade(){
	return prioridade;
}

int Processo::getTempoProcessamento(){
	return tempo_processamento;
}

int Processo::getTamanho(){
	return tamanho;
}

int Processo::getImpressoras(){
	return impressoras;
}

int Processo::getDiscos(){
	return discos;
}

void Processo::setId(int _id){
	id = _id;
}

void Processo::setChegada(int _chegada){
	chegada = _chegada;
}

void Processo::setPrioridade(int _prioridade){
	prioridade = _prioridade;
}

void Processo::setTempoProcessamento(int _tempo_processamento){
	tempo_processamento = _tempo_processamento;
}

void Processo::setTamanho(int _tamanho){
	tamanho = _tamanho;
}

void Processo::setImpressoras(int _impressoras){
	impressoras = _impressoras;
}

void Processo::setDiscos(int _discos){
	discos = _discos;
}
