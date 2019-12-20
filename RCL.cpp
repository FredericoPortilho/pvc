
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "Construcao.h"
#include "Util.h"
#include "Descida.h"
#include "GRASP.h"



int roleta(int nind, std::vector<float> *fo_pop);


float RCL(int n, std::vector<int> *s, float **d,  int GRASP_max){
   int *s_star;
   int nAlphas = 4;
   std::vector<float> alphas = {0.1,0.2,0.3,0.4};
   std::vector<float> probabilidades = {0.25,0.25,0.25,0.25};
   std::vector<float> soma = {0,0,0,0};
   std::vector<float> media = {0,0,0,0};
   std::vector<float> q = {0,0,0,0};
   std::vector<float> n = {0,0,0,0};
   std::vector<float> roleta = {0.25,0.50,0.75,1};
   float melhor;


   int k = 20;

   s_star = *s;
   while (iter < GRASP_max){
     iter++;
     if (tipo_construcao == 1)
       unsigned seed = chrono::system_clock::now().time_since_epoch().count();
       default_random_engine r(seed);
       int i;
       for(i=0; i<nAlphas-1; i++){
            if(r > roleta[i]){
                alpha = alphas[i];
                n[i]++;
                break;
            }
       }

       constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n,&s,d,alpha);
       fo = descida_completa(n,s,d);


       soma[i] += fo;

       if(fo < fo_star){
         fo_star = fo;
         s_star = *s;
       }

       if(melhor < fo_star){
         melhor = fo;
       }

       int somaQ = 0;
      if(iter = k){
        for(int i=0; i<nAlphas-1;i++){
            media[i] = soma[i]/n[i];
            q[i] = melhor/media[i];
            somaQ += q[i];
        }
        for(int i=0; i<nAlphas-1;i++){
           p[i] = q[i]/somaQ;
           if(i==0)
                roleta[i] = p[i]
           else
                roleta[i] = roleta[i-1] + p[i];
        }


        k = 1;



      }
      k++;
      melhor = 1000000000;
   }
   atualiza_vetor(s, s_star, n);

   libera_vetor(s_star);
   return fo_star;
}

