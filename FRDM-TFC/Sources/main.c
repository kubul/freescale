#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"
#include "Spices.h"
#include "demo.h"
#include "i2c.h"


void Init_I2C();
void Init_Accel();
void Init_Interrupts();

int main(void) {

	TFC_Init();
	Init_I2C();
	Init_Accel();
	Init_Interrupts();
	Init_Demo();

	for(;;) {
		//TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check)
		TFC_Task();

		if(TFC_GetDIP_Switch()&0x01) {
			// Run MCP
			MasterControlProgram();
		} else {
			// Run Demo Program
			DemoProgram();
		}
	}
	return 0;
}

void Init_Interrupts() {
	EnableInterrupts;
	int irq_num = INT_PORTA - 16;   // enable IRQ on PORTA 
	enable_irq(irq_num);
	//NVIC_ICPR |= 1 << irq_num;  //Clear any pending interrupts on PORTA
	//NVIC_ISER |= 1 << irq_num;  //Enable interrupts from PORTA
	//NVIC_IPR7 |= // Set interrupt priority
}



void PORTA_IRQHandler() {  
	PORTA_ISFR=0xFFFFFFFF;
	TFC_SetMotorPWM(0,0); //Make sure motors are off
	TFC_HBRIDGE_DISABLE;
	TFC_BAT_LED0_ON;
	I2CReadRegister(MMA8451_I2C_ADDRESS, INT_SOURCE);
	I2CReadRegister(MMA8451_I2C_ADDRESS, PULSE_SRC);
} 

// EXAMPLES

//		//Demo mode 0 just tests the switches and LED's
//		if(TFC_PUSH_BUTTON_0_PRESSED)
//			TFC_BAT_LED0_ON;
//		else
//			TFC_BAT_LED0_OFF;
//
//		if(TFC_PUSH_BUTTON_1_PRESSED)
//			TFC_BAT_LED3_ON;
//		else
//			TFC_BAT_LED3_OFF;
//
//
//		if(TFC_GetDIP_Switch()&0x01)
//			TFC_BAT_LED1_ON;
//		else
//			TFC_BAT_LED1_OFF;
//
//		if(TFC_GetDIP_Switch()&0x08)
//			TFC_BAT_LED2_ON;
//		else
//			TFC_BAT_LED2_OFF;

//Let's put a pattern on the LEDs
/*
if(TFC_Ticker[1] >= 125)
{
	TFC_Ticker[1] = 0;
	t++;
	if(t>4)
	{
		t=0;
	}			
	TFC_SetBatteryLED_Level(t);
}
 */
/*

//accel crap

 * 	//I2CWriteRegister(MMA8451_I2C_ADDRESS, FF_MT_CFG, 0x11);

	status = (int)I2CReadRegister(MMA8451_I2C_ADDRESS, STATUS);
	TERMINAL_PRINTF("\r\n");
	TERMINAL_PRINTF("System status: %d" , status);
	status = (int)I2CReadRegister(MMA8451_I2C_ADDRESS, SYSMOD);
	TERMINAL_PRINTF("\r\n");
	TERMINAL_PRINTF("System mode: %d" , status);

 */


/*
	accel = I2CReadRegister(MMA8451_I2C_ADDRESS, OUT_X_MSB);
	TERMINAL_PRINTF("\r\n");
	TERMINAL_PRINTF("X axis acceleration: %c" , accel);

	status = (int)I2CReadRegister(MMA8451_I2C_ADDRESS, FF_MT_SRC);
			accel = (int)I2CReadRegister(MMA8451_I2C_ADDRESS, FF_MT_SRC);
			TERMINAL_PRINTF("\r\n");
			TERMINAL_PRINTF("Freefall?: %d" , accel);
			accel = 0;

 */

