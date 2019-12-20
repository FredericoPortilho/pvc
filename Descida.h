#ifndef _DESCIDA_H_
#define _DESCIDA_H_

#include <vector>
#include <chrono>

float calcula_delta(int n, std::vector<int> *s, float **d, int i, int j);
float descida_completa(int n, std::vector<int> *s, float **d);
float melhor_vizinho(int n, std::vector<int> *s, float **d, float fo, int *melhor_i, int *melhor_j);
float descida_randomica(int n, std::vector<int> *s, float **d, int IterMax);
float descida_primeiro_melhora(int n, std::vector<int> *s, float **d);
float vizinho_primeiro_melhora(int n, std::vector<int> *s, float **d, float fo, int *melhor_i, int *melhor_j);
float vizinho_aleatorio(int n, std::vector<int> *s, float **d, float fo, int *melhor_i, int *melhor_j);
//float melhor_vizinho_adjacente(int n, std::vector<int> *s, float **d, float fo, int *melhor_i, int *melhor_j);

#endif