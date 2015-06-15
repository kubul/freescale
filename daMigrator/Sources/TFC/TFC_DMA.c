/*
 * TFC_dma.c
 *
 *  Created on: May 3, 2015
 *      Author: p746
 */


#include "TFC\TFC.h"

void TFC_InitDMA() {
	// DMAMUX and DMA
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
}
