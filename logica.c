#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "logica.h"
#include "entradaSaida.h"

Melodia* obtemTamanhoMelodia(FILE* fp, FILE* fs, bool* excecao){
    char string[50];
    fgets(string, 50, fp);
    char* token = strtok(string, " ");
    int tamanhoMusica = atoi(token);
    token = strtok(NULL, " ");
    int tamanhoPadrao = atoi(token);

    if(tamanhoMusica == 0 && tamanhoPadrao == 0)
        return NULL;
    
    if(tamanhoMusica < tamanhoPadrao){
        padraoMaior(fp, fs, tamanhoPadrao, excecao);
        return NULL;
    }
        
    

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
        default:
            return -1;
    }
}

// Calcula se essa distância é o menor módulo ou o seu complementar, para aceitar o ciclo.
int distanciaMin(int diff){
    int absDiff = abs(diff);
    int multiplicador;
    if(diff < 0)
        multiplicador = 1;
    else
        multiplicador = -1;
    return absDiff < 12-absDiff ? diff : multiplicador*(12-absDiff);
} 

Melodia* criaMelodias(int tamMusica, int tamPadrao){
    Melodia* m = (Melodia*)malloc(sizeof(Melodia));
    m->musica = (int*)calloc(tamMusica, sizeof(int));
    m->tamMusica = tamMusica;
    m->padrao = (int*)calloc(tamPadrao, sizeof(int));
    m->tamPadrao = tamPadrao;

    //KMP
    m->LPS = (int*)calloc(tamPadrao, sizeof(int));

    //BMH e Shift-And
    m->intervalosPadrao = (int*)calloc(tamPadrao-1, sizeof(int));
    m->tamIntervaloPadrao = tamPadrao-1;
    m->intervalosMusica = (int*)calloc(tamMusica-1, sizeof(int));
    m->tamIntervaloMusica = tamMusica-1;
    m->tabelaDeslocamento = (int*)calloc(13, sizeof(int));
    
    //Shift-And
    //Essa parte é para saber se o tamanho ultrapassa 64 bits para criar outro vetor para compor ao fazer o shift
    int n = (int)(tamPadrao-1)/64;
    if(((tamPadrao-1)%64) > 0)
        n++;
    m->linhaMascara = n;
    m->mascara = (long**)calloc(n, sizeof(long*));
    for(int i=0; i < n; i++)
        m->mascara[i] = (long*)calloc(13, sizeof(long));
    return m;
}

void destroiMelodia(Melodia* m){
    if(m == NULL)
        return;
    //KMP
    free(m->LPS);

    //BMH
    free(m->intervalosPadrao);
    free(m->intervalosMusica);
    free(m->tabelaDeslocamento);

    //Shift-And
    for(int i = 0; i < m->linhaMascara; i++){
        free(m->mascara[i]);
        m->mascara[i] = NULL;
    }
    free(m->mascara);

    free(m->musica);
    free(m->padrao);
    free(m);
}

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

void criaLPS(Melodia* melodia){
    if(melodia == NULL)
        return;
    melodia->LPS[0] = 0;
    int len = 0;

    int i = 1;
    while(i < melodia->tamIntervaloPadrao){
        if(melodia->intervalosPadrao[i] == melodia->intervalosPadrao[len]){
            len++;
            melodia->LPS[i] = len;
            i++;
        } else {
            if(len != 0){
                len = melodia->LPS[len-1];
            } else {
                melodia->LPS[i] = 0;
                i++;
            }
        }
    }
}

int KMP(Melodia* melodia){
    criaLPS(melodia);
    
    int i = 0, j = 0;
    while(i < melodia->tamIntervaloMusica){
        if(j == melodia->tamIntervaloPadrao)
            return i - melodia->tamIntervaloPadrao;

        if(melodia->intervalosMusica[i] == melodia->intervalosPadrao[j]){
            i++;
            j++;
        } else {
            if(j != 0){
                j = melodia->LPS[j-1];
            } else {
                i++;
            }
        }
    }

    if(j == melodia->tamIntervaloPadrao)
        return i - melodia->tamIntervaloPadrao;

    return -1;
}

void criaTabelaDeslocamentos(Melodia* melodia){
    int j;
    for(j = 0; j < 13; j++)
        melodia->tabelaDeslocamento[j] = melodia->tamIntervaloPadrao;
    for(j = 1; j < melodia->tamIntervaloPadrao; j++)
        melodia->tabelaDeslocamento[6+melodia->intervalosPadrao[j-1]] = melodia->tamIntervaloPadrao-j;
}

int BMH(Melodia* melodia){
    if(melodia == NULL)
        return -1;
    criaTabelaDeslocamentos(melodia);
    
    int i = melodia->tamIntervaloPadrao;
    while(i <= melodia->tamMusica-1){
        int k = i;
        int j = melodia->tamIntervaloPadrao;
        while(j > 0 && melodia->intervalosPadrao[j-1] == melodia->intervalosMusica[k-1]){
            k--;
            j--;
        }
        if(j == 0){
            return k;
        }
        i += melodia->tabelaDeslocamento[6+melodia->intervalosMusica[i-1]];
    }
    
    return -1;
}

int shiftAnd(Melodia* melodia){
    if(melodia == NULL)
        return -1;
    //define as mascaras
    int i, k;
    int m = melodia->tamIntervaloPadrao;
    int j = (melodia->tamIntervaloPadrao%64)-1; 
    for(k = 0; k < melodia->linhaMascara; k++)//separa o vetor com a divisão do tamanho do padrão para suportar mais que 64 de tamamnho
        for(i = 0; i < 13; i++) //para cada diferença existente
            melodia->mascara[k][i] = 0;
    i=0;
    while(i < m){
        for(k = 0; k < melodia->linhaMascara; k++){
            for(; j>=0; j--){
                melodia->mascara[k][6+melodia->intervalosPadrao[i]] |= (1L << j); //lê a diferença e coloca 1 na sua posição correspondente
                i++;
            }
            j = 63;
        }
    }   
    //procura o plagio
    j = (melodia->tamIntervaloPadrao%64)-1;
    long* r = (long*)calloc(melodia->linhaMascara, sizeof(long)); //vetor R que é feito para suportar tamanho do padrão maior que 65
    bool* boolean = (bool*)calloc(melodia->linhaMascara, sizeof(bool)); //vetor que vai verificar se anteriormente o LSB do anterior estava setado e com isso o MSB do atual deve ser setado
    for(k = 0; k < melodia->linhaMascara; k++)
        boolean[k] = false; //inicializa todos como false
    for(i = 0; i < melodia->tamIntervaloMusica; i++){
        for(k = 0; k < melodia->linhaMascara; k++){
            if(boolean[k]){ //verifica se no shift anterior se o LSB do seu antecessor tava setado
                r[k] |= (1L << 63); //complementa com 1 no seu MSB
                boolean[k] = false;
            }
            if(k == 0){ //shift complementa com 1 e faz and com a mascara
                r[k] = ((r[k] >> 1) | (1L << j)) & melodia->mascara[k][6+melodia->intervalosMusica[i]];
            }
            else //se não for o k=0 só precisa shiftar e fazer and
                r[k] = ((r[k] >> 1)) & melodia->mascara[k][6+melodia->intervalosMusica[i]];
            
            if((k != 0) && (r[k-1]%2 == 1)) //verifica se o LSB do antecessor está setado para que no proximo shift haja o complemento
                boolean[k] = true;
        }
        if(r[melodia->linhaMascara-1]%2 == 1){ // verifica se na ultima parte está com o LSB setado, caso esteja encontrou casamento
            free(r);
            free(boolean);
            return i - (m-1);
        }
    }
    free(r);
    free(boolean);
    return -1;
}

int achePlagio(Melodia* melodia, int tipo){
    if(melodia == NULL)
        return -1;
    int retorno = -1;
    switch (tipo){
    case 1:
        retorno = forcaBruta(melodia);
        break;
    case 2:
        retorno = KMP(melodia);
        break;
    case 3:
        retorno = BMH(melodia);
        break;
    case 4:
        retorno = shiftAnd(melodia);
        break;
    default:
        printf("Tipo inválido\n");
        return -2;
    }
    return retorno;   
}