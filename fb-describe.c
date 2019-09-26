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

int main() {
    context_t * context = context_get_dimensions();
   
    if(context != NULL){
        printf("%s %d %d",  context->fb_name, context->width, context->height);
    } else {
        printf("Error: could not describe framebuffer.\n");
        return 1;
    }

    return 0;
}
 
