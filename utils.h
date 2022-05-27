#ifndef UTILS_H
#define UTILS_H


#include <allegro.h>
#include <pthread.h> // libreria inclusa per creare i thread
#include "time.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

//-----------------------------------------------------
// GRAPHICS CONSTANTS:  setto le dimensioni del display fisico e grafico
//-----------------------------------------------------
#define  IN_WIDTH 1800
#define IN_HEIGHT 1000// height of graphic window
#define CHAR_W 8 //HORIZONTAL PIXELS OCCUPIED BY A CHAR
#define CHAR_H 10 //vertical pixels occupied by a char
//-----------------------------------------------------
#define C_DEPTH 8
#define BLACK 0
#define WHITE 15
#define GREEN 10
#define GREY 8
#define RED 4
#define GND 0
#define ORANGE		42
//-----------------------------------------------------
// TASKS CONSTANTS
//-----------------------------------------------------
#define DIM    10                // max thread number
#define DIM_DATI 6000
//-----------------------------------------------------
// STRUCT
//-----------------------------------------------------
struct parametri_task {
    int index;              //task index in order to find it
    long wcet;               // us
    int period;             // ms
    int deadline;            //relative (ms)
    int priority;           // [0,99]
    int dmiss;              //??
    struct timespec at;        // ??
    struct timespec dl;        // abs deadline
};
//-----------------------------------------------------
// PUBLIC VARIABLES
//-----------------------------------------------------
//static int n_task = 0; // active task counter
extern int task_signals;
//-----------------------------------------------------
// TASK VARIABLES
//-----------------------------------------------------
pthread_t tid[DIM]; //array each element is one thread identifier
pthread_attr_t att[DIM]; //struct that contains the thread attributes
struct parametri_task pt[DIM];
//-----------------------------------------------------
// PUBLIC VARIABLES
//-----------------------------------------------------


//-----------------------------------------------------
// GRAPHIC VARIABLES
//-----------------------------------------------------
BITMAP *buffer_screen;    // double buffer integration
//-----------------------------------------------------
// TASK HANDLING FUNCTIONS
//-----------------------------------------------------
void set_period(int index);

void wait_for_period(int index);

//int get_task_index(void *arg);
int function__start_task(void *task_fun, int period, int deadline, int priority);

void close_all_task();

char choose_ecg();

//-----------------------------------------------------
//ALLEGRO FUNCTIONS
//-----------------------------------------------------
void get_keycodes(char *scan, char *ascii);

char listen_scancode();

void get_string(char *str, int x, int y, int c, int b);

void inizilizzazione_grafica(); //inizializzation funcion

void readraw_ecg();

#endif