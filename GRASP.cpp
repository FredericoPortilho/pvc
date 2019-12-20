#include "Util.h"
#include "Construcao.h"
#include "Descida.h"
#include "GRASP.h"

float GRASP(int n, int *s, float **d, int iterMax, int metodo)
{
    float fo, fo_star;
    int iter = 0;
    int *s_star = cria_vetor(n);

    atualiza_vetor(s_star, s, n);

    fo = calcula_fo(n, s, d);
    fo_star = fo;

    while (iter < iterMax) {
        iter++;
        switch(metodo) {
            case 1:
                constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n, s, d, 0.05);
                break;
            case 2:
                constroi_solucao_parcialmente_gulosa_insercao_mais_barata(n, s, d, 0.05);
                break;
        }

        fo = descida_primeiro_melhora(n, s, d);

        if (fo < fo_star) {
            fo_star = fo;
            atualiza_vetor(s_star, s, n);
        }
    }

    fo = fo_star;
    atualiza_vetor(s, s_star, n);
    libera_vetor(s_star);
    return fo;
}
