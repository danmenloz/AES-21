/* A Simple GPIO application
*  Written by Derek Molloy for the book "Exploring Raspberry Pi" */

/* ECE 785 Modifications:
   Infinite loop to generate bursts of pulses, alternating between
   setValue and streamWrite methods.
*/

#include<iostream>
#include<unistd.h>  // for the usleep() function
#include"GPIO.h"
using namespace exploringRPi;
using namespace std;

int main(){
   GPIO outGPIO(17);    // pin 11

   while (1) {
	   outGPIO.setDirection(OUTPUT);    // basic output example
	   for (long i=0; i<10000; i++){        // flash the LED 
	      outGPIO.setValue(HIGH);       // turn the LED on
	      outGPIO.setValue(LOW);        // turn the LED off
	   }

	   outGPIO.streamOpen();            // fast write example
	   for (long i=0; i<500000; i++){   // flash the LED
	      outGPIO.streamWrite(HIGH);    // high
	      outGPIO.streamWrite(LOW);     // immediately low, repeat
	   }
	   outGPIO.streamClose();           // close the stream
	}
   return 0;
}
