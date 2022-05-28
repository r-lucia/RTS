//IN task.ecg HO GIA' CREATO DUE VETTORI AL CUI INTERNO
//CI SONO I VALORI DI TENSIONE E QUELLI IN SECONDI OPPORTUNAMENTE SCALATI

#include "Patologie.h"


//-----------------------------------------------------
// GLOBAL VARIABLE
//-----------------------------------------------------
extern FILE *fp;
extern int x_i;
extern int y_i;
extern int x_f;
extern int y_f;
extern float vett_x[DIM_DATI];
extern float vett_y[DIM_DATI];
extern float vett_R[DIM_DATI];
extern float time_R[DIM_DATI];
extern int indice_R[DIM_DATI];
extern int num_R;
float P = 0;
extern int window_PR;



//-----------------------------------------------------
// PRIVATE VARIABLE
//-----------------------------------------------------

//-----------------------------------------------------
//FUNZIONI AUSILIARI
//-----------------------------------------------------
void finestraRP() {
    float t_camp;
    t_camp = vett_x[1] - vett_x[0];
    t_camp = t_camp * 1000;
    window_PR = (int) (WINDOW_PR_MAX /
                       t_camp); //fornisce il numero di passi da sottrarre all'indice del picco R per andare a rilevare se c'è un picco P

}


//-----------------------------------------------------
//         PICCO R
//-----------------------------------------------------

void picco_R() {

    int i = -1;
    float R = 1;
    for (int k = 1; k < DIM_DATI; k++) {
        if (vett_y[k] > vett_y[k - 1] &&
            vett_y[k] > R) { //questo if mi serve per verificare se il valore successivo è maggiore del
            //precedente e se sto su un picco R
            if (i == -1 ||
                vett_x[k] - time_R[i] > 00.05) { //la prima condizione serve a poter scrivere anche nella posizione i=o
                //la seconda mi serve a capire se sto valutando picchi tra loro lontani
                i++;                                        //se sono verificate queste condizioni allora incremento i
                num_R++;
            }
            time_R[i] = vett_x[k];                          //aggiorno i vettori che mi registrano i tempi dei picchi R
            vett_R[i] = vett_y[k];
            indice_R[i] = k;                              //salva l'indice k dei vettori costruitleggendo il file csv
            printf("\n%f %f %d\n", time_R[i], vett_R[i], indice_R[i]);
        }

    }


}

//-----------------------------------------------------
//         PICCO P
//-----------------------------------------------------
void picco_P() {
    int k;
    finestraRP();
    for (int i = 0; i < num_R; i++) {
        k = indice_R[i];
        for (int m = k - window_PR; m < k - 3; m++) {
            if (vett_y[m] > P) {
                P = vett_y[m];

            }

        }
        printf("\n il picco P è: %f e il passo è: %d\n", P, window_PR);
    }

}

//-----------------------------------------------------
//         ARITMIA 4
//-----------------------------------------------------
void aritmia() {
    int j = 1;
    int h = 0;
    for (int k = 1; k < num_R; k++) {
        if (time_R[k] - time_R[k - 1] > 1.1) { //non deve superare 2 quadettirni quindi 0,4s=> 400ms
            printf("%d aritmia  \n", j);
            return;
        } else {
            printf("%d nessuna aritmia  \n", h);
            return;

        }
    }
}

//-----------------------------------------------------
//         FIBRILLAZIONE ATRIALE 3
//-----------------------------------------------------
void fibr_atriale() {

    int j = 1;
    int k = 0;
    if (P == 0) {
        printf("%d fibrillazioni atriali \n", j);
    } else printf("%d fibrillazioni atriali \n", k);
}

//-----------------------------------------------------
//         TACHICARDIA SINUSALE 5
//-----------------------------------------------------
void tachicardia_sinusale() {
    finestraRP();
    int j = 1;
    for (int k = 1; k < num_R; k++) {
        // if (time_R[k] - time_R[k - 1] > 00.300) { //non supera i 300ms quindi fa più di 100 battiti/min
        if (window_PR > 100) {
            printf("%d tachicardia sinusale  \n", j);
            return;
        } else {printf("%d nessuna tachicardia sinusale  \n", j);
            return;}
    }

}

//-----------------------------------------------------
//        DECESSO 2
//-----------------------------------------------------
void decesso() {
    int count = 0;
    int j = 1;
    for (int i = 0; i < DIM_DATI; i++) {
        if (fp!=0 && vett_y[i] == 0) {
            count++;

            if (count > 3000) {

                printf(" %d decesso  \n", j);
                return;
            }

        }
    }
}