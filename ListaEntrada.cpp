#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <list>
using namespace std;

#include "headers/Processo.h"


static void ordernarPorChegada(list<Processo*> &processos){

    processos.sort([](Processo* p1, Processo* p2){
        
        if( p1->getChegada() == p2->getChegada() ){

            if( p1->getPrioridade() == p2->getPrioridade() ){

                if( p1->getImpressoras() == p2->getImpressoras() ){

                    if( p1->getDiscos() == p2->getDiscos() ){

                        return p1->getTempoProcessamento() < p2->getTempoProcessamento();

                    }
                    return p1->getDiscos() < p1->getDiscos();

                }
                return p1->getImpressoras() < p2->getImpressoras();

            }
            return p1->getPrioridade() < p2->getPrioridade();

        }
        return p1->getChegada() < p2->getChegada();

    });

};


static list<Processo*> lerProcessosDeArquivo(string nomeArq){
    vector<string> entrada;
    ifstream arqEntrada(nomeArq);
    if(arqEntrada.is_open()){ // abertura do arquivo, caso tudo de certo
        string linha;
        while(getline(arqEntrada, linha)){ // percorre o arquivo de linha em linha
            entrada.push_back(linha); // armazeno no vetor entrada cada linha do arquivo
        }
        arqEntrada.close(); // fechamento do arquivo
    }
    else
    {
        cout <<"Nao foi possivel abrir o arquivo";
    }

    vector<string> linhaProcessos;
    for (auto dados : entrada){ //inicia a variavel "dados" que seleciona cada string do vetor 
        stringstream ss(dados); //buffer de string "ss" armazena a cadeia de caracteres "dados"
        string dado;
        string linhaP;
        while((getline(ss, dado, ','))){ //percorro o buffer usando a string "dado" e descontando as virgulas
            linhaP+=dado; // somo os caracteres desejados numa string
        }
        linhaProcessos.push_back(linhaP); // armazeno a string referente a um processo em uma posicao do vetor
    }

    vector<string> dados;
    for (auto elementos : linhaProcessos){ // inicializo a variavel "elementos" que seleciona cada elemento do vetor "linhaProcessos"
        stringstream ss(elementos); //buffer "ss" armazena o conteudo de elementos
        string dado; 
        while((getline(ss, dado, ' '))){ // percorro o buffer usando a string "dado" e descontando os espacos
            dados.push_back(dado); // armazeno cada dado referente a um processo em uma posicao do vetor
        }
    }

    list<Processo*> processos;
    int cont_processos = 0; // contador que irá controlar a selecao dos arquivos, so sera incrementada depois que forem achados 6 dados em funcao do indice
    int chegada, prioridade, tempo_processamento, tamanho, impressoras, discos;
    for(vector<string>::size_type i = 0; i != dados.size(); i++){ // percorre vetor pelo indice
        if(i / 6 == cont_processos){ // aritmetica para conseguir selecionar os dados de 6 em 6 
            if(i%6 == 0){
                chegada = stoi(dados[i]);
            }
            if(i%6 == 1){
                prioridade = stoi(dados[i]);
            }
            if(i%6 == 2){
                tempo_processamento = stoi(dados[i]);
            }
            if(i%6 == 3){
                tamanho = stoi(dados[i]);
            }
            if(i%6 == 4){
                impressoras = stoi(dados[i]);
            }
            else {
                discos = stoi(dados[i]);
            }
        }
        else {
            processos.push_back(new Processo(chegada, prioridade, tempo_processamento, tamanho, impressoras, discos));
            cont_processos++; // incremento "de 6 em 6" baseado na aritmetica de divisao
            if(i%6 == 0){
                chegada = stoi(dados[i]);
            }
        }
    }
    processos.push_back(new Processo(chegada, prioridade, tempo_processamento, tamanho, impressoras, discos)); // cria processo dos ultimos 6 dados que nao entraram no else do for
    ordernarPorChegada(processos);
    return processos;
}
