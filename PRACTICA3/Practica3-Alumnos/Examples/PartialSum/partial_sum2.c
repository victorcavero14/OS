#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

// He arreglado la parte crítica en la que accedián los dos hilos mediante un semaforo.

sem_t sem;

int total_sum = 0;
void * partial_sum(void * arg) {
  int j = 0;
  int tmp;
  int ni=((int*)arg)[0];
  int nf=((int*)arg)[1];

  for (j = ni; j <= nf; j++) {
	sem_wait(&sem);
    tmp=total_sum;
    sched_yield();
    tmp=tmp+j;
    sched_yield();
    total_sum = tmp;
    sem_post(&sem);
  }

  pthread_exit(0);
}

int main(void) {
  pthread_t th1, th2;
  sem_init(&sem, 0, 1);
  int num1[2]={  1,   4999};
  int num2[2]={5000, 10000};

  /* Two threads are created */
  pthread_create(&th1, NULL, partial_sum, (void*)num1);
  pthread_create(&th2, NULL, partial_sum, (void*)num2);

  /* the main thread waits until both threads complete */
  pthread_join(th1, NULL);
  pthread_join(th2, NULL);

  printf("total_sum=%d and it should be 50005000\n", total_sum);
  sem_destroy(&sem);
  return 0;
}
