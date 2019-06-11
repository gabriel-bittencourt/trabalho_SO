#include <stdlib.h>
#include <iostream> 
using namespace std;

#include "headers/Processo.h"
#include "ConsoleManager.cpp"

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
	tempo_uso_imp = 0;
	tempo_uso_disc = 0;
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
	tempo_uso_imp = p.tempo_uso_imp;
	tempo_uso_disc = p.tempo_uso_disc;
} 

int Processo::id_atual = 0; //gerador de ID de cada processo

// imprime os dados de um processo
void Processo::imprimir(){
	printCharRep('-', 100, true);
	imprimirLinhaFormatadaDef(to_string(chegada), (Alinhamento::CENTRO), 8, false);
	cout << " | ";
    imprimirLinhaFormatadaDef(to_string(id), (Alinhamento::CENTRO), 9, false);
	cout << " | ";
	imprimirLinhaFormatadaDef(to_string(prioridade), (Alinhamento::CENTRO), 11,  false);
	cout << " | ";
	imprimirLinhaFormatadaDef(to_string(tempo_processamento), (Alinhamento::CENTRO), 23, false);
	cout << " | ";
    imprimirLinhaFormatadaDef(to_string(tamanho), (Alinhamento::CENTRO), 8, false);
	cout << " | ";
    imprimirLinhaFormatadaDef(to_string(impressoras), (Alinhamento::CENTRO), 12, false);
	cout << " | ";
    imprimirLinhaFormatadaDef(to_string(discos), (Alinhamento::CENTRO), 7, true);
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

int Processo::getUltimaFila(){
	return ultima_fila;
}

int Processo::getTempoUsoImp(){
	return tempo_uso_imp;
}

int Processo::getTempoUsoDisc(){
	return tempo_uso_disc;
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

void Processo::setUltimaFila(int _ultima_fila){
	ultima_fila = _ultima_fila;
}

void Processo::setTempoUsoImp(int _tempo_uso_imp){
	tempo_uso_imp = _tempo_uso_imp;
}

void Processo::setTempoUsoDisc(int _tempo_uso_disc){
	tempo_uso_disc = _tempo_uso_disc;
}
