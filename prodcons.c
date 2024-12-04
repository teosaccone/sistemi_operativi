#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(MonitorPC * p) {

    printf("Inizializzazione monitor\n");

    /* TBD: Inizializzare il monitor */
    init_monitor(&(p->m), NUMVARCOND);
    p->num_liberi=0;
    p->num_occupati_tipo1=0;
    p->num_occupati_tipo2=0;
    
    for(int i=0; i<DIM; i++)
        p->stato[i]=LIBERO;
}


void rimuovi(MonitorPC * p) {

    printf("Rimozione monitor\n");

    /* TBD: Disattivare il monitor */
    remove_monitor(&(p->m));

}


void produci_tipo_1(MonitorPC * p, int valore) {

    int index = inizio_produzione_tipo_1(p);

    /* TBD: Aggiungere la sincronizzazione,
            in base allo schema con vettore di stato
     */


    printf("Produzione di tipo 1 in corso...\n");

    sleep(1);
    p->vettore[index] = valore;

    fine_produzione_tipo_1(p, index);

    printf("Produzione di tipo 1 completata (valore=%d, index=%d)\n", valore, index);

}


void produci_tipo_2(MonitorPC * p, int valore) {

    int index = inizio_produzione_tipo_2(p);

    /* TBD: Aggiungere la sincronizzazione,
            in base allo schema con vettore di stato
     */


    printf("Produzione di tipo 2 in corso...\n");

    sleep(1);
    p->vettore[index] = valore;

    fine_produzione_tipo_2(p, index);

    printf("Produzione di tipo 2 completata (valore=%d, index=%d)\n", valore, index);

}


int consuma_tipo_1(MonitorPC * p) {

    int index = inizio_consumazione_tipo_1(p);

    int valore = 0;

    /* TBD: Aggiungere la sincronizzazione,
            in base allo schema con vettore di stato
     */


    printf("Consumazione di tipo 1 in corso...\n");

    sleep(1);
    valore = p->vettore[index];

    fine_consumazione_tipo_1(p, index);

    printf("Consumazione di tipo 1 completata (valore=%d, index=%d)\n", valore, index);


    /* NOTA: Dopo la consumazione, occorre risvegliare un produttore di tipo 1
             oppure un produttore di tipo 2. È indifferente dare la priorità ai
             produttori di tipo 1 oppure di tipo 2.
     */

    return valore;
}


int consuma_tipo_2(MonitorPC * p) {

    int index = inizio_consumazione_tipo_2(p);

    int valore = 0;

    /* TBD: Aggiungere la sincronizzazione,
            in base allo schema con vettore di stato
     */


    printf("Consumazione di tipo 2 in corso...\n");

    sleep(1);
    valore = p->vettore[index];

    fine_consumazione_tipo_1(p, index);

    printf("Consumazione di tipo 2 completata (valore=%d, index=%d)\n", valore, index);


    /* NOTA: Dopo la consumazione, occorre risvegliare un produttore di tipo 1
             oppure un produttore di tipo 2. È indifferente dare la priorità ai
             produttori di tipo 1 oppure di tipo 2.
     */

    return valore;
}

int inizio_produzione_tipo_1(MonitorPC * p){
    
    enter_monitor(&(p->m));

    int i=0;

    if(p->num_liberi==0)
        wait_condition(&(p->m), CV_PROD);

    while(p->stato[i] != LIBERO && i < DIM)
        i++;

    p->stato[i]=INUSO;
    p->num_liberi--;

    leave_monitor(&(p->m));

    return i;
}

void fine_produzione_tipo_1(MonitorPC * p, int i){
    enter_monitor(&(p->m));

    p->stato[i]=OCCUPATO1;
    p->num_occupati_tipo1++;
    signal_condition(&(p->m), CV_CONS1);

    leave_monitor(&(p->m));
}

int inizio_produzione_tipo_2(MonitorPC * p){
    
    enter_monitor(&(p->m));

    int i=0;

    if(p->num_liberi==0)
        wait_condition(&(p->m), CV_PROD);

    while(p->stato[i] != LIBERO && i < DIM)
        i++;

    p->stato[i]=INUSO;
    p->num_liberi--;

    leave_monitor(&(p->m));

    return i;
}

void fine_produzione_tipo_2(MonitorPC * p, int i){
    enter_monitor(&(p->m));

    p->stato[i]=OCCUPATO1;
    p->num_occupati_tipo2++;
    signal_condition(&(p->m), CV_CONS2);

    leave_monitor(&(p->m));
}

int inizio_consumazione_tipo_1(MonitorPC * p){

    enter_monitor(&(p->m));

    int i=0;

    if(p->num_occupati_tipo1 == 0)
        wait_condition(&(p->m), CV_CONS1);

    while(p->stato[i] != OCCUPATO1 && i < DIM)
        i++;

    p->stato[i]=INUSO;
    p->num_occupati_tipo1--;

    leave_monitor(&(p->m));

    return i;
}

void fine_consumazione_tipo_1(MonitorPC * p, int i){

    enter_monitor(&(p->m));

    p->stato[i]=LIBERO;
    p->num_liberi++;
    signal_condition(&(p->m),CV_PROD);

    leave_monitor(&(p->m));
}

int inizio_consumazione_tipo_2(MonitorPC * p){

    enter_monitor(&(p->m));

    int i=0;

    if(p->num_occupati_tipo2 == 0)
        wait_condition(&(p->m), CV_CONS2);

    while(p->stato[i] != OCCUPATO2 && i < DIM)
        i++;

    p->stato[i]=INUSO;
    p->num_occupati_tipo2--;

    leave_monitor(&(p->m));

    return i;
}

void fine_consumazione_tipo_2(MonitorPC * p, int i){ // Le funzioni sono ridondanti ma le lascio così per leggibilità

    enter_monitor(&(p->m));

    p->stato[i]=LIBERO;
    p->num_liberi++;
    signal_condition(&(p->m),CV_PROD);

    leave_monitor(&(p->m));
}
