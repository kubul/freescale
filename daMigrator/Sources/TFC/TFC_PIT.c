
#include "derivative.h"

void TFC_InitPIT() {
	
	// Enable clock to PIT interface
	SIM_SCGC6  |= SIM_SCGC6_PIT_MASK;

}

/*! Initialises a PIT channel
 *
 * @param channel  - channel (0-3) to configure
 * @param interval - PIT interval (use PIT_MILLISECOND_TO_TICKS() macro)
 *
 * Configures:
 *   - Enables PIT clock
 *   - Sets PIT re-load value
 *   - Enables PIT
 *   - Enables interrupts
 */
void InitPit(int channel, uint32_t interval, uint8_t interrupt) {
	
	// Set re-load value
	PIT_LDVAL_REG(PIT_BASE_PTR,channel) = interval-1;

	if (interrupt) {
		// Enable PIT interrupts in NVIC
		enable_irq(INT_PIT0+channel-16);
		
		// Enable this channel with interrupts
		PIT_TCTRL_REG(PIT_BASE_PTR,channel) = PIT_TCTRL_TIE_MASK;
	}

}

void StartPits() {
	// Enable PIT module
	PIT_MCR = 0;//PIT_MCR_FRZ_MASK;
}
