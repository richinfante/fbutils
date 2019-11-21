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

int main(int argc, char* argv[]) {
    context_t * context = context_create();
   
    if(context != NULL){
       unsigned char color = 0x0;
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "-g") == 0 && i+1 < argc) {
                color = atoi(argv[i+1]) & 0xFF;
            }
        }

        if (color == 0x0) {
          clear_context(context);
        } else {
          clear_context_gray(context, color);
        }
        context_release(context);
    }
    return 0;
}
 
