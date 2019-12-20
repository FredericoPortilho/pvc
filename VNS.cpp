#include "Util.h"
#include "Descida.h"
#include "VND.h"
#include "VNS.h"

float VNS(int n, int *s, float **d, int r, int iterMax)
{
    float fo, fo_star;
    int k, iter;
    int *s_star = cria_vetor(n);

    atualiza_vetor(s_star, s, n);
    fo = calcula_fo(n, s, d);
    fo_star = fo;
    iter = 0;

    while(iter < iterMax) {
        k = 1;
        iter++;

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
    }

    fo = fo_star;
    atualiza_vetor(s, s_star, n);
    libera_vetor(s_star);
    return fo;
}
