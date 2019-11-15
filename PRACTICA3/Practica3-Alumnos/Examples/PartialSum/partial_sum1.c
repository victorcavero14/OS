#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <stdbool.h>

sem_t sem;
int total_sum = 0;

void * partial_sum(void * arg) {

  int j = 0;
  int ni=((int*)arg)[0];
  int nf=((int*)arg)[1];

  for (j = ni; j <= nf; j++)
  {
	sem_wait(&sem);
    total_sum = total_sum + j;
    sem_post(&sem);
  }
  pthread_exit(0);
}

int main(int argc, char* argv[])
{
  bool impar;
  int hilos, limite, tramo;
  sem_init(&sem,0,1); //sem_init (ref a semaforo, 0- No compartido entre procesos, valor inicializacion (solo un hilo))
  pthread_t th1, th2, th3, th4, th5;
  int num1[2], num2[2], num3[2], num4[2], num5[2];

  sscanf(argv[1], "%d",&hilos);
  sscanf(argv[2], "%d", &limite);

  printf("Hilos: %d \n", hilos);
  printf("Limite: %d \n\n", limite);

  tramo = (limite / hilos);
  impar = (tramo % 2 == 1);

  num1[0] = 1; num1[1] = tramo;

  num2[0] = tramo + 1; num2[1] = (2 * tramo);
  if(impar && hilos == 2) {num2[0] = tramo + 1; num2[1] = (2 * tramo) + 1;}

  num3[0] = (2 * tramo) + 1; num3[1] = (3 * tramo);
  if(impar && hilos == 3) {num3[0] = (2 * tramo) + 1; num3[1] = (3 * tramo) + 1;}

  num4[0] = (3 * tramo) + 1; num4[1] = (4 * tramo);
  if(impar && hilos == 4) {num4[0] = (3 * tramo) + 1; num4[1] = (4 * tramo) + 1;}

  num5[0] = (4 * tramo) + 1; num5[1] = (5 * tramo);
  if(impar && hilos == 5) {num5[0] = (4 * tramo) + 1; num5[1] = (5 * tramo) + 1;}


  /* Two threads are created */

  pthread_create(&th1, NULL, partial_sum, (void*)num1);
  if (hilos > 1) pthread_create(&th2, NULL, partial_sum, (void*)num2);
  if (hilos > 2) pthread_create(&th3, NULL, partial_sum, (void*)num3);
  if (hilos > 3) pthread_create(&th4, NULL, partial_sum, (void*)num4);
  if (hilos > 4) pthread_create(&th5, NULL, partial_sum, (void*)num5);

  /* the main thread waits until both threads complete */
  pthread_join(th1, NULL);
  if (hilos > 1) pthread_join(th2, NULL);
  if(hilos > 2) pthread_join(th3, NULL);
  if(hilos > 3) pthread_join(th4, NULL);
  if(hilos > 4) pthread_join(th5, NULL);

  sem_destroy(&sem);

  printf("total_sum=%d and it should be 50005000\n", total_sum);

  return 0;
}
