#include "TFC\TFC.h"


volatile uint16_t  *LineScanImage0WorkingBuffer;
volatile uint16_t  *LineScanImage0;
volatile uint16_t  LineScanImage0Buffer[2][128];
volatile uint8_t  LineScanWorkingBuffer;

volatile uint8_t LineScanImageReady = 0;

void TFC_InitLineScanCamera()
{
	PORTC_PCR4 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   //Enable GPIO on on the pin for the CLOCK Signal
	PORTD_PCR1 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   //Enable GPIO on on the pin for SI signal
	
	//PORTD_PCR1 = PORT_PCR_MUX(0); //Make sure AO signal goes to an analog input
		
	//Make sure the Clock and SI pins are outputs
    GPIOC_PDDR |= (1<<4);
    GPIOD_PDDR |= (1<<1);
            
	TAOS_CLK_LOW;
	TAOS_SI_LOW;

	LineScanWorkingBuffer = 0;
	
	LineScanImage0WorkingBuffer = &LineScanImage0Buffer[0][0];
	
	LineScanImage0 = &LineScanImage0Buffer[1][0];
}


void TFC_SetLineScanExposureTime(uint32_t  TimeIn_uS)
{
		float t;
		
		//Figure out how many Pit ticks we need for for the exposure time
		t = (TimeIn_uS /1000000.0) * (float)(PERIPHERAL_BUS_CLOCK);
		PIT_LDVAL0 = (uint32_t)t;
	
}
