/*
 * TCS34725_I2C.c
 *
 *  Created on: Nov 13, 2025
 *      Author: Lou
 */
#include <TCS34725_I2C.h>
#include "MKL46Z4.h"

void initI2C() {
	// Enable Clock Gating for I2C module and Port
	SIM->SCGC4 |= SIM_SCGC4_I2C0(1);
	PORTC->PCR[9] &= ~(0x700);
	PORTC->PCR[9] |= PORT_PCR_MUX(2); //MUX bit 8-10 ALT2
	PORTC->PCR[8] &= ~(0x700);
	PORTC->PCR[8] |= PORT_PCR_MUX(2); //MUX bit 8-10 ALT2
	I2C0->S = 0x00; // Write 0 to all I2C registers
	I2C0->A1 = 0x00; //Address Register 1
	I2C0->F = 0x00; //Frequency Divider register
	I2C0->C1 = 0x00; // Control Register 1
	I2C0->S = 0x00; //Status register
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
	I2C0->S |= I2C_S_ARBL_MASK | I2C_S_IICIF_MASK; //arbitration lost, interrupt flag
	// Set I2C Divider Register (Choose a clock frequency less than 100 KHz)
	I2C0->F |= I2C_F_MULT(0); //mul = 1
	I2C0->F |= I2C_F_ICR(0x2B); // SCLD divider = 512 -> clock freq = 93.750KHz
	// Set bit to enable I2C Module
	I2C0->C1 |= I2C_C1_IICEN(1); //write 1 bit 7
	PRINTF("INIT I2C");
}
void clearStatusFlags() {
	// Clear STOPF and Undocumented STARTF bit 4 in Filter Register
	I2C0->FLT |= I2C_FLT_STOPF_MASK;
	I2C0->FLT |= I2C_FLT_STARTF_MASK;
	// Clear ARBL (bit4) and IICIF (bit1) bits in Status Register
	I2C0->S |= I2C_S_ARBL_MASK | I2C_S_IICIF_MASK;
}
void TCFWait() {
	// Wait for TCF bit to Set in Status Register
	while(!(I2C0->S & 0x80)){}
}
void IICIFWait() {
	// Wait for IICIF bit to Set in Status Register
	while(!(I2C0->S & 0x02)){}
}
void SendStart() {
	// Set MST(bit5) and TX(bit4) bits in Control 1 Register
	I2C0->C1 |= I2C_C1_MST(1);
	I2C0->C1 |= I2C_C1_TX(1);
}
void RepeatStart() {
	// Set MST(bit5), TX(bit4) and RSTA(bit2) bits in Control 1 Register
	I2C0->C1 |= I2C_C1_MST(1);
	I2C0->C1 |= I2C_C1_TX(1);
	I2C0->C1 |= I2C_C1_RSTA(1);
	// Wait 6 cycles
	for (int i = 0; i < 6; i++){
		__NOP();
	}
}
void SendStop() {
	// Clear MST(bit5), TX(bit4) and TXAK(bit3) bits in Control 1 Register
	I2C0->C1 &= ~I2C_C1_MST(1);
	I2C0->C1 &= ~I2C_C1_TX(0);
	I2C0->C1 &= ~I2C_C1_TXAK(0);
	// Wait for BUSY bit to go low in Status Register
	while(I2C0->S & I2C_S_BUSY(1)){}
}
void clearIICIF() {
	// Clear IICIF(bit1) bit in Status Register
	I2C0->S |= I2C_S_IICIF_MASK;
}
int RxAK() {
	// Return 1 if byte has been ACK'd. (See RXAK(bit0) in Status Register) ?Do I need to return 0
	if(I2C0->S & 0x01){
		return 0;
	} else {
		return 1;
	}
}
void I2C_WriteByte (uint8_t Register_Address, uint8_t Data) {
	clearStatusFlags();
	TCFWait();
	SendStart();
	// TODO: Write Device Address, R/W = 0 to Data Register
	I2C0->D = (DEVICE_ADDRESS << 1) | 0; //7 MSB address, 1 LSB R/W //0x52
	IICIFWait();
	if (!RxAK()){
		PRINTF("NO RESPONSE - Address\n");
		SendStop();
		return;
	}
	clearIICIF();
	// TODO: Write Register address to Data Register
	I2C0->D = Register_Address;
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Register\n");
		SendStop();
		return;
	}
	TCFWait();
	clearIICIF();
	// Write Data byte to Data Register
	I2C0->D = Data;
	IICIFWait();
	if (!RxAK()){
		printf("Incorrect ACK - Data\n");
	}
	clearIICIF();
	SendStop();
}
int Read_Block(uint8_t Register_Address, uint8_t *Destination_Data_Byte_Array, uint8_t Length) {
	unsigned char dummy = 0;
	clearStatusFlags();
	TCFWait();
	SendStart();
	dummy++; // Do something to suppress the warning.
	//TODO: Write Device Address, R/W = 0 to Data Register
	I2C0->D = (DEVICE_ADDRESS << 1) | 0; //7 MSB address, 1 LSB R/W
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Address\n");
		SendStop();
		return 0;
	}
	clearIICIF();
	// Write Register address to Data Register
	I2C0->D = Register_Address;
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Register\n");
		SendStop();
		return 0;
	}
	clearIICIF();
	RepeatStart();
	// Write device address again, R/W = 1 to Data Register
	I2C0->D = (DEVICE_ADDRESS << 1) | 1; //7 MSB address, 1 LSB R/W
	IICIFWait();
	if (!RxAK()){
		printf("NO RESPONSE - Restart\n");
		SendStop();
		return 0;
	}
	TCFWait();
	clearIICIF();
	// Switch to RX by clearing TX(bit4) and TXAK(bit3) bits in Control 1 register
	I2C0->C1 &= ~I2C_C1_TX_MASK;
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;
	if(Length==1){
		// Set TXAK to NACK in Control 1 - No more data!
		I2C0->C1 |= I2C_C1_TXAK(1);
	}
	dummy = I2C0->D; // Dummy Read
	for(int index=0; index<Length; index++){
		IICIFWait();
		clearIICIF();
		if(index == (Length-2)){ /*Second to last byte*/
			// Set TXAK to NACK in Control 1 - No more data!
			I2C0->C1 |= I2C_C1_TXAK(1);
		}
		if(index == (Length-1)){ /*Last Byte*/
			SendStop();
		}
		// Read Byte from Data Register into Array
		Destination_Data_Byte_Array[index] = I2C0->D;
	}
	return 1;
}
