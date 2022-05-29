#include "Task.h"
#include "string.h"
#include "utils.h"
#include "Patologie.h"

//-----------------------------------------------------
// GLOBAL VARIABLES
//-----------------------------------------------------
extern FILE *fp;
extern int x_i;
extern int y_i;
extern int x_f;
extern int y_f;
extern int task_signals;
extern float vett_x[];
extern float vett_y[];
extern FONT *fontECG;
pthread_mutex_t mut1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mut2 = PTHREAD_COND_INITIALIZER;
//-----------------------------------------------------
// DEFINIZIONE NUOVI TASK
//-----------------------------------------------------

/**
 * aim of function task is to read the ECG file
 * connect to the button pressed
 */
void *task_ecg(struct parametri_task *arg) {
    int index;
    index = arg->index;

    float t_draw;
    float s_draw;
    float t;
    float s;
    char *sp;
    int esci;
    int i = 0;
    char lines[100];  // in questo array di caratteri inserisco una riga del file
    while (!key[KEY_ESC]) //!!se premo esc esce da qui e quinsi start task torna l'id
    {
        char str[20];
        if (choose_ecg()) {
            clear(buffer_screen);
            x_i = 0;
            y_i = 0;

           /* FONT *font1 = load_font("CG44.PCX", NULL, NULL);
            sprintf(str, "ECG");
            textout_ex(buffer_screen, font1, "ECG", 500, 100, WHITE, GND);*/
        }
        set_period(index);
        while (fp != NULL && fgets(lines, 100, fp) != NULL) {
            //prendiamo una riga del file
            if (key[KEY_ALT]) {

                clear_to_color(buffer_screen, GND);
                clear(screen_ecg);
                blit(screen_base, buffer_screen,0,0,0,0, screen_base->w, screen_base->h);
                fp = 0;
                break;
            }
            sp = strtok(lines, ",");
            t = atof(sp);
            t_draw = t * 150;
            x_f = (int) t_draw;

            pthread_mutex_lock(&mut1);
            vett_x[i] = t; //questo vettore mi servirà per il check sulle patologie
            pthread_mutex_unlock(&mut1);

            sp = strtok(NULL, ",");
            s = atof(sp);
            s_draw = s * (-100);
            pthread_mutex_lock(&mut2);
            vett_y[i] = s;
            pthread_mutex_unlock(&mut2);

            i++;
            y_f = (int) s_draw;
            printf("\n%d %d", x_f, y_f);
            line(screen_ecg, x_i, 400 + y_i, x_f, 400 + y_f,
                 WHITE); //200 è l'offset di partenza per buttar giù tutto il grafico
                 textout_ex(screen_ecg,font_medio,"Frequenza dei picchi R:", 5,800,WHITE,GND);
            x_i = x_f;
            y_i = y_f;
            blit(screen_ecg, buffer_screen,0,0,0,0, screen_ecg->w, screen_ecg->h);
            wait_for_period(index);
        }

    }
    return 0;
}

void *task_diagnosi(struct parametri_task *arg) {
    int index;
    index = arg->index;
    set_period(index);
    while (!task_signals) {

        picco_R();
       // picco_P();
       // fibr_atriale();
        //decesso();
        //tachicardia_sinusale();
        aritmia();
        //textout_ex(screen_ecg,font_medio,"Frequenza dei picchi R:", 5,800,WHITE,GND);

        wait_for_period(index);
    }
}