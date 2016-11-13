// https://www.arduino.cc/en/Reference/PortManipulation
// INCLUDES
    #include "best-alarm-clock.h"
    //#include <LiquidCrystal.h>
    //#include <OneWire.h>    // required for thermistor
    //#include <DallasTemperature.h>    // required for thermistor
    //#include "LED_Clock_helper.h"
    
// DEFINES
    #define THERMO_PIN 53
// Global Variables
    clockInfo cInfo;     // This is the info thing for the clock
    menuInfo myMenu;        // This contains the info for the menu position and text
    
    // This function calls out the pins for the display: LiquidCrystal(rs, rw, enable, d4, d5, d6, d7)
    LiquidCrystal lcd(48,49,50,51,52,53);
    // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
    OneWire oneWire(10);
    DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature. 
    
    
void setup(){
    // LCD
        lcd.begin(16, 2);           // set up LCD with 16 columns and 2 rows
    // INPUT & OUTPUT PINS
        initializeClockTimers(1,1); // initialize both the timer to count time and the timer for the LED display
    setupClockOutputs();        // setup the output pins associated to the LED display
    cInfo.t = 7*3600 + 29*60;    // Initialize the time to be 5:35 (or whatever)
    Serial.begin(9600);         // Begin serial communication, not sure why but the clock won't work without this
    myMenu.x = 1;    myMenu.y = 2;
    } //setup
  
void loop() {
    setClockPins(&cInfo); // Set the pins going to the Clock to match the current time 
    setMenu(&myMenu, &cInfo);
    lcd.setCursor(0,0);
    lcd.print(myMenu.topLine);
    lcd.setCursor(0,1);
    lcd.print(myMenu.botLine);
    
    delay(1000);
    
    } //loop  

ISR(TIMER4_COMPA_vect) { // The lower the Interrupt Vector address, the higher the priority. Table 14.1, p101
  cInfo.t = cInfo.t + 1; // increment time by 1 second
} // ISR TIMER4

ISR(TIMER3_COMPA_vect) {
    // In this ISR, we switch the cathode, and set the anode pins accordingly
    PORTC = PORTC ^ 0x03; // Toggle the cathode between 0 and 1, so toggle both pins C0 and C1
    // Set the PORT A pins depending on which Cathode is currently enabled
    PORTA = ((PORTC&0x01)* cInfo.PortAc1Pins) + (((PORTC&0x02)/2)*cInfo.PortAc2Pins); 
    // Set the PORT C pins depending on which Cathode is currently enabled
    PORTC = (PORTC&0x03) + ((PORTC&0x01)* cInfo.PortCc1Pins) + (((PORTC&0x02)/2)*cInfo.PortCc2Pins);
} // ISR TIMER3



