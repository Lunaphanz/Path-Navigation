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

#include "color_sensor.h"

//Configuration
void setup_PIT();
void PIT_IRQHandler();

//led
void setup_LED();
void LED_on();
void LED_off();

//switch logic
void setup_Switch();
bool SW1_press();
bool SW2_press();

//maze logic
extern color_type start_color;
extern color_type finish_color;
extern bool finish_reached;
color_type map_color(color_type start);
bool check_finish();

void get_out();
void keep_going();
void get_in();
#endif /* FUNCTION_H_ */
