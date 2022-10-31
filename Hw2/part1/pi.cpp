#include <iostream>
#include <string>
#include <cstdlib>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
// #include "simdxorshift128plus.h"
using namespace std;

// seed
unsigned int seed = 123;

// mutex
pthread_mutex_t lock;

// shared data between thread
long long int total_number_in_circle = 0;

double get_pi_seq(long long int number_of_tosses){
    long long int number_in_circle = 0;
    double x, y, distance_squared;
    double min = -1, max = 1, numerator = max - min, denominator = RAND_MAX + 1.0; // diff: max - min, 
    for (long long int toss = 0; toss < number_of_tosses; toss++){
        x = numerator * rand() / denominator + min;
        y = numerator * rand() / denominator + min;
        distance_squared = x * x + y * y;
        if ( distance_squared <= 1)
            number_in_circle++;
    }
    long double pi_estimate = 4 * number_in_circle / (( long double ) number_of_tosses);
    return pi_estimate;
}

void* child(void* __restrict args){
    // __builtin_assume(N == 1024);
    long number_of_tosses = *(long *)args;
    long number_in_circle = 0;
    unsigned int seed = (unsigned int) pthread_self();
    for(long i = 0; i < number_of_tosses; i++){
        if(pow((double) rand_r(&seed) / (RAND_MAX), 2) + pow((double) rand_r(&seed) / (RAND_MAX), 2) <= 1)
            number_in_circle++;
    }

    pthread_mutex_lock(&lock);
    total_number_in_circle += number_in_circle;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

double get_pi_boost(long long int total_number_of_tosses, int thread_num = 1){
    pthread_t threads[thread_num];
    long number_of_tosses = ceil(total_number_of_tosses / thread_num);
    total_number_in_circle = 0;

    for(int i = 0; i < thread_num; i++)
        pthread_create(&threads[i], NULL, child, (void*) &number_of_tosses);

    for(int i = 0; i < thread_num; i++)
        pthread_join(threads[i], NULL);

    long double pi_estimate = 4 * total_number_in_circle / (( long double ) total_number_of_tosses);
    return pi_estimate; //3.141592;
}

int main(int argc, char *argv[]){
    assert(argc > 2);
    // char array to long long int
    int thread_num = atoi(argv[1]);
    long long int tosses = atoll(argv[2]);
    pthread_mutex_init(&lock, NULL);
    
    // long double pi = get_pi_seq(tosses);
    long double pi = get_pi_boost(tosses, thread_num);

    cout << pi << endl;
    
    return 0;
}
