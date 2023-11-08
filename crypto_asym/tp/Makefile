CODEMAINDIRS=./src

MAINCFILES=$(foreach D, $(CODEMAINDIRS), $(wildcard $(D)/*.c)) 

CFILES=$(MAINCFILES) $(LIBCFILES)

BINARY=$(patsubst ./src/%.c, ./bin/%, $(MAINCFILES))


#dossier ou y'a les .h
INCDIRS=. ./include/ /usr/local/include/flint
LIBRARIES=gmp flint
INCLIB=/usr/local/include/flint
FLINTFLAG= -L /usr/local/lib

COMPILER=gcc
OPT=-O3 
DEBUG=-Og 

DEPFLAGS=-MP -MD

#le loop va ajouter chaque -I... des dossiers ou chercher les .h
LIBFLAGS=$(foreach D, $(LIBRARIES), -l$(D))
CFLAGS1=-Wall -g $(foreach D, $(INCDIRS), -I$(D)) $(FLINTFLAG) $(DEBUG) $(DEPFLAGS) $(OPT)


MAINOBJECTS = $(patsubst %.c,%.o,$(MAINCFILES))
OBJECTS = $(patsubst %.c,%.o,$(CFILES))


#les DEPFLAGS créent des .d pour dependencies (les .h sont pas reconnus par make car c'est spécifique au C) qui
#vont permettre de compiler les .h avec make.
DEPFILES = $(patsubst %.c,%.d,$(CFILES))



#target: dependencies
#		action

all:$(BINARY)

binary2:$(BINARY2)
#.PHONY est une target représentant all, qui dit a make d'éxécuter la commande peu importe si elle est à jour ou non.
.PHONY:all


$(BINARY): ./bin/%: ./src/%.c
	$(COMPILER) $(foreach D, $(INCLIB), -I D) $(CFLAGS1) $(FLINTFLAG) -o $@ $^ $(LIBFLAGS)





clean:
		rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

