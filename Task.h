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
// HELLO TASK CONSTANTS
//-----------------------------------------------------
pthread_mutex_t mut1;
extern float vett_R[DIM_DATI];
extern float time_R[DIM_DATI];

extern int indice_R[DIM_DATI];
int abilita_diagnosi;
//-----------------------------------------------------

void *task_ecg();

void *task_diagnosi();
//void *task_base(void *arg);

#endif