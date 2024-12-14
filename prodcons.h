#ifndef _PRODCONS_H_
#define _PRODCONS_H_

#include <pthread.h>

#define DIM 4

typedef struct { // Done

    int buffer[DIM];

    int testa;
    int coda;
    int contatore;

    int uscita_timer;

    pthread_mutex_t mutex;

    pthread_cond_t cv_cons;
    pthread_cond_t cv_prod;

    pthread_t timer;


} MonitorProdCons;

void inizializza_monitor(MonitorProdCons * m);
void distruggi_monitor(MonitorProdCons * m);
int produci(MonitorProdCons * m, int val);
int consuma(MonitorProdCons * m, int * val);

#endif