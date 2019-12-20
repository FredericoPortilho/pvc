#include <cstdlib>
#include <iostream>
#include <climits>
#include <vector>
#include <algorithm>
#include <random>
#include "Util.h"
#include "Construcao.h"

using namespace std;

void constroi_solucao(int n, vector<int> *s, float **distancia)
{
    s->clear();
    for (int j=0; j < n; j++) s->push_back(j);
}


/* Constroi uma solucao de forma gulosa, no caso,
   implementa o Metodo construtivo do vizinho mais proximo */
void constroi_solucao_gulosa_vizinho_mais_proximo(int n, vector<int> *s, float **d)
{
    vector<int> nao_visitada;

    /* Inicio da Fase de Construcao de uma solucao */
    cout << "\n Construindo nova solucao ...\n";

    for (int i=1; i<n; i++){

      /* vou inserir um registro no final de uma lista  */
      nao_visitada.push_back(i);

    }
    s->clear();
    s->push_back(0);  /* A cidade origem é a cidade 0 */

    int i = 1;
    int mais_proxima;
    float dist;
    while (i < n){
      dist = INT_MAX;
      int j = 0;
      int pos = 0;

      while (j < nao_visitada.size()) {
        if (d[s->at(i-1)][nao_visitada[j]] < dist){
          dist = d[s->at(i-1)][nao_visitada[j]];
          mais_proxima = nao_visitada[j];
          pos = j;
        }
        j++;
      }
      /* Insere a cidade mais proxima apos a ultima cidade inserida*/
        s->push_back(mais_proxima);
      /* Apaga a cidade mais_proxima da lista de nao visitadas */
      nao_visitada.erase(nao_visitada.begin() + pos);

      i++;
    }
}




/* Constroi uma solucao de forma aleatoria */
void constroi_solucao_aleatoria(int n, vector<int> *s, float **d)
{

    constroi_solucao(n, s, d);

    //Para c++ 11
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine r(seed);
    shuffle ( s->begin(), s->end(), r );

    //Para c++ 98
    //random_shuffle ( s->begin(), s->end() );

}


/* Constroi uma solucao parcialmente gulosa pelo metodo do vizinho mais proximo */
void constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(int n, vector<int> *s, float **d, float alpha)
{

    vector<int> nao_visitadas;
    int tamanho_LC;

    /* Inicio da Fase de Construcao de uma solucao */
    for (int i=1; i<n; i++){

        /* vou inserir um registro no final de uma lista  */
        nao_visitadas.push_back(i);

    }

    s->clear();
    s->push_back(0);  /* A cidade origem � a cidade 0 */

    //imprime_rota(&nao_visitadas, nao_visitadas.size());

    //Ordena lista
    ordena_dist_crescente ordem;
    ordem.d = d; // fornece a matriz de distancia para usar na ordenacao
    ordem.index = s->at(0);

    /* Ordenando a lista de cidade nao visitadas */
    stable_sort(nao_visitadas.begin(), nao_visitadas.end(), ordem);


    int j = 1;
    int cidade_escolhida;
    while (j < n){

      tamanho_LC = nao_visitadas.size();
      //printf("Tamanho da lista de candidatos = %d \n",tamanho_LC);

      //alpha = 0;
      int tamanho_LRC = MAX(1, alpha * tamanho_LC);

      //printf("Tamanho da lista restrita de candidatos = %d \n",tamanho_LRC);
      int posicao_escolhida = rand()%tamanho_LRC;
      cidade_escolhida = nao_visitadas[posicao_escolhida];
      //printf("Cidade escolhida = %d \n",cidade_escolhida);

      /* Insere a cidade escolhida apos a ultima cidade inserida*/
      s->push_back(cidade_escolhida);
      /* Apaga a cidade escolhida da lista de nao visitadas */
      nao_visitadas.erase(nao_visitadas.begin()+posicao_escolhida);

        /* Ordenando a lista de cidade nao visitadas */
      ordem.index = cidade_escolhida; //atualiza ultima cidade inserida
      sort(nao_visitadas.begin(), nao_visitadas.end());  //ordena pelos indices (para manter a estabilidade)
      stable_sort(nao_visitadas.begin(), nao_visitadas.end(), ordem); //ordena pelas distancias

      j++;
    }
}



/* Constroi uma solucao pela insercao mais barata */
void constroi_solucao_gulosa_insercao_mais_barata(int n, vector<int> *s, float **d, int random_start)
{
    vector<int> nao_visitada;

    /* Inicio da Fase de Construcao de uma solucao */
    cout << "\n Construindo nova solucao ...\n";

    for (int i=1; i<n; i++){

        /* vou inserir um registro no final de uma lista  */
        nao_visitada.push_back(i);

    }
    s->clear();
    s->push_back(0);  /* A cidade origem é a cidade 0 */

    int j = 1;
    int mais_proxima;
    float dist;

    int cid_i, cid_j, cid_k, pos;
    int melhor_i, melhor_j, melhor_k;
    float sij, melhor_sij;


    if (random_start) /* Insere as duas proximas cidades aleatoriamente */
    {
        int pos = rand() % nao_visitada.size();
        s->push_back(nao_visitada[pos]);
        nao_visitada.erase(nao_visitada.begin() + pos);
        j++;

    } else /* Insere as duas proximas cidades pela heuristica do vizinho mais proximo */
    while ( j < 3){
        dist = INT_MAX;
        int i = 0;
        int pos_i = 0;

        while (i < nao_visitada.size()) {
            if (d[s->at(j-1)][nao_visitada[i]] < dist){
                dist = d[s->at(j-1)][nao_visitada[i]];
                mais_proxima = nao_visitada[i];
                pos_i = i;
            }
            i++;
        }
        /* Insere a cidade mais proxima apos a ultima cidade inserida*/
        s->push_back(mais_proxima);
        /* Apaga a cidade mais_proxima da lista de nao visitadas */
        nao_visitada.erase(nao_visitada.begin() + pos_i);

        j++;
    }


   /* Formada uma subrota inicial com tres cidades, aplicar o metodo da
      insercao mais barata para decidir qual cidade inserir entre cada
      par de cidades i e j. A cidade k escolhida sera aquela que minimizar
      custo(k) = d(i,k) + d(k,j) - d(i,j) */

    while (j < n) {

      melhor_sij = INT_MAX;

      int k = 0;
      int pos_k;
      /* Calcula os custos para cada cidade k ser inserida entre as cidades i e j */
      while (k < nao_visitada.size()) {
        cid_k = nao_visitada[k];
        for (int i = 0; i < j; i++) {
	      cid_i = s->at(i);
          if ((i+1) != j) cid_j = s->at(i+1);
          else cid_j = 0;

          sij = d[cid_i][cid_k] + d[cid_k][cid_j] - d[cid_i][cid_j];

          if (sij < melhor_sij) {
 	        melhor_i = cid_i;
            melhor_k = cid_k;
            melhor_sij = sij;
            pos = i + 1;
            pos_k = k;
          }
        }
        k++;
      }

      /* Adiciona cada nova cidade k entre as cidades i e j que produzir a insercao mais barata */
      s->insert(s->begin()+pos,melhor_k);

      /* Apaga a cidade mais barata da lista de nao visitadas */
        nao_visitada.erase(nao_visitada.begin() + pos_k);

      j++;
    }
}
//
//
// /*  HA ERRO NO METODO ABAIXO !!!!!!!!!!!!!!!! */
///* Constroi uma solucao parcialmente gulosa com base no metodo da insercao mais barata */
//void constroi_solucao_parcialmente_gulosa_insercao_mais_barata(int n, vector *s, float **d, float alpha)
//{
//    struct lista *inicio_LC, *final_LC, *nao_visitada;
//    int tamanho_LC;
//    struct lista *registro;
//
//    int j = 1;
//    int mais_proxima;
//    float dist;
//
//    int cid_i, cid_j, cid_k, pos;
//    int melhor_i, melhor_j, melhor_k;
//    float sij, melhor_sij;
//
//    /* Inicio da Fase de Construcao de uma solucao */
//    inicio_LC = final_LC = NULL;
//    for (int j=1; j<n; j++){
//      nao_visitada = (struct lista *)malloc(sizeof(struct lista));
//      if (!nao_visitada) {
//         printf("Faltando memoria ...\n");
//         exit(1);
//      }
//      /* vou inserir um registro no final de uma lista  */
//      nao_visitada->campo1 = j;
//      nao_visitada->campo2 = 0;
//      insere_final_lista(nao_visitada, &inicio_LC, &final_LC);
//    }
//    s[0]=0;  /* A cidade origem � a cidade 0 */
//
//
//    /* Constr�i uma subrota com 3 cidades pela heur�stica do vizinho mais pr�ximo */
//    while (j < 3){
//      dist = INT_MAX;
//      registro = inicio_LC;
//      int pos = 1;
//      while (registro) {
//        if (d[s[j-1]][registro->campo1] < dist){
//          dist = d[s[j-1]][registro->campo1];
//          mais_proxima = registro->campo1;
//        }
//        registro = registro->proximo;   // obtem pr�ximo endere�o
//        pos++;
//      }
//      /* Insere a cidade mais proxima apos a ultima cidade inserida*/
//      s[j] = mais_proxima;
//      /* Apaga a cidade mais_proxima da lista de nao visitadas */
//      registro = encontra_registro(mais_proxima, 0,&inicio_LC);
//      apaga_registro(registro,&inicio_LC,&final_LC);
//      j++;
//    }
//
//    int cidade_escolhida, cidade_i;
//
//    while (j < n){
//      registro = inicio_LC;
//      tamanho_LC = tamanho_lista(&inicio_LC,&final_LC);
//      melhor_sij = INT_MAX;
//
//      /* Ordenando a lista encadeada em fun��o do custo de inser��o */
//
//      struct lista *inicio_LC_ordenada, *final_LC_ordenada;
//      inicio_LC_ordenada = final_LC_ordenada = NULL;
//
//      while (registro){
//        nao_visitada = (struct lista *)malloc(sizeof(struct lista));
//        if (!nao_visitada) {
//           printf("Faltando memoria ...\n");
//           exit(1);
//        }
//
//        /* Calcula os custos para cada cidade k ser inserida entre as cidades i e j */
//        cid_k = registro->campo1;
//
//        for (int i = 0; i < j; i++) {
//	  cid_i = s[i];
//          if ((i+1) != j) cid_j = s[i+1];
//          else cid_j = 0;
//          sij = d[cid_i][cid_k] + d[cid_k][cid_j] - d[cid_i][cid_j];
//
//          if (sij < melhor_sij) {
// 	    melhor_i = cid_i;
//            melhor_sij = sij;
//          }
//        }
//
//        nao_visitada->campo1 = registro->campo1;
//        nao_visitada->campo2 = melhor_i;
//        nao_visitada->campo3 = -melhor_sij; /* Como a ordenacao � crescente,
//                                               p�e-se o sinal de menos para
//                                               indicar que a primeira da lista
//                                               � a que tem a maior economia */
//        /* vou inserir um registro de forma ordenada em uma lista  */
//        insere_ordenado_lista(nao_visitada, &inicio_LC_ordenada, &final_LC_ordenada);
//        registro = registro->proximo;   /* obtem pr�ximo endere�o */
//      }
//
//      int tamanho_LRC = MAX(1, alpha * tamanho_LC);
//      int posicao_escolhida = rand()%tamanho_LRC;
//      registro = encontra_registro_lista(posicao_escolhida,&inicio_LC_ordenada);
//      cidade_escolhida = registro->campo1;
//      cidade_i = registro->campo2;
//
//      /* Apaga a lista de candidatos ordenada */
//      apaga_lista(&inicio_LC_ordenada, &final_LC_ordenada);
//
//      /* procura a posi��o do vetor a ser inserida a cidade */
//      for (int i = 0; i < j; i++)
//        if (s[i] == cidade_i) pos = i+1;
//
//      /* Adiciona cada nova cidade k entre as cidades i e j que produzir a inser��o mais barata */
//      insere_meio_vetor(s,cidade_escolhida,pos,j);
//
//      /* Apaga a cidade mais barata da lista de nao visitadas */
//      registro = encontra_registro(cidade_escolhida, 0,&inicio_LC);
//      apaga_registro(registro,&inicio_LC,&final_LC);
//
//      j++;
//    }
//}
//
//
