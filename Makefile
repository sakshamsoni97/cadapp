# Makefile for the project

## Macros
CC = g++


all: drawing.o GUI.o cadapp.o
        ${CC} -o Binary drawing.o GUI.o cadapp.o
 
main.o: cadapp.cpp
        ${CC} -c cadapp.cpp -I ./
 
GUI.o: GUI.cpp
        ${CC} -c GUI.cpp -I ./

drawing.o: drawing.cpp
        ${CC} -c drawing.cpp -I ./
 
clean:
        rm -rf *.o
        rm -rf Binary