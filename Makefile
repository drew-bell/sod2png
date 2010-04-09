PROGRAM_NAME = sod2png
COMPILER = gcc
CCFLAGS = -Wall -g
PATHFLAGS = -I/usr/include/libxml2 -I/usr/local/include/cairo
LLFLAGS = -lxml2 -lcairo -lsvg-cairo

sod2png: main.o arg.o editing.o pngout.o
	${COMPILER} ${CCFLAGS} ${PATHFLAGS} ${LLFLAGS} -o ${PROGRAM_NAME} main.o arg.o editing.o pngout.o

pngout.o: pngout.c pngout.h
	gcc    -c ${PATHFLAGS} pngout.c

editing.o: editing.c editing.h
	gcc    -c ${PATHFLAGS} editing.c

arg.o: arg.c arg.h
	gcc    -c arg.c

main.o: main.c arg.h editing.h 
	gcc    -c ${PATHFLAGS} main.c 	

clean: 
	rm -rf *.o sod2png
