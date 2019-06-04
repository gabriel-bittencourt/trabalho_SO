#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <list>
#include "Processo.cpp"

typedef struct no{
    int processo;
    bool ocupado;
    int inicio; // em MB
    int tamanho; // em MB
} No;

using namespace std;

class MP {

    private:
        int tamanho;
        list<No> espacos;

    public:
        void realocar(){
            
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
    
        bool first_fit(Processo p){

            No novo;
            novo.ocupado = true;
            novo.tamanho = tamanho;

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

        Lista* busca(Processo p){
            Lista* l = this->espacos;

            while(l->prox){
                if( l->processo.id == p.id ){
                    return l;
                }
                l = l->prox;
            }

            return NULL;
        }

        void remover(Processo p){
            Lista* f = this->busca(p);
            l->ocupado = false;
            
        }

        Processo suspender(Processo p){

        }

        int memoria_usada(){
            int m = 0;

            for(list<No>::iterator it = espacos.begin(); it != espacos.end(); it++)
                if(it->ocupado)  m+= it->tamanho;

            return m;
        }

};
