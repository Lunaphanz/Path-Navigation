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
	setup_SCL_SDA();
	color_sensor_init();
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
	//color_sensor_read();
}
void color_sensor_read(){
	Read_Block(0xB4, raw_color, 8); //read from 0x14 - 0x1B 0x80 | 0x14
	//print_raw_color();
}
color_type get_color(){
	//extract color from raw_color
	uint16_t clear = (raw_color[1] << 8) | (raw_color[0]);
	uint16_t red   = (raw_color[3] << 8) | (raw_color[2]);
	uint16_t green = (raw_color[5] << 8) | (raw_color[4]);
	uint16_t blue  = (raw_color[7] << 8) | (raw_color[6]);
	//adding 10 to create a confidence gap
	if (clear < 60){ //Red, Blue, Black
		if (red > blue && red > green){
			return RED;
		} else if (blue > red && blue > green){
			return BLUE;
		} else if (green > red && green > blue){
			return GREEN;
		} else {
			return UNKNOWN;
		}
	} else {  //Green, Yellow, White
		if (red > blue && red > green){
			return YELLOW;
		} else {
			return UNKNOWN;
		}
	}
//    if (clear == 0) return UNKNOWN;
//    float r = (float)red/clear;
//    float g = (float)green/clear;
//    float b = (float)blue/clear;
//    if (clear < 20) return UNKNOWN; //Black
    
}
void print_raw_color()
{
    for (int i = 0; i < 8; i++) {
        PRINTF("raw_color[%d] = 0x%02X\n", i, raw_color[i]);
    }
}
