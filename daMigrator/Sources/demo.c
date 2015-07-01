
#include "demo.h"


//ACCEL

char accel[6];

//FEEDBACK

float iAver[2];
float vAver[2];
float iq, aq;

//OVCAM

volatile int counter = 0;
volatile int VSync = 0;
volatile int frameReady = 0;
int frame = 0;

char buffer1[VRES][HRES];
char buffer2[VRES][HRES];
volatile char (*outpointer)[HRES];

//OVHLAM

uint8_t ch;
//int n = 1;		
int i,j,k,m;
int q;


//SSENSORS

// ??

char packet[500];
int packet_index;

signed int ReadShort(char * r);
void Init_Demo() {

	//gain = 1;
	//exposureTime = 50000;

	iAver[0] = 0.3; 
	iAver[1] = 0.3;
	vAver[0] = 0.3; 
	vAver[1] = 0.3; 

	iq = 0.95;
	aq = 0.95;

	packet_index = 0;


}


int DemoProgram() {
	int c = 0;
	uint8_t sw;
	sw = (TFC_GetDIP_Switch()>>1)&0x07;
	switch(sw) {

	default:

		TFC_SetMotorPWM(0,0); //Make sure motors are off
		TFC_SetServo(0);
		TFC_HBRIDGE_DISABLE;

		TFC_BAT_LED0_OFF;
		TFC_BAT_LED1_OFF;
		TFC_BAT_LED2_OFF;
		TFC_BAT_LED3_OFF;

		//RED_LED_OFF;
		GREEN_LED_OFF;
		BLUE_LED_OFF;

		break;

	case 1: 

		TFC_BAT_LED0_OFF;
		TFC_BAT_LED1_OFF;
		TFC_BAT_LED2_ON;
		TFC_BAT_LED3_ON;
		//RED_LED_OFF;
		BLUE_LED_ON;
		GREEN_LED_OFF;

		if(TFC_PUSH_BUTTON_0_PRESSED) {
			InitSpeedControlPID();
			TFC_HBRIDGE_ENABLE;
		}

		if(TFC_PUSH_BUTTON_1_PRESSED) {
			TFC_SetMotorPWM(0,0);
			TFC_HBRIDGE_DISABLE;
		}		

		//MOTOR

		if(TFC_Ticker[0]>=20) {
			TFC_Ticker[0] = 0;

			TFC_SetServo(TFC_ReadPot(0));			
			GetSensorInfo();
			//TFC_SetMotorPWM(TFC_ReadPot(1),TFC_ReadPot(1));
			daCar.speedDesired = TFC_ReadPot(1)*5;
			SetSpeedControlPID();
		}

		//SERVOS

		if(TFC_Ticker[1]>=20) {
			TFC_Ticker[1] = 0;


		}

		//FEEDBACK

		if(TFC_Ticker[2]>=20) {

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



		//

		if(TFC_Ticker[3]>=20) {

			TFC_Ticker[3] = 0;


		}

		//ADC

		if(TFC_Ticker[4]>=20) {

			TFC_Ticker[4] = 0;

			runADC();	
		}

		break;

	case 3:

		TFC_BAT_LED0_ON;
		TFC_BAT_LED1_ON;
		TFC_BAT_LED2_ON;
		TFC_BAT_LED3_ON;
		//RED_LED_OFF;
		BLUE_LED_OFF;
		GREEN_LED_ON;

		if(TFC_PUSH_BUTTON_0_PRESSED) {
			InitSpeedControlPID();
			daCar.speedDesired = 2;
			TFC_Ticker[1] = 0;
			TFC_HBRIDGE_ENABLE;
			c = 1;
		}

		if(TFC_PUSH_BUTTON_1_PRESSED) {
			TFC_SetMotorPWM(0,0);
			TFC_HBRIDGE_DISABLE;
		}		

		//MOTOR

		if(TFC_Ticker[0]>=20) {
			TFC_Ticker[0] = 0;

			TFC_SetServo(TFC_ReadPot(0));			
			GetSensorInfo();
			//TFC_SetMotorPWM(TFC_ReadPot(1),TFC_ReadPot(1));
			//daCar.speedDesired = TFC_ReadPot(1)*5;
			if (daCar.wheelspeed_front > 2) {
				TFC_Ticker[1] = 0;
				c = 1;

			}
			if (daCar.speedDesired == 0 && daCar.wheelspeed_front < 0.1) {
				TFC_HBRIDGE_DISABLE;
			}

			SetSpeedControlPID();
			
			PrintSensorInfo();
			
			sprintf(packet, "%6d%6d%6d%6d", (int)(iAver[0]*1000),(int)(iAver[1]*1000),(int)(vAver[0]*1000),(int)(vAver[1]*1000));
			TERMINAL_PRINTF(packet);			
			uart_putchar(13);
			uart_putchar(10);
			
		}

		//SERVOS

		if(TFC_Ticker[1]>=1000) {
			if (c) {
				daCar.speedDesired = 0;
				TFC_Ticker[1] = 0;
				TFC_HBRIDGE_DISABLE;
			}
		}

		//FEEDBACK

		if(TFC_Ticker[2]>=20) {

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



		//

		if(TFC_Ticker[3]>=5) {
			TFC_Ticker[3] = 0;		

		}

		//ADC

		if(TFC_Ticker[4]>=20) {

			TFC_Ticker[4] = 0;

			runADC();	
		}

		break;



	case 2:


		TFC_BAT_LED0_ON;
		TFC_BAT_LED1_ON;
		TFC_BAT_LED2_OFF;
		TFC_BAT_LED3_OFF;
		//RED_LED_OFF;
		BLUE_LED_OFF;
		GREEN_LED_ON;


		//CAMERA

		if (VSync) {
			VSync = 0;
		}

		if (frameReady==1) {			

			uart_putchar(13);
			uart_putchar(10);

			for (i = 0; i<VRES; ++i) {
				for (j = 0; j<HRES/8*8; j+=8) {
					ch = 0;
					for (k = 0; k<8; ++k) {
						ch |= (outpointer[i][j+k] & 1) << k;
					}		
					uart_putchar(ch);
				}				
				if (j<HRES) {
					ch = 0;
					for (k=j, m=0; k<HRES; ++k, ++m) {
						ch |= (outpointer[i][k] & 1) << m;
					}

					uart_putchar(ch);
				}
			}		

			frameReady = 0;

			for (i = 0; i<10; ++i) {
				uart_putchar(9);
			}		

		}


		break;

	case 4:			//calibration

		
		GREEN_LED_OFF;
		BLUE_LED_OFF;
		RED_LED_OFF;

		if(TFC_PUSH_BUTTON_0_PRESSED) {

			for (c = 0; c < CMP_QUANTISATION_LEVELS; c++) {
				if (TFC_PUSH_BUTTON_1_PRESSED) {
					break;
				}
				CMP_DACCR_REG(CMP1_BASE_PTR) &= ~CMP_DACCR_VOSEL_MASK;
				CMP_DACCR_REG(CMP1_BASE_PTR) |= CMP_DACCR_VOSEL(c); 

				TFC_BAT_LED0_OFF;
				TFC_BAT_LED1_OFF;
				TFC_BAT_LED2_OFF;
				TFC_BAT_LED3_OFF;
				GREEN_LED_OFF;
				BLUE_LED_OFF;

				if ((c>>0) & 1) {
					TFC_BAT_LED0_ON;					
				}
				if ((c>>1) & 1) {
					TFC_BAT_LED1_ON;					
				}
				if ((c>>2) & 1) {
					TFC_BAT_LED2_ON;					
				}
				if ((c>>3) & 1) {
					TFC_BAT_LED3_ON;					
				}
				if ((c>>4) & 1) {
					GREEN_LED_ON;	
				}
				if ((c>>5) & 1) {
					BLUE_LED_ON;	
				}



				//CAMERA
				while (!frameReady) {
					//	asm("nop");
				}			
				frameReady = 0;

				while (!frameReady) {
					//	asm("nop");
				}			
				frameReady = 0;


				while (!frameReady) {
					//	asm("nop");				
				}

				uart_putchar(13);
				uart_putchar(10);
				uart_printint(c+10);

				for (i = 0; i<VRES; ++i) {
					for (j = 0; j<HRES/8*8; j+=8) {
						ch = 0;
						for (k = 0; k<8; ++k) {
							ch |= (outpointer[i][j+k] & 1) << k;
						}		
						uart_putchar(ch);
					}				
					if (j<HRES) {
						ch = 0;
						for (k=j, m=0; k<HRES; ++k, ++m) {
							ch |= (outpointer[i][k] & 1) << m;
						}

						uart_putchar(ch);
					}
				}		

				frameReady = 0;

				for (i = 0; i<10; ++i) {
					uart_putchar(9);
				}
			}
		}
		break;
	}
	return 0;
}

/*
	if (packet_index > 42 ) {		
		packet[packet_index] = 0;
		packet_index = 0;
		//TERMINAL_PRINTF(packet);
		//TERMINAL_PRINTF("\r\n");
	}
	return 0;
 */
