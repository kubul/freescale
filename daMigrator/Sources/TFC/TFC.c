#include "TFC\TFC.h"


void TFC_Init()
{
	
	TFC_InitClock();
	TFC_InitSysTick();
	TFC_InitGPIO();
	
	TFC_InitServos();
	TFC_SetServo(0.0);
	
	TFC_InitMotorPWM();
	TFC_HBRIDGE_DISABLE;
	TFC_SetMotorPWM(0,0);
	
    TFC_InitADCs();
    //TFC_InitLineScanCamera();
    
    TFC_InitTerminal();
	TFC_InitUARTs();
	TFC_InitDMA();
	TFC_InitI2C();
	
	TFC_InitPIT();
	TFC_InitOVCamera();
	TFC_InitSensorCapture();

	StartPits();
	
	InitSpeedControl();
	
}

void TFC_Task()
{
	#if defined(TERMINAL_USE_SDA_SERIAL)
		TFC_UART_Process();
	#endif
	 
    TFC_ProcessTerminal();
}
