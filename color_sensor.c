/*
 * color_sensor.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */
#include "MKL46Z4.h"

void initI2C() {
	// Enable Clock Gating for I2C module and Port
	SIM->SCGC4 |= SIM_SCGC4_I2C0(1);
	// Setup Pin Mode for I2C
	PORTC->PCR[8] |= PORT_PCR_MUX(2); //I2C SCL MUX ALT2
	PORTC->PCR[9] |= PORT_PCR_MUX(2); //I2C SDA MUX ALT2
	I2C0->S = 0x00; // Write 0 to all I2C registers
	I2C0->A1 = 0x00; //Address Register 1
	I2C0->F = 0x00; //Frequency Divider register
	I2C0->C1 = 0x00; // Control Register 1
	I2C0->S |= I2C_S_ARBL_MASK | I2C_S_IICIF_MASK; //Status register
	I2C0->D = 0x00; // Data I/O register
	I2C0->C2 = 0x00; // Control Register 2
	I2C0->FLT = 0x00; // Programmable INput Glitch Filter register
	I2C0->RA = 0x00; // Range Address register
	I2C0->SMB = 0x00; // SMBus Control and Status register
	I2C0->A2 = 0x00; // Address Register 2
	I2C0->SLTH = 0x00; // SCL Low Timeout Register High
	I2C0->SLTL = 0x00; // SCL Low Timeout Register Low
	// Write 0x50 to FLT register (clears all bits)
	I2C0->FLT = 0x50; // Write bit 4,6 STOPF FLT
	// TODO:Clear status flags ?is it to clear STARTF and STOPF
	I2C0->S |= I2C_S_ARBL_MASK | I2C_S_IICIF_MASK;
	// Set I2C Divider Register (Choose a clock frequency less than 100 KHz)
	I2C0->F = I2C_F_MULT(0); //mul = 1
	I2C0->F = I2C_F_ICR(0x2B); // SCLD divider = 512 -> clock freq = 93.750KHz
	// Set bit to enable I2C Module
	I2C0->C1 |= I2C_C1_IICEN(1); //write 1 bit 7
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
