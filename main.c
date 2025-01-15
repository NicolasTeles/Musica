#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "logica.h"

int main(int argc, char* argv[]){
    FILE* fp = fopen(argv[1], "r");
    int tipo = atoi(argv[2]);
    if(fp == NULL){
        printf("Erro na abertura do arquivo de entrada!\n");
        return 0;
    }
    while(1){
        if(feof(fp))
            break;
        Melodia* melodia = leMelodia(fp);
        if(melodia == NULL)
            break;
        int index  =  forcaBruta(melodia);
        printf("%d\n", index);
        for(int i = 0; i < melodia->tamMusica; i++)
            printf("%d ", melodia->musica[i]);
        printf("\n");
        for(int i = 0; i < melodia->tamPadrao; i++)
            printf("%d ", melodia->padrao[i]);
        printf("\n");
        destroiMelodia(melodia);
    }
    fclose(fp);
    return 0;
}