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

int main(int argc, char *argv[]) {
  // Attempt to open the tty specified
  char* tty = "/dev/tty1";
  
  int mode = KD_TEXT;

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "--tty") == 0 && i+1 < argc) {
        tty = argv[i+1]
    }

    if (strcmp(argv[i], "--mode") == 0 && i+1 < argc) {
      if (strcmp(argv[i+1], "graphics") == 0) {
        mode = KD_GRAPHICS;
      }
    }
  }

  int ttyfd = open(tty, O_RDWR);

  if (ttyfd == -1) {
    printf("[!] Error: could not open the tty\n");
  } else {
    // This line enables graphics mode on the tty.
    // If you're getting segfaults comment this out so you don't need to
    // reboot to fix the tty.
    if (strcmp(argv[1], "graphics") == 0) {
      if(ioctl(ttyfd, KDSETMODE, KD_GRAPHICS) == -1) {
        printf("ioctl failed, are you root?\n");
      }
    } else {
      if (ioctl(ttyfd, KDSETMODE, KD_TEXT) == -1){
        printf("ioctl failed, are you root?\n");
      }
    }
  }
}
