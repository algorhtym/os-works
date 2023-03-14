#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>


void *decompose_number(void *ptr);

int main(int argc, char *argv[]) {

  int user_input;
  pthread_t decompose; 
  

  /* making sure of input format*/
  if (argc != 2) {
    printf("Use %s [even number greater than 2] format! \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Saving user input as int */
  user_input = atoi(argv[1]);

  /* verifying the input number is even and greater than 2 */
  if (user_input % 2 != 0 || user_input <= 2) {
    printf("The number is not even and greater than 2!\n");
    exit(EXIT_FAILURE);
  }

  /* Creating the thread to decompose number */
  pthread_create(&decompose, NULL, decompose_number, &user_input);

  pthread_join(decompose, NULL);

  return EXIT_SUCCESS;
}


void *decompose_number(void *ptr) {
  /* Converting the originally int pointer - currently void pointer argument into 
     local int variable. 
  */
  int num = *(int*)ptr;
  int d1 = 0, d2 = 0;
  int i;

  /* Determining all prime numbers until the given number to use the
   * 'sieve of Erastothenes algorithm'. 
  */
  bool is_prime[num+1];
  is_prime[0] = is_prime[1] = false;

  for ( i = 2; i <= num ; i++){
       is_prime[i] = true;
  }

  /*
   * The Sieve of Erastothenes algorithm which sets all prime numbers up to user_input as prime
   * by setting all non-prime factors to false
  */
  for (i = 2; i <= num / 2; i++) {
    if (is_prime[i] == true) {
      for (int j = i * i; j <= num; j = j+i) {
	is_prime[j] = false;
      }
    }
  }
  /* Finding prime sum pairs */
  for (i = 0; i <= num / 2; i++) {
    if (is_prime[i] && is_prime[num - i]) {
      d1 = i;
        d2 = num - i;
      break;
    }
  }

  printf("Input: %d \nOutput: %d + %d\n", num, d1, d2);

  
  pthread_exit(NULL);
}

  
  
  

  
  

  


  
  

  

  
