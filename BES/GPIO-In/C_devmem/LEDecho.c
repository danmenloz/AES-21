#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <stdint.h>   // for uint32_t - 32-bit unsigned integer
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>	// Defines signal-handling functions (i.e. trap Ctrl-C)

// Modified version to test speed of output changes
// Can pass integer argument to set toggle delay
// Still accepts change on input pin to stop 

#define TOG_DELAY (100)
int32_t tog_delay = TOG_DELAY;

// To find GPIO base  address, cat /proc/iomem and look for .gpio entry
#define GPIO_BASE    0xFE200000   // on RPi 4
#define GPSET0       0x1c         // from Figure 6-X in BCM Peripheral Manual
#define GPCLR0       0x28
#define GPLVL0       0x34
static volatile uint32_t *gpio;   // pointer to the gpio (*int)

int run = 1;

// Signal handler callback function to gracefully exit (releasing GPIO mmap) when SIGINT (from Ctrl-C) is received
// void signal_handler(int sig);

void signal_handler(int sig)
{
  printf( "\nCtrl-C pressed, cleaning up and exiting.\n" );
  run = 0;
}

int main(int argc, char * argv []) {
  int fd;
  unsigned int level_in;
  printf("Start of GPIO memory-manipulation test program.\n");
  if(getuid()!=0) {
    printf("You must run this program as root. Exiting.\n");
    return -EPERM;
  }
  if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
    printf("Unable to open /dev/mem: %s\n", strerror(errno));
    return -EBUSY;
  }
  // get a pointer that points to the peripheral base for the GPIOs
  gpio = (uint32_t *) mmap(0, getpagesize(), PROT_READ|PROT_WRITE,
			   MAP_SHARED, fd, GPIO_BASE);

  if (((void *) gpio) == MAP_FAILED) {
    printf("Memory mapping failed: %s [%d, 0x%x]\n", strerror(errno), errno,
	   (int32_t)gpio);
    return -EBUSY;
  }

  if (argc == 2) {
    if (1 == sscanf(argv[1], "%d", &tog_delay)) {
      printf("tog_delay set to %d\n", tog_delay);
    } else {
      printf("Bad argument: %s\n", argv[1]);
    }
  }

  // Set the signal callback for Ctrl-C
  signal(SIGINT, signal_handler);

  // at this point gpio points to the GPIO peripheral base address

  // Output: GPIO 17 (header pin 11)
  // Set up the LED GPIO FSEL17 mode = 001 at addr GPFSEL1 (0004)
  // remember that adding one 32-bit value moves the addr by 4 bytes
  // writing NOT 7 (i.e., ~111) clears bits 21, 22 and 23.
  *(gpio + 1) = (*(gpio + 1) & ~(7 << 21) | (1 << 21));

  // Input: GPIO 27 (header pin 13)
  // set up the button GPIO FSEL27 mode = 000 at addr GPFSEL2 (0008)
  // both FSEL17 and FSEL27 are 21 bits in, but on different registers
  *(gpio + 2) = (*(gpio + 2) & ~(7 << 21) | (0 << 21));
  // writing the 000 is not necessary but is there for clarity

  while (run) {
    level_in = (*(gpio+(GPLVL0/4))) & (1<<27); // get bit 27
    if (level_in)
      *(gpio + (GPSET0/4)) = 1 << 17;  // Turn the LED on
    else
      *(gpio + (GPCLR0/4)) = 1 << 17;  // turn the LED off
  } 
  close(fd);
  return 0;
}
