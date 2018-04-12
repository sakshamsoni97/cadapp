# Makefile for the project

## Macros
CC = g++
CC2 = gcc
FLAGS = -std=c++11
CFLAGS = `pkg-config gtk+-3.0 --cflags` `freetype-config --cflags` -export-dynamic
LIBS = -lGL -lGLU -lglut -lm
CLIBS = `pkg-config gtk+-3.0 --libs` -lz -lfreetype
DEPS = drawing.h


all: drawing.o GUI.o cinterface.o
	${CC} $(FLAGS) -rdynamic -o CADAPP $^ $(LIBS) $(CLIBS) -I ./

%.o: %.cpp $(DEPS)
	${CC} ${FLAGS} -c $< ${LIBS} -I ./ -I ../lib/ -w

%.o: %.c
	${CC2} ${CFLAGS} -c $< ${CLIBS} -I ./ -I ../lib/ -w

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf Binary
