#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "prodcons.h"

#define NUMPROCESSI 6

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

	int shm_id = shmget(IPC_PRIVATE, sizeof(MonitorPC), IPC_CREAT | 0664); // Creo memoria condivisa

	MonitorPC * p = shmat(shm_id, NULL, 0); /* TBD: Allocare lo oggetto monitor in modo condiviso fra i processi */
	// Faccio attach della memoria condivisa

	inizializza(p); 
    

	// Creazione di 2 produttori di tipo 1

	for(i=0; i<2; i++) {

		/* TBD: Creare i processi figli, facendogli eseguire
		        la funzione "produttore_1"
		 */
		pid_t pid_prod_1 = fork();

		if(pid_prod_1<0){
			perror("Errore nella creazione del processo figlio"); // Messaggio di errore
		} else if (pid_prod_1==0){
			printf("Figlio PROD_1 n. %d creato con successo. [PID %d]", i, getpid()); // Messaggio di DEBUG per la corretta creazione dei processi figli
			produttore_1(p); // Mando in ingresso alla funzione produttore_1 il MonitorPC richiesto
			printf("[%d] Termino processo.", getpid()); // DEBUG terminazione processo con pid 
			exit(0); // Uccisione dei processi figli
		}
	}

	// Creazione di 2 produttori di tipo 2

	for(i=0; i<2; i++) {

		/* TBD: Creare i processi figli, facendogli eseguire
		        la funzione "produttore_2"
		 */
		pid_t pid_prod_2 = fork();

		if(pid_prod_2<0){
			perror("Errore nella creazione del processo figlio");
		} else if (pid_prod_2==0){
			printf("Figlio PROD_1 n. %d creato con successo. [PID %d]", i, getpid()); // Messaggio di DEBUG per la corretta creazione dei processi figli
			produttore_2(p);
			printf("[%d] Termino processo.", getpid()); // DEBUG terminazione processo con pid
			exit(0); // Uccisione dei processi figli
		}
	}



	// Creazione di 1 consumatore di tipo 1
	pid_t pid_cons_1 = fork();

	if(pid_cons_1<0){
		perror("Errore nella creazione del processo figlio.");
	} else if(pid_cons_1==0){
		printf("Figlio CONS_1 n. 1 creato con successo. [%d]", getpid());
		consumatore_1(p);
		printf("[%d] Termino processo. ", getpid());
		exit(0); 
	}
	/* TBD: Creare i processi figli, facendogli eseguire
			la funzione "consumatore_1"
	 */



	// Creazione di 1 consumatore di tipo 2

	/* TBD: Creare i processi figli, facendogli eseguire
			la funzione "consumatore_2"
	 */
	pid_t pid_cons_2 = fork();

	if(pid_cons_2<0){
		perror("Errore nella creazione del processo figlio.");
	} else if(pid_cons_2==0){
		printf("Figlio CONS_1 n. 1 creato con successo. [%d]", getpid());
		consumatore_2(p);
		printf("[%d] Termino processo. ", getpid());
		exit(0); 
	}



	/* TBD: Attendere la terminazione di tutti i processi figli */
	for(int i=0; i<NUMPROCESSI; i++){
		printf("[%d] Attendo terminazione processi figli.", getpid());
		wait(NULL);
	}

	rimuovi(p);

	/* TBD: De-allocare lo oggetto monitor condiviso */
	rimuovi(p); // De-alloco il monitor condiviso
	shmctl(shm_id, IPC_RMID, 0); // De-alloco la memoria condivisa
}

