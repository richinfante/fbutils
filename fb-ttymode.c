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

int main(int arcg, char *argv[]) {
  char* tty = "/dev/tty1";
  if (argc == 2) {
    tty = argv[1];
  }

  printf("using tty: %c", tty);

  // Attempt to open the 
  int ttyfd = open(tty, O_RDWR);
  if (ttyfd == -1) {
    printf("[!] Error: could not open the tty\n");
  } else {
    // This line enables graphics mode on the tty.
    // If you're getting segfaults comment this out so you don't need to
    // reboot to fix the tty.
    if (strcmp(argv[0], "graphics") == 0) {
      ioctl(ttyfd, KDSETMODE, KD_GRAPHICS);
    } else {
      ioctl(ttyfd, KDSETMODE, KD_TEXT);
    }
  }
}