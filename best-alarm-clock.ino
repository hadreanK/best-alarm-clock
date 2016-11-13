// https://www.arduino.cc/en/Reference/PortManipulation
// INCLUDES
    #include "best-alarm-clock.h"
    
// DEFINES
    
// Global Variables
    menuInfo myMenu;        // This contains the info for the menu position and text
    
    
    
void setup(){
    
    myMenu.x = 1;    myMenu.y = 2;
    } //setup
  
void loop() {
    
    
    } //loop  

ISR(TIMER4_COMPA_vect) { // The lower the Interrupt Vector address, the higher the priority. Table 14.1, p101
  //cInfo.t = cInfo.t + 1; // increment time by 1 second
} // ISR TIMER4

ISR(TIMER3_COMPA_vect) {
    // In this ISR, we switch the cathode, and set the anode pins accordingly
    PORTC = PORTC ^ 0x03; // Toggle the cathode between 0 and 1, so toggle both pins C0 and C1
    // Set the PORT A pins depending on which Cathode is currently enabled
    //PORTA = ((PORTC&0x01)* cInfo.PortAc1Pins) + (((PORTC&0x02)/2)*cInfo.PortAc2Pins); 
    // Set the PORT C pins depending on which Cathode is currently enabled
    //PORTC = (PORTC&0x03) + ((PORTC&0x01)* cInfo.PortCc1Pins) + (((PORTC&0x02)/2)*cInfo.PortCc2Pins);
} // ISR TIMER3



