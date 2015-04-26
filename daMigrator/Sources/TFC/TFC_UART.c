#include "TFC\TFC.h"

void UART1_init (int sysclk, int baud);

ByteQueue SDA_SERIAL_OUTGOING_QUEUE;
ByteQueue SDA_SERIAL_INCOMING_QUEUE;


uint8_t SDA_SERIAL_OUTGOING_QUEUE_Storage[SDA_SERIAL_OUTGOING_QUEUE_SIZE];
uint8_t SDA_SERIAL_INCOMING_QUEUE_Storage[SDA_SERIAL_INCOMING_QUEUE_SIZE];


void TFC_InitUARTs()
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

	InitByteQueue(&SDA_SERIAL_OUTGOING_QUEUE,SDA_SERIAL_OUTGOING_QUEUE_SIZE,SDA_SERIAL_OUTGOING_QUEUE_Storage);
	InitByteQueue(&SDA_SERIAL_INCOMING_QUEUE,SDA_SERIAL_INCOMING_QUEUE_SIZE,SDA_SERIAL_INCOMING_QUEUE_Storage);

	PORTB_PCR16 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;   
	PORTB_PCR17 = PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;  

	/*
	//Select PLL/2 Clock
	SIM_SOPT2 &= ~(3<<26);
	SIM_SOPT2 |= SIM_SOPT2_UART1SRC(1); 
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK;
	*/
	
	//We have to feed this function the clock in KHz!
	UART1_init (CORE_CLOCK, SDA_SERIAL_BAUD);
	//Enable recieve interrupts

	UART1_C2 |= UART_C2_RIE_MASK;
	enable_irq(INT_UART1_RX_TX-16);

}

void TFC_UART_Process()
{
	if(BytesInQueue(&SDA_SERIAL_OUTGOING_QUEUE)>0 && (UART1_S1 & UART_S1_TDRE_MASK))
		UART1_C2 |= UART_C2_TIE_MASK; //Enable Transmitter Interrupts
}


void UART1_IRQHandler()
{
	uint8_t Temp;

	if(UART1_S1 & UART_S1_RDRF_MASK)
	{
		ByteEnqueue(&SDA_SERIAL_INCOMING_QUEUE,UART1_D);
	}
	if(UART1_S1 & UART_S1_TDRE_MASK)
	{
		if(BytesInQueue(&SDA_SERIAL_OUTGOING_QUEUE)>0)
		{
			ByteDequeue(&SDA_SERIAL_OUTGOING_QUEUE,&Temp);
			UART1_D = Temp;
		}
		else
		{
			//if there is nothing left in the queue then disable interrupts
			UART1_C2 &= ~UART_C2_TIE_MASK; //Disable the  Interrupts
		}
	}
}



void UART1_init (int sysclk, int baud)
{
	uint32 sbr_val;
	uint32 baud_rate;
	uint32 temp = 0;
	uint32 brfa = 0;

	SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;

	// Disable UART1 before changing registers
	UART1_C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

	// Initialize baud rate
	baud_rate = baud;

	/* Calculate baud settings */
	sbr_val = (uint16)(sysclk/(baud_rate * 16));

	/* Save off the current value of the UARTx_BDH except for the SBR field */
	temp = UART1_BDH & ~(UART_BDH_SBR(0x1F));

	UART1_BDH = temp |  UART_BDH_SBR(((sbr_val & 0x1F00) >> 8));
	UART1_BDL = (uint8)(sbr_val & UART_BDL_SBR_MASK);

	/* Determine if a fractional divider is needed to get closer to the baud rate */
	brfa = (((sysclk*32)/(baud_rate * 16)) - (sbr_val * 32));

	/* Save off the current value of the UARTx_C4 register except for the BRFA field */
	temp = UART1_C4 & ~(UART_C4_BRFA(0x1F));

	UART1_C4 = temp |  UART_C4_BRFA(brfa);    

	/* Enable receiver and transmitter */
	UART1_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);

	/* Enable receiver and transmitter */
	UART1_C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);

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


