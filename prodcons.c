#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(MonitorPC * p) {

    printf("Inizializzazione monitor\n");

    /* TBD: Inizializzare il monitor */
    init_monitor(&(p->m), 4); // Sono 4 CV
    p->num_liberi=DIM; // Inizializzo i contatori
    p->num_occupati_tipo1=0;
    p->num_occupati_tipo2=0;

    for(int i=0; i<DIM; i++){
        p->stato[i]=LIBERO;
    }

    for(int i=0; i<DIM; i++){
        p->vettore[i]=0;
    }

    printf("Monitor inizializzato.");

}


void rimuovi(MonitorPC * p) {

    printf("Rimozione monitor\n");
    remove_monitor(&(p->m));
    /* TBD: Disattivare il monitor */

}


void produci_tipo_1(MonitorPC * p, int valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione,
            in base allo schema con vettore di stato
     */
    enter_monitor((&p->m));

    if(p->num_liberi==0){
        wait_condition(&(p->m),CV_PROD1);
    }

    while(p->stato[index]!=LIBERO && index<DIM)
        index++;

    p->stato[index]=INUSO; 
    printf("Produzione di tipo 1 in corso...\n");

    sleep(1);
    p->vettore[index] = valore;
    p->stato[index]=OCCUPATO1;
    p->num_occupati_tipo1++;
    p->num_liberi--;
    printf("Produzione di tipo 1 completata (valore=%d, index=%d)\n", valore, index);
    signal_condition(&(p->m), CV_CONS1);
    leave_monitor(&(p->m));
}


void produci_tipo_2(MonitorPC * p, int valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione,
            in base allo schema con vettore di stato
     */
    enter_monitor(&(p->m));

    if(p->num_liberi==0)
        wait_condition(&(p->m), CV_PROD2);

    while(p->stato[index]!=LIBERO && index<DIM);
        index++;

    p->stato[index]=INUSO;
    printf("Produzione di tipo 2 in corso...\n");

    sleep(1);
    p->vettore[index] = valore;
    p->stato[index] = OCCUPATO2;
    p->num_occupati_tipo2++;
    p->num_liberi--;

    printf("Produzione di tipo 2 completata (valore=%d, index=%d)\n", valore, index);
    signal_condition(&(p->m), CV_CONS2);
    leave_monitor(&(p->m));
}


int consuma_tipo_1(MonitorPC * p) {

    int index = 0;

    int valore = 0;

    /* TBD: Aggiungere la sincronizzazione,
            in base allo schema con vettore di stato
     */
    enter_monitor(&(p->m));

    if(p->num_occupati_tipo1==0){
        wait_condition(&(p->m), CV_CONS1);
    }

    while(p->stato[index]!=OCCUPATO1 && index<DIM)
        index++;

    p->stato[index]=INUSO;

    printf("Consumazione di tipo 1 in corso...\n");
    
    sleep(1);
    valore = p->vettore[index];
    p->stato[index]=LIBERO;
    p->num_occupati_tipo1--;
    p->num_liberi++;
    printf("Consumazione di tipo 1 completata (valore=%d, index=%d)\n", valore, index);


    /* NOTA: Dopo la consumazione, occorre risvegliare un produttore di tipo 1
             oppure un produttore di tipo 2. È indifferente dare la priorità ai
             produttori di tipo 1 oppure di tipo 2.
     */

    signal_condition(&(p->m), CV_PROD2);
    signal_condition(&(p->m), CV_PROD1);
    leave_monitor(&(p->m));
    return valore;
}


int consuma_tipo_2(MonitorPC * p) {

    int index = 0;

    int valore = 0;

    /* TBD: Aggiungere la sincronizzazione,
            in base allo schema con vettore di stato
     */
    enter_monitor(&(p->m));

    if(p->num_occupati_tipo2==0)
        wait_condition(&(p->m), CV_CONS2);
    
    while(p->stato[index]!=LIBERO && index<DIM)
        index++;

    p->stato[index]=INUSO;

    printf("Consumazione di tipo 2 in corso...\n");

    sleep(1);
    valore = p->vettore[index];
    p->stato[index]=LIBERO;
    p->num_occupati_tipo2--;
    p->num_liberi++;
    printf("Consumazione di tipo 2 completata (valore=%d, index=%d)\n", valore, index);


    /* NOTA: Dopo la consumazione, occorre risvegliare un produttore di tipo 1
             oppure un produttore di tipo 2. È indifferente dare la priorità ai
             produttori di tipo 1 oppure di tipo 2.
     */
    leave_monitor(&(p->m));
    return valore;
}