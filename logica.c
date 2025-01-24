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

int distanciaMin(int diff){
    int absDiff = abs(diff);
    int multiplicador;
    if(diff < 0)
        multiplicador = 1;
    else
        multiplicador = -1;
    return absDiff < 12-absDiff ? diff : multiplicador*(12-absDiff);
} 

Melodia* criaMelodias(int tamMusica, int tamPadrao){
    Melodia* m = (Melodia*)malloc(sizeof(Melodia));
    m->musica = (int*)calloc(tamMusica, sizeof(int));
    m->tamMusica = tamMusica;
    m->padrao = (int*)calloc(tamPadrao, sizeof(int));
    m->tamPadrao = tamPadrao;

    //KMP
    m->LPS = (int*)calloc(tamPadrao, sizeof(int));

    //BMH e Shift-And
    m->intervalosPadrao = (int*)calloc(tamPadrao-1, sizeof(int));
    m->tamIntervaloPadrao = tamPadrao-1;
    m->intervalosMusica = (int*)calloc(tamMusica-1, sizeof(int));
    m->tamIntervaloMusica = tamMusica-1;
    m->mascara = (int*)calloc(13, sizeof(int));
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

void criaMascara(Melodia* melodia){
    int j;
    for(j = 0; j < 13; j++)
        melodia->mascara[j] = melodia->tamPadrao-1;
    for(j = 1; j < melodia->tamPadrao-1; j++)
        melodia->mascara[6+melodia->intervalosPadrao[j-1]] = melodia->tamPadrao-1-j;
}

int BMH(Melodia* melodia){
    if(melodia == NULL)
        return -1;
    criaMascara(melodia);
    
    int i = melodia->tamPadrao-1;
    while(i <= melodia->tamMusica-1){
        int k = i;
        int j = melodia->tamPadrao-1;
        while(j > 0 && melodia->intervalosPadrao[j-1] == melodia->intervalosMusica[k-1]){
            k--;
            j--;
        }
        if(j == 0){
            return k;
        }
        i += melodia->mascara[6+melodia->intervalosMusica[i-1]];
    }
    
    return -1;
}

int shiftAnd(Melodia* melodia){
    if(melodia == NULL)
        return -1;
    //define as mascaras
    int i;
    int m = melodia->tamIntervaloPadrao;
    int j = m-1;
    for(i = 0; i < 13; i++)
        melodia->mascara[i] = 0;
    i = 0;
    while(i < m){
        melodia->mascara[melodia->intervalosPadrao[i]] |= (1 << j);
        i++;
        j--;
    }
    //procura o plagio
    int r = 0;
    for(i = 0; i < melodia->tamIntervaloMusica; i++){
        r = ((r >> 1) | (1 << (m-1))) & melodia->mascara[melodia->intervalosMusica[i]];
        if(r%2 == 1){
            return i - (m-1);
        }
    }
    return -1;
}