#include "Task.h"
#include "string.h"
#include "utils.h"
#include "Patologie.h"

//-----------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------

pthread_mutex_t mut1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_COND_INITIALIZER;
int num_tachicardia = 0;
int num_fibr_atriali = 0;
int num_aritmia_sinusale = 0;

//-----------------------------------------------------
//  TASK FUNCTION
//-----------------------------------------------------


/**
 * aim of function task is to refresh screen and
 * transferring
 */
void *task_refresh_grafica(struct parametri_task *arg) {
    int index;
    index = arg->index;
    char str[50];

    set_period(index);


    blit(screen_base, buffer_screen, 0, 0, 0, 0, screen_base->w, screen_base->h);

    while (!task_signals) {

        blit(buffer_screen, screen, 0, 0, 0, 0, buffer_screen->w, buffer_screen->h);
        wait_for_period(index);
    }
}

/**
 * aim of function task is to read the ECG file connect
 *  to the button pressed and write the data in two vector
 * one for time and one for the signal in mV
 */

void *task_lettura_file(struct parametri_task *arg) {
    int index;
    index = arg->index;
    float t;
    float s;
    char *sp;
    int i = 0;
    char lines[100];


    while (!task_signals) {

        if (choose_ecg()) {  //selezia il file da cui leggere

            svuota_vett_float(DIM_DATI, vett_y);
            svuota_vett_float(DIM_DATI, vett_x);
            clear_to_color(screen, GND);
            abilita_diagnosi = 0;

        }
        set_period(index);
        while (fp != NULL && fgets(lines, 100, fp) != NULL) {
            sp = strtok(lines, ",");
            t = atof(sp);
            pthread_mutex_lock(&mut1);
            vett_x[i] = t;          // sarà utilizzato per il check sulle patologie
            pthread_mutex_unlock(&mut1);
            printf("\n  %f %d\n", t, i);
            sp = strtok(NULL, ",");
            s = atof(sp);

            pthread_mutex_lock(&mut2);
            vett_y[i] = s;      //
            pthread_mutex_unlock(&mut2);
            printf("\n  %f\n", s);
            i++;

            // wait_for_period(index);
        }
        if (i != 0) {
            abilita_diagnosi = 1;
            i = 0;
        }
        wait_for_period(index);
    }
}

/**
 * aim of function task is to read the ECG file
 * connect to the button pressed
 */
void *task_ecg(struct parametri_task *arg) {
    int index;
    index = arg->index;
    float t_draw;
    float s_draw;
    grafica_statica();
    int i;
    char lines[100];
    while (!key[KEY_ESC])  // ESC: chiudo il task: return ID task_ecg: esco dal main
    {

        char str[20];
        set_period(index);
        while (abilita_diagnosi == 1 && i < DIM_DATI) {
            if (key[KEY_ALT]) {            // ALT: stop ecg e ritorna a screen_base
                clear(screen);
                clear(screen_ecg);
                grafica_statica();
                // blit(screen_base, buffer_screen, 0, 0, 0, 0, screen_base->w, screen_base->h);
                fp = NULL;
                num_R = 0;
                P = 0;
                x_i = 0;
                y_i = 0;
                x_f = 0;
                y_f = 0;
                abilita_diagnosi = 0;
                svuota_vett_float(DIM_DATI, vett_R);
                svuota_vett_float(DIM_DATI, time_R);
                svuota_vett_int(DIM_DATI, indice_R);
                i = 0;
                svuota_vett_float(DIM_DATI, vett_y);
                svuota_vett_float(DIM_DATI, vett_x);

                break;
            }

            pthread_mutex_lock(&mut1);
            t_draw = vett_x[i] * 150; //[s]: ascissa dell'ECG
            pthread_mutex_unlock(&mut1);
            x_f = (int) t_draw;

            pthread_mutex_lock(&mut2);
            s_draw = vett_y[i] * (-100); //[mV]: ordinata dell'ECG
            pthread_mutex_unlock(&mut2);
            y_f = (int) s_draw;
            line(screen, x_i, 400 + y_i, x_f, 400 + y_f, WHITE);
            // printf("\n%d %d", x_f, y_f);
            grafica_dinamica();
            printf("\n  vettori iniziali %d %d\n", x_i, y_i);
            // blit(screen_ecg, screen, 0, 0, 0, 0, screen_ecg->w, screen_ecg->h);
            x_i = x_f;
            y_i = y_f;
            i++;
            printf("\n  indice vettori  %d\n", i);

            wait_for_period(index);
        }
        abilita_diagnosi = 0;


    }
    return 0;
}

/**
 * aim of function task is to check up
 * from ECG's data
 */
void *task_diagnosi(struct parametri_task *arg) {

    int index;
    index = arg->index;
    set_period(index);

    while (!task_signals) {

        if (!abilita_diagnosi) {

            picco_R();
            picco_P();

            fibr_atriale();
            tachicardia_sinusale();
            aritmia();
            // decesso();
        } //check sulla deadline miss, visualizzare a schermo le deadline miss di tutti i task
        wait_for_period(index);
        num_tachicardia = 0;
        num_fibr_atriali = 0;
        num_aritmia_sinusale = 0;

    }

}