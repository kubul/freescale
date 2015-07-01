#include "TFC\TFC.h"

#ifndef TFC_ADC_H_
#define TFC_ADC_H_

void TFC_InitADCs();

void ADC0_IRQ();
void ADC1_IRQ();
void runADC();
	
float TFC_ReadPot(uint8_t Channel);
float TFC_ReadBatteryVoltage();
float TFC_ReadHBridgeFeedBack(uint8_t Channel);

#endif /* __ADC16_H__ */

