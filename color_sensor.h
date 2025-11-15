/*
 * color_sensor.h
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */

#ifndef COLOR_SENSOR_H_
#define COLOR_SENSOR_H_

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
void Read_Block(uint8_t Register_Address, uint8_t Destination_Data_Byte_Array, uint8_t Length);


#endif /* COLOR_SENSOR_H_ */
