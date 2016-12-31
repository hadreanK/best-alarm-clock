// https://www.arduino.cc/en/Reference/PortManipulation

// INCLUDES
    #include "best-alarm-clock.h"   // Helper Functions
    #include <LiquidCrystal.h>      // for LCD
    #include <OneWire.h>            // required for thermistor
    #include <DallasTemperature.h>  // required for thermistor

// DEFINES
    #define THERMOMETER_PIN 2       // the pin with the thermometer attached
    #define HEATER_PIN 3            // the pin connected to the relay controlling the heater
// Global Variables
    clockInfo   Clock;          // This is the info thing for the clock
    menuInfo    Menu;           // This contains the info for the menu position and text
    thermoInfo  Thermo;         // This contains the info for managing the thermostat
    alarmInfo   Alarm;          // Contains info for managing the alarm
    int button1Flag; int button2Flag; // These are the flags for button 1 and 2 that are set in interrupts below
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
        Clock.t = 2*3600 + 56*60;    // Initialize the time to be 5:35 (or whatever)
    // INTERRUPTS
        EICRB = 0x0F; // Set INT4 and INT 5 to rising edge interrupt, set to 0b11, 15.2.2, page 111
        EIMSK = 0x30;// Enable interrupts INT4 and INT5

    // THERMOSTAT
        pinMode(THERMOMETER_PIN,OUTPUT);
        pinMode(HEATER_PIN,OUTPUT);
        digitalWrite(HEATER_PIN,0);

    // MENU
        Menu.x = 1;    Menu.y = 3;

    // ALARM
        Alarm.time = 7*3600 + 15*60; // set the time for alarm
        Alarm.set = 1; // Set the alarm to go off
        Alarm.heaterOnBefore = 25*60; // Set how early heater should come on
        Alarm.heaterOnAfter = 20*60; // Set how long heater should stay on
        Alarm.lightOnBefore = 5*60; // How early lights come on
        Alarm.lightOnAfter = 30*60; // How long lights stay on
    } //setup
  
void loop() {
    // CLOCK
        setClockPins(&Clock); // Set the pins going to the Clock to match the current time 
        Clock.t = Clock.t%(24*60*60);// Reset it after 24 hours
    // THERMOSTAT
        sensors.requestTemperatures(); // Send the command to get temperatures
        Thermo.roomTemp = sensors.getTempCByIndex(0)*9/5+32; 
        Thermo.desiredTemp = 70.0;
        heaterControl(&Thermo); // figure out if the heater should be off or on
        digitalWrite(HEATER_PIN,Thermo.heaterOn); // turn on or off the heater
    // MENU
        setMenu(&Menu, &Clock, &Thermo);
        Lcd.setCursor(0,0);
        Lcd.print(Menu.topLine);
        Lcd.setCursor(0,1);
        Lcd.print(Menu.botLine);
    // ALARM
    

    delay(1000);
    
    } //loop  

// Timer ISR for counting time
ISR(TIMER4_COMPA_vect) { // The lower the Interrupt Vector address, the higher the priority. Table 14.1, p101
    Clock.t = Clock.t + 1; // increment time by 1 second
} // ISR TIMER4

// Timer ISR for switching LED cathodes
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
    button1Flag = 1;// Set interrupt flag for button 1
} // INT1 ISR

// Interrupt ISR for button 2, pin 3, INT 5
ISR(INT5_vect) { // Interrupt vectors on page 101, this is pin 
    button2Flag = 1;// Set interrupt flag for button 2
} // INT1 ISR
