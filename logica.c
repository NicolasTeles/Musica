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
    printf("diff=%d\tabs=%d\tsubtracao=%d\n", diff, absDiff, 12-absDiff);
    return absDiff < 12-absDiff ? diff : -1*(12-absDiff);
} 

Melodia* criaMelodias(int tamMusica, int tamPadrao){
    Melodia* m = (Melodia*)malloc(sizeof(Melodia));
    m->musica = (int*)calloc(tamMusica, sizeof(int));
    m->tamMusica = tamMusica;
    m->padrao = (int*)calloc(tamPadrao, sizeof(int));
    m->tamPadrao = tamPadrao;

    m->tamFita = 3 + tamMusica + tamPadrao;
    m->fitaLeitura = (int*)calloc(m->tamFita, sizeof(int));
    return m;
}

void destroiMelodia(Melodia* m){
    free(m->fitaLeitura);
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

int KMP(Melodia* melodia){
    int index = 0;
    int diferenca;
    int retorno = -1;

    int* pilha = (int*)calloc(melodia->tamPadrao, sizeof(int));
    for(int i = 0; i < melodia->tamFita; i++){
        if(melodia->fitaLeitura[i] == 0)
            continue;
        if(melodia->fitaLeitura[i] == -1)
            break;
        
        pilha[i-1] = melodia->fitaLeitura[i];
    }
    
    for(int i = melodia->tamFita-1; ; i--){
        if(melodia->fitaLeitura[i] == 13)
            continue;
        if(melodia->fitaLeitura[i] == -1)
            break;
        if(index == 0){
            diferenca = distanciaMin(melodia->fitaLeitura[i] - pilha[melodia->tamPadrao - 1]);
            index++;
            continue;
        }
        if(diferenca != distanciaMin(melodia->fitaLeitura[i] - pilha[melodia->tamPadrao - (index + 1)])){
            index = 1;
            diferenca = distanciaMin(melodia->fitaLeitura[i] - pilha[melodia->tamPadrao - 1]);
            continue;
        }else{
            index++;
            if(index == melodia->tamPadrao){
                retorno = (i - melodia->tamPadrao - 2);
                break;
            }
        }
    }
    free(pilha);
    return retorno;
}