/*
	Víctor Manuel Cavero Gracia - DNI: 45355080T
	Iván Fernández Sánchez - DNI: 52902115E
*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_PARADAS 5 // número de paradas de la ruta
#define EN_RUTA 0// autobús en ruta
#define EN_PARADA 1 // autobús en la parada
#define MAX_USUARIOS 40 // capacidad del autobús
#define USUARIOS 4 // numero de usuarios

// estado inicial

int estado = EN_RUTA;
int parada_actual = 0; // parada en la que se encuentra el autobus
int n_ocupantes = 0; // ocupantes que tiene el autobús

//Autobus y usuarios hilos

pthread_t autobus;
pthread_t usuario[MAX_USUARIOS];

//Added para proteger las variables compartidas y las variables condicionales

pthread_mutex_t cerrojo;
//sem_t sem;

//var condicionales

pthread_cond_t bus_en_parada = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t usuarios_pueden_subir = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t usuarios_pueden_bajar = PTHREAD_MUTEX_INITIALIZER;

// personas que desean subir en cada parada
int esperando_parada[N_PARADAS] = {2,0,1,1,0}; //= {0,0,...0};

// personas que desean bajar en cada parada
int esperando_bajar[N_PARADAS] = {0,0,2,1,1}; //= {0,0,...0};

//prototipos

void * thread_autobus(void * args);
void * thread_usuario(void * arg);
void Usuario(int id_usuario, int origen, int destino);
void Autobus_En_Parada();
void Conducir_Hasta_Siguiente_Parada();
void Subir_Autobus(int id_usuario, int origen);
void Bajar_Autobus(int id_usuario, int destino);



void * thread_autobus(void * args)
{
	while (1) {

		// esperar a que los viajeros suban y bajen
		Autobus_En_Parada();
		// conducir hasta siguiente parada
		Conducir_Hasta_Siguiente_Parada();

	}
	pthread_exit(0);
}

void * thread_usuario(void * arg)
{
	int id_usuario = (int)arg; //id usuario

	while (1) //los usuarios que acaban el trayecto vuelven a hacer otro
	{

		int b, a=rand() % N_PARADAS;  //Destinos y origenes aleatorios
		do{
			b=rand() % N_PARADAS;
		} while(a==b);
		Usuario(id_usuario,a,b);
	}

	pthread_exit(0);
}

void Usuario(int id_usuario, int origen, int destino) {
	// Esperar a que el autobus esté en parada origen para subir
	Subir_Autobus(id_usuario, origen);
	// Bajarme en estación destino
	Bajar_Autobus(id_usuario, destino);
}

void Autobus_En_Parada(){
	/* Ajustar el estado y bloquear al autobús hasta que no haya pasajeros que
	quieran bajar y/o subir la parada actual. Después se pone en marcha */

	pthread_mutex_lock(&cerrojo);

	printf("---- Autobus en parada %d.\n", parada_actual);
	estado = EN_PARADA;

	pthread_cond_broadcast(&bus_en_parada);
	//pthread_cond_broadcast(&usuarios_pueden_subir);
	//pthread_cond_broadcast(&usuarios_pueden_bajar);

	pthread_mutex_unlock(&cerrojo);

	sleep(33333);

}

void Conducir_Hasta_Siguiente_Parada(){
	/* Establecer un Retardo que simule el trayecto y actualizar numero de parada */

	pthread_mutex_lock(&cerrojo);

	estado = EN_RUTA;
	parada_actual += 1;
	if(parada_actual >= N_PARADAS) parada_actual = 0;

	pthread_mutex_unlock(&cerrojo);

	printf("Circulando...\n");
	sleep(rand() % 8);

}

void Subir_Autobus(int id_usuario, int origen){
	/* El usuario indicará que quiere subir en la parada ’origen’, esperará a que
	el autobús se pare en dicha parada y subirá. El id_usuario puede utilizarse para
	proporcionar información de depuración */

	int subido = 0;

	while(subido == 0)
	{
	while(estado == EN_RUTA) pthread_cond_wait(&bus_en_parada, &cerrojo);

	//pthread_cond_wait(&bus_en_parada, &cerrojo);
	//pthread_cond_wait(&usuarios_pueden_subir, &cerrojo);

	printf("Estado: %d\n",estado);
	printf("Parada actual: %d\n",parada_actual);
	printf("Origen: %d\n",origen);

	if(estado == EN_PARADA && parada_actual == origen && n_ocupantes < MAX_USUARIOS)
	{
		pthread_mutex_lock(&cerrojo);

		n_ocupantes += 1;
		subido = 1;

		pthread_mutex_unlock(&cerrojo);
	}
	}

	printf("Usuario %d ha subido al autobus.\n", id_usuario);
}

void Bajar_Autobus(int id_usuario, int destino){
	/* El usuario indicará que quiere bajar en la parada ’destino’, esperará a que
	el autobús se pare en dicha parada y bajará. El id_usuario puede utilizarse para
	proporcionar información de depuración */

	int llegar = 0;
	while(llegar == 0)
	{
		//pthread_cond_wait(&bus_en_parada, &cerrojo);
		pthread_cond_wait(&usuarios_pueden_bajar, &cerrojo);

		if(estado == EN_PARADA && parada_actual == destino)
		{
			pthread_mutex_lock(&cerrojo);

			n_ocupantes -= 1;
			llegar = 1;

			pthread_mutex_unlock(&cerrojo);
		}
	}

	printf("Usuario %d ha bajado del autobus.\n", id_usuario);
}

int main(int argc, char *argv[]) {
	unsigned long i, j = 1;
	// Definición de variables locales a main
	// Opcional: obtener de los argumentos del programa la capacidad del
	// autobus, el numero de usuarios y el numero de paradas
	// Crear el thread Autobus

	//sem_init(&sem,0,1); //inicializar el semaforo
	pthread_mutex_init(&cerrojo, NULL);

	pthread_create(&autobus,NULL,thread_autobus, (void*)j); //thread bus

	for (i = 0; i < USUARIOS; i++) 	// Crear thread para el usuario i
			pthread_create(&usuario[i],NULL,thread_usuario,(void*)i);

	// Esperar terminación de los hilos

	for(i=0; i< USUARIOS; i++)
	        pthread_join(usuario[i],NULL);

	pthread_join(autobus, NULL);

	pthread_mutex_destroy(&cerrojo);
	//sem_destroy(&sem);

	return 0;
}
