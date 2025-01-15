#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
    //printf("ihttps://github.com/NicolasTeles/Musica.gitt=%d\n\n", tamanhoPadrao);

    if(tamanhoMusica == 0 && tamanhoPadrao == 0)
        return NULL;

    return criaMelodias(tamanhoMusica, tamanhoPadrao);
}

void leLinha(Melodia* melodia, FILE* fp, int tam, char caracter){
    char string[500];
    fgets(string, 500, fp);
    char* token = strtok(string, " ");
    for(int i = 0; i < tam; i++){
        int nota = converteNota(token[0]);
        if(strlen(token) > 1)
            if(token[1] == 'b')
                nota--;
            if(token[1] == '#')
                nota++;

        if(caracter == 'm')
            melodia->musica[i] = nota;
        if(caracter == 'p')
            melodia->padrao[i] = nota;
        token = strtok(NULL, " ");
    }
}

Melodia* leMelodia(FILE* fp){
    Melodia* melodia = obtemTamanhoMelodia(fp);
    if(melodia == NULL)
        return NULL;
    leLinha(melodia, fp, melodia->tamMusica, 'm');
    leLinha(melodia, fp, melodia->tamPadrao, 'p');
    return melodia;
}

void printaResultado(FILE* fp, int index){
    if(index == -1)
        fprintf(fp, "N\n");
    if(index >= 0)
        fprintf(fp, "S %d\n", index);
    
}