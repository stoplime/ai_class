#ifndef __GENETIC_ALGORITHM_H__
#define __GENETIC_ALGORITHM_H__

#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>

using namespace std;

struct sample{
    string code;
    int value;
    int eval;
};

void show_samples(vector<sample> sample_in);

int to_int(string sample_in);

#endif