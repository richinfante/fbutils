CC=gcc
CCFLAGS=-g -std=c99 -Wall -DDEBUG
LFLAGS=-lm -lpng -ljpeg
BINFILE=fbdemo

all: fbdemo

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

fbdemo: main.o draw.o img-png.o img-jpeg.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

clean: 
	rm -rf *.o $(BINFILE)
