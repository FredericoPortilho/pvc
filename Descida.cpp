
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>
#include <utility> //funcao swap (para versao anterior a C11 usar <algorithm>)
#include "Arquivos.h"
#include "Construcao.h"
#include "Descida.h"
#include "Util.h"

float calcula_delta(int n, vector<int> *s, float **d, int i, int j)
{
  int i_antes, i_depois, j_antes, j_depois;

  i_antes = i - 1;
  i_depois = i + 1;
  j_antes = j - 1;
  j_depois = j + 1;
  if (i == 0) i_antes = n-1;
  if (i == n-1) i_depois = 0;
  if (j == 0) j_antes = n-1;
  if (j == n-1) j_depois = 0;

  return  d[s->at(i_antes)][s->at(i)] + d[s->at(i)][s->at(i_depois)] + d[s->at(j_antes)][s->at(j)] + d[s->at(j)][s->at(j_depois)];
//  delta = d[s[i-1]][s[i]] + d[s[i]][s[i+1]] + d[s[j-1]][s[j]] + d[s[j]][s[j+1]]

}

// Encontra o melhor vizinho utilizando o movimento de troca de posicao de duas cidades
float melhor_vizinho(int n, vector<int> *s, float **d, float fo, int *melhor_i, int *melhor_j)
{
  int aux;
  float fo_melhor_viz = fo;

  float fo_viz;
  for(int i=0 ; i < n - 1 ; i++){
    for(int j=i+1 ; j < n ; j++) {
      // Calcula a variacao de custo com a realizacao do movimento
      float delta1 = calcula_delta(n,s,d,i,j);

      // Faz o movimento
      swap(s->at(i),s->at(j));

      float delta2 = calcula_delta(n,s,d,i,j);

      // Calcular a nova distancia
      fo_viz = fo - delta1 + delta2;

      // Armazenar o melhor movimento (melhor troca)
      if(fo_viz < fo_melhor_viz){
        *melhor_i = i;
        *melhor_j = j;
        fo_melhor_viz = fo_viz;
      }

      // desfaz o movimento
      swap(s->at(i),s->at(j));
    }
  }
  // retornar a distancia do melhor vizinho
  return fo_melhor_viz;

}//melhor_vizinho

//Vizinho aleatorio usado nas descidas e algumas meta-heurisiticas
float vizinho_aleatorio(int n, vector<int> *s, float **d, float fo, int *melhor_i, int *melhor_j)
{
    float fo_viz = fo;
    int i, j, aux;
    float delta1, delta2;

    //sorteia a posição para troca
    j = rand()%n;
    do{
        i = rand()%n;
    }while (i == j);

    delta1 = calcula_delta(n,s,d,i,j);

    // Faz o movimento
    swap(s->at(i),s->at(j));

    delta2 = calcula_delta(n,s,d,i,j);
    fo_viz = fo - delta1 + delta2;

    *melhor_i = i;
    *melhor_j = j;

    // Desfaz o movimento
    swap(s->at(i),s->at(j));

    // retornar a distancia do  vizinho
    return fo_viz;

}//vizinho_aleatorio PP

float melhor_vizinho_adjacente(int n, vector<int> *s, float **d, float fo, int *melhor_i, int *melhor_j)
{
   int aux;
   float fo_melhor_viz = fo;

   float fo_viz;
   for(int i=0 ; i < n - 1 ; i++){

       int j = i+1;

       // Calcula a variacao de custo com a realizacao do movimento
       float delta1 = calcula_delta(n,s,d,i,j);

       // Faz o movimento
       swap(s->at(i),s->at(j));

       float delta2 = calcula_delta(n,s,d,i,j);

       // Calcular a nova distancia
       fo_viz = fo - delta1 + delta2;

       // Armazenar o melhor movimento (melhor troca)
       if(fo_viz < fo_melhor_viz){
           *melhor_i = i;
           *melhor_j = j;
           fo_melhor_viz = fo_viz;
       }

       // Desfaz o movimento
       swap(s->at(i),s->at(j));

   }
   // retornar a distancia do melhor vizinho
   return fo_melhor_viz;

}//melhor_vizinho_adjacente

/* Método da descida com best improvement */
float descida_completa(int n, vector<int> *s, float **d)
{
  int aux, melhor_i, melhor_j, iter;
  float fo_viz, fo;
  bool melhorou;
  clock_t inicio_CPU, fim_CPU;

  fo = fo_viz = calcula_fo(n, s, d);
  limpa_arquivo((char*)"DescidaBI.txt");
  inicio_CPU = fim_CPU = clock();
  iter = 0;
  imprime_fo((char*)"DescidaBI.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
  do{
     melhorou = false;
     fo_viz = melhor_vizinho(n, s, d, fo, &melhor_i, &melhor_j);
     if (fo_viz < fo){
          iter = iter + 1;

          swap(s->at(melhor_i),s->at(melhor_j));

          fo = fo_viz;
          melhorou = true;
          fim_CPU = clock();
          imprime_fo_viz((char*)"DescidaBI.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter,melhor_i,melhor_j);

     }
  } while (melhorou);

  fim_CPU = clock();
  imprime_fo((char*)"DescidaBI.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
  return fo;
}

float descida_randomica(int n, vector<int> *s, float **d, int IterMax)
{
  int i, j, iter, aux;
  float delta1, delta2, fo, fo_viz;

  int melhor_i, melhor_j;
  clock_t inicio_CPU, fim_CPU;

  fo = calcula_fo(n,s,d);
  iter = 0;
  limpa_arquivo((char*)"DescidaRandomica.txt");
  inicio_CPU = fim_CPU = clock();

  while (iter < IterMax){
    iter ++;

    fo_viz = vizinho_aleatorio(n, s, d, fo, &melhor_i, &melhor_j);

    if (fo_viz < fo){
      fo = fo_viz;

      swap(s->at(melhor_i),s->at(melhor_j));

      fim_CPU = clock();
      imprime_fo_viz((char*)"DescidaRandomica.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter,melhor_i,melhor_j);
      iter = 0;
    }

  }

  fim_CPU = clock();
  imprime_fo((char*)"DescidaRandomica.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);

  return fo;
}



float vizinho_primeiro_melhora(int n, vector<int> *s, float **d, float fo, int *melhor_i, int *melhor_j)
{
 int aux;
 float fo_melhor_viz = fo;
 bool melhorou = false;
 vector<int> vet;

 for (int i=0; i < n; i++) vet.push_back(i);
 //Para c++ 11
 unsigned seed = chrono::system_clock::now().time_since_epoch().count();
 default_random_engine r(seed);
 shuffle ( vet.begin(), vet.end(), r );
 //Para c++ 98
 //random_shuffle ( vet.begin(), vet.end() );

 float fo_viz;
 for(int i=0 ; i < n - 1 ; i++){
   for(int j=i+1 ; j < n ; j++) {
     // Calcula a variacao de custo com a realizacao do movimento
     float delta1 = calcula_delta(n,s,d,vet[i],vet[j]);

     // Faz o movimento
     swap(s->at(vet[i]),s->at(vet[j]));

     float delta2 = calcula_delta(n,s,d,vet[i],vet[j]);

     // Calcular a nova distancia
     fo_viz = fo - delta1 + delta2;

     // Armazenar o melhor movimento (melhor troca)
     if(fo_viz < fo_melhor_viz){
       *melhor_i = vet[i];
       *melhor_j = vet[j];
       fo_melhor_viz = fo_viz;
       melhorou = true;
     }


     // Desfaz o movimento
     swap(s->at(vet[i]),s->at(vet[j]));

     if (melhorou) break;
   }
   if (melhorou) break;
 }

 // retornar a distancia do melhor vizinho
 return fo_melhor_viz;

}//vizinho_primeiro_de_melhora


float descida_primeiro_melhora(int n, vector<int> *s, float **d)
{
 int aux, melhor_i, melhor_j, iter;
 float fo_viz, fo;
 bool melhorou;
 clock_t inicio_CPU, fim_CPU;

 fo = fo_viz = calcula_fo(n, s, d);
 limpa_arquivo((char*)"DescidaPrimeiroMelhora.txt");
 inicio_CPU = fim_CPU = clock();
 iter = 0;
 imprime_fo((char*)"DescidaPrimeiroMelhora.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);

 /* Implementar a descida First Improvement armazenando, a cada iteração:
     o tempo de execução
     o valor da função de avaliação da solução corrente
     Para fazer essa impressão, use o procedimento imprime_fo, disponível em Arquivos.cpp */
 do{
    melhorou = false;
    fo_viz = vizinho_primeiro_melhora(n, s, d, fo, &melhor_i, &melhor_j);
    if (fo_viz < fo){
         iter = iter + 1;

         swap(s->at(melhor_i),s->at(melhor_j));

         fo = fo_viz;
         melhorou = true;
         fim_CPU = clock();
         imprime_fo_viz((char*)"DescidaPrimeiroMelhora.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter,melhor_i,melhor_j);

    }
 } while (melhorou);

 fim_CPU = clock();
 imprime_fo((char*)"DescidaPrimeiroMelhora.txt", (fim_CPU - inicio_CPU)/CLOCKS_PER_SEC,fo,iter);
 return fo;
}
