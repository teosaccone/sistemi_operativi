#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "prodcons.h"

void * produttore(void *);
void * consumatore(void *);

int main() {


    printf("[MAIN] inizializza_monitor\n");
    /* Creare e inizializzare il monitor */
    MonitorProdCons * m = (MonitorProdCons *) malloc(sizeof(MonitorProdCons));
    inizializza_monitor(m);

    printf("[MAIN] pthread_create\n");
    /* Avviare i thread */
    pthread_t prod[5];
    pthread_t cons;
    
    for(int i=0; i<5; i++)
        pthread_create(&prod[i], NULL, produttore, (void *) m);

    pthread_create(&cons, NULL, consumatore, (void *) m);


    printf("[MAIN] pthread_join\n");

    /* TBD: Attendere la terminazione dei thread */
    for(int i=0; i<5; i++)
        pthread_join(prod[i], NULL); 

    pthread_join(cons, NULL);
    pthread_join(m->timer, NULL);

    printf("[MAIN] distruzione_monitor\n");

    /* TBD: Disattivazione e deallocazione del monitor */
    distruggi_monitor(m);
    free((void *) m);

}


void * produttore(void * p) {

    printf("[MAIN] Avvio thread produttore\n");

    MonitorProdCons * m = (MonitorProdCons *) p;

    for(int i=0; i<4; i++) {

        int val = rand() % 10;

        printf("[MAIN] Produttore ha generato %d\n", val);

        int ret = produci(m, val); /* Chiamare il metodo di produzione */

        while(ret < 0) {

            sleep(1);

            printf("[MAIN] Produttore ritenta inserimento di %d\n", val);

            ret = produci(m, val); /* TBD: Chiamare il metodo di produzione */
        }

    }


    pthread_exit(NULL);
}

void * consumatore(void * p) {

    printf("[MAIN] Avvio thread consumatore\n");

    MonitorProdCons * m = (MonitorProdCons *) p;

    for(int i=0; i<20; i++) {

        int val;

        int ret = consuma(m, &val); /* Chiamare il metodo di consumazione */

        printf("[MAIN] Consumatore ha prelevato %d\n", val);

        sleep(2);
    }


    pthread_exit(NULL);
}