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
#include "wheel.h"
#include "line_sensor.h"
#include "color_sensor.h"

//configuration
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
//maze logic
color_type map_color(color_type start){
	switch(start){
		case RED:
			return GREEN;
			break;
		case GREEN:
			return BLUE;
			break;
		case BLUE:
			return RED;
			break;
		case YELLOW:
			return RED;
			break;
		default:
			return UNKNOWN;
	}
}
bool check_finish(){
	return finish_color && get_color();
}
void keep_going(){
	go_straight();
	if (calibrate_left() > THRESHOLD_left){
		stop();
		turn_right45();
	}
	if (calibrate_right() > THRESHOLD_right){
		stop();
		turn_left45();
	}
}
void get_out(){
	go_straight();
	delay_ms(timeGoStraight);
	stop();
	delay_ms(300);
	turn_right();
}
void get_in(){
	go_straight();
	delay_ms(500);
	stop();
	delay_ms(300);
	turn_right();
	go_straight();
	delay_ms(timeGoStraight);
	stop();
}


