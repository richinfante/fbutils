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
int ttyfd = -1;

// Intercept SIGINT
void sig_handler(int signo) {
    if (signo == SIGINT) {
        printf("SIGINT\n");
        runflag = 0;
    }

    // If we segfault in graphics mode, we can't get out.
    if (signo == SIGSEGV) {
        if (ttyfd == -1) {
            printf("[!] Error: could not open the tty\n");
        } else {
            ioctl(ttyfd, KDSETMODE, KD_TEXT);
        }

        printf("Segmentation Fault.\n");

        exit(1);
    }
}

int main()
{
    printf("Hello\n");

    // Intercept SIGINT so we can shut down graphics loops.
    if (signal(SIGINT, sig_handler) == SIG_ERR) {
         printf("\ncan't catch SIGINT\n");
    }

    if (signal(SIGSEGV, sig_handler) == SIG_ERR) {
        printf("\ncan't catch SIGSEGV\n");
    }

    context_t * context = context_create();
    // fontmap_t * fontmap = fontmap_default();
    printf("[+] Graphics Context: 0x%x\n", context);

    
    // Attempt to open the 
    ttyfd = open("/dev/tty1", O_RDWR);
    if (ttyfd == -1) {
      printf("[!] Error: could not open the tty\n");
    } else {
      // This line enables graphics mode on the tty.
      // If you're getting segfaults comment this out so you don't need to
      // reboot to fix the tty.
      ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    }

    
    
   
    if(context != NULL){
        image_t * jpegImage = read_jpeg_file("./nyc.jpg");
        image_t * scaledBackgroundImage = scale(jpegImage, context->width, context->height);
        
        clear_context(context);
        draw_image(0, 0, scaledBackgroundImage, context);
        draw_rect(-100, -100, 200, 200, context, 0xFF0000);
        draw_rect(context->width - 100, context->height - 100, 200, 200, context, 0xFFFF00);
        draw_rect(context->width - 100, -100, 200, 200, context, 0x00FF00);
        draw_rect(-100, context->height - 100, 200, 200, context, 0x0000FF);
        draw_rect(context->width / 2 - 200, context->height / 2 - 200, 400, 400, context, 0x00FFFF);
      
        while (runflag) {
            sleep(1);
        }

        image_free(jpegImage);
        image_free(scaledBackgroundImage);
        // fontmap_free(fontmap);
        context_release(context);
    }
    
    if (ttyfd == -1) {
      printf("[!] Error: could not open the tty\n");
    } else {
      ioctl(ttyfd, KDSETMODE, KD_TEXT);
    }

    close(ttyfd);
    
    printf("[+] Shutdown successful.\n");
    return 0;
}
 
