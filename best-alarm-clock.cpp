#ifndef best_alarm_clock_c
#define best_alarm_clock_c

// INCLUDES
	
	#include <Arduino.h>
	#include <LiquidCrystal.h>
	#include "LED_Clock_helper.h"
	#include "best-alarm-clock.h"

// DEFINES
#define LO_TEMP_BUFFER 1.0     // The thermostat will turn on if the room temperature
                                // is this many degrees below the desired temperature
#define HI_TEMP_BUFFER 1.0     	// The thermostat will turn off if the room temperature
                               	// is this many degrees above the desired temperature
#define NUM_MENUS	6			// The number of menus to scroll through

	

	
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

void setMenu(alarmInfo *ap, clockInfo *cp, LiquidCrystal *lp, menuInfo *mp, thermoInfo *tp){
	//int coord = 100*mp->x + mp->y; // the coord will be 101 for 1,1; 203 for 2,3 etc.
	int alarmD1 = round(ap->time/36000);;
	int alarmD2 = round((ap->time - alarmD1*36000)/3600);
	int alarmD3 = round((ap->time - alarmD1*36000 - alarmD2*3600)/60);
	int alarmD4 = ap->time%60;
	switch (mp->y){
		case 1:
		 	sprintf(mp->topLine, "Clock Hr     %iF", round(tp->roomTemp));
		 	sprintf(mp->botLine, "Time is %i%i:%i%i",(cp->d1)%10,cp->d2,cp->d3,cp->d4);
		 	break;
		case 2:
		 	sprintf(mp->topLine, "Clock Min    %iF", round(tp->roomTemp));
		 	sprintf(mp->botLine, "Time is %i%i:%i%i",(cp->d1)%10,cp->d2,cp->d3,cp->d4);
		 	break;
		case 3:
		 	sprintf(mp->topLine, "Alarm Set     %iF", round(tp->roomTemp));
		 	if(ap->set>0){sprintf(mp->botLine, "Alarm is ON       ");
		 	}else{sprintf(mp->botLine, "Alarm is OFF      ");}
		 	break;
		case 4:
		 	sprintf(mp->topLine, "Alarm Hr     %iF", round(tp->roomTemp));
		 	sprintf(mp->botLine, "Alarm at   %i%i:%i%i",alarmD1, alarmD2, alarmD3, alarmD4);
		 	break;
		case 5:
		 	sprintf(mp->topLine, "Alarm Min    %iF", round(tp->roomTemp));
		 	sprintf(mp->botLine, "Alarm at   %i%i:%i%i",alarmD1, alarmD2, alarmD3, alarmD4);
		 	break;
		case 6:
		 	sprintf(mp->topLine, "Thermostat   %iF", round(tp->roomTemp));
		 	sprintf(mp->botLine, "Desired: %i     ", round(tp->desiredTemp));
			break;
		}// switch coord

	// Print the menu out to the screen
	lp->setCursor(0,0);
    lp->print(mp->topLine);
    lp->setCursor(0,1);
    lp->print(mp->botLine);
	} // menuOptions

void processButtons(flagInfo *fp, alarmInfo *ap, clockInfo *cp, menuInfo *mp, thermoInfo *tp){
	if(fp->b3>0){	// button3 changes the menu
		mp->y = mp->y%NUM_MENUS + 1;
		}//if b3
	
	switch (mp->y){
		case 1:
			cp->t = cp->t + 3600*fp->b1;
			cp->t = cp->t - 3600*fp->b2;
			break;
		case 2:
			cp->t = cp->t + 60*fp->b1;
			cp->t = cp->t - 60*fp->b2;
			break;
		case 3:
			if(fp->b1>0){ap->set = 1;}
			if(fp->b2>0){ap->set = 0;}
			break;
		case 4:
			ap->time = ap->time + 3600*fp->b1;
			ap->time = ap->time - 3600*fp->b2;
			break;
		case 5:
			ap->time = ap->time + 60*fp->b1;
			ap->time = ap->time - 60*fp->b2;
			break;
		case 6:
			if(fp->b1>0){tp->desiredTemp = tp->desiredTemp + 1.0;}
			if(fp->b2>0){tp->desiredTemp = tp->desiredTemp - 1.0;}
			break;
		}// switch coord

		// Clear all button flags
		fp->b1 = 0;
		fp->b2 = 0;
		fp->b3 = 0;
	} // processButtons

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

int heaterLightScheduling(alarmInfo *ap, clockInfo *cp, thermoInfo *tp){
	if(!tp->heaterOverride){ // If the heater override isn't on
		if( ((ap->time - ap-> heaterOnBefore) > cp->t) | (cp->t > (ap->time + ap-> heaterOnAfter))) {// And it's not time for the heater ot be on 
			tp->heaterOn = 0;// Keep the heater off, 
		} // if 
	} // if not heaterOverRide
	if(((ap->time - ap->lightOnBefore)< cp->t) & (cp->t < (ap->time + ap->lightOnAfter))){// If the lights should be on
		return(1);// Turn them on, i.e. return 1
		} else { return(0);} // otherwise turn them off, i.e. return 0
	} // heaterLightScheduling

unsigned int timeToSec(int hours, int minutes){
	unsigned int time = 60*60*hours + 60*minutes; // convert the timeit to seconds
	time = time%(24*60*60); // mod it to make sure it's just in one day
	return(time);
}



#endif
