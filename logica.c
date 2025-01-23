#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "logica.h"

Melodia* obtemTamanhoMelodia(FILE* fp){
    char string[10];
    fgets(string, 10, fp);
    char* token = strtok(string, " ");
    //printf("str=%s\n", token);
    int tamanhoMusica = atoi(token);
    //printf("%d\n", tamanhoMusica);
    token = strtok(NULL, " ");
    //printf("str=%s\n", token);
    int tamanhoPadrao = atoi(token);
    //printf("int=%d\n\n", tamanhoPadrao);

    if(tamanhoMusica == 0 && tamanhoPadrao == 0)
        return NULL;

    return criaMelodias(tamanhoMusica, tamanhoPadrao);
}

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


//problema de ser tudo forca bruta, tem q olhar com monitor
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

    //KMP
    m->LPS = (int*)calloc(tamPadrao, sizeof(int));

    //BMH
    m->intervalosPadrao = (int*)calloc(tamPadrao-1, sizeof(int));
    m->intervalosMusica = (int*)calloc(tamMusica-1, sizeof(int));
    m->mascara = (int*)calloc(12, sizeof(int));
    return m;
}

void destroiMelodia(Melodia* m){
    //KMP
    free(m->LPS);

    //BMH
    free(m->intervalosPadrao);
    free(m->intervalosMusica);
    free(m->mascara);

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

void criaLPS(Melodia* melodia){
    if(melodia == NULL)
        return;
    melodia->LPS[0] = 0;
    int len = 0;

    int i = 1;
    while(i < melodia->tamPadrao){
        if(melodia->padrao[i] == melodia->padrao[len]){
            len++;
            melodia->LPS[i] = len;
            i++;
            continue;
        }else{
            if(len != 0){
                len = melodia->LPS[len-1];
                continue;
            } else {
                melodia->LPS[i] = 0;
                i++;
                continue;
            }
        }
    }
}

int KMP(Melodia* melodia){
    criaLPS(melodia);
    
    int i = 0, j = 0, posCasamento = -1;
    int diferenca = distanciaMin(melodia->musica[0] - melodia->padrao[0]);
    while(i < melodia->tamMusica){
        if(j == melodia->tamPadrao)
            return posCasamento - (melodia->tamPadrao - 1);
        int diferencaAtual = distanciaMin(melodia->musica[i] - melodia->padrao[j]);
        if(diferenca == diferencaAtual){
            posCasamento = i;
            i++;
            j++;
            continue;
        } else {
            if(j != 0){
                j = melodia->LPS[j-1];
                if(j == 0)
                    diferenca = distanciaMin(melodia->musica[i] - melodia->padrao[j]);
            } else {
                posCasamento = -1;
                i++;
                diferenca = distanciaMin(melodia->musica[i] - melodia->padrao[j]);
            }
        }
    }
    return -1;
}

int BMH(Melodia* melodia){
    
    return -1;
}