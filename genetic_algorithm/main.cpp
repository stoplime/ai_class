#include "genetic.h"

using namespace std;

/*
Genetic Algorithm
generalize the function f(x)=x^2
1. "randomly" generate candidates
2. apply the fitness function "evaluate"
3. 

*/

int candidates = 6;
int candidate_size = 5;


int main()
{
    srand(time(NULL));
    cout << "genetic algorithm" << endl;


    vector<string> samples;
    for(int i=0; i < candidates; i++)
    {
        string temp_sample = "";
        for(int j=0; j < candidate_size; j++)
        {
            int chance = rand() % 100;
            if(chance >= 50)
            {
                temp_sample += "1";
            }
            else
            {
                temp_sample += "0";
            }
        }
        samples.push_back(temp_sample);
        
    }
    show_samples(samples);
    for(unsigned int i=0; i < samples.size(); i++)
    {
        cout << to_int(samples[i]) << endl;
    }    
}

void show_samples(vector<string> sample_in)
{
    for(unsigned int i=0; i < sample_in.size(); i++)
    {
        cout <<  "Sample " << i << " : " << sample_in[i] << endl;
    }
}

int to_int(string sample_in)
{
    int value = 0;
    int power = 0;
    for(int i=sample_in.size()-1; i >= 0; i--)
    {
        value += (pow(2,power++) * (sample_in[i] - '0'));
    }
    return value;
}
