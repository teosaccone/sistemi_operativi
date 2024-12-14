#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "prodcons.h"

/* Dichiarazione di metodo privato, che sarà
 * eseguito da un thread "timer" */
static void * thread_timer(void *);


void inizializza_monitor(MonitorProdCons * m) { // Done

    printf("[MONITOR] Inizializzazione monitor\n");

    m->testa=0;
    m->coda=0;
    m->contatore=0;

    m->uscita_timer=0;

    pthread_mutex_init(&(m->mutex), NULL);

    pthread_cond_init(&(m->cv_prod), NULL);
    pthread_cond_init(&(m->cv_cons), NULL);

    pthread_create(&(m->timer), NULL, thread_timer, (void *) m); // Dopo devo fare il casting del monitor
}

void distruggi_monitor(MonitorProdCons * m) { // Done

    printf("[MONITOR] Distruzione monitor\n");

    pthread_mutex_destroy(&(m->mutex));

    pthread_cond_destroy(&(m->cv_cons));
    pthread_cond_destroy(&(m->cv_prod));

    m->uscita_timer=1;
}

int produci(MonitorProdCons * m, int val) {


    // PRIMA DELL'ATTESA, SALVA L'ORARIO
    struct timeval t1;
    gettimeofday(&t1,NULL);
    long start_time = t1.tv_sec*1000000 + t1.tv_usec;
    

    while(m->contatore == DIM)
        pthread_cond_wait(&(m->cv_prod), &(m->mutex));

    // DOPO L'ATTESA, SALVA DI NUOVO L'ORARIO, E CONTROLLA SE SONO PASSATI 3 SECONDI
    struct timeval t2;
    gettimeofday(&t2,NULL);
    long current_time = t2.tv_sec*1000000 + t2.tv_usec;
    
    if(current_time - start_time >= 3000000) {

        printf("[MONITOR] Produttore ha atteso oltre 3 secondi, esce\n");

        return -1;
    }



    m->buffer[m->testa] = val;
    m->testa=(m->testa + 1) % DIM;
    m->contatore++;

    printf("[MONITOR] Produzione %d\n", val);

    pthread_cond_signal(&(m->cv_cons));

    return 0;
}

int consuma(MonitorProdCons * m, int * val) {

    /* TBD: Introdurre la sincronizzazione */

    struct timeval t1;
	gettimeofday(&t1,NULL);
    long start_time = t1.tv_sec*1000000 + t1.tv_usec;

    while(m->contatore==0)
        pthread_cond_wait(&(m->cv_cons), &(m->mutex));

    struct timeval t2;
	gettimeofday(&t2,NULL);
    long current_time = t2.tv_sec*1000000 + t2.tv_usec;

    if(current_time - start_time >= 3000000) {

        printf("[MONITOR] Consumatore ha atteso oltre 3 secondi, esce\n");

        return -1;
    }



    *val = m->buffer[m->coda]; // Unici dubbi sull'utilizzo di questo puntatore
    m->coda=(m->coda + 1) % DIM;
    m->contatore--;

    printf("[MONITOR] Consumazione %d\n", *val);

    pthread_cond_signal(&(m->cv_prod));

    return 0;
}

void * thread_timer(void * p) {

    MonitorProdCons * m = (MonitorProdCons *) p;

    printf("[MONITOR] Avvio thread timer\n");


    while(1) {

        int uscita = 0;

        sleep(1);

        /* Il thread timer periodicamente attiva tutti i thread sospesi */
        pthread_cond_broadcast(&(m->cv_prod));
        pthread_cond_broadcast(&(m->cv_cons));

        if(uscita != 0) {
            break;
        }
    }

    printf("[MONITOR] Uscita thread timer\n");

    pthread_exit(NULL);
}