#include "TFC\TFC.h"


//set I/O for H-BRIDGE enables, switches and LEDs
void TFC_InitGPIO()
{
	//enable Clocks to all ports
	
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

	//Setup Pins as GPIO
	PORTB_PCR20 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   //HBRIDGE ENABLE
	//PORTE_PCR20 = PORT_PCR_MUX(1);    //HBRIDGE FAULT
	
	//Port for Pushbuttons
	PORTE_PCR26 = PORT_PCR_MUX(1);   
	PORTB_PCR18 = PORT_PCR_MUX(1);   
	
	//Ports for DIP Switches
	PORTC_PCR5 = PORT_PCR_MUX(1); 
	PORTC_PCR7 = PORT_PCR_MUX(1);
	PORTC_PCR0 = PORT_PCR_MUX(1); 
	PORTC_PCR1 = PORT_PCR_MUX(1);
	
	//Ports for LEDs
	PORTC_PCR16 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   
	PORTC_PCR17 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   
	PORTB_PCR9 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   
	PORTB_PCR23 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   
	
	
	//Ports for accelerometer interrupts
	PORTC_PCR6 = PORT_PCR_MUX(1) | PORT_PCR_IRQC(0xA) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Set interrupt on falling edge for pullup
	PORTC_PCR13 = PORT_PCR_MUX(1);
	
	//Setup the output pins
    GPIOB_PDDR |=  TFC_HBRIDGE_EN_LOC;  
    GPIOC_PDDR |=  TFC_BAT_LED0_LOC | TFC_BAT_LED1_LOC;
    GPIOB_PDDR |=  TFC_BAT_LED2_LOC | TFC_BAT_LED3_LOC;

   // TFC_SetBatteryLED_Level(0);
    TFC_HBRIDGE_DISABLE;
    
}
/*

void TFC_SetBatteryLED_Level(uint8_t BattLevel)
{
	switch(BattLevel)
	{
		default:
		case 0:
			GPIOB_PCOR = 0xF<<8;
		break;
	
		case 1:
			GPIOB_PSOR = 0x01<<8;
			GPIOB_PCOR = ~(0x01<<8) & (0xF<<8);
		break;
		
		case 2:
			GPIOB_PSOR = 0x03<<8;
			GPIOB_PCOR = ~(0x03<<8) & (0xF<<8) ;
		break;
		
		case 3:
			GPIOB_PSOR = 0x07<<8;
			GPIOB_PCOR = ~(0x07<<8) & (0xF<<8);
		break;
		
		case 4:
			GPIOB_PSOR = 0x0F<<8;
		break;
		
	}
}
*/

uint8_t TFC_GetDIP_Switch()
{
	uint8_t DIP_Val=0;
	
	DIP_Val  = ((GPIOC_PDIR>>5) & 1) << 0;
    DIP_Val |= ((GPIOC_PDIR>>7) & 1) << 1;
    DIP_Val |= ((GPIOC_PDIR>>0) & 1) << 2;
    DIP_Val |= ((GPIOC_PDIR>>1) & 1) << 3;

	return DIP_Val;
	/* PTC
	5 - 1
	7 - 2
	0 - 3
	1 - 4
	*/
}

uint8_t TFC_GetAccelInterrupts()
{
	uint8_t Interrupts_Val = 0;
	
	Interrupts_Val = ((GPIOC_PDIR>>6) & 1) << 0;
	Interrupts_Val |= ((GPIOC_PDIR>>13) & 1) << 1;

	return Interrupts_Val;
}
