
#include "demo.h"


//ACCEL

char accel[6];
uint8_t interrupt_source;

//CAMERA

uint32_t i=0;
int pixel, z, exposureTime;
float gain, e;
char buff[100];

//FEEDBACK

float iAver[2];
float vAver[2];
float iq, aq;


void Init_Demo() {

	gain = 1;
	exposureTime = 50000;
	
	iAver[0] = 0.3; 
	iAver[1] = 0.3;
	vAver[0] = 0.3; 
	vAver[1] = 0.3; 
	
	iq = 0.95;
	aq = 0.95;
}

int DemoProgram() {
	
	switch((TFC_GetDIP_Switch()>>1)&0x03) {
	
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

		if(TFC_Ticker[3] >= 10)
		{
			TFC_Ticker[3] = 0;

			interrupt_source = TFC_GetAccelInterrupts();
			//TERMINAL_PRINTF("\r\n");
			//TERMINAL_PRINTF("Interrupt pins: 1: %d, 2: %d", (int)(interrupt_source&0x1), (int)(interrupt_source>>1));

			if ((interrupt_source & 0x1) == 0) {
				TFC_SetMotorPWM(0,0); //Make sure motors are off
				TFC_HBRIDGE_DISABLE;

				interrupt_source = I2CReadRegister(MMA8451_I2C_ADDRESS, INT_SOURCE);
				interrupt_source = I2CReadRegister(MMA8451_I2C_ADDRESS, PULSE_SRC);
			}
			//Get accelerometer info

			I2CReadMultiRegisters(MMA8451_I2C_ADDRESS, OUT_X_MSB, 6, accel);

			//TERMINAL_PRINTF ("Axes acceleration: X: %d, Y: %d, Z: %d", readShort(accel), readShort(accel+2), readShort(accel+4));
			TERMINAL_PRINTF ("a%c%c%c", ReadShort(accel), ReadShort(accel+2), ReadShort(accel+4));
			TERMINAL_PRINTF("\r\n");
		}

		//CAMERA

		if((TFC_Ticker[4] >= 100) && LineScanImageReady==1) 
		{

			TFC_Ticker[4] = 0;

			LineScanImageReady=0;
			z = 0;
			TERMINAL_PRINTF("c");
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

			//sprintf(buff,"exp %d   z %d   e %d   gain %d   log(z) %d   exp(gain) %d\n\r", exposureTime, z, (int)(e*1000), (int)(gain*1000), (int)(log((float)z)*1000), (int)exp(gain));				
			//TERMINAL_PRINTF(buff);
			//TERMINAL_PRINTF("%d, ",exposureTime);
			//TERMINAL_PRINTF("\r\n");

		}
		break;

	}
	return 0;
}

