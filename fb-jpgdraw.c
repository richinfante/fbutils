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
#include "img-jpeg.h"

int main(int argc, char* argv[]) {
    context_t * context = context_create();
   
    if(context != NULL){
        char* file = NULL;
        int width = context->width;
        int height = context->width;
        int x = 0;
        int y = 0;
        int grayscale = 0;
        int grayscale_threshold = HUE_THRESHOLD_DEFAULT;
        int invert = 0;
        int hue_shift = 0;
        int hue_mask = MASK_NONE;

        for (int i = 0; i < argc; i++) {
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

            if (strcmp(argv[i], "-hue-threshold") == 0 && i+1 < argc) {
                grayscale_threshold = atoi(argv[i+1]);
            }

            if (strcmp(argv[i], "-grayscale") == 0) {
                grayscale = 1;
            }

            if (strcmp(argv[i], "-invert") == 0) {
                invert = 1;
            }

            if (strcmp(argv[i], "-hue") == 0 && i+1 < argc) {

                // enable hue shifting
                hue_shift = 1;

                // iterate over valid types of hues
                for (int j = 0; j < strlen(argv[i+1]); j++) {
                
                    // Mask types (rgbcymw)
                    if (argv[i+1][j] == 'r') {
                        hue_mask = hue_mask | MASK_RED;
                    } else if (argv[i+1][j] == 'g') {
                        hue_mask = hue_mask | MASK_GREEN;
                    } else if (argv[i+1][j] == 'b') {
                        hue_mask = hue_mask | MASK_BLUE;
                    } else if (argv[i+1][j] == 'c') {
                        hue_mask = hue_mask | MASK_CYAN;
                    } else if (argv[i+1][j] == 'y') {
                        hue_mask = hue_mask | MASK_YELLOW;
                    } else if (argv[i+1][j] == 'm') {
                        hue_mask = hue_mask | MASK_MAGENTA;
                    } else if (argv[i+1][j] == 'w') {
                        hue_mask = hue_mask | MASK_WHITE;
                    }
                }
            }
        }

        if (file != NULL) {
            image_t * jpeg_image = read_jpeg_file(file);

            // Image invert filter
            if (invert == 1) {
                invert_image(jpeg_image);
            }

            // perform grayscale
            if (grayscale == 1) {
                grayscale_image(jpeg_image);
            }

            // perform hue shift
            if (hue_shift == 1) {
                hueify_image(jpeg_image, hue_mask, grayscale_threshold);
            }

            image_t * scaled_image = scale(jpeg_image, width, height);
        
            draw_image(x, y, scaled_image, context);
        } else {
            printf("could not open file\n");
        }
        context_release(context);
    }
    
    return 0;
}
 
