CC=gcc
CCFLAGS=-g -std=c99 -Wall -DDEBUG
LFLAGS=-lm -lpng -ljpeg
BINFILE=fbdemo

all: fbdemo fb-ttymode

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

fbdemo: main.o draw.o font.o img-png.o img-jpeg.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

fb-ttymode: fb-ttymode.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

clean: 
	rm -rf *.o $(BINFILE)
