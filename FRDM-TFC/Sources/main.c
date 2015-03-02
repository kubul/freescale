#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"
#include "math.h"
#include "Spices.h"
#include "i2c.h"

int DemoProgram();
void Init_I2C();
signed int readShort(char * r);

int main(void)
{

	TFC_Init();
	Init_I2C();
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, 0x01); //start accel

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


int DemoProgram() {

	//ACCEL

	char accel[6];

	//CAMERA

	uint32_t t,i=0;
	int pixel, z;
	float gain = 1;
	float e;
	int exposureTime = 50000;	


	//FEEDBACK

	float iAver[2];
	float vAver[2];
	float iq = 0.95;
	float aq = 0.95;

	iAver[0] = 0.3; iAver[1] = 0.3;  
	vAver[0] = 0.3; vAver[1] = 0.3;



	switch((TFC_GetDIP_Switch()>>1)&0x03)

	{
	default:

		TFC_SetMotorPWM(0,0); //Make sure motors are off
		TFC_SetServo(0,0);
		TFC_HBRIDGE_DISABLE;

		break;

	case 1:

		if(TFC_PUSH_BUTTON_0_PRESSED) {
			TFC_HBRIDGE_ENABLE;
		}

		if(TFC_PUSH_BUTTON_1_PRESSED) {
			TFC_HBRIDGE_DISABLE;
		}


		//MOTOR

		if(TFC_Ticker[0]>=20)
		{
			TFC_Ticker[0] = 0;

			TFC_SetMotorPWM(TFC_ReadPot(1),TFC_ReadPot(1));
		}

		//SERVOS

		if(TFC_Ticker[1]>=20)
		{
			TFC_Ticker[1] = 0;

			TFC_SetServo(0,TFC_ReadPot(0));
		}

		//FEEDBACK

		if(TFC_Ticker[2]>=20)
		{
			TFC_Ticker[2] = 0;

			iAver[0] = iAver[0]*iq + (1.0-iq)*(TFC_ReadHBridgeFeedBack(0)-iAver[0]);
			iAver[1] = iAver[1]*iq + (1.0-iq)*(TFC_ReadHBridgeFeedBack(1)-iAver[1]);

			vAver[0] = vAver[0]*aq + (1.0-aq)*(TFC_ReadBatteryVoltage()*TFC_ReadPot(0)-vAver[0]);
			vAver[1] = vAver[1]*aq + (1.0-aq)*(TFC_ReadBatteryVoltage()*TFC_ReadPot(1)-vAver[1]);

			//TERMINAL_PRINTF("A = %d mA, B = %d mA,       Va = %d mV, Vb = %d mV", (int)(iAver[0]*1000),(int)(iAver[1]*1000),(int)(vAver[0]*1000),(int)(vAver[1]*1000));
			//TERMINAL_PRINTF("\r\n");

		}

		//ACCELEROMETER

		if(TFC_Ticker[3] >= 200)
		{
			TFC_Ticker[3] = 0;

			//Get accelerometer info

			I2CReadMultiRegisters(MMA8451_I2C_ADDRESS, OUT_X_MSB, 6, accel);
			//TERMINAL_PRINTF("\r\n");
			//TERMINAL_PRINTF ("Axes acceleration: X: %d, Y: %d, Z: %d", readShort(accel), readShort(accel+2), readShort(accel+4));
		}

		break;

		//CAMERA

		if((TFC_Ticker[4] >= 100) && LineScanImageReady==1) 
		{

			TFC_Ticker[4] = 0;

			LineScanImageReady=0;
			z = 0;

			if(t==0)
				t=3;
			else
				t--;

			for(i=0;i<128;i++)
			{
				pixel = LineScanImage0[i]>>4;
				z += pixel;
				TERMINAL_PRINTF("%c",pixel);

				if(i==127) {
					TERMINAL_PRINTF("\r\n");
				}
			}		

			z /= 128;
			e = 5 - log((float)z);
			gain += e;

			if (gain>12)
				gain = 12;
			if (gain<-12)
				gain = -12;

			exposureTime = 4000+(int)exp(gain);
			TFC_SetLineScanExposureTime(exposureTime);

			//sprintf(buff,"exp %d   z %d   e %d   gain %d   log(z) %d   exp(gain) %d\n\r", exposureTime, z, (int)(e*1000), (int)gain, (int)(log((float)z)*1000), (int)exp(gain));				
			//TERMINAL_PRINTF(buff);
			//TERMINAL_PRINTF("%d, ",exposureTime);

		}

	}
	return 0;
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
//Demo Mode 3 will be in Freescale Garage Mode.  It will beam data from the Camera to the 
//Labview Application
TFC_HBRIDGE_DISABLE;

if(TFC_Ticker[0]>100 && LineScanImageReady==1)
{
	TFC_Ticker[0] = 0;
	LineScanImageReady=0;
	z = 0;
	//TERMINAL_PRINTF("\r\n");
	//TERMINAL_PRINTF("L:");

	if(t==0)
		t=3;
	else
		t--;

	TFC_SetBatteryLED_Level(t);







						for(i=0;i<128;i++)
						{
							TERMINAL_PRINTF("%X",LineScanImage1[i]);
							if(i==127)
								TERMINAL_PRINTF("\r\n",LineScanImage1[i]);
							else
								TERMINAL_PRINTF(",",LineScanImage1[i]);
						}	
						}		
 */							


//accel crap

/* 
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

