/* ECE 785 Project 02
*  Main blocks with busy-wait */

#include<iostream>
#include<unistd.h>  // for the usleep() function
#include"GPIO.h"
using namespace exploringRPi;
using namespace std;

int main(){
   GPIO outGPIO(17), inGPIO(27);    // pin 11 and pin 13

   outGPIO.setDirection(OUTPUT);    // output
   outGPIO.setValue(LOW);           // clear output

   while(1){
       // Wait for rising edge
       while(inGPIO.getValue() != 1){
       }
       outGPIO.setValue(HIGH);
       outGPIO.setValue(LOW);  
       // Wait for falling edge
       while(inGPIO.getValue() != 0){
       }
   }

   return 0;
}
