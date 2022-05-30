#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <pthread.h>
#include <allegro.h>
#include "utils.h"
#include <stdlib.h>
#include <wfdb/wfdb.h>
#include <wfdb/ecgmap.h>


//-----------------------------------------------------
//  GLOBAL VARIABLES
//-----------------------------------------------------
pthread_mutex_t mut1;
extern FILE *fp;
extern int x_i;
extern int y_i;
extern int x_f;
extern int y_f;
extern float vett_x[];
extern float vett_y[];
extern float vett_R[DIM_DATI];
extern float time_R[DIM_DATI];
extern int indice_R[DIM_DATI];
volatile int abilita_diagnosi;

extern  int num_tachicardia;
extern int num_fibr_atriali;
extern int num_aritmia_sinusale;


volatile int check_end_file;
//-----------------------------------------------------
//  TASK FUNCTION
//-----------------------------------------------------
void *task_refresh_grafica(struct parametri_task *arg);

void *task_ecg();

void *task_diagnosi();

void *task_lettura_file();


#endif