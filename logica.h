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

/*
    0 -> # : caracter de inicio
    -1 -> $ : parar de empilhar
    13 -> φ : fim do texto
*/

typedef struct melodia{
    int* LPS;

    //apagar isso dps
    int* fitaLeitura;
    int tamFita;
    
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

int KMP(Melodia* melodia);

#endif