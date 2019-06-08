#include <iostream>
#include <vector>
#include <string>
#include "ListaEntrada.cpp"
using namespace std;

int main(void){
    vector<Processo*> processosEntrada;
    processosEntrada = lerProcessosDeArquivo("entrada.txt");

    return 0;
}