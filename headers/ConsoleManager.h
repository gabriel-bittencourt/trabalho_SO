#include <iostream>
#include <string>
#include <sstream>

#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H

using namespace std;

enum class Alinhamento{
    ESQUERDA,
    CENTRO,
    DIREITA
};

void imprimirLinhaFormatadaDef(string linha, Alinhamento alinhamento, int TAM_LINHA, bool novaLinha);

void imprimirLinhaFormatada(string linha, Alinhamento alinhamento, bool novalinha);

void novaLinha(int n);

void printCharRep(char a, int n, bool newline); 

#endif