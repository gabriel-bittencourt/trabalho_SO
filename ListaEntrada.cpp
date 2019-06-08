#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <list>
#include "Processo.cpp"
using namespace std;

vector<Processo*> lerProcessosDeArquivo(string nomeArq){
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
    vector<Processo*> processos;
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
    return processos;
}