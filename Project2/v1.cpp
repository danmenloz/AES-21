/* ECE 785 Project 02
*  Main blocks with busy-wait */

#include<iostream>
#include<unistd.h>  // for the usleep() function
#include"GPIO.h"
using namespace exploringRPi;
using namespace std;

int main(){
   GPIO outGPIO(17), inGPIO(27);    // pin 11 and pin 13

   inGPIO.setDirection(INPUT);      // input
   outGPIO.setDirection(OUTPUT);    // output
   outGPIO.streamOpen();          //fast write, ready file
   outGPIO.streamWrite(LOW);      //turn the LED off

   while(1){
       // Wait for rising edge
       while(inGPIO.getValue() != 1){
       }
       outGPIO.streamWrite(HIGH);    // high
       outGPIO.streamWrite(LOW);     // immediately low, repeat
       // Wait for falling edge
       while(inGPIO.getValue() != 0){
       }
   }

   outGPIO.streamClose();         //close the output stream
   return 0;
}
