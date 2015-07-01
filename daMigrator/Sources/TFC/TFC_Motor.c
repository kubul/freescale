/*
 * Pwm_Motor.c
 *
 *  This file sets up the Pulse Width Modulation Timer Module for use by other modules
 */

#include "TFC/TFC.h"

#define FTM_MOD_VALUE	(int)((float)(PERIPHERAL_BUS_CLOCK)/TFC_MOTOR_SWITCHING_FREQUENCY) //15000?


void TFC_InitMotorPWM()  
{
	
	//Enable the Clock to the FTM3 Module
	SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK; 
	
	//Enable CLock to Port C so the Timer can get out.
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK; 
	            
	//route the output of that channel to the pin... (pick a different multiplexer value for routing the timer)
    PORTC_PCR8  = PORT_PCR_MUX(3)  | PORT_PCR_DSE_MASK;
    PORTC_PCR9  = PORT_PCR_MUX(3)  | PORT_PCR_DSE_MASK;
    PORTC_PCR10  = PORT_PCR_MUX(3)  | PORT_PCR_DSE_MASK;
    PORTC_PCR11  = PORT_PCR_MUX(3)  | PORT_PCR_DSE_MASK;

    // Choose EDGE-Aligned PWM:  selected when QUADEN=0, DECAPEN=0, COMBINE=0, CPWMS=0, and MSnB=1  (page 964)
    // Properly set up Flex Timer Module
    
    //FTM3_MODE[WPDIS] = 1; //Disable Write Protection - enables changes to QUADEN, DECAPEN, etc.  
    FTM3_MODE |= FTM_MODE_WPDIS_MASK;
    
    //FTMEN is bit 0, need to set to zero so DECAPEN can be set to 0
    FTM3_MODE &= ~FTM_MODE_FTMEN_MASK; 
    
    //Set Edge Aligned PWM
    FTM3_QDCTRL &=~FTM_QDCTRL_QUADEN_MASK;  
    
    /*
    • FTMEN = 1
    • QUADEN = 0
    • DECAPEN = 0
    • COMBINE = 1
    • CPWMS = 0, and
    • COMP = 1
   
    

    
    FTM3_COMBINE &=~FTM_COMBINE_DECAPEN2_MASK;
    FTM3_COMBINE &=~FTM_COMBINE_DECAPEN3_MASK;
    
    FTM3_COMBINE |=FTM_COMBINE_COMBINE2_MASK | FTM_COMBINE_COMBINE3_MASK;
 
    FTM3_SC &=~FTM_SC_CPWMS_MASK;
    
    FTM3_COMBINE |=FTM_COMBINE_COMP2_MASK | FTM_COMBINE_COMP3_MASK;
    
     */
    

    
    //QUADEN is Bit 1, Set Quadrature Decoder Mode (QUADEN) Enable to 0,   (disabled)
    //FTM0_SC = 0x16; //Center Aligned PWM Select = 0, sets FTM Counter to operate in up counting mode, 
    //it is field 5 of FTMx_SC (status control) - also setting the pre-scale bits here

    
  /* For proper Servo operation set period to:  50 ms, pulse width to 25ms 
   * diagram of FTM Module is on page 899
   * 
   * period on chip determined by MOD-CNTIN +0x0001 
   * pulse width determined by (CnV-CNTIN)  (good descriptions in ref manual on page 956)
   * 
   * Peripheral bus clock -> 60Mhz  (set up in clock.h) and described in manual on (pg 963)  
   * 
   * 16 bit counter has 65,536 counts.   
   * Set pre-scaler to desired value to have correct clock period.      
   *  
   * The value of the prescaler is selected by the PS[2:0] bits.  (FTMx_SC field bits 0-2 are Prescale bits - 
   * set above in FTM_SC Setting) 
   *  001 Divide by 2
   *  010 Divide by 4
   *  011 Divide by 8
   *  100 Divide by 16
   *  101 Divide by 32
   *  110 Divide by 64 - 
   *  111 Divide by 128
   */
    
  //   Also need to setup the FTM3C0SC channel control register  - Page 897   section 37.3.6

  
  FTM3_CNT = 0x0; //FTM Counter Value - (initialize the CNT before writing to MOD)  (16 bit available - bits 0-15 are count)
  FTM3_MOD = FTM_MOD_VALUE; //Set the Modulo resister (16 bit available - bits 0-15), Mod is set to 15000
  FTM3_CNTIN = 0; //Set the Counter Initial Value to 0   (pg 915)
  
  //change MSnB = 1
  
  FTM3_C4SC |= FTM_CnSC_ELSB_MASK;
  FTM3_C4SC &= ~FTM_CnSC_ELSA_MASK;
  FTM3_C4SC |= FTM_CnSC_MSB_MASK;
  FTM3_C4V = FTM_MOD_VALUE/2; //Set the Channel n Value to  (16 bit available - bits 0-15)
  
  FTM3_C6SC |= FTM_CnSC_ELSB_MASK;
  FTM3_C6SC &= ~FTM_CnSC_ELSA_MASK;
  FTM3_C6SC |= FTM_CnSC_MSB_MASK;
  FTM3_C6V = FTM_MOD_VALUE/2; //Set the Channel n Value to  (16 bit available - bits 0-15)
  
  //Set the complimentary pinout
  
  FTM3_C5SC |= FTM_CnSC_ELSB_MASK;
  FTM3_C5SC &= ~FTM_CnSC_ELSA_MASK;
  FTM3_C5SC |= FTM_CnSC_MSB_MASK;
  FTM3_C5V = FTM_MOD_VALUE/2;
  
  FTM3_C7SC |= FTM_CnSC_ELSB_MASK;
  FTM3_C7SC &= ~FTM_CnSC_ELSA_MASK;
  FTM3_C7SC |= FTM_CnSC_MSB_MASK;
  FTM3_C7V = FTM_MOD_VALUE/2;


  FTM3_SC = FTM_SC_PS(0) | FTM_SC_CLKS(1);  
  
}

//A right   B left
void TFC_SetMotorPWM(float MotorA ,float MotorB)
{
	
	if (MotorA>1.0) {
		MotorA = 1.0;
	} else if (MotorA<-1.0) {
		MotorA = -1.0;
	}
	if (MotorB>1.0) {
		MotorB = 1.0;
	} else if (MotorB<-1.0) {
		MotorB = -1.0;
	}	
	
	if (MotorA>=0.0) {
		FTM3_C4V = (int) ((float)FTM_MOD_VALUE * (float)(MotorA));
		FTM3_C5V = (int) (0);
	} else {      
		FTM3_C4V = (int) (0);
		FTM3_C5V = (int) ((float)FTM_MOD_VALUE * (float)(-MotorA));
	}
	
	if (MotorB>=0.0) {
		FTM3_C6V = (int) ((float)FTM_MOD_VALUE * (float)(MotorB));
		FTM3_C7V = (int) (0);
	} else {
		FTM3_C6V = (int) (0);
		FTM3_C7V = (int) ((float)FTM_MOD_VALUE * (float)(-MotorB));
	}	
	
	/*
	FTM3_C4V = (int) ((float)FTM_MOD_VALUE * (float)((MotorA + 1.0)/2.0));
	//FTM3_C5V = (int) ((float)FTM_MOD_VALUE * (float)((MotorA + 1.0)/2.0));
	FTM3_C6V = (int) ((float)FTM_MOD_VALUE * (float)((MotorB + 1.0)/2.0));
	//FTM3_C7V = (int) ((float)FTM_MOD_VALUE * (float)((MotorB + 1.0)/2.0));
	*/
}


