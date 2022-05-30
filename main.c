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
volatile int num_R = 0;
volatile int num_R_abs =0;
 volatile float P=0;
int indice_R[DIM_DATI];
int window_PR;




int main() {

pthread_t id_ecg;
    inizilizzazione_grafica();

    id_ecg=function__start_task(task_ecg, 15, 15, 2, TASK_ECG_INDEX);
    function__start_task(task_diagnosi, 80, 80, 2, TASK_PATOLOGIE_INDEX);

    pthread_join(id_ecg, NULL);


    allegro_exit();
    return 0;
}




