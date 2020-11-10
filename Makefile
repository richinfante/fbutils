CC=gcc
CFLAGS=-g -std=c99 -Wall -Wextra `pkg-config --cflags librsvg-2.0 glib-2.0` -DDEBUG 
LFLAGS=-lm -lpng16 -ljpeg `pkg-config --libs librsvg-2.0 glib-2.0`
BINFILE=fbdemo fb-ttymode fb-clear fb-pngdraw fb-describe fb-jpgdraw fb-svgdraw

all: $(BINFILE)

%.o: %.c
		$(CC) -c $(CFLAGS) $^ -o $@

fbdemo: main.o draw.o font.o img-png.o img-jpeg.o img-svg.o
		$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

fb-ttymode: fb-ttymode.o
		$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS) 

fb-clear: fb-clearscreen.o draw.o
		$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS) 

fb-pngdraw: fb-pngdraw.o draw.o img-png.o
		$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS) 

fb-jpgdraw: fb-jpgdraw.o draw.o img-jpeg.o
		$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS)

fb-svgdraw: fb-svgdraw.o draw.o img-svg.o
		$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS) 

fb-describe: fb-describe.o draw.o
		$(CC) $(CFLAGS) $^ -o $@ $(LFLAGS) 

clean:
		rm -rf *.o $(BINFILE)

install:
	sudo cp fb-ttymode /usr/local/bin
	sudo cp fb-clear /usr/local/bin
	sudo cp fb-pngdraw /usr/local/bin
	sudo cp fb-jpgdraw /usr/local/bin
	sudo cp fb-svgdraw /usr/local/bin
	sudo cp fb-describe /usr/local/bin
