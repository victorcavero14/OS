#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <semaphore.h>

sem_t sem;
int total_sum = 0;

void * partial_sum(void * arg) {
  sem_wait(&sem);
  int j = 0;
  int ni=((int*)arg)[0];
  int nf=((int*)arg)[1];

  for (j = ni; j <= nf; j++)
    total_sum = total_sum + j;

  sem_post(&sem);
  //pthread_exit(0);
}

int main(int argc, char* argv[])
{
  //printf("%s ", argv[2]);
  // argv[1] y argv[2] tienen el numero
  // de hilos a usar y el limite de la suma.

  sem_init(&sem,0,1);
  pthread_t th1, th2;

  int limite = (int) argv[2];
  int hilos = (int) argv[1];

  printf("hilos: %d \n", hilos);
  printf("limite: %d \n", limite);

  int limiteprimero = limite / hilos - 1;
  int limitesegundo =  limite/hilos;

  printf("Limiteprimero: %d \n", limiteprimero);
  printf("LimiteSegundo: %d \n", limitesegundo);


  int num1[2]={  1,  limiteprimero};
  int num2[2]={limitesegundo, *argv[2]};


  /* Two threads are created */
  pthread_create(&th1, NULL, partial_sum, (void*)num1);
  pthread_create(&th2, NULL, partial_sum, (void*)num2);

  /* the main thread waits until both threads complete */
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);
  sem_destroy(&sem);

  printf("total_sum=%d and it should be 50005000\n", total_sum);

  return 0;
}
