#include <allegro.h>
#include <wfdb/wfdb.h>
#include "utils.h"
#include <wfdb/ecgmap.h>
#include <stdio.h>
#include <stdlib.h>
#include "Task.h"


#define WINDOW_PR_MAX 300
 extern float P;
extern volatile int num_R;
void finestraRP();
void picco_R(); //uesta funzione mi registra i picchi R eil relativo tempo
void picco_P();
void aritmia();
void fibr_atriale();
void tachicardia_sinusale();
void decesso();
