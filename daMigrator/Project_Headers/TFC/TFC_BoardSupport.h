#include <stdint.h>

#ifndef TFC_BOARDSUPPORT_H_
#define TFC_BOARDSUPPORT_H_

#define TFC_HBRIDGE_EN_LOC			(uint32_t)(1<<20)
//#define TFC_HBRIDGE_FAULT_LOC     	(uint32_t)(1<<20)

#define TFC_HBRIDGE_ENABLE			GPIOB_PSOR = TFC_HBRIDGE_EN_LOC	
#define TFC_HBRIDGE_DISABLE			GPIOB_PCOR = TFC_HBRIDGE_EN_LOC	

#define TFC_DIP_SWITCH0_LOC			((uint32_t)(1<<5))
#define TFC_DIP_SWITCH1_LOC			((uint32_t)(1<<7))
#define TFC_DIP_SWITCH2_LOC			((uint32_t)(1<<0))
#define TFC_DIP_SWITCH3_LOC			((uint32_t)(1<<1))

#define TFC_PUSH_BUTTON0_LOC		((uint32_t)(1<<3))
#define TFC_PUSH_BUTTON1_LOC		((uint32_t)(1<<18))	

#define TFC_BAT_LED0_LOC			((uint32_t)(1<<16))
#define TFC_BAT_LED1_LOC			((uint32_t)(1<<17))
#define TFC_BAT_LED2_LOC			((uint32_t)(1<<9))
#define TFC_BAT_LED3_LOC			((uint32_t)(1<<1))

#define RED_LED_LOC					((uint32_t)(1<<22))
#define GREEN_LED_LOC				((uint32_t)(1<<26)) //MADE A BOARD PIN - WHY FREESCALE, WHY?
#define BLUE_LED_LOC				((uint32_t)(1<<21))

#define RED_LED_ON					GPIOB_PCOR = RED_LED_LOC
#define GREEN_LED_ON				GPIOE_PCOR = GREEN_LED_LOC
#define BLUE_LED_ON					GPIOB_PCOR = BLUE_LED_LOC

#define RED_LED_OFF					GPIOB_PSOR = RED_LED_LOC
#define GREEN_LED_OFF				GPIOE_PSOR = GREEN_LED_LOC 
#define BLUE_LED_OFF				GPIOB_PSOR = BLUE_LED_LOC

#define TFC_BAT_LED0_ON				GPIOC_PSOR = TFC_BAT_LED0_LOC
#define TFC_BAT_LED1_ON				GPIOC_PSOR = TFC_BAT_LED1_LOC
#define TFC_BAT_LED2_ON				GPIOB_PSOR = TFC_BAT_LED2_LOC
#define TFC_BAT_LED3_ON				GPIOA_PSOR = TFC_BAT_LED3_LOC

#define TFC_BAT_LED0_OFF			GPIOC_PCOR = TFC_BAT_LED0_LOC
#define TFC_BAT_LED1_OFF			GPIOC_PCOR = TFC_BAT_LED1_LOC
#define TFC_BAT_LED2_OFF			GPIOB_PCOR = TFC_BAT_LED2_LOC
#define TFC_BAT_LED3_OFF			GPIOA_PCOR = TFC_BAT_LED3_LOC

#define TFC_BAT_LED0_TOGGLE			GPIOC_PTOR = TFC_BAT_LED0_LOC
#define TFC_BAT_LED1_TOGGLE			GPIOC_PTOR = TFC_BAT_LED1_LOC
#define TFC_BAT_LED2_TOGGLE			GPIOB_PTOR = TFC_BAT_LED2_LOC
#define TFC_BAT_LED3_TOGGLE			GPIOA_PTOR = TFC_BAT_LED3_LOC

#define TFC_PUSH_BUTTON_0_PRESSED	((GPIOD_PDIR&TFC_PUSH_BUTTON0_LOC)>0)
#define TFC_PUSH_BUTTON_1_PRESSED	((GPIOB_PDIR&TFC_PUSH_BUTTON1_LOC)>0)

void    TFC_InitGPIO();
void    TFC_SetBatteryLED_Level(uint8_t BattLevel);
uint8_t TFC_GetDIP_Switch();
uint8_t TFC_GetAccelInterrupts();

#endif /* TFC_BOARDSUPPORT_H_ */
