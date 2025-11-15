/*
 * function.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

#include "function.h"
#include "color_sensor.h"
//configuration
void setup_SIM_SCGC5(){
	//enable clock gating Port A,B,C,D, E
	//                        9,10,11,12,13
	SIM->SCGC5 |= SIM_SCGC5_PORTA(1) |
				  SIM_SCGC5_PORTB(1) |
				  SIM_SCGC5_PORTC(1) |
				  SIM_SCGC5_PORTD(1) |
				  SIM_SCGC5_PORTE(1);
}
void setup_SIM_SCGC6(){
	//enable clock gating TPM0, TPM1, TPM2, PIT, ADC0
	// 					   24	 25    26   23   27
	SIM->SCGC6 |= SIM_SCGC6_TPM0(1) |
				  SIM_SCGC6_TPM1(1) |
				  SIM_SCGC6_TPM2(1) |
				  SIM_SCGC6_PIT(1)  |
				  SIM_SCGC6_ADC0(1);
}
void setup_SOPT2(){
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(2); //OSCERCLK bit 24-25
}
void setup_TPM0(){
	TPM0->SC |= TPM_SC_PS(7); // Set Prescaler 128
	TPM0->CONF |= TPM_CONF_CSOO_MASK; // Stop on Overflow
}
void setup_Wheel(){
	setup_PortB(); //wheel motor
	setup_PortC(); //wheel motor and switch
	setup_TPM2();  //for PWM
}
void setup_line_sensor(){
	setup_ADC0();
	setup_PTE16(); //left line sensor
	setup_PTE17(); //right line sensor
}
void setup_color_sensor(){
	initI2C();
	//setup_PTC9(); //SDA
	//setup_PTC8(); //SCL
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
void setup_PortB(){
	//PTB2, PTB3 TPM2
	//PTB1, PTB0 right motor
	//PTB0, PTB1 to GPIO, output
	PORTB->PCR[0] |= PORT_PCR_MUX(1);
	PORTB->PCR[1] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1<<0) | (1<<1);
	// PTB2 & PTB3 as PWM control
	PORTB->PCR[2] = PORT_PCR_MUX(3);
	PORTB->PCR[3] = PORT_PCR_MUX(3);
}
void setup_PortC(){
	//PTC1, PTC2 to GPIO, output
	PORTC->PCR[1] |= PORT_PCR_MUX(1);
	PORTC->PCR[2] |= PORT_PCR_MUX(1);
	GPIOC->PDDR |= (1<<1) | (1<<2);
	//PTC3 (SW1) to GPIO, pull enable, pull select, input
	PORTC->PCR[3] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
	GPIOC->PDDR &= ~(1<<3); // Clear Data direction (input)
	//PTC12 (SW2) to GPIO, pull enable, pull select, input
	PORTC->PCR[12] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
	GPIOC->PDDR &= ~(1<<12); // Clear Data direction (input)
}
bool SW1_press(){
	return !(GPIOC->PDIR & (1<<3));
}
bool SW2_press(){
	return !(GPIOC->PDIR & (1<<12));
}
void setup_LED(){
	SIM->SCGC5 |= SIM_SCGC5_PORTD(1);
	PORTD->PCR[5] |= PORT_PCR_MUX(1);
	GPIOD->PDDR |= (1<<5);
}
void LED_on(){
	GPIOD->PDOR &= ~(1<<5); // Turn on Green LED
}
void LED_off(){
	GPIOD->PDOR |= (1<<5);
}

