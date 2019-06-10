#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

enum class Alinhamento{
    ESQUERDA,
    CENTRO,
    DIREITA
};

void imprimirLinhaFormatadaDef(string linha, Alinhamento alinhamento, int TAM_LINHA, bool novaLinha){
    if (alinhamento == Alinhamento::CENTRO)
    
        if (linha.length() % 2 == 0){
            cout << setw(TAM_LINHA);
            cout << linha;
        }
        else{
            cout << setw(TAM_LINHA);
            cout << linha + " ";
        }

    if (alinhamento == Alinhamento::DIREITA)
        if (linha.length() % 2 == 0){
            cout << setw(TAM_LINHA - (linha.length()));
            cout << linha;
        }
        else{
            cout << setw(TAM_LINHA - (linha.length()));
            cout << linha + " ";
        }  
    if (alinhamento == Alinhamento::ESQUERDA){
        cout << linha;
    }
    if (novaLinha)
        cout << '\n' << endl;
    
}

void imprimirLinhaFormatada(string linha, Alinhamento alinhamento, bool novalinha){
    imprimirLinhaFormatadaDef(linha, alinhamento, (linha.length() + (linha.length()/2)), novalinha);
}

void novaLinha(int n) {
    for (int i = 0; i < n; i++)
        cout << '\n' << endl;
}

void printCharRep(char a, int n, bool newline){
    for (int i = 0; i < n; i++)
        cout << a;
    cout << endl;
    if (newline)
        novaLinha(1);
}