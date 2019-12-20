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

void pertubacao(int n, std::vector<int> *s, float **d, int nivel){
     std:vector<int> s_estrela = *s;
     std::srand(std::time(nullptr)); // use current time as seed for random generator

     std::random_device rd;  //Will be used to obtain a seed for the random number engine
     std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
     std::uniform_int_distribution<> dis(1, n);
     int p1 = dis(gen);

     int nmodificacoes = nivel +1;
     int cont = 1;
     while(cont <= nmodificacoes){
         if(p1 == n-1)
            swap(s[0],s[n-1]);
         else
            swap(s[p1],s[p1+1]);

         cont++;
     }
}

float ils(int n, std::vector<int> *s, float **d, int iter_max)
{
    float fo_star, fo;
    clock_t inicio_CPU, fim_CPU;
    vector<int> s_star;


    fo = descida_completa(n, s, d);

    int iter = 0;
    int melhorIter = 0;
    int nivel = 1;


    fo_star = 1000000;

    //atualiza s_star
    s_star = *s;

    fo = calcula_fo(n, s, d);

    limpa_arquivo((char*)"ILS.txt");
    inicio_CPU = fim_CPU = clock();
    imprime_fo((char*)"ILS.txt", (fim_CPU - inicio_CPU)/CLK_TCK,fo_star,0);
    int aux = 0;
    int ilsMax = 20;
    while (iter < ilsMax){
        iter ++;
        pertubacao(n, &s_star, d, nivel);
        fo_star = descida_randomica(n, &s_star, d, 1000);
        if(fo < fo_star){
            melhorIter = iter;
            nivel = 1;
        }
        else{
            nivel++;
        }

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
