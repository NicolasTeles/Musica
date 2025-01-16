#include <stdio.h>
#include <stdlib.h>
#include "entradaSaida.h"
#include "logica.h"

int main(int argc, char* argv[]){
    FILE* fe = fopen(argv[1], "r");

    //remover arquivo de saida, mantendo assim para ficar mais organizado e ser possivel debugar pelo terminal
    FILE* fs = fopen("saida.txt", "w");
    int tipo = atoi(argv[2]);
    if(fe == NULL){
        printf("Erro na abertura do arquivo de entrada!\n");
        return 0;
    }
    while(1){
        if(feof(fe))
            break;
        Melodia* melodia = leMelodia(fe);
        if(melodia == NULL)
            break;
        for(int i = 0; i < melodia->tamFita; i++)
            printf("%d ", melodia->fitaLeitura[i]);
        printf("\n\n\n");
        int index = KMP(melodia);
        printaResultado(fs, index);
        printf("%d\n", index);
        for(int i = 0; i < melodia->tamMusica; i++)
            printf("%d ", melodia->musica[i]);
        printf("\n");
        for(int i = 0; i < melodia->tamPadrao; i++)
            printf("%d ", melodia->padrao[i]);
        printf("\n");
        destroiMelodia(melodia);
    }
    fclose(fs);
    fclose(fe);
    return 0;
}