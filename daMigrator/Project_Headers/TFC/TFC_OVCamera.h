
#include "derivative.h"

#define HSWAIT_index 0
#define CMPEN_index 1
#define HSDIS_index 2
#define CMPDATA_index 3
#define CMPDIS_index 4
#define RSTADR_index 5

#define CMP_QUANTISATION_LEVELS 64

#define HSYNC_ENABLE			PORTD_PCR0 |=  PORT_PCR_IRQC(0x1)   // DMA rising  edge 
#define HSYNC_DISABLE			PORTD_PCR0 &= ~PORT_PCR_IRQC_MASK

#define VSYNC_ENABLE			PORTD_PCR2 |=  PORT_PCR_IRQC(0xA)   // IRQ falling  edge 
#define VSYNC_DISABLE			PORTD_PCR2 &= ~PORT_PCR_IRQC_MASK

#define PIT_ENABLE(channel)		PIT_TCTRL_REG(PIT_BASE_PTR,channel) |= PIT_TCTRL_TEN_MASK;
#define PIT_DISABLE(channel)	PIT_TCTRL_REG(PIT_BASE_PTR,channel) &= ~PIT_TCTRL_TEN_MASK;

//SQCIF	128 × 96
//QCIF	176 × 144

//CAM_VS	PTA1 / FTM0_CH6
//CAM_HS	PTA2 / FTM0_CH7
//CAM_D		PTC3 / CMP1_IN1

#define HRES 160
#define VRES 288
#define WAITVS 1112.64 //1112.64 us
#define WAITHS 6.6 //6.6 us
#define LINETIME 56.1 //56.1 us

void TFC_InitOVCamera();
void CMPInit();
void ConfigureCameraDMA();
