#ifndef ENTRADA_SAIDA_H
#define ENTRADA_SAIDA_H

#include "logica.h"


void leLinha(Melodia* melodia, FILE* fp, FILE* fs,int tam, char caractere, bool* excecao);

Melodia* leMelodia(FILE* fp, FILE* fs, bool* excecao);

void printaResultado(int index, FILE* fs);

void padraoMaior(FILE* fp, FILE* fs, int tamPadrao, bool* excecao);

void notaInvalida(Melodia* m, FILE* fp, FILE* fs, char nota, char caractere,bool* excecao);

void tamanhoInvalido(Melodia*m, FILE* fp, FILE* fs, int index, int tam, char caractere, bool* excecao);

#endif