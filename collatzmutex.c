#include <stdlib.h>
#include <pthread.h>
#include "collatz.h"
#include <stdio.h>
#include <time.h>
//Mutex version of collatz

pthread_mutex_t mutex; //mutex decleration

typedef struct{  //struct to give data to threat.
    unsigned long max_num;    // threats only work with type void* so structs are used to "supply" data.
    int           max_iterations;
} thread_args;

unsigned long colsum = 0; // variable for all iterations sum
unsigned long nextcol = 1; //variable for collatz calc

void* thread_collatz(void* args){ //function that uses collatz.c to calc 
    thread_args* params = (thread_args*)args; //casts values in struct to be usable.
    params->max_iterations = -1;
    params->max_num = 0;
    unsigned long localcolsum = 0;
    int local_max_iter = -1;
    unsigned long local_max_num = 0;
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);

/*for(unsigned long i = nextcol; i <= 100000000; i++){ //usess start and end to set range and calc.
        int iterations = collatz(nextcol); // outside function
            nextcol++;
            colsum    += iterations;
        if(iterations > params->max_iterations){
            params->max_iterations = iterations;
            params->max_num = i;
    }
}*/

while(1){

unsigned long curent;

pthread_mutex_lock(&mutex);
curent = nextcol++;
pthread_mutex_unlock(&mutex);

if(curent > 100000000) break;

int iterations = collatz(curent);
localcolsum += iterations;

if(iterations > local_max_iter){
    local_max_iter = iterations;
    local_max_num = curent;
    }
}
pthread_mutex_lock(&mutex);

colsum += localcolsum;
if(local_max_iter > params->max_iterations){
    params->max_iterations = local_max_iter;
    params->max_num = local_max_num;
}
pthread_mutex_unlock(&mutex);



clock_gettime(CLOCK_MONOTONIC, &end);

double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

printf("Thread: took %f seconds. Max. It: %d with num: %lu\n",time_taken, params-> max_iterations, params->max_num);
return NULL; //end threat if end is reached }
}

int main(){

int num_threads; //number of threads 

struct timespec start, end; //struct for clock_gettime function

unsigned long global_max_num = 0;
int global_max_iterations = -1;
    
printf("Enter Number Of Threads: ");

if(scanf("%d", &num_threads) < 1){ //scanf to determine how many threads are used. if < 1 abort.
        printf("\nEnter Positive Integer!");
    return 1;
}

pthread_t threads[num_threads]; //Creating multiple thread IDs and storing them in Arrays 

thread_args args[num_threads]; //Array to store work ranges


if(clock_gettime(CLOCK_MONOTONIC, &start) == -1) { //start of measurring event
    perror("clock_gettime");
    return 1;
}

unsigned long range = 100000000 / num_threads; // Range, each thread has to calculate

pthread_mutex_init(&mutex, NULL);

for(int i = 0; i < num_threads; i++){ //Loop to creat the users intended amount of threads
    pthread_create (&threads[i], NULL, thread_collatz, &args[i]); //creating threads 
}

for(int i = 0; i < num_threads; i++){ //waiting for all threads to finish
    pthread_join(threads[i], NULL);
}
    // Find global maximum across all threads
    for(int i = 0; i < num_threads; i++) {
        if(args[i].max_iterations > global_max_iterations) {
            global_max_iterations = args[i].max_iterations;
            global_max_num = args[i].max_num;
        }
    }
   
if(clock_gettime(CLOCK_MONOTONIC, &end) == -1){ //end of meassuring event
    perror("clock_gettime");
    return 1;
}
double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9; //how long all threads took to finish

printf("Threads: %d | Time: %.6f seconds. Biggest number with most itertations is: %lu with %d iterations.\n The sum of all iterations is: %lu\n",num_threads, time_taken, global_max_num, global_max_iterations, colsum);

 
return 0;




}
