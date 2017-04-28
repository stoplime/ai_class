/// ***** Steffen Lim ******
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

int run = 211;

int candidates = 18;
int candidate_size = 42;
float keep = 0.5;
float mutation_rate = 0.2;
int epoch = 500;
long long int goal = 10024000;

sample global_best;

int main()
{
    srand(time(NULL));
    cout << "genetic algorithm" << endl;

    global_best.code = "";
    global_best.value = 0;
    global_best.eval = -1.0e+100;

    //generate samples
    vector<sample> samples;
    vector<string> record;
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
    ofstream out;
    out.open("/home/stoplime/ai_class/genetic_algorithm/test_runs/run"+to_string(run)+".txt");
    sample epoch_best;
    epoch_best.code = "";
    epoch_best.value = 0;
    epoch_best.eval = -1.0e+100;
    for(int i=0; i < epoch; i++){

        // insert previous best epoch and remove the worst set
        if(i != 0){
            samples.push_back(epoch_best);
        }
        sort_samples(samples);
        if(i != 0){
            samples.pop_back();
        }

        crossover(samples);
        mutate(samples);

        // mutate until it is new 
        for (size_t j = 0; j < samples.size(); j++){
            bool in_record = false;
            for (size_t k = 0; k < record.size(); k++){
                if(record[k] == samples[j].code){
                    // it has been found in record
                    in_record = true;
                }
            }
            for (int giveup = 0; giveup < 100 && in_record; giveup++){
                // mutate
                mutate_single(samples[j]);
                // recheck to see if it is in the record now
                in_record = false;
                for (size_t k = 0; k < record.size(); k++){
                    if(record[k] == samples[j].code){
                        // it has been found in record
                        in_record = true;
                    }
                }
                // if still in record, then rerun the mutation and recheck until give up
            }

            // when it is not in record
            if(!in_record){
                record.push_back(samples[j].code);
            }
        }

        run_evaluation(samples);
        
        float average = 0;
        for(size_t j=0; j < samples.size(); j++){
            if(global_best.eval < samples[j].eval){
                global_best = samples[j];
            }
            if(epoch_best.eval < samples[j].eval){
                epoch_best = samples[j];
            }
            average += (float)samples[j].eval;
        }

        average /= samples.size();
        out << "Best @" << i << ": \t";
        out << epoch_best.value << "\t" << epoch_best.eval << "\t";
        out << global_best.value << "\t" << global_best.eval << "\t";
        out << average;

        out << endl;
    }
    out.close();
    cout << "Done: run"+to_string(run)+".txt" << endl;
}

long long int evaluate(long long int value_in)
{
    /// Fit Fuction is a triangle with one max
    long long int result = -abs(value_in-goal);
    return result;
}

void show_samples(vector<sample> sample_in, ofstream& out)
{
    for(size_t i=0; i < sample_in.size(); i++)
    {
        out <<  "Sample " << i << ": \t" << sample_in[i].code << "\t" << sample_in[i].value << "\t" << sample_in[i].eval << endl;
    }
}

long long int to_int(string samples_in)
{
    long long int value = 0;
    long long int power = 0;
    for(int i=samples_in.size()-1; i >= 0; i--)
    {
        value += (pow(2,power++) * (samples_in[i] - '0'));
    }
    return value;
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

void mutate_single(sample& samples_in)
{
    // to mutate or not to mutatethat is the question
    if((rand() % 100)/(float)100 < mutation_rate){
        // which element to mutate
        int mutate_index = rand() % candidate_size;

        if(samples_in.code[mutate_index] == '0'){
            samples_in.code[mutate_index] = '1';
        }
        else{
            samples_in.code[mutate_index] = '0';
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