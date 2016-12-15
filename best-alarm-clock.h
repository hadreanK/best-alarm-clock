/*
This is just a test
*/
#ifndef best_alarm_clock_h
#define best_alarm_clock_h


typedef struct{
	int x; // contains current x location in the menu
	int y; // contains current y location in the menu
	char topLine[16]; // Contains the characters to display in the top line
	char botLine[16]; // Contains the characters to display in the bottom line
} menuInfo;
	
void preSetupEverything (int LCD, int LCDStartPin, int Thermometer, int thermoPin);

void setupEverything (int LCD, int Thermometer, int thermoPin);

void setMenu(menuInfo *mp, clockInfo *cp);
// This function will 
// Note: x and y must both be positive 
// Increasing x goes to the right (->) in the menu
// Increasing y goes down in the menu

#endif