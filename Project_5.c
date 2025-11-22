/**
 * @file    Project_5.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#include "function.h"
#include "wheel.h"
#include "line_sensor.h"
#include "color_sensor.h"

/* TODO: insert other definitions and declarations here. */
color_type start_color;
color_type finish_color;
bool finish_reached = false;
/*
 * @brief   Application entry point.
 */

int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    setup_wheel();
    setup_color_sensor();
    setup_line_sensor();
    setup_Switch();
    //PRINTF("START\n");
    start_color = get_color();
    finish_color = map_color(start_color);
    while(1){
    	if (SW2_press()){
    		delay_ms(500);
    		get_out();
    		while(!finish_reached){
    			color_sensor_read();
    			keep_going();
				finish_reached = check_finish();
    		}
    		get_in();
    	}
//    	color_sensor_read();
//    	PRINTF("left: %d\n", calibrate_left());
//    	PRINTF("right: %d\n", calibrate_right());
//    	PRINTF("color: %d\n", get_color());
//    	PRINTF("------------\n");
    }

    return 0 ;
}
