#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "prodcons.h"


void produttore_1(MonitorPC * p)
{
	srand(getpid());

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		produci_tipo_1(p, valore);

		sleep(1);
	}

}


void produttore_2(MonitorPC * p)
{
	srand(getpid());

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		produci_tipo_2(p, valore);

		sleep(1);
	}

}


void consumatore_1(MonitorPC * p)
{
	srand(getpid());

	for(int i=0; i<8; i++) {

		int valore = consuma_tipo_1(p);

		sleep(1);
	}

}


void consumatore_2(MonitorPC * p)
{
	srand(getpid());
	
	for(int i=0; i<8; i++) {

		int valore = consuma_tipo_2(p);

		sleep(1);
	}

}

int main(int argc, char *argv[])
{

	int i;
	pid_t pid;

	int id_shm = shmget(IPC_PRIVATE, sizeof(MonitorPC), IPC_CREAT | 0664);

	MonitorPC * p = shmat(id_shm, NULL, 0); /* TBD: Allocare lo oggetto monitor in modo condiviso fra i processi */

	inizializza(p);
    

	// Creazione di 2 produttori di tipo 1

	for(i=0; i<2; i++) {

		/* TBD: Creare i processi figli, facendogli eseguire
		        la funzione "produttore_1"
		 */
		pid = fork();

		if(pid < 0){
			perror("Errore nella creazione del processo figlio.");
		} else if(pid == 0){
			printf("[%d] Generato produttore di tipo 1\n ", getpid());
			produttore_1(p);
			exit(0);
		}
	}

	// Creazione di 2 produttori di tipo 2

	for(i=0; i<2; i++) {

		/* TBD: Creare i processi figli, facendogli eseguire
		        la funzione "produttore_2"
		 */
		pid = fork();

		if(pid < 0){
			perror("Errore nella creazione del processo figlio.");
		} else if(pid == 0){
			printf("[%d] Generato produttore di tipo 2\n", getpid());
			produttore_2(p);
			exit(0);
		}
	}



	// Creazione di 1 consumatore di tipo 1

	/* TBD: Creare i processi figli, facendogli eseguire
			la funzione "consumatore_1"
	 */

	pid = fork();

	if(pid < 0){
			perror("Errore nella creazione del processo figlio.");
	} else if(pid == 0){
			printf("[%d] Generato consumatore di tipo 1\n", getpid());
			consumatore_1(p);
			exit(0);
	}



	// Creazione di 1 consumatore di tipo 2

	/* TBD: Creare i processi figli, facendogli eseguire
			la funzione "consumatore_2"
	 */

	pid = fork();

	if(pid < 0){
			perror("Errore nella creazione del processo figlio.");
	} else if(pid == 0){
			printf("[%d] Generato consumatore di tipo 2\n", getpid());
			consumatore_2(p);
			exit(0);
	}


	/* TBD: Attendere la terminazione di tutti i processi figli */
	
	for(i=0; i<6; i++)
		wait(NULL);

	rimuovi(p);

	/* TBD: De-allocare lo oggetto monitor condiviso */
	shmctl(id_shm, IPC_RMID, 0);
}

