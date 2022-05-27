#include <allegro.h>
#include <wfdb/wfdb.h>
#include "utils.h"
#include <wfdb/ecgmap.h>
#include <stdio.h>
#include <stdlib.h>
#include "Task.h"
#include "Patologie.h"


FILE *fp = NULL; //STEP1: creo un puntatore a file
int x_i;
int y_i;
int x_f;
int y_f;
float vett_x[DIM_DATI];
float vett_y[DIM_DATI];
float vett_R[DIM_DATI];
float time_R[DIM_DATI];
int num_R = 0;
int indice_R[DIM_DATI];
int window_PR;



int main() {


    inizilizzazione_grafica();


    while (!key[KEY_ESC]) {
        function__start_task(task_ecg(), 8, 8, 2);


    }

    picco_R();
    picco_P();
    fibr_atriale();
    decesso();
    tachicardia_sinusale();
    aritmia();


    close_all_task();
    allegro_exit();
    return 0;
}




