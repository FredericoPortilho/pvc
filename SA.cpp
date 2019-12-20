//
// Created by Puca Penna on 13/09/2018.
//
#include <cmath>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "SA.h"
#include "Util.h"
#include "Descida.h"

using namespace std;

float SA(int n, int *s, float **d, float alpha, float SAmax, double temp_inicial, double temp_final)
{
    float fo, fo_viz, fo_star;
    int melhor_i, melhor_j, aux;
    float delta, x;
    int iterT = 0;
    int *s_star;
    double temp = temp_inicial;
    int i,j;
    s_star = cria_vetor(n);
    atualiza_vetor(s_star, s, n);
    fo = fo_star = fo_viz = calcula_fo(n, s, d);

    // implementar
    while(temp>temp_final)
    {

        while(iterT<SAmax)
        {
            iterT++;
            i = rand()%n;
            do{
                j = rand()%n;
            }while (j == i);

            float delta1 = calcula_delta(n,s,d,i,j);
            aux = s[i];
            s[i] = s[j];
            s[j] = aux;
            float delta2 = calcula_delta(n,s,d,i,j);
            fo_viz = fo - delta1 + delta2;
            delta = fo_viz - fo;
            if(delta<0)
            {
                fo = fo_viz;
                if(fo_viz<fo_star)
                {
                     fo_star = fo_viz;
                     atualiza_vetor(s_star, s, n);
                }
            }else
            {
                double x = randomico(0,1);
                if(x<exp(-delta/temp))
                {
                    fo = fo_viz;
                }
                else{
                    aux = s[i];
                    s[i] = s[j];
                    s[j] = aux;
                }
            }
        }
        temp = alpha*temp;
        iterT=0;
    }
    atualiza_vetor(s,s_star,n);
    libera_vetor(s_star);
    return fo_star;
}

float temperaturaInicial(int n, int *s, float **d, float beta, float gama, float SAmax, double temp_inicial)
{
    double temp = temp_inicial;
    int melhor_i, melhor_j;
    bool continua = true;
    float fo_viz, fo, fo_star;
    int aceitos;
    double delta;
    double x;

    fo = fo_star = fo_viz = calcula_fo(n, s, d);

    while(continua){
        aceitos = 0;
        for (int iterT = 1; iterT <= SAmax; iterT++){
            fo_viz = vizinho_aleatorio(n, s, d, fo, &melhor_i, &melhor_j);
            delta = fo_viz - fo;
            if (delta < 0)
                aceitos++;
            else{
                x = randomico(0,1);
                if ( x < exp(-delta/temp))
                    aceitos++;
            }
        }

        if(aceitos >= gama * SAmax)
            continua = false;
        else
            temp = beta * temp;

    }

    return temp;
}

