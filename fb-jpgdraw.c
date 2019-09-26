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
        }

        if (file != NULL) {
            image_t * png_image = read_jpeg_file(file);
            image_t * scaled_image = scale(png_image, width, height);
        
            draw_image(x, y, scaled_image, context);
        } else {
            printf("could not open file\n");
        }
        context_release(context);
    }
    
    return 0;
}
 
