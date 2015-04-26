#ifndef TFC_LINESCANCAMERA_H_
#define TFC_LINESCANCAMERA_H_


void TFC_InitLineScanCamera();
void TFC_LineScanImageCamera();

void TFC_SetLineScanExposureTime(uint32_t  TimeIn_uS);


#define TAOS_CLK_HIGH  GPIOC_PSOR = (1<<4)  
#define TAOS_CLK_LOW   GPIOC_PCOR = (1<<4)  
#define TAOS_SI_HIGH   GPIOA_PSOR = (1<<0)
#define TAOS_SI_LOW    GPIOA_PCOR =	(1<<0)

extern volatile uint16_t  *LineScanImage0WorkingBuffer;
extern volatile uint16_t  *LineScanImage0;
extern volatile uint16_t  LineScanImage0Buffer[2][128];
extern volatile uint8_t  LineScanWorkingBuffer;
extern volatile uint8_t  LineScanImageReady;

#endif /* TFC_LINESCANCAMERA_H_ */
