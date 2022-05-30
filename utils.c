/**
* ---------------------------------------------------------------------------
 * General popruse utility,...
 * --------------------------------------------------------------------------
*/
#include "utils.h"
#include <stddef.h>
#include "allegro.h"
#include <stdio.h>
#include <stdlib.h>
#include "Task.h"


//-----------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------
int task_signals = 0;

//-----------------------------------------------------
// TIME HANDLING FUNCTIONS
//-----------------------------------------------------

/**
 * Keeping the time of task into another variables
 * @param td destination time variable
 * @param ts source time variable
 */
void keep_time(struct timespec *td, struct timespec ts) {
    td->tv_sec = ts.tv_sec;
    td->tv_nsec = ts.tv_nsec;
}

/**
 * Add fixed ms to a given time
 * @param t time that will be incremented
 * @param ms  time in ms to add to given time
 */
void time_add_ms(struct timespec *t, int ms) {
    t->tv_sec += ms / 1000;
    t->tv_nsec += (ms % 1000) * 1000000;
    if (t->tv_nsec > 1000000000) {
        t->tv_nsec -= 1000000000;
        t->tv_sec += 1;
    }
}
//-----------------------------------------------------
// TASK HANDLING FUNCTIONS
//-----------------------------------------------------

/**
* Computes time of next activation and
* abs deadline of a task identified by his index
*/
void set_period(int index) {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    keep_time(&(pt[index].at), time);
    keep_time(&(pt[index].dl), time);
    time_add_ms(&(pt[index].at), pt[index].period);
    time_add_ms(&(pt[index].dl), pt[index].deadline);
}

/**
 * Suspends the colling task until the next activation
 * and when awaken, updates activation time and deadline
 */
void wait_for_period(int index) {
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &(pt[index].at), NULL);
    time_add_ms(&(pt[index].at), pt[index].period);
    time_add_ms(&(pt[index].dl), pt[index].period);
}

/**
 *passed an pointer to void function
 * and return an int value that is index of function task
 * @param arg is a casted void task_param structure
 */
int get_task_index(struct parametri_task *arg) {
    //struct parametri_task *tp;
    // arg = (struct parametri_task *) arg;
    return arg->index;
}


/** Create a new FIFO syncronous task with specifics attributes and function
 * @param task_fun function that task will execute
 * @param period  repetition time, ms
 * @param deadline max relative time from start to end of task
 * @return  index of next  task
 */
pthread_t function__start_task(void *task_fun, int period, int deadline, int priority, int n_task) {
    struct sched_param sched_par;
    pt[n_task].index = n_task;  //pt è una variabile globale di tipo parametri_task
    pt[n_task].period = period;
    pt[n_task].deadline = deadline;
    pt[n_task].priority = priority;
    pt[n_task].dmiss = 0;
    pthread_attr_init(&att[n_task]);
    pthread_attr_setinheritsched(&att[n_task], PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&att[n_task], SCHED_FIFO);
    sched_par.sched_priority = pt[n_task].priority;
    pthread_attr_setschedparam(&att[n_task], &sched_par);
    pthread_create(&tid[n_task], &att[n_task], task_fun, &pt[n_task]);


// NULL riempie il campo che definisce l'argomento da passare alla funzione
//chiamata task_fun, questa è la funzione che definisce ciò che fa il thread
// questa funzione generica crea un nuovo thread con i parametri settati in att
    //è il numero assoccito ad ogni task e quindi il numero associato ad ogni task dipende dall'ordine in cui li chiamo
    return tid[n_task];
}

/**
 * Bloking function that waits all given thread to terminate,
 * then print num of deadline miss for each task
 */
void close_all_task() {
    task_signals = 1;
}

/**
 *check if a key is pressed return 1
 * and give to the pointer fp the file.csv connected with key pressed
 */
char choose_ecg() {

    if (key[KEY_1]) {
        printf("You pressed 'ECGstd'\n");
        fp = fopen("ECGstd.csv", "r"); //cerchiamo di aprire il nostro file in modalità read
        return 1;
    }
    if (key[KEY_2]) {
        printf("You pressed 'ECGpiatto'\n");
        fp = fopen("ECGpiatto.csv", "r");
        return 1;
    }
    if (key[KEY_3]) {
        printf("You pressed 'fibr_atriale'\n");
        fp = fopen("fibr_atriale.csv", "r");
        return 1;
    }
    if (key[KEY_4]) {
        printf("You pressed 'tachicardia'\n");
        fp = fopen("aritmia.csv", "r");
        return 1;
    }
    if (key[KEY_5]) {
        printf("You pressed 'tachicardia_sinusale'\n");
        fp = fopen("tachicardia_sinusale.csv", "r");
        return 1;
    }
    if (key[KEY_ESC]) {
        printf("You pressed 'tachicardia_sinusale'\n");
        allegro_exit();
        return 0;
    }
    return 0;
}

//-----------------------------------------------------
// GENERAL FUNCTIONS
//-----------------------------------------------------

/** Create a three new Font:
 * font_titolo :one for the title
 * font_medio: one medium
 * font_piccolo: one for small character

 */
void fonts() {

    font_titolo = load_font("CG44.PCX", NULL, NULL);
    font_medio = load_font("CG26.PCX", NULL, NULL);
    font_piccolo = load_font("CG20.PCX", NULL, NULL);
}

/** empty function for vector of int
 * @param dimensioni_vett: dimention of vector
 * @param *vett: pointer to vector
 */

void svuota_vett_int(int dimensioni_vett, int *vett) {
    for (int i = 0; i < dimensioni_vett; i++) {
        vett[i] = 0;
    }
}


/** empty function for vector of float
 * @param dimensioni_vett: dimention of vector
 * @param *vett: pointer to vector
 */
void svuota_vett_float(int dimensioni_vett, float *vett) {
    for (int i = 0; i < dimensioni_vett; i++) {
        vett[i] = 0;
    }
}

/**
 * Base commands to start Allegro, with a task that will refresh screen
 */
void inizilizzazione_grafica() {
    allegro_init();
    set_color_depth(C_DEPTH);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, IN_WIDTH, IN_HEIGHT, 0, 0);

    install_keyboard();

    buffer_screen = create_bitmap(SCREEN_W, SCREEN_H);
    screen_base = create_bitmap(SCREEN_W, SCREEN_H);
    screen_ecg = create_bitmap(SCREEN_W, SCREEN_H);
    clear(screen_base);
    clear(screen_ecg);
    clear_to_color(buffer_screen, WHITE);

    fonts();
    grafica_statica();
    function__start_task(task_refresh_grafica, 40, 40, 1, TASK_GRAFIC_INDEX);


}


//-----------------------------------------------------
// BITMAP FUNCTIONS
//-----------------------------------------------------


/** Draw the initial screen with instruction
 * in the screen_base bitmap
*/
void grafica_statica() {

    textout_ex(screen_base, font_titolo, "ECG", (IN_WIDTH / 2) - 50, 5, RED, GND);
    textout_ex(screen_base, font_titolo, " Il seguente programma mostra:", (IN_WIDTH / 2) - 250, (IN_HEIGHT / 2) - 200,
               BLU, GND);
    textout_ex(screen_base, font_titolo, "- ECG di diversi pazienti", (IN_WIDTH / 2) - 250, (IN_HEIGHT / 2) - 150,
               WHITE, GND);
    textout_ex(screen_base, font_titolo, "- anomalie patologiche ", (IN_WIDTH / 2) - 250, (IN_HEIGHT / 2) - 100, WHITE,
               GND);

    textout_ex(screen_base, font_titolo, " ISTRUZIONI:", 10, 600, BLU, GND);
    textout_ex(screen_base, font_medio, " 1 : primo paziente", 10, 680, WHITE, GND);
    textout_ex(screen_base, font_medio, " 2 : secondo paziente", 10, 720, WHITE, GND);
    textout_ex(screen_base, font_medio, " 3 : terzo paziente", 10, 760, WHITE, GND);
    textout_ex(screen_base, font_medio, " 4 : quarto paziente", 10, 800, WHITE, GND);
    textout_ex(screen_base, font_medio, " 5 : quinto paziente", 10, 840, WHITE, GND);
    textout_ex(screen_base, font_medio, " ESC : chiudi applicazione", 10, 880, WHITE, GND);
    textout_ex(screen_base, font_medio, " ALT : torna alla pagina iniziale", 10, 920, WHITE, GND);


}


/** Draw the dynamic screen of ECG and check up
 * in the screen_ecg bitmap
*/
void grafica_dinamica() {
    line(screen_ecg, x_i, 400 + y_i, x_f, 400 + y_f,
         WHITE); //200 è l'offset di partenza per buttar giù tutto il grafico
    textout_ex(screen_ecg, font_titolo, "ECG", (IN_WIDTH / 2) - 50, 5, RED, GND);
    textout_ex(screen_ecg, font_medio, "Diagnosi :", 5, 650, WHITE, GND);
    textout_ex(screen_ecg, font_medio, "Dati del paziente", 1500, 650, WHITE, GND);
    textout_ex(screen_ecg, font_medio, " Frequenza battito cardiaco", 5, 700, WHITE, GND);
    textout_ex(screen_ecg, font_medio, " Fibirllazione atriale", 5, 750, WHITE, GND);
    textout_ex(screen_ecg, font_medio, " Aritmia sinusale", 5, 800, WHITE, GND);


}
