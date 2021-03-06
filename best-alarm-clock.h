#ifndef best_alarm_clock_h
#define best_alarm_clock_h

#include "LED_Clock_helper.h" // I'm not sure why this wasn't working when it was in just the .c file, but hey. I just want this damn thing to work right now.

typedef struct{
	int x; // contains current x location in the menu
	int y; // contains current y location in the menu
	char topLine[16]; // Contains the characters to display in the top line
	char botLine[16]; // Contains the characters to display in the bottom line
	} menuInfo;

typedef struct{
	float roomTemp; 		// the most recent temperature in the room in degrees F
	float desiredTemp;		// The desired temperature in the room in degrees F
	int heaterOn; 			// 1 if the heater should be on, 0 if it should be off
	int heaterOverride; 	// 1 if the user wants the heater to be on outside of normal scheduling
	unsigned int lastTempTimestamp; // this is the last time the temperature was read
	} thermoInfo;

typedef struct{
	unsigned int time; 	// time that the alarm is set in seconds
	int set; 		// 1 i the alarm is set to go off, 
	int heaterOnBefore; 	// how many seconds before the alarm goes off the heater will come on
	int heaterOnAfter; 	// how many seconds after the alarm goes off the heater will be on
	int lightOnBefore; 	// how may seconds before the alarm goes off the light will come on
	int lightOnAfter; 	// how many seconds after the alarm goes off the light will be on
	} alarmInfo;
typedef struct{
	int b1; 			// Flag for button 1
	int b2; 			// Flag for button 2
	int b3; 			// Flag for button 3
	} flagInfo;

void preSetupEverything (int LCD, int LCDStartPin, int Thermometer, int thermoPin);

void setupEverything (int LCD, int Thermometer, int thermoPin);

void setMenu(alarmInfo *ap, clockInfo *cp, LiquidCrystal *lp, menuInfo *mp, thermoInfo *tp);
// This function will 
// Note: x and y must both be positive 
// Increasing x goes to the right (->) in the menu
// Increasing y goes down in the menu

void processButtons(flagInfo *fp, alarmInfo *ap, clockInfo *cp, menuInfo *mp, thermoInfo *tp);
// This function will process 

void heaterControl(thermoInfo *tp);
// This function will change the heaterOn value in the thermoInfo struct
// to reflect whether or not it should be on given the room temperature and 
// desired temperature

int heaterLightScheduling( alarmInfo *ap, clockInfo *cp, thermoInfo *tp);
// This will return 1 if the lights should be turned on, 0 if the light should be off
// This should be called after the heaterControl function, but before writing to the
// pin connected to the heater's relay
unsigned int timeToSec(int hours, int minutes);
// This function takes time in mintues and hours and returns it in second format
// If the time is greater than 24 hours, it resets it to a 24 hour clock number

#endif