// https://www.arduino.cc/en/Reference/PortManipulation

// INCLUDES
    #include "best-alarm-clock.h"   // Helper Functions
    #include <LiquidCrystal.h>      // for LCD
    #include <OneWire.h>            // required for thermistor
    #include <DallasTemperature.h>  // required for thermistor

// DEFINES
    #define THERMO_PIN 2
// Global Variables
    clockInfo   cInfo;          // This is the info thing for the clock
    menuInfo    myMenu;         // This contains the info for the menu position and text
    thermoInfo  myThermo;       // This contains the info for managing the thermostat
    //LCD
        LiquidCrystal lcd(48,49,50,51,52,53);
    // Thermostat
        OneWire oneWire(THERMO_PIN); // Set up oenWire instance to the thermometer
        DallasTemperature sensors(&oneWire); // Pass ther oneWire reference to Dallas Temperature. 
        
    
void setup(){
    // LCD
        lcd.begin(16, 2);           // set up LCD with 16 columns and 2 rows
    // INPUT & OUTPUT PINS
        initializeClockTimers(1,1); // initialize both the timer to count time and the timer for the LED display
        setupClockOutputs();        // setup the output pins associated to the LED display
        cInfo.t = 2*3600 + 56*60;    // Initialize the time to be 5:35 (or whatever)
    // THERMOSTAT
        pinMode(THERMO_PIN,OUTPUT);
        myThermo.desiredTemp = 69;

    // MENU
        myMenu.x = 1;    myMenu.y = 3;

    } //setup
  
void loop() {
    setClockPins(&cInfo); // Set the pins going to the Clock to match the current time 
    // Thermostat
        sensors.requestTemperatures(); // Send the command to get temperatures
        myThermo.roomTemp = sensors.getTempCByIndex(0)*9/5+32; 
        myThermo.desiredTemp = 70.0;
        turnOnOffFurnace(&myThermo);
    // Set the menu things
        setMenu(&myMenu, &cInfo, &myThermo);
        lcd.setCursor(0,0);
        lcd.print(myMenu.topLine);
        lcd.setCursor(0,1);
        lcd.print(myMenu.botLine);
    

    delay(1000);
    
    } //loop  

// Timer ISR for counting time
ISR(TIMER4_COMPA_vect) { // The lower the Interrupt Vector address, the higher the priority. Table 14.1, p101
    cInfo.t = cInfo.t + 1; // increment time by 1 second
} // ISR TIMER4

// Timer ISR for switching LED cathodes
ISR(TIMER3_COMPA_vect) {
    // In this ISR, we switch the cathode, and set the anode pins accordingly
    PORTC = PORTC ^ 0x03; // Toggle the cathode between 0 and 1, so toggle both pins C0 and C1
    // Set the PORT A pins depending on which Cathode is currently enabled
    PORTA = ((PORTC&0x01)* cInfo.PortAc1Pins) + (((PORTC&0x02)/2)*cInfo.PortAc2Pins); 
    // Set the PORT C pins depending on which Cathode is currently enabled
    PORTC = (PORTC&0x03) + ((PORTC&0x01)* cInfo.PortCc1Pins) + (((PORTC&0x02)/2)*cInfo.PortCc2Pins);
} // ISR TIMER3



