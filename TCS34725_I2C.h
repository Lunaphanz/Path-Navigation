/*
 * TCS34725_I2C.h
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */

#ifndef TCS34725_I2C_H_
#define TCS34725_I2C_H_
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

#define DEVICE_ADDRESS 0x29

void initI2C();
void clearStatusFlags();
void TCFWait();
void IICIFWait();
void SendStart();
void RepeatStart();
void SendStop();
void clearIICIF();
int RxAK();
void I2C_WriteByte (uint8_t Register_Address, uint8_t Data);
int Read_Block(uint8_t Register_Address, uint8_t *Destination_Data_Byte_Array, uint8_t Length); //return 64bit.
//can return desitation databyte


#endif /* TCS34725_I2C_H_ */
