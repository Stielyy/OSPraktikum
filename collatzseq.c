#include <stdio.h>
#include "collatz.h"
void collatz(unsigned long n){
    int it_counter = 0;
    //printf("%d ", n); 
	while(n > 1){
		//printf("%d = ", n);
		if(n % 2 ==0){
			n = n/2;
            it_counter++;
		} else {
			n = 3 * n+1;
            it_counter++;
		}
	}
   //printf("= %d\n", it_counter);
}
