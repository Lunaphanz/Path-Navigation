/*
 * function.h
 *
 *  Created on: Nov 13th, 2025
 *      Author: Lou
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

//Configuration
void setup_PIT();
void PIT_IRQHandler();
void setup_TPM0(); //for delay

//colorsensor logic
void setup_color_sensor();
void setup_PTC9(); //SDA
void setup_PTC8(); //SCL

//led
void setup_LED();
void LED_on();
void LED_off();

//switch logic
void setup_Switch();
bool SW1_press();
bool SW2_press();

//delay
void delay_ms(int ms);

#endif /* FUNCTION_H_ */
