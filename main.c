#include <allegro.h>
#include <wfdb/wfdb.h>
#include "utils.h"
#include <wfdb/ecgmap.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    // int x = 400;
    //int y = 300;
    static int c = 200;
    static int d = 20;
    FILE *fp; //STEP1: creo un puntatore a file
    fp = fopen("samples(2).csv", "r"); //STEP2: cerchiamo di aprire il nostro file in modalità read
    if (fp == NULL) {
        printf("trouble reading file! \nProgram Terminating..");   //STEP3:check se il file non viene aperto
        exit(0);                                                   //questi comandi sono possibili grazie all'inclusione della stdlib
    }
    inizilizzazione_grafica();
    //STEP4: leggo nel filed
    int x_i;
    int y_i;
    int x_f;
    int y_f;
    float t;
    float s;
    char *sp;
    char lines[100]; // in questo array di caratteri inserisco una riga del file
      while (!key[KEY_ESC]) {
    while (fgets(lines, 100, fp) != NULL) {
        //prendiamo una riga del file
        sp = strtok(lines, ","); //tronchiamo la riga ad ogni virgola che incontriamo

        t = atof(sp);
        t=t*150;
        x_f= (int)t;
        sp = strtok(NULL, ",");
        s = atof(sp);
        s=s*(-200);
        y_f=(int)s;
        printf("\n%d %d", x_f, y_f);
        line(buffer_screen,  x_i, 300+y_i,x_f, 300+y_f, WHITE); //200 è l'offset di partenza per buttar giù tutto il grafico
           // rectfill(buffer_screen, x_off, y_off,x2, y2, GREEN);
           x_i= x_f;
            y_i =y_f;
        usleep(4000);
        }
    }
    fclose(fp); //STEP5: chiusura file

}

/* char string [29];
     float ore= 3.14159;
 sprintf(string, "ore = %5.2f", ore);
// textout_ex(buffer_screen,font,&string,500, 600,GREEN,GND);
 int x = 400;
 int y = 300;
 static int c = 200;
 static int d = 20;

 inizilizzazione_grafica(); //funzione di iniziallizzazione di allegro e grafica
 while (!key[KEY_ESC]) {
     rectfill(buffer_screen, x, y, x + d, y + d, GREEN);
     textout_ex(buffer_screen, font, string, 200, 250, GREEN, GND);
 }

 close_all_task();
 allegro_exit();
 return 0;}*/





