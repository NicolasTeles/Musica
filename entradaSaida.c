#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "entradaSaida.h"
#include "logica.h"

void leLinha(Melodia* melodia, FILE* fp, FILE* fs, int tam, char caractere, bool* excecao){
    char* string = (char*)calloc(melodia->tamMusica*3, sizeof(char));
    fgets(string, melodia->tamMusica*3, fp);
    char* token = strtok(string, " ");
    int i;
    for(i = 0;  ; i++){
        if(token == NULL)
            break;
        if(i == tam){
            i = tam+1;
            break;
        }
        int nota = converteNota(token[0]);
        if(nota == -1){
            notaInvalida(melodia, fp, fs, token[0], caractere, excecao);
            i = tam;
            break;
        }
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
    tamanhoInvalido(melodia, fp, fs, i, tam, caractere, excecao);
    free(string);
}

Melodia* leMelodia(FILE* fp, FILE* fs, bool* excecao){
    Melodia* melodia = obtemTamanhoMelodia(fp, fs, excecao);
    if(melodia == NULL)
        return NULL;
    leLinha(melodia, fp, fs,  melodia->tamMusica, 'm', excecao);
    if((*excecao)){
        destroiMelodia(melodia);
        melodia = NULL;
        return NULL;
    }
    leLinha(melodia, fp, fs, melodia->tamPadrao, 'p', excecao);
    if((*excecao)){
        destroiMelodia(melodia);
        melodia = NULL;
        return NULL;
    }
    return melodia;
}

void printaResultado(int index, FILE* fs){
    if(index == -1)
        fprintf(fs, "N\n");
    if(index >= 0)
        fprintf(fs, "S %d\n", index);
    
}

// Funções para Tratamento de Exceções
void padraoMaior(FILE* fp, FILE* fs, int tamPadrao, bool* excecao){
    char* string = (char*)calloc(tamPadrao*3, sizeof(char));
    fgets(string,tamPadrao*3 , fp);
    fgets(string,tamPadrao*3 , fp);
    fprintf(fs, "Tamanho do padrão maior que a música!\n");
    *excecao = true;
    free(string);
}

void notaInvalida(Melodia* m, FILE* fp, FILE* fs, char nota, char caractere,bool* excecao){
    char* string = (char*)calloc(m->tamMusica*3, sizeof(char));
    if(caractere == 'm')
        fgets(string, m->tamMusica*3, fp);
    fprintf(fs,"Nota %c inválida!\n", nota);
    *excecao = true;
    free(string);
}

void tamanhoInvalido(Melodia*m, FILE* fp, FILE* fs, int index, int tam, char caractere, bool* excecao){
    char* string = (char*)calloc(m->tamMusica*3, sizeof(char));
    if(caractere == 'm')
        fgets(string, m->tamMusica*3, fp);
    fprintf(fs,"Tamanho de ");

    if(caractere == 'm')
        fprintf(fs,"musica ");
    if(caractere == 'p')
        fprintf(fs,"padrao ");

    if(index < tam)
        fprintf(fs,"menor ");
    if(index > tam)
        fprintf(fs,"maior ");
    fprintf(fs, "que o esperado!\n");
    *excecao = true;
    free(string);
}