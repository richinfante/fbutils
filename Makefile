CC=gcc
CCFLAGS=-g -std=c99 -Wall
LFLAGS=-lm -lpng -ljpeg
BINFILE=fbdemo

all: fbdemo

fbdemo: 
	$(CC) $(CFLAGS) *.c $(LFLAGS) -o $(BINFILE)

clean: 
	rm -rf *.o fbdemo