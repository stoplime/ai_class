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

void show_samples(vector<sample> samples_in);

int to_int(string sample_in);

int evaluate(int value_in);

void sort_samples(vector<sample>& samples_in);

void crossover(vector<sample>& samples_in);

void mutate(vector<sample>& samples_in);

void run_evaluation(vector<sample>& samples_in);

#endif