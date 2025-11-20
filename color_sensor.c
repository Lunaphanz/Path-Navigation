/*
 * color_sensor.c
 *
 *  Created on: Nov 17, 2025
 *      Author: Lou
 */
#include "MKL46Z4.h"
#include "color_sensor.h"
#include "function.h"
#include "TCS34725_I2C.h"
uint8_t raw_color[8];
color_type actual_color = UNKNOWN;
void setup_color_sensor(){
	color_sensor_init();
	setup_SCL_SDA();
}
void setup_SCL_SDA(){ //PTC9 - SDA, PTC8 - SCL
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1); //enable clock gating Port C bit 11
	//PTC9 to ALT2 - I2C0_SDA input
	PORTC->PCR[9] &= ~(0x700);
	PORTC->PCR[9] |= PORT_PCR_MUX(2); //MUX bit 8-10 ALT2
	//GPIOC->PDDR &= (1<<9);
	//PTC9 to ALT2 - I2C0_SCL output
	PORTC->PCR[8] &= ~(0x700);
	PORTC->PCR[8] |= PORT_PCR_MUX(2); //MUX bit 8-10 ALT2
	//GPIOC->PDDR |= (1<<8);
}
void color_sensor_init(){
	initI2C(); //I2C setup
	//I2C_WriteByte(0x80, 0x01); //set PON in the Enable register
	//delay_ms(2); //wait 2.4ms
	I2C_WriteByte(0x80, 0x0B); //set PON,WEN,AEN in the Enable register
	I2C_WriteByte(0x8D, 0x00); //clear WLONG to be 0
	I2C_WriteByte(0x83, 0xFF); //set WTIME wait time to 1
	I2C_WriteByte(0x81, 0xFF); //set ATIME Integ_cycles to 1
}
void color_sensor_read(){
	Read_Block(0xB4, raw_color, 8); //read from 0x14 - 0x1B 0x80 | 0x14
	print_raw_color();
}
void print_raw_color()
{
    for (int i = 0; i < 8; i++) {
        printf("raw_color[%d] = 0x%02X\n", i, raw_color[i]);
    }
}
