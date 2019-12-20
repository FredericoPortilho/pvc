//
// Created by Puca Penna on 13/09/19.
//


#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "Descida.h"
#include "Util.h"
#include "Construcao.h"
#include "Arquivos.h"

#ifndef CLK_TCK
#define  CLK_TCK 1000000
#endif

float MS(int n, std::vector<int> *s, float **d, int iter_max)
{
    float fo_star, fo;
    clock_t inicio_CPU, fim_CPU;
    vector<int> s_star;

    //atualiza s_star
    s_star = *s;

    fo = calcula_fo(n, s, d);
    fo_star = 1000000;
    limpa_arquivo((char*)"MS.txt");
    inicio_CPU = fim_CPU = clock();
    imprime_fo((char*)"MS.txt", (fim_CPU - inicio_CPU)/CLK_TCK,fo_star,0);
    int aux = 0;
    while (aux < iter_max){
        constroi_solucao_aleatoria(n, s, d);
        fo = descida_randomica(n, s, d, 1000);

        if (fo < fo_star){
            s_star = *s;
            fo_star = fo;
        }
        else
            aux++;

    }
    *s = s_star;
    return fo_star;


    /*
     *
     * implementar o loop da multi-start
     *
     *
     */

    fim_CPU = clock();
    imprime_fo((char*)"MS.txt", (fim_CPU - inicio_CPU)/CLK_TCK,fo_star,0);

    // atualiza s
    *s = s_star;
    return fo_star;
}


