#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "entradaSaida.h"
#include "logica.h"
#include <sys/time.h>
#include <sys/resource.h>

void calculaTempoProcessador(struct rusage usage){
    getrusage(RUSAGE_SELF, &usage);
    
    printf("\nTempo no modo de usuário rodando o processo: %ld.%06ld segundos\n", 
        usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("Tempo no modo de sistema rodando o processo: %ld.%06ld segundos\n",
        usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    
    long total_sec = usage.ru_utime.tv_sec + usage.ru_stime.tv_sec, 
        total_usec = usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
    
    if(total_usec > 1000000){
        total_usec -= 1000000;
        total_sec++;
    }
    printf("Tempo total do processo no processador: %ld.%06ld segundos\n",
        total_sec, total_usec);
    printf("Memória máxima usada ao mesmo tempo: %ld KB\n", usage.ru_maxrss);
}

void calculaTempoReal(struct timeval start, struct timeval end){
    long tempoRealMicro = abs(start.tv_usec - end.tv_usec);
    int diminuirSegundo = 0;
    if(start.tv_usec > end.tv_usec){
        tempoRealMicro = 1000000 - tempoRealMicro;
        diminuirSegundo = 1;
    }
    long tempoRealSegundo = abs(start.tv_sec - end.tv_sec);
    if(diminuirSegundo)
        tempoRealSegundo--;
    printf("Tempo passado na vida real: %ld.%06ld segundos\n",
        tempoRealSegundo, tempoRealMicro);
}

int main(int argc, char* argv[]){
    FILE* fe = fopen(argv[1], "r");
    struct timeval start, end;
    struct rusage usage;
    bool* excecao = (bool*)malloc(sizeof(bool));
    *excecao = false;
    FILE* fs = fopen("saida.txt", "w");
    int tipo = atoi(argv[2]);
    if(fe == NULL){
        printf("Erro na abertura do arquivo de entrada!\n");
        return 0;
    }
    while(1){
        gettimeofday(&start, NULL);
        if(feof(fe))
            break;
        Melodia* melodia = leMelodia(fe, fs, excecao);
        if(melodia == NULL && !(*excecao))
            break;
        if(melodia == NULL && (*excecao)){
            *excecao = false;
            continue;
        }
        int index = achePlagio(melodia, tipo);
        if(index == -2){
            destroiMelodia(melodia);
            break;
        }

        // printf("LPS = ");

        // for(int i = 0; i < melodia->tamPadrao; i++)
        //     printf("%d ", melodia->LPS[i]);
        // printf("\n");

        printaResultado(index, fs);
        // printf("%d\n", index);
        // for(int i = 0; i < melodia->tamMusica; i++)
        //     printf("%d ", melodia->musica[i]);
        // printf("\n");
        // for(int i = 0; i < melodia->tamPadrao; i++)
        //     printf("%d ", melodia->padrao[i]);
        // printf("\n");


        destroiMelodia(melodia);
        gettimeofday(&end, NULL);
        printf("====================================================================================\n");

        calculaTempoProcessador(usage);
        calculaTempoReal(start, end);

        printf("====================================================================================\n");
    }
    fclose(fs);
    fclose(fe);
    free(excecao);
    return 0;
}