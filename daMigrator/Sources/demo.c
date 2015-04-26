
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


char packet[500];
int packet_index;

signed int ReadShort(char * r);
void Init_Demo() {

	gain = 1;
	exposureTime = 50000;

	iAver[0] = 0.3; 
	iAver[1] = 0.3;
	vAver[0] = 0.3; 
	vAver[1] = 0.3; 

	iq = 0.95;
	aq = 0.95;
	
	packet_index = 0;

}


int DemoProgram() {

	switch((TFC_GetDIP_Switch()>>1)&0x03) {

	default:

		TFC_SetMotorPWM(0,0); //Make sure motors are off
		TFC_SetServo(0);
		TFC_HBRIDGE_DISABLE;
		
		TFC_BAT_LED0_OFF;
		TFC_BAT_LED1_OFF;
		TFC_BAT_LED2_OFF;
		TFC_BAT_LED3_OFF;

		break;

	case 1:

		if(TFC_PUSH_BUTTON_0_PRESSED) {
			TFC_HBRIDGE_ENABLE;
		}

		if(TFC_PUSH_BUTTON_1_PRESSED) {
			TFC_SetMotorPWM(0,0);
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

			TFC_SetServo(TFC_ReadPot(0));
		}

		//FEEDBACK

		if(TFC_Ticker[2]>=20)
		{
			TFC_Ticker[2] = 0;

			iAver[0] = iAver[0]*iq + (1.0-iq)*(TFC_ReadHBridgeFeedBack(0)-iAver[0]);
			iAver[1] = iAver[1]*iq + (1.0-iq)*(TFC_ReadHBridgeFeedBack(1)-iAver[1]);

			vAver[0] = vAver[0]*aq + (1.0-aq)*(TFC_ReadBatteryVoltage()*TFC_ReadPot(1)-vAver[0]);
			vAver[1] = vAver[1]*aq + (1.0-aq)*(TFC_ReadBatteryVoltage()*TFC_ReadPot(1)-vAver[1]);

			if (packet_index==18) {
				sprintf(packet+packet_index, "%6d%6d%6d%6d", (int)(iAver[0]*1000),(int)(iAver[1]*1000),(int)(vAver[0]*1000),(int)(vAver[1]*1000));
				packet_index = packet_index + 24;
				//TERMINAL_PRINTF("%d", (int)(iAver[0]*1000));
				//TERMINAL_PRINTF("\r\n");
			}

		}

		//ACCELEROMETER

		if(TFC_Ticker[3] >= 10)
		{
			TFC_Ticker[3] = 0;

			//interrupt_source = TFC_GetAccelInterrupts(); Still exists as example of reading GPIO

			//TERMINAL_PRINTF("\r\n");
			//TERMINAL_PRINTF("Interrupt pins: 1: %d, 2: %d", (int)(interrupt_source&0x1), (int)(interrupt_source>>1));

			//Get accelerometer info

			I2CReadMultiRegisters(MMA8451_I2C_ADDRESS, OUT_X_MSB, 6, accel);

			if (packet_index == 0) {
				sprintf(packet+packet_index, "%6d%6d%6d", ReadShort(accel), ReadShort(accel+2), ReadShort(accel+4));
				packet_index = packet_index + 18;
				//TERMINAL_PRINTF ("Axes acceleration: X: %d, Y: %d, Z: %d", readShort(accel), readShort(accel+2), readShort(accel+4));
				//TERMINAL_PRINTF("a);
				//TERMINAL_PRINTF("\r\n");
			}
		}

		//CAMERA

		if((TFC_Ticker[4] >= 100) && LineScanImageReady==1) 
		{

			TFC_Ticker[4] = 0;

			LineScanImageReady=0;
			z = 0;
			//TERMINAL_PRINTF("c");
			for(i=0;i<128;i++)
			{
				pixel = LineScanImage0[i]>>4;
				z += pixel;

				if (packet_index > 41) {
					packet[packet_index++] = pixel;
				}
				//TERMINAL_PRINTF("%c",pixel);

				if(i==127) {
					//TERMINAL_PRINTF("\r\n");
				}
			}

			z /= 128;
			e = 5 - log((float)z);
			gain += e;

			if (gain>10)
				gain = 10;
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
		
	case 2:
		

		
		break;
		
	case 3:
		

		
		break;
		
	case 4:
		

		
		break;
		
	}
	if (packet_index > 42 ) {		
		packet[packet_index] = 0;
		packet_index = 0;
		TERMINAL_PRINTF(packet);
		TERMINAL_PRINTF("\r\n");
	}
	return 0;
}

