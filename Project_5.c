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
//#include <TCS34725_I2C.h>
/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */
void dummy(){
	color_type color = get_color();
	switch (color){
		case RED:
			PRINTF("RED\n");
			break;
		case BLUE:
			PRINTF("BLUE\n");
			break;
		case GREEN:
			PRINTF("GREEN\n");
			break;
		case YELLOW:
			PRINTF("YELLOW\n");
			break;
		case UNKNOWN:
			PRINTF("UNKNOWN\n");
			break;
		default:
			PRINTF("Error\n");
	}
}
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
    //PRINTF("complete color sensor\n");
    setup_line_sensor();

    PRINTF("START\n");
    while(1){
    	color_sensor_read();
    	dummy();
    }
    return 0 ;
}
