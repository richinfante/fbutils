CC=gcc
CCFLAGS=-g -std=c99 -Wall -DDEBUG
LFLAGS=-lm -lpng -ljpeg
BINFILE=fbdemo fb-ttymode fb-clear fb-pngdraw fb-describe

all: fbdemo fb-ttymode fb-clear fb-pngdraw fb-describe

%.o: %.c
		$(CC) -c $(CFLAGS) $^ -o $@

fbdemo: main.o draw.o font.o img-png.o img-jpeg.o
		$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

fb-ttymode: fb-ttymode.o
		$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

fb-clear: fb-clearscreen.o draw.o
		$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

fb-pngdraw: fb-pngdraw.o draw.o img-png.o
		$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

fb-pngdraw: fb-jpgdraw.o draw.o img-jpeg.o
		$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

fb-describe: fb-describe.o draw.o
		$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

clean:
		rm -rf *.o $(BINFILE)

install:
		sudo cp fb-ttymode /usr/local/bin
		sudo cp fb-clear /usr/local/bin
		sudo cp fb-pngdraw /usr/local/bin
		sudo cp fb-describe /usr/local/bin
