/*
 * color_sensor.h
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */

#ifndef COLOR_SENSOR_H_
#define COLOR_SENSOR_H_

void initI2C();
void clearStatusFlags();
void TCFWait();
void IICIFWait();
void SendStart();
void RepeatStart();
void SendStop();
void clearIICIF();
void RxAK();
void I2C_WriteByte (Register_Address, Data);
void Read_Block(Register_Address, Destination_Data_Byte_Array, Length);


#endif /* COLOR_SENSOR_H_ */
