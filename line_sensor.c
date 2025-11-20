/*
 * line_sensor.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */
#include "MKL46Z4.h"
#include "line_sensor.h"

void setup_line_sensor(){
	SIM->SCGC6 |= SIM_SCGC6_ADC0(1); // enable clock gaing ADC0 bit 27
	SIM->SCGC5 |= SIM_SCGC5_PORTE(1); // enable clock gating for Port E bit 13
	setup_ADC0();
	setup_PTE16(); //left line sensor
	setup_PTE17(); //right line sensor
}
void setup_ADC0(){
	uint32_t cal_v;
	// Setup ADC Clock ( < 4 MHz)
	ADC0->CFG1 = 0;  // Default everything.
	// Analog Calibrate
	ADC0->SC3 = 0x07; // Enable Maximum Hardware Averaging
	ADC0->SC3 |= 0x80; // Start Calibration
	// Wait for Calibration to Complete (CAL)
	while((ADC0->SC3 & 0x80)){	}
	// Calibration Complete, write calibration registers.
	cal_v = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0->PG = cal_v;
	cal_v = 0;
	cal_v = ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 + ADC0->CLM4 + ADC0->CLMS;
	cal_v = cal_v >> 1 | 0x8000;
	ADC0->MG = cal_v;
	ADC0->SC3 = 0; // Turn off Hardware Averaging
}
void setup_PTE16(){ //left line sensor
	//PTE16 default ADC0_DP1/ADC0_SE1
	//PTE16 to GPIO input
	PORTE->PCR[16] &= ~(0x700);
	PORTE->PCR[16] |= PORT_PCR_MUX(1);
	GPIOE->PDDR &= ~(1<<16); // Clear Data direction (input)
}
void setup_PTE17(){ //right line sensor
	//PTE17 default ADC0_DM1/ADC0_SE5a
	//PTE16 to GPIO input
	PORTE->PCR[17] &= ~(0x700);
	PORTE->PCR[17] |= PORT_PCR_MUX(1);
	GPIOE->PDDR &= ~(1<<17); // Clear Data direction (input)
}
int calibrate_left(){
	ADC0->SC1[0] = 0x01; // Set Channel, starts conversion.
	while(!(ADC0->SC1[0] & 0x80)){	}
	return ADC0->R[0]; // Resets COCO
}
int calibrate_right(){
	ADC0->SC1[0] = 0x05; // Set Channel, starts conversion.
	while(!(ADC0->SC1[0] & 0x80)){	}
	return ADC0->R[0]; // Resets COCO
}


