#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "Util.h"
#include "AG.h"
#include "Construcao.h"
//#include "GRASP.h"
#include "Descida.h"

using namespace std;

float AG(int n,
         std::vector<int> *s,
         float **d,
         int nind,
         float max_desvio,
         double prob_crossover,
         double prob_mutacao,
         int tipo_operador)
{
    vector< vector<int> > pop; // populacao
    vector<float> fo_pop; // funcao objetivo corrente de um dado individuo da populacao
    vector< vector<int> > pop_sobrev; // populacao sobrevivente
    vector<float> fo_pop_sobrev; // funcao objetivo corrente de um dado individuo da populacao sobrevivente
    vector<int> s_star;   // vetor que contem a melhor solucao gerada
    float fo, fo_star;   // valor da melhor solucao gerada

    fo_star = INT_MAX;
    if (nind % 2 != 0){
        printf("Numero de individuos deve ser par!\n");
        printf("Vou aumentar o numero de individuos ...\n");
        getchar();
        nind++;
    }
    nind = nind * 2;

    //cria a matriz pop[nind,n]
    pop.resize(nind, vector<int>(n));


    //cria a matriz pop_sobrev[nind/2, n]
    pop_sobrev.resize(nind/2, vector<int>(n));

    fo_pop.resize(nind);
    fo_pop_sobrev.resize(nind/2);

    /* Geracao da populacao inicial */
    for (int j = 0; j < nind/2; j++){
        constroi_solucao_aleatoria(n, &pop[j], d);
        //constroi_solucao_parcialmente_gulosa_vizinho_mais_proximo(n, &pop[j], d, 0.05);0
        fo_pop[j] = calcula_fo(n, &pop[j], d);
        if (fo_pop[j] < fo_star){
            fo_star = fo_pop[j];
            s_star = pop[j];
        }
    }

    printf("Melhor solucao encontrada na construcao: %f\n", fo_star);

    int ngeracoes = 0;
    float desvio = INT_MAX;
    while (desvio > max_desvio){
        ngeracoes++;
        /* Aplicar crossover */
        int quant_filhos = 0;
        while (quant_filhos < nind/2){
            /* Selecao aleatoria de pais */
            int jpai1, jpai2;
            do{
                jpai1 = rand()%(nind/2);
                jpai2 = rand()%(nind/2);
            }while (jpai1 == jpai2);
            if (randomico(0,1) < prob_crossover){
                /*
                Refinando os pais antes de eles procriarem:
                descida_primeiro_melhora(n,pop[jpai1],d);
                descida_primeiro_melhora(n,pop[jpai2],d);
                */

                if (tipo_operador == 1)
                    crossover_OX(&pop[jpai1], &pop[jpai2], &pop[quant_filhos+nind/2], &pop[quant_filhos+nind/2+1], n);
//                if (tipo_operador == 2)
//                    crossover_ERX(&pop[jpai1], &pop[jpai2], &pop[quant_filhos+nind/2], &pop[quant_filhos+nind/2+1], n);
                quant_filhos+=2;
            }
        }
        /* Aplicar mutacao */
        for (int j = 0; j < nind; j++)
            if (randomico(0,1) < prob_mutacao)
                mutacao(&pop[j], n);

        /* Avaliar a populacao inteira (pais e filhos) */
        for (int j = 0; j < nind; j++){
            fo_pop[j] = calcula_fo(n, &pop[j], d);
            if (fo_pop[j] < fo_star){
                fo_star = fo_pop[j];
                s_star = pop[j];
                printf("Geracao = %4d   fo star = %10.3f\n", ngeracoes, fo_star);
            }
        }

        /* Definir a populacao sobrevivente */
        /* fo de todos os individuos da populacao, isto é, pais e filhos */
        for (int j = 0; j < nind/2; j++){
            /* escolha dos individuos sobreviventes pelo mecanismo da roleta */
            int individuo_escolhido = roleta(nind, &fo_pop);
            /* escolha dos individuos sobreviventes pelo mecanismo da roleta
               colocando aptidao nula para aqueles que estiverem acima de uma dada
               faixa, no caso, duas vezes de desvio padrao acima da media */
            //int individuo_escolhido = roleta_scaling(nind, fo_pop);
            for (int i = 0; i < n; i++) pop_sobrev[j][i] = pop[individuo_escolhido][i];
            fo_pop_sobrev[j] = fo_pop[individuo_escolhido];

        }
        /* Zerar a populacao e seus dados */
        //inicializa_vetor_float(&fo_pop,nind);
        //for (int j = 0; j < nind; j++) inicializa_vetor(&pop[j],n);
        /* Primeira metade da populacao <-- populacao sobrevivente */
        for (int j = 0; j < nind/2; j++){
            for (int i = 0; i < n; i++) pop[j][i] = pop_sobrev[j][i];
            fo_pop[j] = fo_pop_sobrev[j];
        }


        /* Zerar a matriz e os vetores que armazenam os dados da populacao sobrevivente */
        //inicializa_vetor_float(&fo_pop_sobrev,nind/2);
        //for (int j = 0; j < nind/2; j++) inicializa_vetor(&pop_sobrev[j],n);

        /* Calcular o desvio padrão das fos da população */
        desvio = calcula_desvio_padrao(&fo_pop,nind/2);
    }

    *s = s_star;
    fo = fo_star;
    printf("\nNumero de geracoes avaliadas: %d\n", ngeracoes);

    return fo;
}

/* Esta rotina devolve o individuo escolhido pelo mecanismo da roleta */
int roleta(int nind, std::vector<float> *fo_pop)
{
    int j;
    float aux;
    vector<float> escala;
    vector<float> fracao;
    vector<float> faptidao;
    float tg_alfa, soma, fo_max, fo_min;
    int escolhido;

    soma = 0;
    fo_min = INT_MAX;
    fo_max = INT_MIN;

    for (int j = 0; j < nind; j++){
        if (fo_pop->at(j) < fo_min) fo_min = fo_pop->at(j);
        if (fo_pop->at(j) > fo_max) fo_max = fo_pop->at(j);
    }

    tg_alfa = 100 / (fo_max - fo_min);

    for (int j = 0; j < nind; j++){
        faptidao.push_back( tg_alfa * (fo_max - fo_pop->at(j)) );
        soma += faptidao[j];
    }

    for (int j = 0; j < nind; j++)
        fracao.push_back( faptidao[j] / soma );

    escala.push_back(fracao[0]);

    for (int j = 1; j < nind; j++)
        escala.push_back( escala[j-1] + fracao[j] );

    aux = randomico(0,1);
    j = 0;
    while (escala[j] < aux) j++;
    escolhido = j;

    return escolhido;
}


void mutacao(vector<int> *s, int n)
{
    int i, j;

    i = rand()%n;
    do {
        j = rand()%n;
    } while (i == j);

    swap(s->at(i),s->at(j));
}

/* Operador Crossover OX */
void crossover_OX(vector<int> *pai1,
                  vector<int> *pai2,
                  vector<int> *filho1,
                  vector<int> *filho2,
                  int n)
{

    int ponto_de_corte_1, ponto_de_corte_2, i, j;
    bool existe;
    vector<int> lista_pai1, lista_pai2;

    ponto_de_corte_1 = randomico(2,(n-1)/2);
    ponto_de_corte_2 = randomico((n+1)/2,n-3);

    cout <<"\t\n\nPC1: "<< ponto_de_corte_1;
    cout << "\tPC2: "<< ponto_de_corte_2;

    /* Copia os genes entre os 2 pontos de corte em cada filho */
    for (int i = ponto_de_corte_1; i <= ponto_de_corte_2; i++) {
        filho1->at(i) = pai1->at(i);
        filho2->at(i) = pai2->at(i);
    }

    //teste
    cout << "\t\n\nPAI 1: \n";


    for (int i = 0; i < n; i++) {
        cout <<"\t"<< pai1->at(i);
    }

    cout << "\t\n\nPAI 2: \n";
    for (int i = 0; i < n; i++) {
        cout <<"\t"<< pai1->at(i);
    }


    int tam_lista = n - (ponto_de_corte_2 - ponto_de_corte_1 + 1);

    i = ponto_de_corte_2 + 1;
    j = 0;
    do {
        /* procura a cidade pai1[i] no filho2*/
        existe = false;
        for(int k = ponto_de_corte_1; k <= ponto_de_corte_2; k++) {
            if (filho2->at(k) == pai1->at(i)) existe = true;
        }
        if (! existe) {
            lista_pai1.push_back(pai1->at(i));
            j++;
        }
        if (i == (n-1)) i = 0;
        else i++;

    } while(j < tam_lista);

    i = ponto_de_corte_2 + 1;
    j = 0;
    do {
        /* procura a cidade pai2[i] no filho1*/
        existe = false;
        for(int k = ponto_de_corte_1; k <= ponto_de_corte_2; k++) {
            if (filho1->at(k) == pai2->at(i)) existe = true;
        }
        if (! existe) {
            lista_pai2.push_back(pai2->at(i));
            j++;
        }

        if (i == (n-1)) i = 0;
        else i++;

    } while(j < tam_lista);

    /* Completa os genes que faltam */
    i = 0;
    j = ponto_de_corte_2 + 1;
    do {
        filho1->at(j) = lista_pai2[i];
        filho2->at(j) = lista_pai1[i];
        i++;
        if (j == (n-1)) j = 0;
        else j++;
    } while (i < tam_lista);

}

///* Operador Crossover ERX */
//void crossover_ERX(int *pai1,
//                   int *pai2,
//                   int *filho1,
//                   int *filho2,
//                   int n)
//{
//    int **arestas1, **arestas2, pos_cid_p1, pos_cid_p2, j, m;
//    m = 5;
//    arestas1 = cria_matriz(n,m);
//    for(int i = 0; i < n; i++) inicializa_vetor(arestas1[i], m);
//    arestas2 = cria_matriz(n,m);
//    for(int i = 0; i < n; i++) inicializa_vetor(arestas2[i], m);
//
//    /* Preenche a matriz de arestas */
//    for(int i = 0; i < n; i++) {
//        pos_cid_p1 = busca_pos_valor(pai1, i, n);
//        pos_cid_p2 = busca_pos_valor(pai2, i, n);
//        j = 1;
//        if (pos_cid_p1 == 0) {
//            arestas1[i][j] = pai1[pos_cid_p1+1];
//            arestas2[i][j] = pai1[pos_cid_p1+1];
//            j++;
//            arestas1[i][j] = pai1[n-1];
//            arestas2[i][j] = pai1[n-1];
//            j++;
//        }
//        else if (pos_cid_p1 == (n-1)) {
//            arestas1[i][j] = pai1[pos_cid_p1-1];
//            arestas2[i][j] = pai1[pos_cid_p1-1];
//            j++;
//            arestas1[i][j] = pai1[0];
//            arestas2[i][j] = pai1[0];
//            j++;
//        }
//        else {
//            arestas1[i][j] = pai1[pos_cid_p1-1];
//            arestas2[i][j] = pai1[pos_cid_p1-1];
//            j++;
//            arestas1[i][j] = pai1[pos_cid_p1+1];
//            arestas2[i][j] = pai1[pos_cid_p1+1];
//            j++;
//        }
//        if (pos_cid_p2 == 0 ) {
//            arestas1[i][j] = pai2[pos_cid_p2+1];
//            arestas2[i][j] = pai2[pos_cid_p2+1];
//            j++;
//            arestas1[i][j] = pai2[n-1];
//            arestas2[i][j] = pai2[n-1];
//            j++;
//        }
//        else if (pos_cid_p2 == (n-1)) {
//            arestas1[i][j] = pai2[pos_cid_p2-1];
//            arestas2[i][j] = pai2[pos_cid_p2-1];
//            j++;
//            arestas1[i][j] = pai2[0];
//            arestas2[i][j] = pai2[0];
//            j++;
//        }
//        else {
//            arestas1[i][j] = pai2[pos_cid_p2-1];
//            arestas2[i][j] = pai2[pos_cid_p2-1];
//            j++;
//            arestas1[i][j] = pai2[pos_cid_p2+1];
//            arestas2[i][j] = pai2[pos_cid_p2+1];
//            j++;
//        }
//
//        /* Retira cidades repetidas e armazena o nº de arestas */
//        if (arestas1[i][3] == arestas1[i][1] || arestas1[i][3] == arestas1[i][2]) {
//            arestas1[i][3] = -1;
//            arestas2[i][3] = -1;
//            j--;
//        }
//
//        if (arestas1[i][4] == arestas1[i][1] || arestas1[i][4] == arestas1[i][2]) {
//            arestas1[i][4] = -1;
//            arestas2[i][4] = -1;
//            j--;
//        }
//        arestas1[i][0] = j - 1;
//        arestas2[i][0] = j - 1;
//    }
//    int prox_cid, cid_corrente, cid_rota, cid_aleatoria, num_arestas;
//
//    /* Construção do 1º filho */
//    filho1[0] = pai1[0];
//    atualiza_arestas(arestas1,n,m,0);
//    for (int i = 1; i < n; i++) {
//        num_arestas = INT_MAX;
//        cid_rota = filho1[i-1];
//        prox_cid = -1;
//        for (int j = 1; j < m; j++) {
//            cid_corrente = arestas1[cid_rota][j];
//            if (cid_corrente != -1) {
//                if (arestas1[cid_corrente][0] < num_arestas) {
//                    num_arestas = arestas1[cid_corrente][0];
//                    prox_cid = cid_corrente;
//                }
//                else if (arestas1[cid_corrente][0] == num_arestas)
//                    if (cid_corrente < prox_cid)
//                        prox_cid = cid_corrente;
//            }
//        }
//        if (prox_cid == -1) {
//            do {
//                cid_aleatoria = (int) rand()%n;
//            } while (foi_inserida(filho1, cid_aleatoria, i));
//            prox_cid = cid_aleatoria;
//        }
//        filho1[i] = prox_cid;
//        atualiza_arestas(arestas1,n,m,prox_cid);
//    }
//
//    /* Construção do 2º filho */
//    filho2[0] = pai2[0];
//    atualiza_arestas(arestas2,n,m,0);
//    for (int i = 1; i < n; i++) {
//        num_arestas = INT_MAX;
//        cid_rota = filho2[i-1];
//        prox_cid = -1;
//        for (int j = 1; j < m; j++) {
//            cid_corrente = arestas2[cid_rota][j];
//            if (cid_corrente != -1) {
//                if (arestas2[cid_corrente][0] < num_arestas) {
//                    num_arestas = arestas2[cid_corrente][0];
//                    prox_cid = cid_corrente;
//                }
//                else if (arestas2[cid_corrente][0] == num_arestas)
//                    if (cid_corrente < prox_cid)
//                        prox_cid = cid_corrente;
//            }
//        }
//        if (prox_cid == -1) {
//            do {
//                cid_aleatoria = (int) rand()%n;
//            } while (foi_inserida(filho2, cid_aleatoria, i));
//            prox_cid = cid_aleatoria;
//        }
//        filho2[i] = prox_cid;
//        atualiza_arestas(arestas2,n,m,prox_cid);
//    }
//    libera_matriz(arestas1,n);
//    libera_matriz(arestas2,n);
//}
