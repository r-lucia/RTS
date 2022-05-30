#include <allegro.h>
#include <wfdb/wfdb.h>
#include "utils.h"
#include <wfdb/ecgmap.h>
#include <stdio.h>
#include <stdlib.h>
#include "Task.h"

//-----------------------------------------------------
//  GLOBAL VARIABLE
//-----------------------------------------------------
extern volatile float P;
extern volatile int num_R;
extern int task_signals;
extern int window_PR;

//-----------------------------------------------------
//  FISIOLOGICAL MACRO
//-----------------------------------------------------
#define WINDOW_PR_MAX 300

//-----------------------------------------------------
//  DIAGNOSIS FUNCTION
//-----------------------------------------------------
void finestraRP();

void picco_R();

void picco_P();

void aritmia();

void fibr_atriale();

void tachicardia_sinusale();

void decesso();
