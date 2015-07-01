#include "TFC/TFC.h"


//#define PIT_ENABLE(channel)		PIT_TCTRL_REG(PIT_BASE_PTR,channel) |= PIT_TCTRL_TEN_MASK;
//#define PIT_DISABLE(channel)	PIT_TCTRL_REG(PIT_BASE_PTR,channel) &= ~PIT_TCTRL_TEN_MASK;

volatile uint32_t *fbuffer;
volatile uint32_t *lbuffer;
volatile uint32_t *rbuffer;

volatile uint32_t *wfbuffer;
volatile uint32_t *wlbuffer;
volatile uint32_t *wrbuffer;

uint32_t fbuffer1[SBUFSIZE];
uint32_t lbuffer1[SBUFSIZE];
uint32_t rbuffer1[SBUFSIZE];

uint32_t fbuffer2[SBUFSIZE];
uint32_t lbuffer2[SBUFSIZE];
uint32_t rbuffer2[SBUFSIZE];

uint16_t fcounter;
uint16_t lcounter;
uint16_t rcounter;

DMA_TCD_SETTING	SFRONT;
DMA_TCD_SETTING SLEFT;
DMA_TCD_SETTING SRIGHT;

uint8_t firstS = 1;

float fperiod = MAX_PERIOD;
float fdperiod = 0.001;
float lperiod = MAX_PERIOD;
float ldperiod = 0.001;
float rperiod = MAX_PERIOD;
float rdperiod = 0.001;
uint32_t flastPIT 		= (uint32_t)(STARTPIT);
uint32_t flastPITzero 	= (uint32_t)(STARTPIT);
uint32_t llastPIT 		= (uint32_t)(STARTPIT);
uint32_t llastPITzero 	= (uint32_t)(STARTPIT);
uint32_t rlastPIT 		= (uint32_t)(STARTPIT);
uint32_t rlastPITzero 	= (uint32_t)(STARTPIT);
uint32_t before_reset_PIT = (uint32_t)(STARTPIT);

void TFC_InitSensorCapture() {

	fbuffer = fbuffer1;
	lbuffer = lbuffer1;
	rbuffer = rbuffer1;

	PORTC_PCR2   = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x1); // front 
	PORTA_PCR2   = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x1); // left
	PORTB_PCR23  = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0x1); // right

	InitPit(2, (uint32_t)STARTPIT, 0);
	PIT_TCTRL_REG(PIT_BASE_PTR,2) = PIT_TCTRL_TEN_MASK;

	ConfigureSensorDMA();

}

void ConfigureSensorDMA() {


	SFRONT.SADDR		= (uint32_t)(&PIT_CVAL2);
	SFRONT.SOFF			= 0x0;
	SFRONT.ATTR			= (DMA_ATTR_SSIZE(0x2) | DMA_ATTR_DSIZE(0x2)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	SFRONT.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x04);
	SFRONT.SLAST		= 0x0;
	SFRONT.DADDR		= (uint32_t)(fbuffer1);
	SFRONT.DOFF			= 0x04;
	SFRONT.CITER		= DMA_CITER_ELINKNO_CITER(SBUFSIZE);
	SFRONT.DLAST_SGA	= -0x0100;
	SFRONT.CSR			= 0x0;
	SFRONT.BITER		= DMA_BITER_ELINKNO_BITER(SBUFSIZE);

	SLEFT.SADDR			= (uint32_t)(&PIT_CVAL2);
	SLEFT.SOFF			= 0x0;
	SLEFT.ATTR			= (DMA_ATTR_SSIZE(0x2) | DMA_ATTR_DSIZE(0x2)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	SLEFT.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x04);
	SLEFT.SLAST			= 0x0;
	SLEFT.DADDR			= (uint32_t)(lbuffer1);
	SLEFT.DOFF			= 0x04;
	SLEFT.CITER			= DMA_CITER_ELINKNO_CITER(SBUFSIZE);
	SLEFT.DLAST_SGA		= -0x0100;
	SLEFT.CSR			= 0x0;
	SLEFT.BITER			= DMA_BITER_ELINKNO_BITER(SBUFSIZE);

	SRIGHT.SADDR		= (uint32_t)(&PIT_CVAL2);
	SRIGHT.SOFF			= 0x0;
	SRIGHT.ATTR			= (DMA_ATTR_SSIZE(0x2) | DMA_ATTR_DSIZE(0x2)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	SRIGHT.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x04);
	SRIGHT.SLAST		= 0x0;
	SRIGHT.DADDR		= (uint32_t)(rbuffer1);
	SRIGHT.DOFF			= 0x04;
	SRIGHT.CITER		= DMA_CITER_ELINKNO_CITER(SBUFSIZE);
	SRIGHT.DLAST_SGA	= -0x0100;
	SRIGHT.CSR			= 0x0;
	SRIGHT.BITER		= DMA_BITER_ELINKNO_BITER(SBUFSIZE);

	memcpy(&(DMA_BASE_PTR->TCD[SFRONT_index]),	&SFRONT,	sizeof(DMA_TCD_SETTING));
	memcpy(&(DMA_BASE_PTR->TCD[SLEFT_index]), 	&SLEFT, 	sizeof(DMA_TCD_SETTING));
	memcpy(&(DMA_BASE_PTR->TCD[SRIGHT_index]), 	&SRIGHT,	sizeof(DMA_TCD_SETTING));

	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,SFRONT_index)    	= DMAMUX_CHCFG_SOURCE(DMA_PORTC);     // DMA source DMA Mux to tie source to DMA channel
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,SLEFT_index)    	= DMAMUX_CHCFG_SOURCE(DMA_PORTA);	 // DMA source DMA Mux to tie source to DMA channel
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,SRIGHT_index)    	= DMAMUX_CHCFG_SOURCE(DMA_PORTB);     // DMA source DMA Mux to tie source to DMA channel

	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,SFRONT_index)	|= DMAMUX_CHCFG_ENBL_MASK;
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,SLEFT_index) 	|= DMAMUX_CHCFG_ENBL_MASK;
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,SRIGHT_index) 	|= DMAMUX_CHCFG_ENBL_MASK;

	DMA_ERQ_REG(DMA_BASE_PTR) |= DMA_ERQ_ERQ6_MASK | DMA_ERQ_ERQ7_MASK | DMA_ERQ_ERQ8_MASK;


}

void GetSensorInfo() {	

	if (firstS) {

		fbuffer = fbuffer2;
		lbuffer = lbuffer2;
		rbuffer = rbuffer2;

		wfbuffer = fbuffer1;
		wlbuffer = lbuffer1;
		wrbuffer = rbuffer1;

		firstS = 0;			

	} else {

		fbuffer = fbuffer1;
		lbuffer = lbuffer1;
		rbuffer = rbuffer1;

		wfbuffer = fbuffer2;
		wlbuffer = lbuffer2;
		wrbuffer = rbuffer2;
		
		firstS = 1;
	}

	DMA_ERQ_REG(DMA_BASE_PTR) &= ~DMA_ERQ_ERQ6_MASK & ~DMA_ERQ_ERQ7_MASK & ~DMA_ERQ_ERQ8_MASK;

	fcounter = DMA_BASE_PTR->TCD[SFRONT_index].CITER_ELINKNO;
	lcounter = DMA_BASE_PTR->TCD[SLEFT_index].CITER_ELINKNO;
	rcounter = DMA_BASE_PTR->TCD[SRIGHT_index].CITER_ELINKNO;

	DMA_CITER_ELINKNO_REG(DMA_BASE_PTR, SFRONT_index) = DMA_CITER_ELINKNO_CITER(SBUFSIZE);
	DMA_DADDR_REG(DMA_BASE_PTR, SFRONT_index) = DMA_DADDR_DADDR(fbuffer);

	DMA_CITER_ELINKNO_REG(DMA_BASE_PTR, SLEFT_index) = DMA_CITER_ELINKNO_CITER(SBUFSIZE);
	DMA_DADDR_REG(DMA_BASE_PTR, SLEFT_index) = DMA_DADDR_DADDR(lbuffer);

	DMA_CITER_ELINKNO_REG(DMA_BASE_PTR, SRIGHT_index) = DMA_CITER_ELINKNO_CITER(SBUFSIZE);
	DMA_DADDR_REG(DMA_BASE_PTR, SRIGHT_index) = DMA_DADDR_DADDR(rbuffer);

	RestartPIT();

	DMA_ERQ_REG(DMA_BASE_PTR) |= DMA_ERQ_ERQ6_MASK | DMA_ERQ_ERQ7_MASK | DMA_ERQ_ERQ8_MASK;

	CalculateSensor(fcounter, wfbuffer, &fperiod, &fdperiod, &flastPIT, &flastPITzero);
	CalculateSensor(lcounter, wlbuffer, &lperiod, &ldperiod, &llastPIT, &llastPITzero);
	CalculateSensor(rcounter, wrbuffer, &rperiod, &rdperiod, &rlastPIT, &rlastPITzero);	

	daCar.wheelspeed_front = 4/fperiod/1000;
	daCar.wheelspeed_left  = 4/lperiod/1000;
	daCar.wheelspeed_right = 4/rperiod/1000;
	
}

void PrintSensorInfo() {
	uint32_t temp = 0;
		
	temp = (uint32_t)(1000*daCar.wheelspeed_front);		
	uart_printint(temp);
	uart_putchar(32);
	
	temp = (uint32_t)(1000*daCar.wheelspeed_left);	
	uart_printint(temp);	
	uart_putchar(32);
	
	temp = (uint32_t)(1000*daCar.wheelspeed_right);		
	uart_printint(temp);
	uart_putchar(32);
	
}

void RestartPIT() {
	before_reset_PIT = PIT_CVAL2;
	flastPIT 		+= STARTPIT-before_reset_PIT;
	flastPITzero 	+= STARTPIT-before_reset_PIT;
	llastPIT 		+= STARTPIT-before_reset_PIT;
	llastPITzero 	+= STARTPIT-before_reset_PIT;
	rlastPIT 		+= STARTPIT-before_reset_PIT;
	rlastPITzero 	+= STARTPIT-before_reset_PIT;

	PIT_TCTRL_REG(PIT_BASE_PTR,2) &= ~PIT_TCTRL_TEN_MASK;	//disable PIT2
	PIT_TCTRL_REG(PIT_BASE_PTR,2) |= PIT_TCTRL_TEN_MASK;	//enable PIT2

}


void CalculateSensor(uint16_t counter, volatile uint32_t *buff, float *period, float *dperiod, uint32_t *lastPIT, uint32_t *lastPITzero) {
	float newperiod;
	float currperiod = *period;
	float d = *dperiod;
	uint32_t currentPIT = before_reset_PIT - (STARTPIT - PIT_CVAL2);

	counter = SBUFSIZE - counter;

	if (counter == 0) {
		newperiod = currperiod + d*(*lastPITzero-PIT_CVAL2)*TPIT;
		(*lastPITzero) = PIT_CVAL2;
		if (newperiod > MAX_PERIOD ||
				(*lastPIT-STARTPIT)*TPIT > MAX_PERIOD) {			// too slow			
			newperiod = MAX_PERIOD;
			d = 0.001;
			*lastPIT = (uint32_t)(STARTPIT);
			*lastPITzero = (uint32_t)(STARTPIT);
			
			
			newperiod = MAX_PERIOD;
			d = 0.001;
		}
	} else { 
		if (counter == 1) {
			newperiod = TPIT*((*lastPIT)-buff[0]);
			d = newperiod-currperiod;
			newperiod = newperiod + d*(buff[0]-currentPIT)*TPIT;
		} else if (counter <= SENSOR_AVERAGING_THRESH) {
			newperiod = (((*lastPIT)-buff[counter-1])*TPIT)/counter;
			d = newperiod - currperiod;			
		} else {			//take average of last SENSOR_AVERAGING_THRESH samples
			newperiod = ((buff[counter-1-SENSOR_AVERAGING_THRESH] - buff[counter-1])*TPIT)/SENSOR_AVERAGING_THRESH;
			d = newperiod - currperiod;  //TODO
		}
		(*lastPIT) = buff[counter-1];
		(*lastPITzero) = (*lastPIT);
	}
	*period = newperiod;
	*dperiod = d;
	/*uart_putchar(32);
	uart_printint(counter);
	uart_putchar(32);
	uart_printint((*lastPIT));
	uart_putchar(32);
	uart_printint((*lastPITzero));
	uart_putchar(32);
	uart_printint(PIT_CVAL2);
	*/
}
