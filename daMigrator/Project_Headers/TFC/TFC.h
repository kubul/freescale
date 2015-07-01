/*
 * TFC.h
 */

#ifndef TFC_H_
#define TFC_H_

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "Derivative.h"
#include "TFC\TFC_Config.h"
#include "TFC\TFC_Types.h"
#include "TFC\TFC_BoardSupport.h"
#include "TFC\TFC_SysTick.h"
#include "TFC\TFC_arm_cm4.h"
#include "TFC\TFC_CrystalClock.h"
#include "TFC\TFC_PIT.h"
#include "TFC\TFC_Servo.h"
#include "TFC\TFC_Motor.h"
#include "TFC\TFC_ADC.h"
//#include "TFC\TFC_LineScanCamera.h"
#include "TFC\TFC_Queue.h"
#include "TFC\TFC_UART.h"
#include "TFC\TFC_Terminal.h"
#include "TFC\TFC_i2c.h"
#include "TFC\TFC_DMA.h"
#include "TFC\TFC_OVCamera.h"
#include "TFC\TFC_SpeedSensors.h"
#include "TFC\TFC_Controller.h"
#include "TFC\TFC_Controller_PID.h"

void TFC_Task();
void TFC_Init();

#define CAR_WHEELBASE		0.20			// front to rear axis [m]
#define a					0 //huy
#define b					0 //huy
#define CAR_TRACK_WIDTH		0.136			// left to right wheel [m]
#define CAR_MASS			1.500			// [kg]

volatile struct {		
	float steeringAngle;
	float speed;
	float speedDesired;
	float wheelspeed_right;
	float wheelspeed_left;
	float wheelspeed_front;	
} daCar;


#endif /* TFC_H_ */
