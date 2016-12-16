#ifndef best_alarm_clock_c
#define best_alarm_clock_c

// INCLUDES
	#include "best-alarm-clock.h"
	#include "Arduino.h"
	#include "LED_Clock_helper.h"
	
	

	
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
		 	sprintf(mp->topLine, "Temp is: %i", tp->realT);
		 	sprintf(mp->botLine, "Desired: %i", tp->wantT);
		break;
		}// switch coord
	} // menuOptions

#endif
