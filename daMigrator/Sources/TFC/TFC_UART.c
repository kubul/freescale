#include "TFC\TFC.h"

void UART0_init (int sysclk, int baud);

ByteQueue SDA_SERIAL_OUTGOING_QUEUE;
ByteQueue SDA_SERIAL_INCOMING_QUEUE;

uint8_t SDA_SERIAL_OUTGOING_QUEUE_Storage[SDA_SERIAL_OUTGOING_QUEUE_SIZE];
uint8_t SDA_SERIAL_INCOMING_QUEUE_Storage[SDA_SERIAL_INCOMING_QUEUE_SIZE];

void TFC_InitUARTs()
{
	
	InitByteQueue(&SDA_SERIAL_OUTGOING_QUEUE,SDA_SERIAL_OUTGOING_QUEUE_SIZE,SDA_SERIAL_OUTGOING_QUEUE_Storage);
	InitByteQueue(&SDA_SERIAL_INCOMING_QUEUE,SDA_SERIAL_INCOMING_QUEUE_SIZE,SDA_SERIAL_INCOMING_QUEUE_Storage);
		
	//We have to feed this function the clock in KHz!
	UART0_init(CORE_CLOCK, SDA_SERIAL_BAUD);
	
	//Enable recieve interrupts
	UART0_C2 |= UART_C2_RIE_MASK;
	enable_irq(INT_UART0_RX_TX-16);

}

void TFC_UART_Process()
{
	if(BytesInQueue(&SDA_SERIAL_OUTGOING_QUEUE)>0 && (UART0_S1 & UART_S1_TDRE_MASK))
		UART0_C2 |= UART_C2_TIE_MASK; //Enable Transmitter Interrupts
}


void UART0_Status_IRQHandler()
{
	uint8_t Temp;

	if(UART0_S1 & UART_S1_RDRF_MASK)
	{
		ByteEnqueue(&SDA_SERIAL_INCOMING_QUEUE,UART0_D);
	}
	if(UART0_S1 & UART_S1_TDRE_MASK)
	{
		if(BytesInQueue(&SDA_SERIAL_OUTGOING_QUEUE)>0)
		{
			ByteDequeue(&SDA_SERIAL_OUTGOING_QUEUE,&Temp);
			UART0_D = Temp;
		}
		else
		{
			//if there is nothing left in the queue then disable interrupts
			UART0_C2 &= ~UART_C2_TIE_MASK; //Disable the  Interrupts
		}
	}
}


void UART0_init (int sysclk, int baud)
{
	uint32 sbr_val;
	uint32 baud_rate;
	uint32 temp = 0;
	uint32 brfa = 0;

	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;      /*Enable the UART clock*/
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;		/*Enable the PORTB clock*/
	PORTB_PCR16 |= PORT_PCR_MUX(3);
	PORTB_PCR17 |= PORT_PCR_MUX(3);

	// Disable UART1 before changing registers
	UART0_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	/* Configure the UART for 8-bit mode, no parity */
	UART0_C1 = 0; 		/*Dafault settings of the register*/
	
	// Initialize baud rate
	baud_rate = baud;

	/* Calculate baud settings */
	sbr_val = (uint16)(sysclk/(baud_rate * 16));

	/* Save off the current value of the UARTx_BDH except for the SBR field */
	temp = UART0_BDH & ~(UART_BDH_SBR(0x1F));

	UART0_BDH = temp |  UART_BDH_SBR(((sbr_val & 0x1F00) >> 8));
	UART0_BDL = (uint8)(sbr_val & UART_BDL_SBR_MASK);

	/* Determine if a fractional divider is needed to get closer to the baud rate */
	brfa = (((sysclk*2)/baud_rate) - (sbr_val * 32));

	/* Save off the current value of the UARTx_C4 register except for the BRFA field */
	temp = UART0_C4 & ~(UART_C4_BRFA(0x1F));

	UART0_C4 = temp |  UART_C4_BRFA(brfa);    

	/* Enable receiver and transmitter */
	UART0_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);

}

/********************************************************************/
/*
 * Wait for a character to be received on the specified uart
 *
 * Parameters:
 *  channel      uart channel to read from
 *
 * Return Values:
 *  the received character
 */
char uart_getchar (UART_MemMapPtr channel)
{
	/* Wait until character has been received */
	while (!(UART_S1_REG(channel) & UART_S1_RDRF_MASK));

	/* Return the 8-bit data from the receiver */
	return UART_D_REG(channel);
}
/********************************************************************/
/*
 * Wait for space in the uart Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      uart channel to send to
 *  ch			 character to send
 */ 
void uart_putchar (UART_MemMapPtr channel, char ch)
{
	/* Wait until space is available in the FIFO */
	while(!(UART_S1_REG(channel) & UART_S1_TDRE_MASK));

	/* Send the character */
	UART_D_REG(channel) = (uint8)ch;

}
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      uart channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart_getchar_present (UART_MemMapPtr channel)
{
	return (UART_S1_REG(channel) & UART_S1_RDRF_MASK);
}
/********************************************************************/


