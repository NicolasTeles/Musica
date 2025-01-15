#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "logica.h"

int converteNota(char nota) {
    switch (nota) {
        case 'A':
            return 1;
        case 'B': 
            return 3;
        case 'C': 
            return 4;
        case 'D': 
            return 6;
        case 'E': 
            return 8;
        case 'F': 
            return 9;
        case 'G': 
            return 11;
    }
}

int distanciaMin(int diff){
    int absDiff = abs(diff);
    return absDiff < 12-absDiff ? diff : -1*(12-absDiff);
}

Melodia* criaMelodias(int tamMusica, int tamPadrao){
    Melodia* m = (Melodia*)malloc(sizeof(Melodia));
    m->musica = (int*)calloc(tamMusica, sizeof(int));
    m->tamMusica = tamMusica;
    m->padrao = (int*)calloc(tamPadrao, sizeof(int));
    m->tamPadrao = tamPadrao;
    return m;
}

void destroiMelodia(Melodia* m){
    free(m->musica);
    free(m->padrao);
    free(m);
}

// pos atual + tam padrao > tam musica -> parar loop
int forcaBruta(Melodia* melodia){
    int index = 0;
    int diferenca;
    while(true){
        if(index + melodia->tamPadrao > melodia->tamMusica)
            break;
        bool plagio = true;
        diferenca = distanciaMin(melodia->musica[index] - melodia->padrao[0]);
        for(int i = 1; i < melodia->tamPadrao; i++){
            int diferencaAtual = distanciaMin(melodia->musica[index+i] - melodia->padrao[i]);
            if(diferencaAtual != diferenca){
                plagio = false;
                break;
            }
        }
        if(plagio)
            return index;
        index++;
    }
    return -1;
}