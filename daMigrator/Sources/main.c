#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"
#include "Spices.h"
#include "demo.h"
#include "i2c.h"


void Init_I2C();
void Init_Accel();
void Init_Interrupts();

int main(void) {
	int t1;
	int t0; 
	float x = 0;

	TFC_Init();
	Init_I2C();
	Init_Accel();
	Init_Interrupts();
	Init_Demo();

	for(;;) {
		//TFC_Task must be called in your main loop.  This keeps certain processing happy (I.E. Serial port queue check)
		TFC_Task();
		//TFC_SetMotorPWM(x,x);
		//t0 = TFC_ReadPot(0);
		//t1 = TFC_ReadPot(1);
		

		if(TFC_GetDIP_Switch()&0x01) {
			
			x = TFC_ReadBatteryVoltage();
			t0 = (int)((x/7)*4);
			if (t0 == 0) {
				TFC_BAT_LED0_ON;
				TFC_BAT_LED1_OFF;
				TFC_BAT_LED2_OFF;
				TFC_BAT_LED3_OFF;
			} else if (t0 == 1) {
				TFC_BAT_LED0_OFF;
				TFC_BAT_LED1_ON;
				TFC_BAT_LED2_OFF;
				TFC_BAT_LED3_OFF;
			} else if (t0 == 2) {
				TFC_BAT_LED0_OFF;
				TFC_BAT_LED1_OFF;
				TFC_BAT_LED2_ON;
				TFC_BAT_LED3_OFF;
			} else if (t0 == 3) {
				TFC_BAT_LED0_OFF;
				TFC_BAT_LED1_OFF;
				TFC_BAT_LED2_OFF;
				TFC_BAT_LED3_ON;
			}
			//TFC_SetServo(t0);
			
			
			/*
			if (t1 > 0) {
				//TFC_BAT_LED0_ON;
				TFC_BAT_LED1_ON;
				//TFC_BAT_LED2_OFF;
				TFC_BAT_LED3_OFF;
			} else {
				//TFC_BAT_LED0_OFF;
				TFC_BAT_LED1_OFF;
				//TFC_BAT_LED2_ON;
				TFC_BAT_LED3_ON;
			}
			*/
			// Run MCP
			//MasterControlProgram();
			//TFC_SetServo(-0.5);

		} else {
			
			
			// Run Demo Program
			//DemoProgram();
		}
	}
	return 0;
}

void Init_Interrupts() {
	EnableInterrupts;
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
