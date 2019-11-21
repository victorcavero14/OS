/*
 *- Con variable condicional es necesario un bucle while para realizar la espera (Dormir)
 *
 *- Con semaforo no es necesario el bucle infinito while, simplemente ponemos sem_wait(&sem).
 *
 */

int sillasOcupadas = 0;
bool durmiendo = false;
pthread threadBarbero, threadCliente[N_CLIENTES];
p_sem semDurmiendo, semAfeitando;
p_mutex cerrojo;

int main ()
{

	int clienteArg[N_CLIENTES];

	sem_init (&semDurmiendo, 0,0);
	sem_init (&semAfeitando, 0,0);

	pthread_mutex_init(&cerrojo, 0);
	
	pthread_create(&threadBargero,0, barbero, 0);

	for(int i = 0; i < N_CLIENTES; i++)
	{
		clienteArg[i] = i;
		pthread_create[&threadCliente[i], 0, cliente, clienteArg[i]];


	}

	for(int i = ; i < N_CLIENTES; i++)
		pthread_join(&threadCliente[i], 0);
	pthread_mutex_destoy(&cerrojo);


}

void* barbero(void* ptr)
{
	while(1)
	{
		pthread_mutex_lock(&cerrojo);
		if(filasOcupadas==0)
		{
			printf("...");
			durmiendo = true;
			pthread_mutex_unlock(&cerrojo);
			sem_wait(&semDurmiendo);
			pthread_mutex_lock(&cerrojo);
			prinf("...");
			durmiendo = false;
		}

		printf("Levanta y anda hacia mi...");
		sillasOcupadas -= 1;
		pthread_mutex_unlock(&cerrojo);
		sleep(3);
		sem_post(&semAfeitando);
	}
}

void* cliente(void* ptr)
{
	int idCliente = *((int *) ptr);
	pthread_mutex_lock(&cerrojo);
	if(sillasOcupadas < MAX_SILLAS)
	{
		printf("");
		sillasOcupadas += 1;
		if (durmiendo)
			sem_post (&semDurmiendo);
		pthread_mutex_unlock(&cerrojo);
		sem_wait(&semAfeitando);
		printf("Gracias!");
	}
	else
	{
		printf("....");
		pthread_mutex_unlock(&cerrojo);
	}
}
