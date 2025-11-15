/*
 * color_sensor.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */
#include "MKL46Z4.h"

void initI2C() {
	SIM->SCGC4 |= SIM_SCGC4_I2C0(1); // Enable Clock Gating for I2C module and Port
	// Setup Pin Mode for I2C
	// Write 0 to all I2C registers
	// Write 0x50 to FLT register (clears all bits)
	// Clear status flags
	// Set I2C Divider Register (Choose a clock frequency less than 100 KHz)
	// Set bit to enable I2C Module
}
void clearStatusFlags() {
	// Clear STOPF and Undocumented STARTF bit 4 in Filter Register
	// Clear ARBL and IICIF bits in Status Register
}
void TCFWait() {
	// Wait for TCF bit to Set in Status Register
}
void IICIFWait() {
	// Wait for IICIF bit to Set in Status Register
}
void SendStart() {
	// Set MST and TX bits in Control 1 Register
}
void RepeatStart() {
	// Set MST, TX and RSTA bits in Control 1 Register
	// Wait 6 cycles
}
void SendStop() {
	// Clear MST, TX and TXAK bits in Control 1 Register
	// Wait for BUSY bit to go low in Status Register
}
void clearIICIF() {
	// Clear IICIF bit in Status Register
}
void RxAK() {
	// Return 1 if byte has been ACK'd. (See RXAK in Status Register)
}
void I2C_WriteByte (Register Address, Data) {
	clearStatusFlags();
	TCFWait();
	SendStart();
	// TODO: Write Device Address, R/W = 0 to Data Register
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Address");
		SendStop();
		return;
	}
	clearIICIF();
	// TODO: Write Register address to Data Register
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Register");
		SendStop();
		return;
	}
	TCFWait();
	clearIICIF();
	// Write Data byte to Data Register
	IICIFWait();
	if (!RxAK()){
		printf("Incorrect ACK - Data");
	}
	clearIICIF();
	SendStop();
}
void Read_Block(Register_Address, Destination_Data_Byte_Array, Length) {
	unsigned char dummy = 0;
	clearStatusFlags();
	TCFWait();
	SendStart();
	dummy++; // Do something to suppress the warning.
	//TODO: Write Device Address, R/W = 0 to Data Register
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Address");
		SendStop();
		return 0;
	}
	clearIICIF();
	// Write Register address to Data Register
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Register");
		SendStop();
		return 0;
	}
	clearIICIF();
	RepeatStart();
	// Write device address again, R/W = 1 to Data Register
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Restart");
		SendStop();
		return 0;
	}
	TCFWait();
	clearIICIF();
	// Switch to RX by clearing TX and TXAK bits in Control 1 register
	if(len==1){
		// Set TXAK to NACK in Control 1 - No more data!
	}
	dummy = I2C0->D; // Dummy Read
	for(int index=0; index<len; index++){
		IICIFWait();
		clearIICIF();
		if(/*Second to Last Byte*/){
			// Set TXAK to NACK in Control 1 - No more data!
		}
		if(/*Last Byte*/){
			SendStop();
		}
		// Read Byte from Data Register into Array
	}
}
