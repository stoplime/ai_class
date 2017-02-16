// g++ -g -Wall -o genetic1 main.cpp
#include "genetic.h"

using namespace std;

/*
Genetic Algorithm
generalize the function f(x)=x^2
1. "randomly" generate candidates
2. apply the fitness function "evaluate"
3. sort by eval
4. run crossover
5. run mutations

*/

int candidates = 6;
int candidate_size = 5;
float keep = 0.5;
float mutation_rate = 0.2;
int epoch = 10;

sample best;

int main()
{
    srand(time(NULL));
    cout << "genetic algorithm" << endl;

    best.code = "";
    best.value = 0;
    best.eval = -1;

    //generate samples
    vector<sample> samples;
    for(int i=0; i < candidates; i++)
    {
        sample temp_sample;
        temp_sample.code = "";
        temp_sample.value = 0;
        temp_sample.eval = -1;
        for(int j=0; j < candidate_size; j++)
        {
            int chance = rand() % 100;
            if(chance >= 50)
            {
                temp_sample.code += "1";
            }
            else
            {
                temp_sample.code += "0";
            }
        }
        samples.push_back(temp_sample);
        samples[i].value = to_int(samples[i].code);
        samples[i].eval = evaluate(samples[i].value);
    }
    // show results
    for(int i=0; i < epoch; i++){
        cout << "Sorted samples: "<< endl;
        sort_samples(samples);
        show_samples(samples);
        cout << "Crossover samples: "<< endl;
        crossover(samples);
        run_evaluation(samples);
        show_samples(samples);
        cout << "Mutated samples: "<< endl;
        mutate(samples);
        run_evaluation(samples);
        show_samples(samples);
        cout << "Best fit so far: "<< endl;
        for(size_t i=0; i < samples.size(); i++){
            if(best.eval < samples[i].eval){
                best = samples[i];
            }
        }
        cout << "Sample best: " << best.code << "= " << best.value << " =>\t" << best.eval << endl;

        cout << "Average fit: "<< endl;
        float average = 0;
        for(size_t i=0; i < samples.size(); i++){
            average += (float)samples[i].eval;
        }
        average /= samples.size();
        cout << "Eval: " << average << endl;

        cout << endl;
    }
}

void show_samples(vector<sample> sample_in)
{
    for(size_t i=0; i < sample_in.size(); i++)
    {
        cout <<  "Sample " << i << ": " << sample_in[i].code << "= " << sample_in[i].value << " =>\t" << sample_in[i].eval << endl;
    }
}

int to_int(string samples_in)
{
    int value = 0;
    int power = 0;
    for(int i=samples_in.size()-1; i >= 0; i--)
    {
        value += (pow(2,power++) * (samples_in[i] - '0'));
    }
    return value;
}

int evaluate(int value_in)
{
    return pow(value_in, 2);
}

void sort_samples(vector<sample>& samples_in)
{
    for(size_t i=0; i < samples_in.size()-1; i++){
        for(size_t j=i; j < samples_in.size(); j++){
            if(samples_in[i].eval < samples_in[j].eval){
                // switch
                sample temp = samples_in[i];
                samples_in[i] = samples_in[j];
                samples_in[j] = temp;
            }
        }
    }
}

void crossover(vector<sample>& samples_in)
{
    vector<sample> baby_factory;
    for(int i=0; i < (candidates*keep); i++){
        // choose a momy and dady
        sample dad = samples_in[(rand() % candidates) * keep];
        sample mom = samples_in[(rand() % candidates) * keep];
        
        // setup for two kids
        sample child1;
        sample child2;

        // choose an incision
        int incision = rand() % candidate_size;

        // remove the condom
        child1.code = "";
        child2.code = "";
        child1.value = 0;
        child2.value = 0;
        child1.eval = -1;
        child2.eval = -1;

        // make love
        for(int j=0; j < candidate_size; j++){
            if(j < incision){
                child1.code += dad.code[j];
                child2.code += mom.code[j];
            }
            else{
                child1.code += mom.code[j];
                child2.code += dad.code[j];
            }
        }

        baby_factory.push_back(child2);
        baby_factory.push_back(child1);
    }

    for(size_t i=0; i < baby_factory.size(); i++){
        samples_in[i] = baby_factory[i];
    }
}

void mutate(vector<sample>& samples_in)
{
    for(size_t i=0; i < samples_in.size(); i++){
        // to mutate or not to mutatethat is the question
        if((rand() % 100)/(float)100 < mutation_rate){
            // which element to mutate
            int mutate_index = rand() % candidate_size;

            if(samples_in[i].code[mutate_index] == '0'){
                samples_in[i].code[mutate_index] = '1';
            }
            else{
                samples_in[i].code[mutate_index] = '0';
            }
        }
    }
}

void run_evaluation(vector<sample>& samples_in)
{
    for(size_t i=0; i < samples_in.size(); i++){
        samples_in[i].value = to_int(samples_in[i].code);
        samples_in[i].eval = evaluate(samples_in[i].value);
    }
}