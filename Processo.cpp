#include <stdio.h>
#include <stdlib.h>

class Processo {

    private:
    	int id;
        int chegada;
        int prioridade;
        int tempo_processamento;
        int tamanho;
        int impressoras;
        int discos;

    public:

        // funções

};

Processo::Processo(
	int _id,
	int _chegada,
	int _prioridade,
	int _tempo_processamento, 
	int _tamanho,
	int _impressoras,
	int _discos
) {
	id = _id;
    chegada = _chegada;
	prioridade = _prioridade;
	tempo_processamento = _tempo_processamento;
	tamanho = _tamanho;
	impressoras = _impressoras;
	discos = _discos;
}
