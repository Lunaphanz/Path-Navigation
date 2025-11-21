/*
 * wheel.c
 *
 *  Created on: Oct 30, 2025
 *      Author: Lou
 */

#include "MKL46Z4.h"
#include "wheel.h"

void setup_wheel(){
	setup_PortB(); //left wheel motor
	setup_PortC(); //right wheel motor
	setup_TPM2();  //PWM control
	setup_TPM0(); // controlled delay
}
void setup_PortB(){
	SIM->SCGC5 |= SIM_SCGC5_PORTB(1); //enable clock gating Port B bit 10
	//PTB0, PTB1 to GPIO, output - right motor
	PORTB->PCR[0] |= PORT_PCR_MUX(1);
	PORTB->PCR[1] |= PORT_PCR_MUX(1);
	GPIOB->PDDR |= (1<<0) | (1<<1);
	// PTB2 & PTB3 as PWM control - TPM2
	PORTB->PCR[2] = PORT_PCR_MUX(3);
	PORTB->PCR[3] = PORT_PCR_MUX(3);
}
void setup_PortC(){
	SIM->SCGC5 |= SIM_SCGC5_PORTC(1); //enable clock gating Port C bit 11
	//PTC1, PTC2 to GPIO, output
	PORTC->PCR[1] |= PORT_PCR_MUX(1);
	PORTC->PCR[2] |= PORT_PCR_MUX(1);
	GPIOC->PDDR |= (1<<1) | (1<<2);
}
void setup_TPM2(){
	SIM->SCGC6 |= SIM_SCGC6_TPM2(1); //enable clock gating TPM2 bit 26
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(2); //OSCERCLK bit 24-25
	//set up TPM2 for PWM, OSCERCLK, PS = 4, Clock Mod increment
	TPM2->SC |= TPM_SC_PS(2);
	TPM2->SC |= TPM_SC_CMOD(1);
	TPM2->MOD = TPM2_MOD;
	TPM2->CONTROLS[0].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Edge PWMA
	TPM2->CONTROLS[1].CnSC |= TPM_CnSC_MSB_MASK | TPM_CnSC_ELSB_MASK; // Edge PWMB
}
void go_straight(){
	// Left motor (PTB0=AI2 high, PTB1=AI1 low)
	GPIOB->PSOR |= (1<<0);
	GPIOB->PCOR |= (1<<1);
	// Right motor (PTC1=BI1 high, PTC2=BI2 low)
	GPIOC->PSOR |= (1<<1);
	GPIOC->PCOR |= (1<<2);
	// Set PWM outputs
	setMotorSpeed(leftSpeed,rightSpeed);
}
void turn_left45(){
	// Left motor (PTB0=AI2 low, PTB1=AI1 high)
	GPIOB->PSOR |= (1<<1);
	GPIOB->PCOR |= (1<<0);
	// Right motor (PTC1=BI1 high, PTC2=BI2 low)
	GPIOC->PSOR |= (1<<1);
	GPIOC->PCOR |= (1<<2);
	// Set PWM outputs
	setMotorSpeed(leftSpeed,rightSpeed);
	delay_ms(timeTurnLeft45);
	stop();
	delay_ms(300);
}
void turn_right45(){
	// Left motor (PTB0=AI2 high, PTB1=AI1 low)
	GPIOB->PSOR |= (1<<0);
	GPIOB->PCOR |= (1<<1);
	// Right motor (PTC1=BI1 low, PTC2=BI2 high)
	GPIOC->PSOR |= (1<<2);
	GPIOC->PCOR |= (1<<1);
	// Set PWM outputs
	setMotorSpeed(leftSpeed,rightSpeed);
	delay_ms(timeTurnRight45);
	stop();
	delay_ms(300);
}
void turn_right(){
	// Left motor (PTB0=AI2 high, PTB1=AI1 low)
	GPIOB->PSOR |= (1<<0);
	GPIOB->PCOR |= (1<<1);
	// Right motor (PTC1=BI1 low, PTC2=BI2 high)
	GPIOC->PSOR |= (1<<2);
	GPIOC->PCOR |= (1<<1);
	// Set PWM outputs
	setMotorSpeed(leftSpeed,rightSpeed);
	delay_ms(timeTurnRight);
	stop();
	delay_ms(300);
}
void stop(){
	// Left motor (PTB0=AI2 low, PTB1=AI1 low)
	GPIOB->PCOR |= (1<<0);
	GPIOB->PCOR |= (1<<1);
	// Right motor (PTC1=BI1 low, PTC2=BI2 low)
	GPIOC->PCOR |= (1<<2);
	GPIOC->PCOR |= (1<<1);
	// Set PWM outputs
	setMotorSpeed(0,0);
	delay_ms(300);
}
void setMotorSpeed(float left, float right){
	TPM2->CONTROLS[0].CnV = left;
	TPM2->CONTROLS[1].CnV = right;
}
void setup_TPM0(){
	SIM->SCGC6 |= SIM_SCGC6_TPM0(1);
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(2); //OSCERCLK bit 24-25
	TPM0->SC |= TPM_SC_PS(7); // Set Prescaler 128
	TPM0->CONF |= TPM_CONF_CSOO_MASK; // Stop on Overflow
}
void delay_ms(int ms){
	while (ms >0){
		unsigned short chunk;
		if (ms > 1000){
			chunk = 1000;
		} else {
			chunk = ms;
		}
		TPM0->CNT = 0;
		TPM0->SC |= TPM_SC_TOF_MASK; //Reset Timer Overflow Flag
		TPM0->MOD = chunk*62;

		TPM0->SC |= TPM_SC_CMOD(1); // TPM counter increments on every TPM counter clock
		while(!(TPM0->SC & TPM_SC_TOF_MASK)){} // Wait until Overflow Flag
		TPM0->SC |= TPM_SC_TOF_MASK;           // clear TOF
		TPM0->SC &= ~TPM_SC_CMOD_MASK;         // stop
		ms -= chunk;
	}
}


