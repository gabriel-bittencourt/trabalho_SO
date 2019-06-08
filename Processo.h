#ifndef PROCESSO_H
#define PROCESSO_H

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

        Processo();

        Processo(
        	int _chegada,
            int _prioridade,
            int _tempo_processamento, 
            int _tamanho,
            int _impressoras,
            int _discos
        );

        Processo(const Processo &p);


		// funções

		void imprimir();


		int getId();

		int getChegada();

		int getPrioridade();

		int getTempoProcessamento();

		int getTamanho();

		int getImpressoras();

		int getDiscos();

		void setId(int _id);
		
		void setChegada(int _chegada);

		void setPrioridade(int _prioridade);

		void setTempoProcessamento(int _tempo_processamento);

		void setTamanho(int _tamanho);

		void setImpressoras(int _impressoras);

		void setDiscos(int _discos);

};

#endif