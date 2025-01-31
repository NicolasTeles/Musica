#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "logica.h"

void leLinha(Melodia* melodia, FILE* fp, int tam, char caractere){
    char* string = (char*)calloc(melodia->tamMusica*3, sizeof(char));
    fgets(string, melodia->tamMusica*3, fp);
    char* token = strtok(string, " ");
    int i;
    for(i = 0;  ; i++){
        if(token == NULL){
            printf("i: %d\n", i);
            break;
        }
        if(i == tam){
            i = tam+1;
            break;
        }
        int nota = converteNota(token[0]);
        if(strlen(token) > 1)
            if(token[1] == 'b')
                nota--;
            if(token[1] == '#')
                nota++;

        if(caractere == 'm'){
            melodia->musica[i] = nota;
            if(i > 0){
                melodia->intervalosMusica[i-1] = distanciaMin(melodia->musica[i] - melodia->musica[i-1]);
                // contabilizar que subir e descer 6 meio-tons é a mesma coisa
                if(melodia->intervalosMusica[i-1] == -6)
                    melodia->intervalosMusica[i-1] = 6;
            }
        }
        if(caractere == 'p'){
            melodia->padrao[i] = nota;
            if(i > 0){
                melodia->intervalosPadrao[i-1] = distanciaMin(melodia->padrao[i] - melodia->padrao[i-1]);
                // contabilizar que subir e descer 6 meio-tons é a mesma coisa
                if(melodia->intervalosPadrao[i-1] == -6)
                    melodia->intervalosPadrao[i-1] = 6;
            }
        }
        token = strtok(NULL, " ");
    }
    if(i == tam){
        free(string);
        return;
    }
    printf("\nTamanho de ");

    if(caractere == 'm')
        printf("musica ");
    if(caractere == 'p')
        printf("padrao ");

    if(i < tam)
        printf("menor ");
    if(i > tam)
        printf("maior ");
    
    printf("que o esperado!\n");
    printf("i=%d tam=%d\n", i, tam);
    destroiMelodia(melodia);
    free(string);
    exit(1);
    melodia = NULL;
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

void printaResultado(int index, FILE* fs){
    if(index == -1)
        fprintf(fs, "N\n");
    if(index >= 0)
        fprintf(fs, "S %d\n", index);
    
}