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


//-----------------------------------------------------

void *task_ecg();
//void *task_base(void *arg);

#endif