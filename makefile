#---------------------------------------------------
# Target file to be compiled by default
#---------------------------------------------------
MAIN = main
#---------------------------------------------------
# CC is the compiler to be used
#---------------------------------------------------
CC = gcc
#---------------------------------------------------
# CFLAGS are the options passed to the compiler
#---------------------------------------------------
CFLAGS = -Wall -lpthread -lrt -lm
#-lwfdb
#librerie dette da Buttazzo
#---------------------------------------------------
# OBJS are the object files to be linked
#---------------------------------------------------
#OBJ1 =  #qui scrivo i file che sono dipendenti (ovvero altri .c se ci sono)
OBJ2 = utils
#OBJ3 = tasks
OBJS = $(MAIN).o $(OBJ2).o
#OBJS = $(MAIN).o $(OBJ2).o $(OBJ3).o
#---------------------------------------------------
# LIBS are the external libraries to be used
#---------------------------------------------------
LIBS = `allegro-config --libs`
#`wfdb-config --libs`
#---------------------------------------------------
# Dependencies
#---------------------------------------------------
all: $(OBJS)
	$(CC) -o $(MAIN) $(OBJS) $(LIBS) $(CFLAGS) #questo comando esegue su CC (compilatore) il main e le altre dipendenze solo dopo aver eseguiti OBJS ovvero i file .o, inoltre integra la libreria allegro con LIBS e le altre introdotte come FLAGS
$(MAIN).o: $(MAIN).c
	$(CC) -c $(MAIN).c
#$(OBJ1).o: $(OBJ1).c
#	$(CC) -c $(OBJ1).c
$(OBJ2).o: $(OBJ2).c
	$(CC) -c $(OBJ2).c
#$(OBJ3).o: $(OBJ3).c
	#$(CC) -c $(OBJ3).c


#---------------------------------------------------
# Command that can be specified inline: make run
#---------------------------------------------------
run:all
	   sudo ./$(MAIN)
	#con questa istruzione su terminale faccio il run (play per IDE)
	#con il comando make da terminale faccio il build (martello per IDE)

debug: CFLAGS += -g -O0 -Wextra
debug:  clean all
	sudo gdbserver localhost:4562 $(MAIN)
#---------------------------------------------------
# Command that can be specified inline: make clean
#---------------------------------------------------
clean:
	rm -rf *.o $(MAIN)
	# se su terminal eseguo "make clean" allora faccio un clean

.PHONY: all run debug clean  #quando faccio make con uno di questi comandi chiedo che sia rieseguito tutto da capo