/*
 * function.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */
#include <stdio.h>
#include <TCS34725_I2C.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

#include "function.h"
#include "line_sensor.h"

//configuration
void setup_TPM0(){
	SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(2); //OSCERCLK bit 24-25
	TPM0->SC |= TPM_SC_PS(7); // Set Prescaler 128
	TPM0->CONF |= TPM_CONF_CSOO_MASK; // Stop on Overflow
}
void setup_Switch(){
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1); //enable clock gating Port C bit 11
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
void delay_ms(int ms){
	while (ms >0){
		unsigned short chunk = (ms > 1000) ? 1000 : ms;
		TPM0->CNT = 0;
		TPM0->SC |= TPM_SC_TOF_MASK; //Reset Timer Overflow Flag
		TPM0->MOD = ms * 61 + ms/2;

		TPM0->SC |= TPM_SC_CMOD(1); // TPM counter increments on every TPM counter clock
		while(!(TPM0->SC & TPM_SC_TOF_MASK)){} // Wait until Overflow Flag
		TPM0->SC |= TPM_SC_TOF_MASK;           // clear TOF
		TPM0->SC &= ~TPM_SC_CMOD_MASK;         // stop
		ms -= chunk;
	}
}

