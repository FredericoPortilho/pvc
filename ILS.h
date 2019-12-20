
//
// Created by Puca Penna on 13/09/19.
//

#ifndef _ILS_H_
#define _ILS_H_
#include <vector>

float pertubacao(int n, std::vector<int> *s, float **d, int iter_max);

float ils(int n, std::vector<int> *s, float **d, int iter_max);

float smartILS(int n, std::vector<int> *s, float **d, int iter_max);


#endif //PCV_C_ILS_H
