//IN task.ecg HO GIA' CREATO DUE VETTORI AL CUI INTERNO
//CI SONO I VALORI DI TENSIONE E QUELLI IN SECONDI OPPORTUNAMENTE SCALATI

#include "Patologie.h"

//-----------------------------------------------------
//  UTILS FUNCTION
//-----------------------------------------------------
void finestraRP() {
    float t_camp;
    t_camp = vett_x[1] - vett_x[0];
    t_camp = t_camp * 1000;
    window_PR = (int) (WINDOW_PR_MAX /
                       t_camp); //fornisce il numero di passi da sottrarre all'indice del picco R per andare a rilevare se c'è un picco P

}

//-----------------------------------------------------
//  DIAGNOSIS FUNCTION
//-----------------------------------------------------


/*
 * CHECK THE R VALUE:
 *  absolute max value of ECG
 */

void picco_R() {

    int i = -1;
    float R = 1;
    char str[30];
    svuota_vett_float(DIM_DATI, vett_R);
    svuota_vett_float(DIM_DATI, time_R);
    svuota_vett_int(DIM_DATI, indice_R);
    num_R = 0;


    for (int k = 1; k < DIM_DATI; k++) {
        if (vett_y[k] > vett_y[k - 1] &&
            vett_y[k] > R) { //questo if mi serve per verificare se il valore successivo è maggiore del
            //precedente e se sto su un picco R
            if (i == -1 ||
                (vett_x[k] - time_R[i] > 00.05 &&
                 (k - indice_R[i] > 2))) { //la prima condizione serve a poter scrivere anche nella posizione i=o
                //la seconda mi serve a capire se sto valutando picchi tra loro lontani
                i++;                                        //se sono verificate queste condizioni allora incremento i
                num_R++;

            }
            time_R[i] = vett_x[k];                          //aggiorno i vettori che mi registrano i tempi dei picchi R
            vett_R[i] = vett_y[k];
            indice_R[i] = k;                              //salva l'indice k dei vettori costruitleggendo il file csv
            printf("\n%f %f %d\n", time_R[i], vett_R[i], indice_R[i]);

            // sprintf(str,vett_R);

        }
    }


}


/*
 * CHECK THE P VALUE:
 * local max value before R value
 */

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

/*
 * DETECT THE FREQUENCY OF R VALUE:
 * press on screen_ecg if the frequency is in the normal range or not
 * */
void aritmia() {
    int j = 0;

    printf("num_R: %d, j = %d\n", num_R, j);
    if (num_R < 2) {
        return;
    }
    for (int k = 1; k < num_R; k++) {
        if (time_R[k] - time_R[k - 1] > 1.1) { //non deve superare 2 quadettirni quindi 0,4s=> 400ms
            j++;
            printf("%d aritmia  \n", j);
        }
//        else
//            printf("%d nessuna aritmia  \n", j);
    }
    printf("end for aritmia: j = %d\n", j);
    if (j > 1)
        textout_ex(screen_ecg, font_medio, "NON REGOLARE", 350, 700, RED, GND);
    else
        textout_ex(screen_ecg, font_medio, "REGOLARE", 350, 700, GREEN, GND);
}

/*
 * DETECT THE P VALUE:
 * press on screen_ecg if the patient have am atrial fibrillation
 *
 */
void fibr_atriale() {
    int k = 1;
    if (num_R < 3) {
        return;
    }
    if (fp != NULL && P == 0) {
        printf("%d fibrillazioni atriali presente \n", k);

        textout_ex(screen_ecg, font_medio, "PRESENTE", 350, 750, RED, GND);
    } else {
        textout_ex(screen_ecg, font_medio, "ASSENTE", 350, 750, GREEN, GND);
        printf("%d fibrillazioni atriali assente \n", k);
    }
}

/*
 * DETECT BEATS/MINUTE:
 * press on screen_ecg if the patient have an sinus tachicardia
 */

void tachicardia_sinusale() {
    finestraRP();
    int j = 1;
    if (num_R < 4) {
        return;
    }
    if (window_PR > 100) {
        printf("%d tachicardia sinusale  \n", j);
        textout_ex(screen_ecg, font_medio, "PRESENTE", 350, 800, RED, GND);

    } else {
        printf("%d nessuna tachicardia sinusale  \n", j);
        textout_ex(screen_ecg, font_medio, "ASSENTE", 350, 800, GREEN, GND);


    }

}

/*
 * DETECT IF PATIENT DON'T HAVE BEATS
 *
 */
void decesso() {
    int count;


    for (int i = 0; i < DIM_DATI; i++) {

        if (num_R > 4 && vett_y[i] == 0) {
            count++;
            if (count > 4000) {

                printf(" %d decesso  \n", count);
                textout_ex(screen_ecg, font_medio, "DECESSO", 400, 930, RED, GND);
                count = 0;
            }
            printf(" %d decesso  \n", count);
        }
    }
}