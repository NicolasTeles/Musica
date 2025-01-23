#ifndef LOGICA_H
#define LOGICA_H

#include <stdio.h>

/*typedef enum notas{
    A = 1,
    B = 3,
    C = 4,
    D = 6,
    E = 8,
    F = 9,
    G = 11
}Notas;*/

typedef struct melodia{
    //BMH
    int* intervalosMusica;
    int* intervalosPadrao;
    int* mascara;
    
    //KMP
    int* LPS;
    
    int* musica;
    int tamMusica;
    int* padrao;
    int tamPadrao;
}Melodia;


Melodia* obtemTamanhoMelodia(FILE* fp);

int converteNota(char nota);

Melodia* criaMelodias(int tamMusica, int tamPadrao);

void destroiMelodia(Melodia* m);

int forcaBruta(Melodia* melodia);

int distanciaMin(int diff);

void criaLPS(Melodia* melodia);

int KMP(Melodia* melodia);

#endif