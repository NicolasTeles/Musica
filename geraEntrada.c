#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* escreverEntradas(FILE* arquivo, int quantidade) {
    //essa string aq e o trecho original, da pra alterar mas vai ter que mudar no programa inteiro
    char original[3] = {"AB"};

    fprintf(arquivo, "%d 2\n", quantidade); 

    for (int i = 0; i < quantidade-2; i++) { 
        fprintf(arquivo, "A ");
    }

    for (int i = 0; i < 2; i++) {
        if(i == 1){
            fprintf(arquivo, "%c", original[i]);
        }else{
            fprintf(arquivo, "%c ", original[i]);
        }
    }
    fprintf(arquivo, "\n");

    for (int i = 0; i < 2; i++) {
        if(i == 1){
            fprintf(arquivo, "%c", original[i]);
        }else{
            fprintf(arquivo, "%c ", original[i]);
        }
    }
    fprintf(arquivo, "\n");
    
}

int main() {
    //e nesse numero aq que voce altera a quantidade de notas do trecho suspeito
    //basta trocar aq e recompilarr
    FILE* f = fopen("entradaTeste.txt", "w");
    for(int i = 200; i < 1001; i += 200)
        escreverEntradas(f, i);
    fprintf(f,"0 0");
    fclose(f);
    return 0;
}