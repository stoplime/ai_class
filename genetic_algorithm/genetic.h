#ifndef __GENETIC_ALGORITHM_H__
#define __GENETIC_ALGORITHM_H__

#include <cstdlib>
#include <iostream>
#include <vector>
#include <ctime>
#include <cmath>
#include <fstream>

using namespace std;

struct sample{
    string code;
    long long int value;
    double eval;
};

void show_samples(vector<sample> samples_in, ofstream& out);

long long int to_int(string sample_in);

long long int evaluate(long long int value_in);

void sort_samples(vector<sample>& samples_in);

void crossover(vector<sample>& samples_in);

void mutate(vector<sample>& samples_in);
void mutate_single(sample& samples_in);

void run_evaluation(vector<sample>& samples_in);

#endif