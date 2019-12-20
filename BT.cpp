#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "Util.h"
#include "Arquivos.h"
#include "Descida.h"


float melhor_vizinhoBT(int n, int *s, float **d, float fo, int *melhor_i, int *melhor_j, float fo_star, int **T, int iterBT)
{
    int aux, i, j/*, melhorou = 0*/;
    float fo_melhor_viz = INT_MAX, fo_viz, delta1, delta2;

    for(i = 0; i < (n - 1) /*&& !melhorou*/; i++) {
        for(j = (i + 1); j < n; j++) {
            // Calcula a variacao de custo com a realizacao do movimento
            delta1 = calcula_delta(n, s, d, i, j);

            // Faz o movimento
            aux = s[i];
            s[i] = s[j];
            s[j] = aux;

            delta2 = calcula_delta(n, s, d, i, j);

            // Desfaz o movimento
            aux = s[i];
            s[i] = s[j];
            s[j] = aux;

            // Calcular a nova distancia
            fo_viz = fo - delta1 + delta2;

            // Armazenar o melhor movimento (melhor troca)
            if(fo_viz < fo_star)
            {
                fo_star = fo_viz;
                *melhor_i = i;
                *melhor_j = j;
                fo_melhor_viz = fo_viz;
                //melhorou = 1;
                //break;
            } else if(T[i][j] < iterBT) {
                if(fo_viz < fo_melhor_viz)
                {
                    fo_melhor_viz = fo_viz;
                    *melhor_i = i;
                    *melhor_j = j;
                }
            }
        }
    }
    return fo_melhor_viz;
}

float BT(int n, int *s, float **d, int BTmax, int **T, int duracao)
{
    float fo_star,fo;
    float fo_viz;
    int melhorIter, iterBT,*s_star;
    int melhor_i,melhor_j;
    int aux;

    iterBT = 0;
    melhorIter = 0;
    s_star = cria_vetor(n);
    atualiza_vetor(s_star, s, n);
    fo = calcula_fo(n, s, d);
    fo_star = fo;
    while((iterBT - melhorIter) < BTmax)
    {
        iterBT++;

        fo_viz = melhor_vizinhoBT(n, s, d, fo, &melhor_i, &melhor_j, fo_star, T, iterBT);

        T[melhor_i][melhor_j] = iterBT + duracao;

        aux = s[melhor_i];
        s[melhor_i] = s[melhor_j];
        s[melhor_j] = aux;

        fo = fo_viz;

        if(fo < fo_star)
        {
            melhorIter = iterBT;
            atualiza_vetor(s_star, s, n);
            fo_star = fo;
        }

    }
    fo = fo_star;
    atualiza_vetor(s, s_star, n);
    libera_vetor(s_star);
    return fo;
}
