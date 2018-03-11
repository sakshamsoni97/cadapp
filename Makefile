# Makefile for the project

## Macros
CC = g++
FLAGS = -std=c++11 `pkg-config gtk+-3.0 --cflags` `freetype-config --cflags`
LIBS = -lGL -lGLU -lglut `pkg-config gtk+-3.0 --libs` -ljsoncpp -lpng -lpngwriter -lz -lfreetype


all: drawing.o GUI.o cadapp.o
        ${CC} $(FLAGS) -o Binary drawing.o GUI.o cadapp.o
 
#main.o: cadapp.cpp
#        ${CC} ${FLAGS} -c cadapp.cpp ${LIBS} -I ./
 
#GUI.o: GUI.cpp
#         ${CC} ${FLAGS} -c GUI.cpp ${LIBS} -I ./

# drawing.o: drawing.cpp
#         ${CC} ${FLAGS} -c drawing.cpp ${LIBS} -I ./ 

*.o: *.cpp
		${CC} ${FLAGS} -c $< ${LIBS} -I ./
 
clean:
        rm -rf *.o
        rm -rf Binary