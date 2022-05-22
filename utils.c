/**
* ---------------------------------------------------------------------------
 * General popruse utility,...
 * --------------------------------------------------------------------------
*/
#include "utils.h"
#include <stddef.h>
#include "allegro.h"

//-----------------------------------------------------
// PRIVATE VARIABLES
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
// TASK HANDLING FUNCTIONS: global function
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
 * @return  index ov just created task
 */
int function__start_task(void *task_fun, int period, int deadline, int priority, struct parametri_task arg){
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
    pthread_create(&tid[n_task], &att[n_task], task_fun,&arg);


// NULL riempie il campo che definisce l'argomento da passare alla funzione
//chiamata task_fun, questa è la funzione che definisce ciò che fa il thread
// questa funzione generica crea un nuovo thread con i parametri settati in att

    return n_task;
}

/**
 * Bloking function that waits all given thread to terminate,
 * then print num of deadline miss for each task
 */
void close_all_task() {
    task_signals = 1;
    /*while (n_task > 0) {
        n_task--;
        pthread_join(tid[n_task], NULL);

    }*/
}
//-----------------------------------------------------
// ALLEGRO FUNCTIONS
//-----------------------------------------------------
/**
 * aim of function task is to refresh screen and
 * transferring 
 */
void *task_refresh_grafica(struct parametri_task *arg) { //questa è la funzione collegata al task
    int index;
    index = arg->index;
    //index = get_task_index(arg);
    //tp = (struct parametri_task *) arg;
    set_period(index);
    while (!task_signals) {
        blit(buffer_screen, screen, 0, 0, 0, 0, buffer_screen->w, buffer_screen->h);
        wait_for_period(index);
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

    //create a  buffer, where all task can paint
    buffer_screen = create_bitmap(SCREEN_W, SCREEN_H);
    clear_bitmap(buffer_screen);
    clear_to_color(buffer_screen, GND);

    //start a task in order to refresh graphic, lowest priority
    function__start_task(task_refresh_grafica, 40, 40, 1,pt[DIM]);


}

