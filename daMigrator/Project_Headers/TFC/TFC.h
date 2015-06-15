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
#include "TFC\TFC_Servo.h"
#include "TFC\TFC_Motor.h"
#include "TFC\TFC_ADC.h"
#include "TFC\TFC_LineScanCamera.h"
#include "TFC\TFC_Queue.h"
#include "TFC\TFC_UART.h"
#include "TFC\TFC_Terminal.h"
#include "TFC\TFC_i2c.h"
#include "TFC\TFC_DMA.h"



void TFC_Task();
void TFC_Init();


#endif /* TFC_H_ */
