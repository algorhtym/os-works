#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>


void *calculate(void *ptr);
int *fibonacci; // global dynamic array to hold fibonacco numbers
//size_t array_size;

int main(int argc, char *argv[]) {

  

  int user_input;
  pthread_t fib_t;
  size_t arrsize;

  /* making sure of input format*/
  if (argc != 2) {
    printf("Use %s [even number greater than 2] format! \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  /* Saving user input as int */
  user_input = atoi(argv[1]);
  arrsize = user_input * 2;
  
  fibonacci = calloc(arrsize, sizeof(int));
  if (fibonacci == NULL) {
    fprintf(stderr, "Array not allocated!");
    return EXIT_FAILURE;
  }


  pthread_create(&fib_t, NULL, calculate, &user_input);

  pthread_join(fib_t, NULL);

  for (int i = 1; i <= user_input; i++) {
    printf("The %d", i);

    if (i == 1) {
      printf("st");
    } else if (i == 2) {
      printf("nd");
    } else if (i == 3) {
      printf("rd");
    } else {
      printf("th");
    }

    

    printf(" square : (");

    if (i % 2 == 0) {
      printf("%d, %d)\n", fibonacci[i - 1], fibonacci[i]);
    } else {
      printf("0, %d)\n", fibonacci[i + 1]);
    }

  }



  free(fibonacci);

  return EXIT_SUCCESS;

}

void *calculate(void *ptr){
  int num = *(int*) ptr;

  

  fibonacci[0] = 0;
  fibonacci[1] = 1;

  for (int i = 2; i <= num + 1; i++) {
    fibonacci[i] = fibonacci[i-1] + fibonacci[i-2];
  }

  printf("Child thread:\nLength: %d, Width: %d\n", fibonacci[num+1], fibonacci[num] );

  pthread_exit(NULL);

  
}
