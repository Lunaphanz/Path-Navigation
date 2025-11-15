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
void setup_SIM_SCGC5();
void setup_SIM_SCGC6();
void setup_SOPT2();
void setup_PIT();
void PIT_IRQHandler();
void setup_TPM0(); //for delay

//wheel logic
void setup_Wheel();
void setup_TPM2();
void setup_PortB();
void setup_PortC();

//linesensor logic
void setup_line_sensor();
void setup_ADC0();
void setup_PTE16();
void setup_PTE17();

//colorsensor logic
void setup_color_sensor();
void setup_PTC9(); //SDA
void setup_PTC8(); //SCL

//led
void setup_LED();
void LED_on();
void LED_off();

//switch logic
bool SW1_press();
bool SW2_press();
#endif /* FUNCTION_H_ */
