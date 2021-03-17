#include <iostream>
#include <wiringPi.h>
#include <unistd.h>
using namespace std;

#define LED_GPIO      17      // this is GPIO17, Pin 11
#define BUTTON_GPIO   27      // this is GPIO27, Pin 13

// the Interrupt Service Routine (ISR) to light the LED
void lightLED(void){
   digitalWrite(LED_GPIO, HIGH);         // turn the LED on
   for(int volatile i=0;i<1000;i++){}      // make pulse wider, transition is so fast!
   digitalWrite(LED_GPIO, LOW);          // turn the LED off
}

int main() {                             // must be run as root
   wiringPiSetupGpio();                  // use the GPIO numbering
   pinMode(LED_GPIO, OUTPUT);            // the LED
   pinMode(BUTTON_GPIO, INPUT);          // the Button
   digitalWrite (LED_GPIO, LOW);         // LED is off
   static int x = 0;                     // seconds counter

   // call the lightLED() ISR on the rising edge (i.e., button press)
   wiringPiISR(BUTTON_GPIO, INT_EDGE_RISING, &lightLED);
   while(1){
       cout << "Seconds elapsed: " << x++ << endl;
       sleep(1);
   }

   return 0;                             // program ends after 10s
}
