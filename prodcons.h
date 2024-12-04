#ifndef __PROCEDURE_H
#define __PROCEDURE_H

#include "monitor_hoare.h"

#define DIM 4

#define LIBERO 0
#define INUSO 1
#define OCCUPATO1 2
#define OCCUPATO2 3

/* Definisco delle MACRO per identificare le variabili condition */
#define CV_PROD1 0
#define CV_PROD2 1
#define CV_CONS1 2
#define CV_CONS2 3
#define NUMVARCOND 3

typedef struct{

	int vettore[DIM];

	int stato[DIM];

	int num_liberi;
	int num_occupati_tipo1;
	int num_occupati_tipo2;

	/* TBD: aggiungere ulteriori variabili per la sincronizzazione */
	Monitor m; /* Necessito di un monitor per la sincronizzazione */

} MonitorPC;

void inizializza(MonitorPC * p);
void rimuovi(MonitorPC * p);
void produci_tipo_1(MonitorPC * p, int valore);
void produci_tipo_2(MonitorPC * p, int valore);
int consuma_tipo_1(MonitorPC * p);
int consuma_tipo_2(MonitorPC * p);

/* Definisco delle funzioni di inizio e fine produzione e inizio e fine consumazione del tipo 1 */
int inizio_produzione_tipo_1(MonitorPC * p);
void fine_produzione_tipo_1(MonitorPC * p, int i);
int inizio_consumazione_tipo_1(MonitorPC * p);
void fine_consumazione_tipo_1(MonitorPC * p, int i);

/* Definisco delle funzioni di inizio e fine produzione e inizio e fine consumazione del tipo 2 */
int inizio_produzione_tipo_2(MonitorPC * p);
void fine_produzione_tipo_2(MonitorPC * p, int i);
int inizio_consumazione_tipo_2(MonitorPC * p);
void fine_consumazione_tipo_2(MonitorPC * p, int i);


#endif
