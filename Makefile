PROGRAM_NAME = sod2png
COMPILER = gcc
CCFLAGS = -Wall -g
PATHFLAGS = -I/usr/include/libxml2
LLFLAGS = -lxml2

sod2png: main.o arg.o editing.o
	${COMPILER} ${CCFLAGS} ${PATHFLAGS} ${LLFLAGS} -o ${PROGRAM_NAME} main.o arg.o editing.o

editing.o: editing.c editing.h
	gcc    -c ${PATHFLAGS} editing.c

arg.o: arg.c arg.h
	gcc    -c arg.c

main.o: main.c arg.h editing.h 
	gcc    -c ${PATHFLAGS} main.c 	

clean: 
	rm -rf *.o sod2png
