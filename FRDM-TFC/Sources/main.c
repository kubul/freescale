#include "derivative.h" /* include peripheral declarations */
#include "TFC\TFC.h"
#include "math.h"
#include "Spices.h"

int DemoProgram();

int main(void)
{
	TFC_Init();

	
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
	uint32_t t=0,i=0;

	int pixel, z;
	float gain = 1;
	float e;
	int exposureTime = 50000;


	//This Demo program will look at the middle 2 switch to select one of 4 demo modes.
	//Let's look at the middle 2 switches
	switch((TFC_GetDIP_Switch()>>1)&0x03)
	{
	default:

		TFC_HBRIDGE_DISABLE;
		break;

	case 0 :
		//Demo mode 0 just tests the switches and LED's
		if(TFC_PUSH_BUTTON_0_PRESSED)
			TFC_BAT_LED0_ON;
		else
			TFC_BAT_LED0_OFF;

		if(TFC_PUSH_BUTTON_1_PRESSED)
			TFC_BAT_LED3_ON;
		else
			TFC_BAT_LED3_OFF;


		if(TFC_GetDIP_Switch()&0x01)
			TFC_BAT_LED1_ON;
		else
			TFC_BAT_LED1_OFF;

		if(TFC_GetDIP_Switch()&0x08)
			TFC_BAT_LED2_ON;
		else
			TFC_BAT_LED2_OFF;

		break;

	case 1:

		//Demo mode 1 will just move the servos with the on-board potentiometers
		if(TFC_Ticker[0]>=20)
		{
			TFC_Ticker[0] = 0; //reset the Ticker
			//Every 20 mSeconds, update the Servos
			TFC_SetServo(0,TFC_ReadPot(0));
			TFC_SetServo(1,TFC_ReadPot(1));
		}
		//Let's put a pattern on the LEDs
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

		TFC_SetMotorPWM(0,0); //Make sure motors are off
		TFC_HBRIDGE_DISABLE;


		break;

	case 2 :

		//Demo Mode 2 will use the Pots to make the motors move
		TFC_HBRIDGE_ENABLE;
		TFC_SetMotorPWM(TFC_ReadPot(0),TFC_ReadPot(1));
		TERMINAL_PRINTF("\r\n");
		TERMINAL_PRINTF("A: %d, B: %d", TFC_ReadHBridgeFeedBack(0), TFC_ReadHBridgeFeedBack(1));
		//Let's put a pattern on the LEDs
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
		break;

	case 3 :
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
				pixel = LineScanImage0[i]>>4;
				z += pixel;
				TERMINAL_PRINTF("%c",pixel);

				if(i==127) {
					TERMINAL_PRINTF("\r\n");
				}
			}		
			/*
								if (serv < 1) {
									TFC_SetServo(0,serv);
									serv = serv+0.01;
								} else if (serv < 2) {
									TFC_SetServo(0,-1);
									serv = serv+0.01;
								} else {
									serv = -1;
									TFC_SetServo(0,serv);
								}

			 */

			z /= 128;
			e = 5 - log((float)z);
			gain += e;

			if (gain>12)
				gain = 12;
			if (gain<-12)
				gain = -12;

			exposureTime = 4000+(int)exp(gain);

			//sprintf(buff,"exp %d   z %d   e %d   gain %d   log(z) %d   exp(gain) %d\n\r", exposureTime, z, (int)(e*1000), (int)gain, (int)(log((float)z)*1000), (int)exp(gain));				
			//TERMINAL_PRINTF(buff);
			//TERMINAL_PRINTF("%d, ",exposureTime);

			TFC_SetLineScanExposureTime(exposureTime);

			/*
								for(i=0;i<128;i++)
								{
									TERMINAL_PRINTF("%X",LineScanImage1[i]);
									if(i==127)
										TERMINAL_PRINTF("\r\n",LineScanImage1[i]);
									else
										TERMINAL_PRINTF(",",LineScanImage1[i]);
								}			
			 */							

		}



		break;
	}


	return 0;
}
