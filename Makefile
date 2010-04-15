PROGRAM_NAME = sod2png
CC = gcc
CCFLAGS = -Wall -g
CFLAGS = -I/usr/include/libxml2 -I/usr/local/include/cairo
LDFLAGS = -lxml2 -lcairo -lsvg-cairo

all: sod2png

sod2png: src/main.o src/arg.o src/editing.o src/svg2png.o src/types.o
	${CC} ${CCFLAGS} ${CFLAGS} ${LDFLAGS} -o src/${PROGRAM_NAME} src/main.o src/arg.o src/editing.o src/svg2png.o src/types.o

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -rf src/*.o src/sod2png

