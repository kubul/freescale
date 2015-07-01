

#include "TFC/TFC.h"

extern volatile int VSync;
extern volatile int frameReady;
extern volatile int frame;
volatile int line, lineindex, pixel;

extern char buffer1[VRES][HRES];
extern char buffer2[VRES][HRES];
volatile char (*buffer)[HRES];
extern volatile char (*outpointer)[HRES];

uint8_t PITON = 0x1;
uint8_t PITOFF = 0x0;

DMA_TCD_SETTING	HSWAIT;
DMA_TCD_SETTING CMPEN;
DMA_TCD_SETTING HSDIS;
DMA_TCD_SETTING CMPDATA;
DMA_TCD_SETTING CMPDIS;
DMA_TCD_SETTING RSTADR;

int i, j, mod;
uint8_t firstC = 1;
int lineSkipPitDmaTriggerFlag = 0;

int huy;
int huy2;

void TFC_InitOVCamera() {

	buffer = buffer1;
	outpointer = buffer1;	
	
	// Enable clocks on Port A & C
	SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK;

	// Set VS port PTD2:      GPIO | Pull enable      | Pull-up
	PORTD_PCR2 = PORT_PCR_MUX(0x1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	// Set HS port PTD0:      GPIO | Pull enable      | Pull-up	
	PORTD_PCR0 = PORT_PCR_MUX(0x1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	// ports as input by default

	/* Clear interrupt status flag */
	//PORTD_ISFR = PORT_ISFR_ISF(1 << 0);  //clear PTA1
	PORTD_ISFR = PORT_ISFR_ISF(1 << 2);  //clear PTD2

	/* Enable GPIOA interrupts in NVIC */	
	enable_irq(INT_PORTD - 16);	
	//InitHSyncFTM();

	//TODO: DEBUG
	PORTC_PCR16 = PORT_PCR_MUX(0x1);
	PORTC_PCR17 = PORT_PCR_MUX(0x1);

	GPIOC_PDDR = (1 << 16) | (1 << 17) ;
	CMPInit();
	ConfigureCameraDMA();

	InitPit(0, (uint32_t)((float)PERIPHERAL_BUS_CLOCK/1000000*WAITVS), 1);    //1112.64 us
	InitPit(1, (uint32_t)((float)PERIPHERAL_BUS_CLOCK/1000000*WAITHS), 0);    //7 us
	InitPit(3, (uint32_t)((float)PERIPHERAL_BUS_CLOCK/1000000*LINETIME/HRES), 0);   //56.1u 

	//CURRENTCMPDATA = &CMPDATA1;

	HSYNC_DISABLE;
	VSYNC_ENABLE;

}

void PORTD_IRQHandler() {	
	//PORTA_ISFR = PORT_ISFR_ISF(1 << 1);  //clear PTA1
	//PORTA_ISFR = 0xFFFFFFFF; //clear all

		// VSync
		PIT_ENABLE(0);		//wait after VS		
		VSync = 1;
		++frame;
		if (frame == 45) {
			frame = 0;
			//HSYNC_ENABLE;			
		} else {			
		}

//		if (frame % 45 == 0) {
			if (!frameReady) {
				if (firstC) {
					outpointer  = buffer1;
					buffer 		= buffer2;
					
					firstC = 0;			
				} else {
					outpointer  = buffer2;
					buffer    	= buffer1;
					
					firstC = 1;
				}
				frameReady = 1;
			} else {
				//GPIOB_PCOR = (1 << 21);   //blue
			}
//		}
		DMA_ERQ_REG(DMA_BASE_PTR) &= ~DMA_ERQ_ERQ3_MASK;
		DMA_DADDR_REG(DMA_BASE_PTR, CMPDATA_index) = DMA_DADDR_DADDR(buffer);
		DMA_ERQ_REG(DMA_BASE_PTR) |= DMA_ERQ_ERQ3_MASK;

	PORTD_ISFR = 0xFFFFFFFF; //clear both irq flags

}




void PIT0_IRQHandler() {
	PIT_TFLG0 = PIT_TFLG_TIF_MASK;	
	//start frame
	HSYNC_ENABLE;
	PIT_DISABLE(0);  //disable after-VS timer		
	line = -1;
	pixel = 0;
}

void DMA5_IRQHandler() {
		
	DMA_CINT = DMA_CINT_CINT(0x05);
	HSYNC_DISABLE;
	
}

void CMPInit(void) {

	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	//Enable CMP clock
	SIM_SCGC4 |= (SIM_SCGC4_CMP_MASK );
	PORTC_PCR3 = PORT_PCR_MUX(0);

	// Configure CMP
	CMP_CR0_REG(CMP1_BASE_PTR)   = 0x00;  // Filter and digital hysteresis disabled
	//							   CMP enable      |  unfiltered outpt | high-speed         
	CMP_CR1_REG(CMP1_BASE_PTR)   = CMP_CR1_EN_MASK |  CMP_CR1_COS_MASK | CMP_CR1_PMODE_MASK;  
	CMP_FPR_REG(CMP1_BASE_PTR)   = 0x00;  // Filter disabled
	CMP_SCR_REG(CMP1_BASE_PTR)   = 0x00;  // disable dma and interrupts
	//							   vdd/64*(VOSEL+1)    | Vin2=vdd			  | DAC enable
	//			DACO = (V in /64) * (VOSEL[5:0] + 1) , so the DACO range is from V in /64 to V in .
	CMP_DACCR_REG(CMP1_BASE_PTR) = CMP_DACCR_VOSEL(62) | CMP_DACCR_VRSEL_MASK | CMP_DACCR_DACEN_MASK; 
	//							   minus - DAC out   | plus - IN1 		
	CMP_MUXCR_REG(CMP1_BASE_PTR) = CMP_MUXCR_MSEL(7) | CMP_MUXCR_PSEL(1);   

}
 

void ConfigureCameraDMA() 
{
	
	enable_irq(INT_DMA5-16);

	HSWAIT.SADDR		= (uint32_t)(&PITON);
	HSWAIT.SOFF			= 0x0;
	HSWAIT.ATTR			= (DMA_ATTR_SSIZE(0x0) | DMA_ATTR_DSIZE(0x0)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	HSWAIT.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x01);
	HSWAIT.SLAST		= 0x0;
	HSWAIT.DADDR		= (uint32_t)(&((PIT_BASE_PTR)->CHANNEL[1].TCTRL));
	HSWAIT.DOFF			= 0x0;
	HSWAIT.CITER		= DMA_CITER_ELINKNO_CITER((uint32_t)0x1);
	HSWAIT.DLAST_SGA	= 0x0;
	HSWAIT.CSR			= 0x0;
	HSWAIT.BITER		= DMA_BITER_ELINKNO_BITER((uint32_t)0x1);
	
	CMPEN.SADDR			= (uint32_t)(&PITON);
	CMPEN.SOFF			= 0x0;
	CMPEN.ATTR			= (DMA_ATTR_SSIZE(0x0) | DMA_ATTR_DSIZE(0x0)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	CMPEN.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x01);
	CMPEN.SLAST			= 0x0;
	CMPEN.DADDR			= (uint32_t)(&((PIT_BASE_PTR)->CHANNEL[3].TCTRL));
	CMPEN.DOFF			= 0x0;
	CMPEN.CITER			= DMA_CITER_ELINKNO_CITER(0x0001);
	CMPEN.DLAST_SGA		= 0x0;
	CMPEN.CSR			= DMA_CSR_MAJORELINK_MASK | DMA_CSR_MAJORLINKCH(HSDIS_index);
	CMPEN.BITER			= DMA_BITER_ELINKNO_BITER(0x0001);
	
	HSDIS.SADDR			= (uint32_t)(&PITOFF);
	HSDIS.SOFF			= 0x0;
	HSDIS.ATTR			= (DMA_ATTR_SSIZE(0x0) | DMA_ATTR_DSIZE(0x0)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	HSDIS.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x01);
	HSDIS.SLAST			= 0x0;
	HSDIS.DADDR			= (uint32_t)(&((PIT_BASE_PTR)->CHANNEL[1].TCTRL));
	HSDIS.DOFF			= 0x0;
	HSDIS.CITER			= DMA_CITER_ELINKNO_CITER(0x0001);
	HSDIS.DLAST_SGA		= 0x0;
	HSDIS.CSR			= 0x0;
	HSDIS.BITER			= DMA_BITER_ELINKNO_BITER(0x0001);

	CMPDATA.SADDR		= (uint32_t)(((uint32_t)CMP1_BASE_PTR)+0x03);
	CMPDATA.SOFF		= 0x0;
	CMPDATA.ATTR		= (DMA_ATTR_SSIZE(0x0) | DMA_ATTR_DSIZE(0x0)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	CMPDATA.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x01);
	CMPDATA.SLAST		= 0x0;
	CMPDATA.DADDR		= (uint32_t)(buffer1);
	CMPDATA.DOFF		= 0x01;
	CMPDATA.CITER		= DMA_CITER_ELINKNO_CITER((uint32_t)(HRES));
	CMPDATA.DLAST_SGA	= 0x0;
	CMPDATA.CSR			= DMA_CSR_MAJORELINK_MASK | DMA_CSR_MAJORLINKCH(CMPDIS_index);
	CMPDATA.BITER		= DMA_BITER_ELINKNO_BITER((uint32_t)(HRES));
	
	CMPDIS.SADDR		= (uint32_t)(&PITOFF);
	CMPDIS.SOFF			= 0x0;
	CMPDIS.ATTR			= (DMA_ATTR_SSIZE(0x0) | DMA_ATTR_DSIZE(0x0)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	CMPDIS.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x01);
	CMPDIS.SLAST		= 0x0;
	CMPDIS.DADDR		= (uint32_t)(&((PIT_BASE_PTR)->CHANNEL[3].TCTRL));
	CMPDIS.DOFF			= 0x0;
	CMPDIS.CITER		= DMA_CITER_ELINKNO_CITER(VRES);
	CMPDIS.DLAST_SGA	= 0x0;
	CMPDIS.CSR			= DMA_CSR_MAJORELINK_MASK | DMA_CSR_MAJORLINKCH(RSTADR_index);
	CMPDIS.BITER		= DMA_BITER_ELINKNO_BITER(VRES);
	
	RSTADR.SADDR		= (uint32_t)(&buffer);
	RSTADR.SOFF			= 0x0;
	RSTADR.ATTR			= (DMA_ATTR_SSIZE(0x02) | DMA_ATTR_DSIZE(0x02)); // 0:8bit; 1:16-bit, 2:32-bit, 4:16-byte, 5:32-byte
	RSTADR.NBYTES		= DMA_NBYTES_MLNO_NBYTES(0x04);
	RSTADR.SLAST		= 0x0;
	RSTADR.DADDR		= (uint32_t)(&(DMA_BASE_PTR->TCD[CMPDATA_index].DADDR));
	RSTADR.DOFF			= 0x0;
	RSTADR.CITER		= DMA_CITER_ELINKNO_CITER(0x0001);
	RSTADR.DLAST_SGA	= 0x0;
	RSTADR.CSR			= DMA_CSR_INTMAJOR_MASK;
	RSTADR.BITER		= DMA_BITER_ELINKNO_BITER(0x0001);

	memcpy(&(DMA_BASE_PTR->TCD[HSWAIT_index]),	&HSWAIT,	sizeof(DMA_TCD_SETTING));
	memcpy(&(DMA_BASE_PTR->TCD[CMPEN_index]), 	&CMPEN, 	sizeof(DMA_TCD_SETTING));
	memcpy(&(DMA_BASE_PTR->TCD[HSDIS_index]), 	&HSDIS,		sizeof(DMA_TCD_SETTING));
	memcpy(&(DMA_BASE_PTR->TCD[CMPDATA_index]),	&CMPDATA,	sizeof(DMA_TCD_SETTING));
	memcpy(&(DMA_BASE_PTR->TCD[CMPDIS_index]), 	&CMPDIS,  	sizeof(DMA_TCD_SETTING));
	memcpy(&(DMA_BASE_PTR->TCD[RSTADR_index]),	&RSTADR, 	sizeof(DMA_TCD_SETTING));

	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,HSWAIT_index)    	= DMAMUX_CHCFG_SOURCE(DMA_PORTD);     // DMA source DMA Mux to tie source to DMA channel
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,CMPEN_index)    	= DMAMUX_CHCFG_SOURCE(DMA_AE0) | DMAMUX_CHCFG_TRIG_MASK;     // DMA source DMA Mux to tie source to DMA channel
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,HSDIS_index)    	= DMAMUX_CHCFG_SOURCE(DMA_DAC0);     // DMA source DMA Mux to tie source to DMA channel
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,CMPDATA_index)    	= DMAMUX_CHCFG_SOURCE(DMA_AE1) | DMAMUX_CHCFG_TRIG_MASK;     // DMA source DMA Mux to tie source to DMA channel
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,CMPDIS_index)    	= DMAMUX_CHCFG_SOURCE(DMA_DAC0);     // DMA source DMA Mux to tie source to DMA channel
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,RSTADR_index)    	= DMAMUX_CHCFG_SOURCE(DMA_DAC0);     // DMA source DMA Mux to tie source to DMA channel

	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,HSWAIT_index)	|= DMAMUX_CHCFG_ENBL_MASK;
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,CMPEN_index) 	|= DMAMUX_CHCFG_ENBL_MASK;
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,HSDIS_index) 	|= DMAMUX_CHCFG_ENBL_MASK;
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,CMPDATA_index) |= DMAMUX_CHCFG_ENBL_MASK;
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,CMPDIS_index) 	|= DMAMUX_CHCFG_ENBL_MASK;
	DMAMUX_CHCFG_REG(DMAMUX_BASE_PTR,RSTADR_index)  |= DMAMUX_CHCFG_ENBL_MASK;

	DMA_ERQ_REG(DMA_BASE_PTR) |= DMA_ERQ_ERQ0_MASK | DMA_ERQ_ERQ1_MASK | DMA_ERQ_ERQ2_MASK | DMA_ERQ_ERQ3_MASK | DMA_ERQ_ERQ4_MASK | DMA_ERQ_ERQ5_MASK;

}


