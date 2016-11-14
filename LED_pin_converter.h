#ifndef __pin_converter_H
#define __pin_converter_H
/*
			This header file contains arrays that can be used to convert which 
		lights are to be turned on (on the 7 light LED display) to which pins 
		should be enabled for a given cathode. 
 
		array name: ' dXcY ' where X corresponds to the digit number, 
							   and Y corresponds to the cathode number
		The i'th element in the array corresponds to the light to be put on
		
		ex. To turn on light 4C, you'd use the element
		d4c2[3]
		ex. To turn on light 3F, you'd use the element
		d3c1[5]
		
		The elements for digit 1 are (in order):
		UC/LC, 1B, 1C, PM, AM
 
		If the light does not correspond to this cathode, then the array element
		will be a zero
		
		The output is which pins on the screen need to be enabled 
		for the corresponding digit to light up.
		
*/

//int d1c1[7] = {0, pow(2,1), pow(2,2), 0, 0, 0, 0};
int d1c1[7] = {0, 2, 4, 0, 0, 0, 0};
//int d1c2[7] = {pow(2,13), 0, 0, pow(2,1), pow(2,0), 0, 0};
int d1c2[7] = {8192, 0, 0, 2, 1, 0, 0};
		// UC/LC, 1B, 1C, PM, AM, N/C, N/C

//int d2c1[7] = {pow(2,5), pow(2,3), pow(2,4), 0, 0, 0, 0};
int d2c1[7] = {32, 8, 16, 0, 0, 0, 0};
//int d2c2[7] = {0, 0, 0, pow(2,4), pow(2,2), pow(2,5), pow(2,3)};
int d2c2[7] = {0, 0, 0, 16, 4, 32, 8};
		// 2A, 2B, 2C, 2D, 2E, 2F, 2G

//int d3c1[7] = {0, 0, 0, pow(2,8), pow(2,9), pow(2,6), pow(2,7)};
int d3c1[7] = {0, 0, 0, 256, 512, 64, 128};
//int d3c2[7] = {pow(2,6), pow(2,7), pow(2,8), 0, 0, 0, 0};
int d3c2[7] = {64, 128, 256, 0, 0, 0, 0};
		// 3A, 3B, 3C, 3D, 3E, 3F, 3G

//int d4c1[7] = {pow(2,12), pow(2,10), pow(2,11), 0, 0, 0, 0};
int d4c1[7] = {4096, 1024, 2048, 0, 0, 0, 0};
//int d4c2[7] = {0, 0, 0, pow(2,11), pow(2,9), pow(2,12), pow(2,10)};
int d4c2[7] = {0, 0, 0, 2048, 512, 4096, 1024};
		// 4A, 4B, 4C, 4D, 4E, 4F, 4G


		
#endif