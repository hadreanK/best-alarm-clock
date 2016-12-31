#ifndef best_alarm_clock_c
#define best_alarm_clock_c

// INCLUDES
	#include "best-alarm-clock.h"
	#include "Arduino.h"
	#include "LED_Clock_helper.h"

// DEFINES
#define LO_TEMP_BUFFER 1.0     // The thermostat will turn on if the room temperature
                                // is this many degrees below the desired temperature
#define HI_TEMP_BUFFER 1.0     	// The thermostat will turn off if the room temperature
                               	// is this many degrees above the desired temperature

	

	
void preSetupEverything(int LCD, int LCDStartPin, int Thermometer, int thermoPin){
	if(LCD>0){
		int x = LCDStartPin;
		// This function calls out the pins for the display: LiquidCrystal(rs, rw, enable, d4, d5, d6, d7)
		//LiquidCrystal lcd(x,x+1,x+2,x+3,x+4,x+5,x+6);
	} // if
	if(Thermometer>0){
		// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
		//OneWire oneWire(thermoPin);
		// Pass the oneWire reference to Dallas Temperature 
		//DallasTemperature sensors(&oneWire);
	} // if
	} // preSetupEverything

void setupEverything(int LCD, int Thermometer, int thermoPin){
		if(LCD>0){
		//	lcd.begin(16,2);
			}
		if(Thermometer>0){
		//	sensors.begin();
		//	pinMode(thermoPin,OUTPUT);
		}
	}		

void setMenu(menuInfo *mp, clockInfo *cp, thermoInfo *tp){
	int coord = 100*mp->x + mp->y; // the coord will be 101 for 1,1; 203 for 2,3 etc.

	switch (coord){
		case 101:
		 	sprintf(mp->topLine, "Main Menu");
		 	sprintf(mp->botLine, "Time is");
		case 102:
		 	sprintf(mp->topLine, "Set Clock");
		 	sprintf(mp->botLine, "Time is %i%i:%i%i",(cp->d1)%10,cp->d2,cp->d3,cp->d4);
		case 103:
		 	sprintf(mp->topLine, "Temp is: %i", tp->roomTemp);
		 	sprintf(mp->botLine, "Desired: %i", tp->desiredTemp);
		break;
		}// switch coord
	} // menuOptions

void heaterControl(thermoInfo *tp){
	switch (tp->heaterOn){
	case 0:// If the heater is off
		if(tp->roomTemp < (tp->desiredTemp - LO_TEMP_BUFFER)){// And it's too cold
			tp->heaterOn = 1;// turn the heater on
		} else {
			tp->heaterOn = 0;}// Otherwise leave it off
		break;
	case 1:// If the heater is on
		if(tp->roomTemp > (tp->desiredTemp + HI_TEMP_BUFFER)){// And it's too warm
			tp->heaterOn = 0;// turn the heater off
		} else {
			tp->heaterOn =1;}// Otherwise leave it on
		break;
	} // switch heaterOn
	}// heaterControl

void heaterLightScheduling(thermoInfo *tp, clockInfo *cp, alarmInfo *ap){
	if(!heaterOverride){ // If the heater override isn't on
		if( (ap->time - ap-> heaterOnBefore)< cp->t < (ap->time - ap-> heaterOnAfter)// See if it's time for the heater to be on
		// Set heater on
		} // if
	

	} // heaterLightScheduling


#endif
