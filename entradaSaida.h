#ifndef ENTRADA_SAIDA_H
#define ENTRADA_SAIDA_H

#include "logica.h"


void leLinha(Melodia* melodia, FILE* fp, int tam, char caracter);

Melodia* leMelodia(FILE* fp);

void printaResultado(FILE* fp, int index);

#endif