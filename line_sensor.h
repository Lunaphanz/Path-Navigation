/*
 * line_sensor.h
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

#ifndef LINE_SENSOR_H_
#define LINE_SENSOR_H_
//TODO: Macro define
#define THRESHOLD 140 //black ~150 -> >140 !black
//TODO: variable
extern bool left_line;
extern bool right_line;
//TODO: Configuration
void setup_line_sensor();
void setup_ADC0();
void setup_PTE16();
void setup_PTE17();
//TODO: Function
int calibrate_left();
int calibrate_right();


#endif /* LINE_SENSOR_H_ */
