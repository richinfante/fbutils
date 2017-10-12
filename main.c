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

#include "font.h"
#include "draw.h"
#include "img-png.h"
#include "img-jpeg.h"

int runflag = 1;

// Intercept SIGINT
void sig_handler(int signo) {
    if (signo == SIGINT) {
        runflag = 0;
    }
}

int main()
{
    // Intercept SIGINT so we can shut down graphics loops.
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
         printf("\ncan't catch SIGINT\n");
    }

    // Attempt to open the 
    int ttyfd = open("/dev/tty1", O_RDWR);
    if (ttyfd == -1) {
      printf("Error: could not open the tty\n");
    } else {
      ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    }

    context_t * context = context_create();
    // fontmap_t * fontmap = fontmap_default();

    if(context != NULL){
        image_t * jpegImage = read_jpeg_file("./nyc.jpg");
        image_t * scaledBackgroundImage = scale(jpegImage, context->width, context->height);
        
        clear_context(context);
        draw_image(0, 0, scaledBackgroundImage, context);

        sleep(10);
        image_free(jpegImage);
        image_free(scaledBackgroundImage);
        // fontmap_free(fontmap);
        context_release(context);
    }

    if (ttyfd == -1) {
      printf("Error: could not open the tty\n");
    } else {
      ioctl(ttyfd, KDSETMODE, KD_TEXT);
    }

    close(ttyfd);
    
    printf("shutdown successful.\n\n");
    return 0;
}
 