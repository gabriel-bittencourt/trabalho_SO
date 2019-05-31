#include <stdlib.h>
#include <iostream> 
using namespace std;


class Processo {

    private:
    	int id;
		static int id_atual;
        int chegada;
        int prioridade;
        int tempo_processamento;
        int tamanho;
        int impressoras;
        int discos;

    public:

		Processo(
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
		Processo(const Processo &p) {
			id = p.id;
			chegada = p.chegada;
			prioridade = p.prioridade;
			tempo_processamento = p.tempo_processamento;
			tamanho = p.tamanho;
			impressoras = p.impressoras;
			discos = p.discos;
		} 


		// funções


		int getId();

		int getChegada();

		int getPrioridade();

		int getTempoProcessamento();

		int getTamanho();

		int getImpressoras();

		int getDiscos();

		int setId(int _id);
		
		int setChegada(int _chegada);

		int setPrioridade(int _prioridade);

		int setTempoProcessamento(int _tempo_processamento);

		int setTamanho(int _tamanho);

		int setImpressoras(int _impressoras);

		int setDiscos(int _discos);
		
};

int Processo::id_atual = 0;

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

int Processo::setId(int _id){
	id = _id;
}

int Processo::setChegada(int _chegada){
	chegada = _chegada;
}

int Processo::setPrioridade(int _prioridade){
	prioridade = _prioridade;
}

int Processo::setTempoProcessamento(int _tempo_processamento){
	tempo_processamento = _tempo_processamento;
}

int Processo::setTamanho(int _tamanho){
	tamanho = _tamanho;
}

int Processo::setImpressoras(int _impressoras){
	impressoras = _impressoras;
}

int Processo::setDiscos(int _discos){
	discos = _discos;
}
