PROGRAM_NAME = sod2png
COMPILER = gcc
CCFLAGS = -Wall -O2

make_png: main.c arg.o
	${COMPILER} ${CCFLAGS} main.c arg.c -o ${PROGRAM_NAME}

clean: 
	rm -rf *.o sod2png
