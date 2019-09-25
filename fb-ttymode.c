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
  

  // Attempt to open the 
  int ttyfd;
  if (argc == 2) {
    printf("using tty: %s", argv[1]);
    ttyfd = open(argv[1], O_RDWR);
  } else {
    printf("using tty1");
    ttyfd = open("/dev/tty1", O_RDWR)
  }
  if (ttyfd == -1) {
    printf("[!] Error: could not open the tty\n");
  } else {
    // This line enables graphics mode on the tty.
    // If you're getting segfaults comment this out so you don't need to
    // reboot to fix the tty.
    if (strcmp(argv[0], "graphics") == 0) {
      printf("enabling graphics mode");
      ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    } else {
      printf("enabling text mode");
      ioctl(ttyfd, KDSETMODE, KD_TEXT);
    }
  }
}