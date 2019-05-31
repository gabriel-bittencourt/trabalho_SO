#include <stdio.h>
#include <stdlib.h>

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
        // funções
	

};

int Processo::id_atual = 0;