#include <stdlib.h>
#include "Util.h"
#include "Descida.h"
#include "VND.h"

float shake(int n, int *s, float **d, float fo, int k)
{
    int iter = 1;
    int i, j, aux;
    float fo_viz;

    while(iter <= k)
    {
        i = rand() % n;
        do {
            j = rand() % n;
        }
        while(i == j);

        float delta1 = calcula_delta(n, s, d, i, j);

        // Faz o movimento
        aux = s[j];
        s[j] = s[i];
        s[i] = aux;

        float delta2 = calcula_delta(n, s, d, i, j);

        // Calcular a nova distancia
        fo_viz = fo - delta1 + delta2;
        iter++;

    }
    return fo_viz;
}

float VND(int n, int *s, float **d, int r)
{

    float fo, fo_star;
    int k = 1;
    int *s_star = cria_vetor(n);

    atualiza_vetor(s_star, s, n);
    fo = calcula_fo(n, s, d);
    fo_star = fo;

    while(k <= r) {
        fo = shake(n, s, d, fo, k);
        fo = descida(n, s, d);

        if (fo < fo_star) {
            fo_star = fo;
            atualiza_vetor(s_star, s, n);
            k = 1;
        } else {
            k++;
        }
    }

    fo = fo_star;
    atualiza_vetor(s, s_star, n);
    libera_vetor(s_star);
    return fo;
}
