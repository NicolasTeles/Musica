#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "logica.h"

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

        if(caracter == 'm'){
            melodia->musica[i] = nota;
            if(i > 0)
                melodia->intervalosMusica[i-1] = distanciaMin(melodia->musica[i] - melodia->musica[i-1]);
        }
        if(caracter == 'p'){
            melodia->padrao[i] = nota;
            if(i > 0)
                melodia->intervalosPadrao[i-1] = distanciaMin(melodia->padrao[i] - melodia->padrao[i-1]);
        }
        token = strtok(NULL, " ");
    }
}

Melodia* leMelodia(FILE* fp){
    Melodia* melodia = obtemTamanhoMelodia(fp);
    if(melodia == NULL)
        return NULL;
    leLinha(melodia, fp, melodia->tamMusica, 'm');
    leLinha(melodia, fp, melodia->tamPadrao, 'p');
    // for(int i = 0; i < melodia->tamMusica-1; i++)
    //     printf("%d ", melodia->intervalosMusica[i]);
    // printf("\n");
    // for(int i = 0; i < melodia->tamPadrao-1; i++)
    //     printf("%d ", melodia->intervalosPadrao[i]);
    // printf("\n");
    return melodia;
}

void printaResultado(FILE* fp, int index){
    if(index == -1)
        fprintf(fp, "N\n");
    if(index >= 0)
        fprintf(fp, "S %d\n", index);
    
}