#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "logica.h"

void leLinha(Melodia* melodia, FILE* fp, int tam, char caracter){
    char string[500];
    fgets(string, 500, fp);
    char* token = strtok(string, " ");
    melodia->fitaLeitura[0] = 0;
    for(int i = 0; i < tam; i++){
        int nota = converteNota(token[0]);
        if(strlen(token) > 1)
            if(token[1] == 'b')
                nota--;
            if(token[1] == '#')
                nota++;

        if(caracter == 'm'){
            melodia->musica[i] = nota;
            melodia->fitaLeitura[i+melodia->tamPadrao+2] = nota;
        }
        if(caracter == 'p'){
            melodia->padrao[i] = nota;
            melodia->fitaLeitura[i+1] = nota;
        }
        token = strtok(NULL, " ");
    }
    melodia->fitaLeitura[melodia->tamPadrao+1] = -1;
    melodia->fitaLeitura[melodia->tamFita-1] = 13;
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