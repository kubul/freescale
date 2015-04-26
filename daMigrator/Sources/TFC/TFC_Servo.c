/**********************************************************************************************
 *  
 *  This file sets up the Pulse Width Modulation Timer Module for use by a servo.  It is set 
 *  to utilize Edge-Aligned PWM, and this file properly configures the period, and pulse width 
 *  for use by the other modules
 *  a
 *  Several important functions are contained in this file:
 *  1. Init_PWM_Servo () - initializes the timer module 
 *  2. PWM_Servo (double duty_Cycle) - enter the desired duty cycle setting for the servo
 *  3. Servo_Tick - interrupt routine which executes once/servo period
 *  
 *  	
 *  
 * **********  DETAILS *****************
 * 
 *
 * EDGE-Aligned PWM:  selected when QUADEN=0, DECAPEN=0, COMBINE=0, CPWMS=0, and MSnB=1  
 * For typical Servo operation set period to:  20 ms, vary pulse width between .5ms and 1.5ms to set steering angle
 * 
 * Period on chip determined by MOD-CNTIN +0x0001 
 * Pulse width determined by (CnV-CNTIN)  
 * 
 * - see Edge-Aligned PWM (EPWM) Mode in reference manual for details
 * 
 **********************************************************************************************/
#include "TFC/TFC.h"

#define FTM2_CLK_PRESCALE			6  // Prescale Selector value - see comments in Status Control (SC) section for more details
#define FTM2_OVERFLOW_FREQUENCY 	50  // Desired Frequency of PWM Signal - Here 50Hz => 20ms period
// use these to dial in servo steering to your particular servo

#define SERVO_MIN_DUTY_CYCLE		(float)(.0012*FTM2_OVERFLOW_FREQUENCY)  // The number here should be be *pulse width* in seconds to move servo to its left limit
#define SERVO_MAX_DUTY_CYCLE  		(float)(.0018*FTM2_OVERFLOW_FREQUENCY)  // The number here should be be *pulse width* in seconds to move servo to its Right limit
/**********************************************************************************************/


void TFC_InitServos()
{
	
//Enable the Clock to the FTM2 Module
SIM_SCGC6 |= SIM_SCGC6_FTM2_MASK;  

//Enable Clock to Port B so the Timer can get out.
SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK; 

//Pin control Register (MUX allowing user to route the desired signal to the pin.  
PORTB_PCR19  = PORT_PCR_MUX(3)  | PORT_PCR_DSE_MASK;

//FTM2_MODE[WPDIS] = 1; //Disable Write Protection - enables changes to QUADEN, DECAPEN, etc.  
FTM2_MODE |= FTM_MODE_WPDIS_MASK;

//FTMEN is bit 0, need to set to zero so DECAPEN can be set to 0
FTM2_MODE &= ~FTM_MODE_FTMEN_MASK; 

//Set Edge Aligned PWM
FTM2_QDCTRL &=~FTM_QDCTRL_QUADEN_MASK;  
//QUADEN is Bit 1, Set Quadrature Decoder Mode (QUADEN) Enable to 0,   (disabled)

// Also need to setup the FTM2C0SC channel control register 
FTM2_CNT = 0x0; //FTM Counter Value - reset counter to zero
FTM2_MOD = (PERIPHERAL_BUS_CLOCK/(1<<FTM2_CLK_PRESCALE))/FTM2_OVERFLOW_FREQUENCY ;  // Count value of full duty cycle
FTM2_CNTIN = 0; //Set the Counter Initial Value to 0 

// FTMx_CnSC - contains the channel-interrupt status flag control bits
FTM2_C1SC |= FTM_CnSC_ELSB_MASK; //Edge or level select
FTM2_C1SC &= ~FTM_CnSC_ELSA_MASK; //Edge or level Select
FTM2_C1SC |= FTM_CnSC_MSB_MASK; //Channel Mode select

//Edit registers when no clock is fed to timer so the MOD value, gets pushed in immediately
FTM2_SC = 0; //Make sure its Off!

// FTMx_CnV contains the captured FTM counter value, this value determines the pulse width
FTM2_C1V = FTM2_MOD; 

//Status and Control bits 
FTM2_SC =  FTM_SC_CLKS(1); // Selects Clock source to be "system clock" or (01) 

FTM2_SC |= FTM_SC_PS(FTM2_CLK_PRESCALE); //sets pre-scale value see details below

/******begin FTM_SC_PS details ****************************
* Sets the Prescale value for the Flex Timer Module which divides the 
* Peripheral bus clock -> 64Mhz by the set amount 
*  
* The value of the prescaler is selected by the PS[2:0] bits.  
* (FTMx_SC field bits 0-2 are Prescale bits -  set above in FTM_SC Setting)
*  
*  000 - 0 - No divide
*  001 - 1 - Divide by 2
*  010 - 2 - Divide by 4
*  011 - 3 - Divide by 8
*  100 - 4 - Divide by 16
*  101 - 5 - Divide by 32
*  110 - 6 - Divide by 64 - 
*  111 - 7 - Divide by 128 
*  ******end FTM_SC_PS details*****************************/

// Interrupts
//TODO ENABLE IRQ
//FTM2_SC |= FTM_SC_TOIE_MASK; //Enable the interrupt mask.  timer overflow interrupt.. enables interrupt signal to come out of the module itself...  (have to enable 2x, one in the peripheral and once in the NVIC
//enable_irq(INT_FTM2-16);  // Set NVIC location, but you still have to change/check NVIC file sysinit.c under Project Settings Folder

}

/***************************************Functions to Control Servo Angle ********************************/

void TFC_SetServo (float Position)  
{
  TFC_SetServoDutyCycle((((Position + 1.0)/2)*(SERVO_MAX_DUTY_CYCLE - SERVO_MIN_DUTY_CYCLE))+SERVO_MIN_DUTY_CYCLE);
}

void TFC_SetServoDutyCycle(float DutyCycle)
{
  FTM2_C1V = FTM2_MOD * DutyCycle;       
}

/******************************************* Function to control Interrupt ************************************/
volatile unsigned char ServoTickVar;

void FTM2_IRQHandler()
{
if (ServoTickVar < 0xff)//if servo tick less than 255 count up... 
	ServoTickVar++;
//Clear the overflow mask if set
	if(FTM2_SC & FTM_SC_TOF_MASK)
		FTM2_SC &= ~FTM_SC_TOF_MASK;
}
