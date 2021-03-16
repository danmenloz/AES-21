#include<iostream>
#include<unistd.h>
#include"GPIO.h"
using namespace exploringRPi;
using namespace std;

GPIO *outGPIO, *inGPIO;           // global pointers

int activateLED(int var){
   outGPIO->streamWrite(HIGH);    // turn on the LED
   outGPIO->streamWrite(LOW);     // turn off the LED
   return 0;
}

int main(){
   inGPIO = new GPIO(27);         // the button
   outGPIO = new GPIO(17);        // the LED
   inGPIO->setDirection(INPUT);   // the button is an input
   outGPIO->setDirection(OUTPUT); // the LED is an output
   outGPIO->streamOpen();         // use fast write to LED
   outGPIO->streamWrite(LOW);     // turn the LED off
   inGPIO->setEdgeType(RISING);   // wait for rising edge
   inGPIO->waitForEdge(&activateLED); // pass the callback function

   while(1){
      usleep(1000000);            // sleep for 1 second
      cout << "[sec]" << flush;   // display a message
   }

   outGPIO->streamWrite(LOW);     // turn off the LED after 10 seconds
   outGPIO->streamClose();        // shutdown the stream
   return 0;
}
