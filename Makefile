PROGRAM_NAME = sod2png
COMPILER = gcc
CCFLAGS = -Wall -O2 -g
PATHFLAGS = -I/usr/include/libxml2
LLFLAGS = -lxml2

sod2png: main.c arg.o xml_editing.o 
	${COMPILER} ${CCFLAGS} ${PATHFLAGS} ${LLFLAGS} main.c arg.o editing.o -o ${PROGRAM_NAME} 

xml_editing.o: editing.c editing.h
	cc    -c -o editing.o ${PATHFLAGS} editing.c 
	
clean: 
	rm -rf *.o sod2png
