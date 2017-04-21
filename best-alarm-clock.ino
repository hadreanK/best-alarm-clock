// https://www.arduino.cc/en/Reference/PortManipulation

// INCLUDES
    
    #include <LiquidCrystal.h>      // for LCD
    #include <OneWire.h>            // required for thermistor
    #include <DallasTemperature.h>  // required for thermistor
    #include "best-alarm-clock.h"   // Helper Functions

// DEFINES
    #define THERMOMETER_PIN 8       // the pin with the thermometer attached
    #define HEATER_PIN 3            // the pin connected to the relay controlling the heater
    #define LIGHT_PIN 4             // the pin connected to the relay controlling the lights
    #define READ_TEMP_EVERY 20      // How often to read the temperature in the room in seconds

// Global Variables
    clockInfo   Clock;          // This is the info thing for the clock
    menuInfo    Menu;           // This contains the info for the menu position and text
    thermoInfo  Thermo;         // This contains the info for managing the thermostat
    alarmInfo   Alarm;          // Contains info for managing the alarm
    flagInfo    Flags;          // Contains all the flags for the interrupts
    
    //LCD
        LiquidCrystal Lcd(48,49,50,51,52,53);
    // Thermostat
        OneWire oneWire(THERMOMETER_PIN); // Set up oenWire instance to the thermometer
        DallasTemperature sensors(&oneWire); // Pass ther oneWire reference to Dallas Temperature. 

        
    
void setup(){
    // LCD
        Lcd.begin(16, 2);           // set up LCD with 16 columns and 2 rows
    // INPUT & OUTPUT PINS
        initializeClockTimers(1,1); // initialize both the timer to count time and the timer for the LED display
        setupClockOutputs();        // setup the output pins associated to the LED display
        
    // INTERRUPTS
        EICRA = 0xF0;   // Set INT2 and INT3 to rising edge interrupt, set to 0b11, 15.2.2, page 111
        EICRB = 0x03;   // Set INT4 to rising edge interrupt, set to 0b11, 15.2.2, page 111
        EIMSK = 0x1C;   // Enable interrupts INT2, INT3 and INT4 page 111

    // THERMOSTAT
        sensors.begin();
        pinMode(THERMOMETER_PIN,OUTPUT);
        pinMode(HEATER_PIN,OUTPUT);
        digitalWrite(HEATER_PIN,1);
        Thermo.lastTempTimestamp = 0;
        Thermo.desiredTemp = 55.0;
    // MENU & BUTTONS
        Menu.x = 1;    Menu.y = 1;
        // Clear button flags
        Flags.b1 = 0;
        Flags.b2 = 0;
        Flags.b3 = 0;

    // CLOCK
        Clock.t = 11*3600 + 42*60;    // Initialize the time to be 5:35 (or whatever)
    // ALARM
        Alarm.time = 7*3600 + 15*60; // set the time for alarm
        Alarm.set = 1; // Set the alarm to go off
        Alarm.heaterOnBefore = 25*60; // Set how early heater should come on
        Alarm.heaterOnAfter = 20*60; // Set how long heater should stay on
        Alarm.lightOnBefore = 5*60; // How early lights come on
        Alarm.lightOnAfter = 30*60; // How long lights stay on
        pinMode(LIGHT_PIN,OUTPUT);
        digitalWrite(LIGHT_PIN,1);
    // DEBUG
        // Serial.begin(9600);
    } //setup
  
void loop() {
// DEBUG
    // Serial.println(Menu.y);
    // CLOCK
        setClockPins(&Clock); // Set the pins going to the Clock to match the current time 
        Clock.t = Clock.t%(13*60*60);// Reset it after 24 hours
    // THERMOSTAT
       if( (Clock.t > (Thermo.lastTempTimestamp + READ_TEMP_EVERY)) | (Clock.t <Thermo.lastTempTimestamp)){// Only read the temperature if it's been a while 
            sensors.requestTemperatures(); // Send the command to get temperatures
            Thermo.roomTemp = sensors.getTempCByIndex(0)*9/5+32; 
            heaterControl(&Thermo); // figure out if the heater should be off or on
           }
        
    // MENU & BUTTONS
        processButtons(&Flags, &Alarm, &Clock, &Menu, &Thermo);
        setMenu(&Alarm, &Clock, &Lcd, &Menu, &Thermo); // set the menu options
            
        
    // ALARM
    digitalWrite(LIGHT_PIN, heaterLightScheduling(&Alarm, &Clock, &Thermo)); // turn on/off the lights / heater
    digitalWrite(HEATER_PIN,!Thermo.heaterOn); // turn on or off the heater
                            // ^ The ! is becaues a low pin actually turns on the heater...
    } //loop  

// Timer ISR for counting time
ISR(TIMER4_COMPA_vect) { // The lower the Interrupt Vector address, the higher the priority. Table 14.1, p101
    Clock.t = Clock.t + 1; // increment time by 1 second
} // ISR TIMER4

// // Timer ISR for switching LED cathodes
ISR(TIMER3_COMPA_vect) {
    // In this ISR, we switch the cathode, and set the anode pins accordingly
    PORTC = PORTC ^ 0x03; // Toggle the cathode between 0 and 1, so toggle both pins C0 and C1
    // Set the PORT A pins depending on which Cathode is currently enabled
    PORTA = ((PORTC&0x01)* Clock.PortAc1Pins) + (((PORTC&0x02)/2)*Clock.PortAc2Pins); 
    // Set the PORT C pins depending on which Cathode is currently enabled
    PORTC = (PORTC&0x03) + ((PORTC&0x01)* Clock.PortCc1Pins) + (((PORTC&0x02)/2)*Clock.PortCc2Pins);
} // ISR TIMER3

// Interrupt ISR for button 1, pin 2, INT 4
ISR(INT4_vect) { // Interrupt vectors on page 101, this is pin 
    Flags.b1 = 1;// Set interrupt flag for button 1
} // INT4 ISR

// Interrupt ISR for button 2, pin 19, INT 2
ISR(INT2_vect) { // Interrupt vectors on page 101, this is pin 
    Flags.b2 = 1;// Set interrupt flag for button 2
} // INT2 ISR

// Interrupt ISR for button 3, pin 18, INT 3
ISR(INT3_vect) { // Interrupt vectors on page 101, this is pin 
    Flags.b3 = 1;// Set interrupt flag for button 2
} // INT3 ISR
