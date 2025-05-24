#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;

long MAX_CNT = 0;

void* routineplus(){
    for(int i = 0; i < 10000000; i++){
        pthread_mutex_lock(&mutex);
        MAX_CNT++;
        pthread_mutex_unlock(&mutex);
    }
}

void* routineminus(){
    for(int i = 0; i < 10000000; i++){
        pthread_mutex_lock(&mutex);
        MAX_CNT--;
        pthread_mutex_unlock(&mutex);
    }
}

int main(){
pthread_t t1, t2;

pthread_mutex_init(&mutex, NULL);

pthread_create(&t1, NULL, &routineplus, NULL);
pthread_create(&t2, NULL, &routineminus, NULL);

pthread_join(t1, NULL);
pthread_join(t2, NULL);

printf("MAX_CNT = %d", MAX_CNT);
return 0;
}
