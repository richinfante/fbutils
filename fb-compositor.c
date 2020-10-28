#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <string.h>
#include <termios.h>
#include <signal.h>
#include <unistd.h>
#include <math.h>

#include <linux/input.h>
#include <linux/fb.h>
#include <linux/vt.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/kd.h>

#include "draw.h"
#include "img-png.h"

// constants for the arg parser state machine
#define PARSE_AWAITING_MODE 0
#define PARSE_MODE_PNG 1
#define PARSE_MODE_JPG 2
#define PARSE_MODE_CLEAR 3


int main(int argc, char* argv[]) {
    // Create context mapped onto the buffer
    context_t * context = context_create();

    // Write to this temporary context first, then to actually display, we'll do context_copy(temp, actual)
    context_t * working_context = context_temporary(context->width, context->height);

    if (context != NULL) {
        int parse_mode = PARSE_AWAITING_MODE;

        for (int i = 0; i < argc; i++) {


        if (parse_mode == PARSE_AWAITING_MODE) {
          if (strcmp(argv[i], "drawpng") == 0) {
            parse_mode = PARSE_MODE_PNG;
          } else if (strcmp(argv[i], "drawjpg") == 0) {
            parse_mode = PARSE_MODE_JPG;
          } else if (strcmp(argv[i], "clear") == 0) {
            parse_mode = PARSE_MODE_CLEAR;
          }
        }


        if (strcmp(argv[i], ""))

        char* file = NULL;
        int width = context->width;
        int height = context->width;
        int x = 0;
        int y = 0;
        int invert = 0;

        for (i; i < argc; i++) {
            if (strcmp(argv[i], "-f") == 0 && i+1 < argc) {
                file = argv[i+1];
            }

            if (strcmp(argv[i], "-w") == 0 && i+1 < argc) {
                width = atoi(argv[i+1]);
            }

            if (strcmp(argv[i], "-h") == 0 && i+1 < argc) {
                height = atoi(argv[i+1]);
            }

            if (strcmp(argv[i], "-x") == 0 && i+1 < argc) {
                x = atoi(argv[i+1]);
            }

            if (strcmp(argv[i], "-y") == 0 && i+1 < argc) {
                y = atoi(argv[i+1]);
            }

            if (strcmp(argv[i], "-invert") == 0) {
                invert = 1;
            }
        }

        if (file != NULL) {
            image_t * png_image = read_png_file(file);

            if (invert == 1) {
                invert_image(png_image);
            }
            
            image_t * scaled_image = scale(png_image, width, height);
        
            draw_image(x, y, scaled_image, context);
        } else {
            printf("could not open file\n");
        }
        context_release(context);
    }
    
    return 0;
}
 
