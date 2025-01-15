#ifndef ENTRADA_SAIDA_H
#define ENTRADA_SAIDA_H

#include "logica.h"

Melodia* obtemTamanhoMelodia(FILE* fp);

void leLinha(Melodia* melodia, FILE* fp, int tam, char caracter);

Melodia* leMelodia(FILE* fp);

#endif