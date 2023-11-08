BINARY = count test_list

CC = gcc
CPPFLAGS = -I .
CFLAGS = -Wall -Wpointer-arith -O3

LDFLAGS = 

OBJ = $(patsubst %.c, %.o, $(wildcard *.c))

all : $(BINARY)


count : count.o error.o
	$(CC) $(LDFLAGS) $^ -o $@
	
test_list : test_list.o list.o error.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o : %.h %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $*.c

clean :
	rm -rf $(filter-out Makefile %.c %.h, $(wildcard *))
