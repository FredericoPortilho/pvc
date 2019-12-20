
/*
Tecnicas Heuristicas para resolucao do Problema do Caixeiro Viajante
Autor: Marcone Jamilson Freitas Souza
Local: Departamento de Computacao - Universidade Federal de Ouro Preto
Homepage: www.decom.ufop.br/prof/marcone
Data: 21-05-2007
Atualizado por Puca Huachi em ago/2019
*/

//---------------------------------------------------------------------------
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <climits>
#include <ctime>
#include <iomanip>
#include <vector>
#include "Util.h"
#include "Arquivos.h"
#include "Construcao.h"
#include "Menus.h"
#include "Descida.h"
#include "MS.h"
#include "RCL.h"
#include "ILS.h"
#include "SA.h"
#include "AG.h"


//---------------------------------------------------------------------------
using namespace std;


int main(int argc, char* argv[])
{
  int n;                    // numero de cidades
  vector<int> s;            // vetor solucao corrente
  float **d,                // matriz de distancias entre as cidades
        fo,                 // funcao objetivo corrente
        melhor_fo_lit;      // melhor fo da literatura
  clock_t inicio_CPU,       // clock no inicio da aplicacao do metodo
          fim_CPU;          // clock no final da aplicacao do metodo
  int iterMax;

    int nind;
    float max_desvio;
    double prob_crossover;
    double prob_mutacao;

  float temp;

  //srand(1000); // pega o numero 1000 como semente de numeros aleatorios
  srand((unsigned) time(NULL)); // pega a hora do relogio como semente

  obter_parametros_pcv((char*)"data/c50info.txt", &n, &melhor_fo_lit);
  d = cria_matriz_float(n, n); // matriz de distancias
  le_arq_matriz((char*)"data/c50.txt", n, d);


  int escolha = 0;
  do {
    escolha = menu_principal();
    switch (escolha) {
    case 1: /* Geracao de uma solucao inicial */
           switch(menu_solucao_inicial()) {
           case 1: /* Geracao gulosa de uma solucao inicial via metodo do vizinho mais proximo */
                 constroi_solucao_gulosa_vizinho_mais_proximo(n,&s,d);
                 fo = calcula_fo(n, &s, d);
                 printf("\nSolucao construida de forma gulosa (Vizinho Mais Proximo):\n");
                 imprime_rota(&s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           case 2: /* Geracao parcialmente gulosa de uma solucao inicial via metodo do vizinho mais proximo */
                 constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n,&s,d,0.05);
                 fo = calcula_fo(n, &s, d);
                 printf("\nSolucao construida de forma gulosa (Vizinho Mais Proximo):\n");
                 imprime_rota(&s, n);
                 printf("Funcao objetivo = %f\n",fo);
//                   printf("Nao implementado\n");
	         break;
           case 3: /* Geracao gulosa de uma solucao inicial via metodo da insercao mais barata */
                 constroi_solucao_gulosa_insercao_mais_barata(n,&s,d,1);
                 fo = calcula_fo(n, &s, d);
                 printf("\nSolucao construida de forma gulosa (Insercao Mais Barata):\n");
                 imprime_rota(&s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           case 4: /* Geracao parcialmente gulosa de uma solucao inicial via insercao mais barata */
                 printf("Ainda nao implementado...\n");
	         break;
           case 5: /* Geracao aleatoria de uma solucao inicial */
                 constroi_solucao_aleatoria(n, &s, d);
                 fo = calcula_fo(n, &s, d);
                 printf("\nSolucao construida de forma aleatoria:\n");
                 imprime_rota(&s, n);
                 printf("Funcao objetivo = %f\n",fo);
	         break;
           }
           break;
    case 2: /* Descida Completa (best improvement)*/
           inicio_CPU = clock();
           fo = descida_completa(n,&s,d);
           fim_CPU = clock();
           printf("\nSolucao obtida usando a estrategia Best Improvement do Metodo da Descida:\n");
           imprime_rota(&s,n);
           printf("Funcao objetivo = %f\n",fo);
           printf("Tempo de CPU = %f segundos:\n",(double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
           break;

    case 3: /* Descida Randomica */
            inicio_CPU = clock();
            iterMax = n*n;
            fo = descida_randomica(n,&s,d,iterMax);
            fim_CPU = clock();
            printf("\nSolucao obtida usando a estrategia aleatoria do Metodo da Descida:\n");
            imprime_rota(&s,n);
            printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos:\n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
            break;

    case 4: /* Descida com primeiro de melhora */
            inicio_CPU = clock();
            iterMax = n;
            fo = descida_primeiro_melhora(n,&s,d);
            fim_CPU = clock();
            printf("\nSolucao obtida usando a estrategia First Improvement do Metodo da Descida:\n");
            imprime_rota(&s,n);
            printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos:\n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
           break;

    case 5: /* Multi-Start */
            inicio_CPU = clock();
            iterMax = n;
            fo = MS(n, &s, d, iterMax);
            fim_CPU = clock();
            printf("\nSolucao obtida usando a meta-heuristica Multi-start:\n");
            imprime_rota(&s,n);
            printf("Funcao objetivo = %f\n",fo);
            fo = calcula_fo(n, &s, d);
            printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos:\n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
           break;

    case 6: /* Simulated Annealing*/
            inicio_CPU = clock();
            //temp = temperaturaInicial(n, &s, d, 2, 0.9, 100, 10);
            //temp = 1000;
            fo = SA(n,&s,d,0.998,500,temp,0.01);
            fim_CPU = clock();
            printf("Solucao obtida usando Simulated Annealing:\n");
            imprime_rota(&s,n);
            printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos\n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
            break;

    case 7: /* Busca Tabu */
           printf("Nao implementado\n");
           break;

    case 8: /* Iterated Local Search */

            inicio_CPU = clock();
            iterMax = n;
            fo = ils(n, &s, d, iterMax);
            fim_CPU = clock();
            printf("\nSolucao obtida usando a meta-heuristica ILS:\n");
            imprime_rota(&s,n);
            printf("Funcao objetivo = %f\n",fo);
            //fo = calcula_fo(n, &s, d);
           // printf("Funcao objetivo = %f\n",fo);
            printf("Tempo de CPU = %f segundos:\n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
           break;

    case 9: /* GRASP */
           switch(menu_GRASP()) {
           case 1: /* Geracao Parcialmente gulosa de uma solucao inicial via metodo do vizinho mais proximo */
                 printf("Nao implementado\n");
                 break;
           case 2: /* Geracao parcialmente gulosa de uma solucao inicial via metodo da insercao mais barata */
	         break;
           }
           break;
    case 10: /* VND */
           printf("Nao implementado\n");
           break;

    case 11: /* VNS */
           printf("Nao implementado\n");
           break;

    case 12: /* Algoritmos Geneticos */
            switch(menu_AG()) {
            case 1: /* Algoritmos Geneticos usando operador OX */
                 nind = 100; max_desvio = 0.02; prob_crossover = 0.85; prob_mutacao = 0.05;
                  inicio_CPU = clock();
                    fo = AG(n, &s, d, nind, max_desvio, prob_crossover, prob_mutacao, 1 /*operador*/);
                    fim_CPU = clock();
                    printf("Solucao obtida usando Algoritmos Genéticos:\n");
                    imprime_rota(&s,n);
                    printf("Funcao objetivo = %f\n",fo);
                    printf("Tempo de CPU = %f segundos\n", (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC);
                    break;
            case 2: /* Algoritmos Geneticos usando operador ERX */
                  printf("Nao implementado\n");
                  break;
            }
           break;

    case 13: /* Algoritmos Memeticos */
           printf("\n Algoritmos Memeticos ainda nao implementado ... \n");
           break;

    case 14: /* Colonia de Formigas */
           printf("\n Colonia de Formigas ainda nao implementado ... \n");
           break;

           case 0:
            break;


    default:
        printf("\n Opcao invalida ... \n");
        break;
    }
  } while (escolha != 0);


  //libera_vetor(s);
  libera_matriz_float(d, n);
  return 0;
}
//---------------------------------------------------------------------------
