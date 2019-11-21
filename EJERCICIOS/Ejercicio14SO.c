/* De los ejercicios mas dificiles porque mezcla semaforos y cerrojos.
 *
 * */

#define N_CONSTUMERS 12

int eating = 0, waiting = 0;

void main(void)
{
	int i=0;
	int constumer_id[N_CONSTUMERS];
	for(i = 0; i < N_CONSTUMERS; i++)
	{
		constumers_id[i] = i+1;
	}
	pthread_mutex_init(&mutex,0);
	sem_init(&block, 0,0);
	
	for(i = 0; i < N_CONSTUMERS; i++)
		pthread_create(&constumers[i],0, sushi_bar);


	for(i = 0; i < N_CONSTUMERS; i++)
		pthread_join(&constumers[i], NULL);

}


void* sushi_bar(void * ptr)
{
	int i, n;
	pthread_mutex_lock(&mutex);
	if(eating == 5 || waiting > 0)
	{
		waiting += 1; printf("Constumer waiting... ");
		pthread_mutex_unlock(&mutex);
		sem_wait(&block);
		sleep(3);
	}
	else
	{
		eating += 1;
		printf("Constumer eating...");
		pthread_mutex_unlock(&mutex);
		sleep(3);
	}
	pthread_mutex_lock (&mutex);
	eating -= 1;
	printf("Constumer end eating...");
	
	if(eating == 0 && waiting)
	{
		if(waiting < 5)
		{
			n = waiting;

		}	
		else
		{
			n = 5;
		}
		waiting = n;
		wating += n;
		for(i = 0; i < n; i++)
			sem_post(&block); //les pides a los hilos que entren a comer
	}

	pthread_mutex_unlock(&mutex);





}
