#ifndef LOGICA_H
#define LOGICA_H

#include <stdio.h>
#define MAX_ALFABETO = 12
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
    //BMH e Shift-And
    int* intervalosMusica;
    int tamIntervaloMusica;
    int* intervalosPadrao;
    int tamIntervaloPadrao;
    long** mascara;
    int linhaMascara;
    int* tabelaDeslocamento;
    //KMP
    int* LPS;
    
    int* musica;
    int tamMusica;
    int* padrao;
    int tamPadrao;
}Melodia;


Melodia* obtemTamanhoMelodia(FILE* fp, FILE* fs, bool* excecao);

int converteNota(char nota);

Melodia* criaMelodias(int tamMusica, int tamPadrao);

void destroiMelodia(Melodia* m);

int forcaBruta(Melodia* melodia);

int distanciaMin(int diff);

void criaLPS(Melodia* melodia);

int KMP(Melodia* melodia);

void criaMascara(Melodia* melodia);

int BMH(Melodia* melodia);

int shiftAnd(Melodia* melodia);

int achePlagio(Melodia* melodia, int tipo);

#endif