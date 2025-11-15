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

    setup_SIM_SCGC5();
    setup_SIM_SCGC6();
    setup_SOPT2();
    //setup_color_sensor();
    setup_line_sensor();

    PRINTF("START\n");
    while(1){

    	int left_line = calibrate_right();
    	PRINTF("%d\n", left_line);
    }
    return 0 ;
}
