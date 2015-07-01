
#include "TFC_DMA.h"
#include "derivative.h"


#define WFRONT 0
#define WLEFT 1
#define WRIGHT 2

#define SENSOR_AVERAGING_THRESH 3

#define TPIT 1.0/PERIPHERAL_BUS_CLOCK

#define SFRONT_index 6
#define SLEFT_index 7
#define SRIGHT_index 8

#define SBUFSIZE 64
#define MAX_PERIOD 0.1
#define STARTPIT 0x20000000

void TFC_InitSensorCapture();
void ConfigureSensorDMA();
void CalculateSensor(uint16_t counter, volatile uint32_t *buff, float *speed, float *dspeed, uint32_t *lastPIT, uint32_t *lastPITzero);
void RestartPIT();
void PrintSensorInfo();
