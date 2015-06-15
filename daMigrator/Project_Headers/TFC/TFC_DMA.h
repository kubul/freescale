/*
 * TFC_DMA.h
 *
 *  Created on: Apr 30, 2015
 *      Author: p746
 */

#ifndef TFC_DMA_H_
#define TFC_DMA_H_

//DMA MUX request sources

// RESERVED 1
#define DMA_UART0_RX 	2
#define DMA_UART0_TX 	3
#define DMA_UART1_RX 	4
#define DMA_UART1_TX 	5
#define DMA_UART2_RX 	6
#define DMA_UART2_TX 	7
#define DMA_UART3_RX 	8
#define DMA_UART3_TX 	9
#define DMA_UART4_RXTX 	10
#define DMA_UART5_RXTX 	11
#define DMA_I2S0_RX 	12
#define DMA_I2S0_TX 	13
#define DMA_SPI0_RX 	14
#define DMA_SPI0_TX 	15
#define DMA_SPI1_RXTX	16
#define DMA_SPI2_RXTX 	17
#define DMA_I2C0 		18	
#define DMA_I2C1 		19 // SAME CHANNEL FOR C1 and C2
#define DMA_I2C2 		19 // SAME CHANNEL FOR C1 and C2
#define DMA_FTM0_CH0 	20
#define DMA_FTM0_CH1 	21
#define DMA_FTM0_CH2 	22
#define DMA_FTM0_CH3 	23
#define DMA_FTM0_CH4 	24
#define DMA_FTM0_CH5 	25
#define DMA_FTM0_CH6 	26
#define DMA_FTM0_CH7 	27
#define DMA_FTM1_CH0 	28
#define DMA_FTM1_CH1 	29
#define DMA_FTM2_CH0 	30
#define DMA_FTM2_CH1 	31
#define DMA_FTM3_CH0 	32
#define DMA_FTM3_CH1 	33
#define DMA_FTM3_CH2 	34
#define DMA_FTM3_CH3 	35
#define DMA_FTM3_CH4 	36
#define DMA_FTM3_CH5 	37
#define DMA_FTM3_CH6 	38
#define DMA_FTM3_CH7 	39
#define DMA_ADC0 		40
#define DMA_ADC1 		41
#define DMA_CMP0 		42
#define DMA_CMP1 		43
#define DMA_CMP2 		44
#define DMA_DAC0 		45
#define DMA_DAC1 		46
#define DMA_CMT 		47	
#define DMA_PDB 		48
#define DMA_PORTA 		49
#define DMA_PORTB 		50
#define DMA_PORTC 		51
#define DMA_PORTD 		52
#define DMA_PORTE 		53
#define DMA_IEEE1588_T0 54
#define DMA_IEEE1588_T1 55
#define DMA_IEEE1588_T2 56
#define DMA_IEEE1588_T3 57
#define DMA_MUX0_AE 	58
#define DMA_MUX1_AE		59
#define DMA_MUX2_AE 	60
#define DMA_MUX3_AE 	61
#define DMA_MUX4_AE 	62
#define DMA_MUX5_AE 	63


void TFC_InitDMA();
void Configure_DMA(int index, uint8_t dma_source, uint32_t iterations, uint32_t minor_loop, uint32_t source, uint32_t destination, uint32_t source_bytes_per, uint32_t dest_bytes_per);

#endif /* TFC_DMA_H_ */
