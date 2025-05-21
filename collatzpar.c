#include <stdlib.h>
#include <pthread.h>
#include "collatz.h"
#include <stdio.h>
#include <time.h>


typedef struct{  //struct to give data to threat.
    unsigned long start; // threats only work with type void* so structs are used to "supply" data.
    unsigned long end;
} thread_args;



void* thread_collatz(void* args){ //function that uses collatz.c to calc 
    thread_args* params = (thread_args*)args; //casts values in struct to be usable.
struct timespec start, end;
clock_gettime(CLOCK_MONOTONIC, &start);
for(unsigned long i = params->start; i <= params->end; i++){ //usess start and end to set range and calc.

        collatz(i); // outside function
  }
clock_gettime(CLOCK_MONOTONIC, &end);

double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

printf("Thread: %lu - %lu took %f seconds\n", params->start, params->end, time_taken);

return NULL; //end threat if end is reached 
}


int main(){

int num_threads; //number of threads 

printf("Enter Number Of Threads: ");

if(scanf("%d", &num_threads) < 1){ //scanf to determine how many threads are used. if < 1 abort.
        printf("\nEnter Positive Integer!");
    return 1;
}

pthread_t threads[num_threads]; //Creating threads. Array to store thread IDs

thread_args args[num_threads]; //Array to store work ranges

struct timespec start, end;

if(clock_gettime(CLOCK_MONOTONIC, &start) == -1) { //start of measurring event
    perror("clock_gettime");
    return 1;
}

unsigned long range = 100000000 / num_threads; // Range, each thread has to calculate


for(int i = 0; i < num_threads; i++){ //Loop to equaly devide work between threads 
    args[i].start = (range * i) + 1; //start of each thread
    args[i].end   = (i == num_threads -1) ? 100000000 : range * (i + 1); //end of each thread
    pthread_create (&threads[i], NULL, thread_collatz, &args[i]); //creating threads 
}

for(int i = 0; i < num_threads; i++){ //waiting for all threads to finish
    pthread_join(threads[i], NULL);
}
   
if(clock_gettime(CLOCK_MONOTONIC, &end) == -1){ //end of meassuring event
    perror("clock_gettime");
    return 1;
}
double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9; //how long all threads took to finish

printf("Threads: %d | Time: %.6f seconds\n",num_threads, time_taken);

 
return 0;


}
