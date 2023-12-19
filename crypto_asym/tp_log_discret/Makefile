
CODEMAINDIRS=./src
CODELIBDIRS=./lib

MAINCFILES=$(foreach D, $(CODEMAINDIRS), $(wildcard $(D)/*.c)) 
LIBCFILES=$(foreach D, $(CODELIBDIRS), $(wildcard $(D)/*.c)) 

CFILES=$(MAINCFILES) $(LIBCFILES)

BINARY=$(patsubst ./src/%.c, ./bin/%, $(MAINCFILES))

INCDIRS=./include

COMPILER=gcc
OPT=-O3 
DEBUG=-Og 

DEPFLAGS=-MP -MD

CFLAGS1=-Wall -g $(foreach D, $(INCDIRS), -I$(D)) $(FLINTFLAG) $(DEBUG) $(DEPFLAGS) $(OPT)


MAINOBJECTS = $(patsubst %.c,%.o,$(MAINCFILES))
LIBOBJECTS = $(patsubst %.c,%.o,$(LIBCFILES))
OBJECTS = $(patsubst %.c,%.o,$(CFILES))


#les DEPFLAGS créent des .d pour dependencies (les .h sont pas reconnus par make car c'est spécifique au C) qui
#vont permettre de compiler les .h avec make.
DEPFILES = $(patsubst %.c,%.d,$(CFILES))




all:$(BINARY)

#.PHONY est une target représentant all, qui dit a make d'éxécuter la commande peu importe si elle est à jour ou non.
.PHONY:all


$(BINARY): ./bin/%: $(LIBOBJECTS) ./src/%.o
	$(COMPILER) -o $@ $^




%.o: %.c
	$(COMPILER) $(CFLAGS1) -c -o $@ $^

clean:
		rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

