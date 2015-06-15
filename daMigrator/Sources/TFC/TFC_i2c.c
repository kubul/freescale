#include "derivative.h"
#include "TFC/TFC.h"

#define MAX_FIFO_SIZE 32
#define AXES 3
#define mg_per_LSB 0.488
#define g 2048
#define cg 2048
#define calibration_samples 100
#define T 0.00125 


int16_t accel_data[MAX_FIFO_SIZE*AXES];
//FMSTR_USE_USB_CDC
int32_t prev_vx;
int32_t vx;
int32_t prev_vy;
int32_t vy;
int32_t prev_vz;
int32_t vz;


int32_t prev_px;
int32_t px;
int32_t prev_py;
int32_t py;
int32_t prev_pz;
int32_t pz;

int32_t accavgx;
int32_t accavgy;
int32_t accavgz;

//int16_t velocity_data[MAX_FIFO_SIZE*AXES];
//int16_t position_data[MAX_FIFO_SIZE*AXES];
//double data[MAX_FIFO_SIZE*3];

void TFC_InitI2C(void)
{
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;

	PORTE_PCR24 = PORT_PCR_MUX(5);
	PORTE_PCR25 = PORT_PCR_MUX(5);

	//Ports for accelerometer interrupts
	PORTC_PCR6 = PORT_PCR_MUX(1);// | PORT_PCR_IRQC(0x2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Set interrupt on falling edge for pullup
	PORTC_PCR13 = PORT_PCR_MUX(1);// | PORT_PCR_IRQC(0x2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; //Set interrupt on falling edge for pullup

	I2C0_F  = 0x14; //set clock rate


	I2C0_C1 = I2C_C1_IICEN_MASK;

	//enable_irq(INT_DMA0 - 16);

	Init_Accel();
	//void Configure_DMA(int index, uint8_t dma_source, uint32_t iterations, uint32_t minor_loop, uint32_t source, uint32_t destination) 
	//Configure_DMA(0, DMA_MUX0_AE, 8, 6, I2C0_D, DATA_BUFFER[0]);
	//Start_I2C_Multi_Read(MMA8451_I2C_ADDRESS, OUT_X_MSB);
}

/* 
 * Starts Transmission I2C
 * SlaveID: address of the slave
 * "Mode" sets mode Read (1) or Write (0)
 */
void IIC_StartTransmission (char SlaveID, char Mode)
{
	SlaveID = SlaveID << 1;
	SlaveID |= (Mode & 0x01);
	i2c_Start();
	i2c_write_byte(SlaveID);
}

void Pause(void){
	int n;
	for(n=1;n<50;n++) {
		asm("nop");
	}
}

/* 
 * One slave I2C register
 * SlaveID: address of the slave
 * RegisterAddress: address of the internal slave register
 * Returns the value of the register
 */
char I2CReadRegister(char SlaveID, char RegisterAddress)
{
	char result;

	IIC_StartTransmission(SlaveID,MWSR);
	i2c_Wait();
	i2c_write_byte(RegisterAddress); 
	i2c_Wait();
	i2c_RepeatedStart();
	i2c_write_byte((SlaveID << 1) | 0x01);
	i2c_Wait();
	i2c_EnterRxMode();
	i2c_DisableAck(); // Disable ACK for being the penultimate byte to read
	result = I2C0_D ; // Dummy read
	i2c_Wait();
	i2c_Stop(); // Send STOP to be the last byte
	result = I2C0_D ; // Byte number 0
	Pause();
	return result;
}

/* 
 * Write a byte in the internal register of the I2C slave
 * SlaveID: address of the slave
 * RegisterAddress: address of the internal slave register
 * Date: Value to write
 */
void I2CWriteRegister(char SlaveID, char RegisterAddress, char Data)
{
	IIC_StartTransmission(SlaveID,MWSR);
	i2c_Wait();
	i2c_write_byte(RegisterAddress);
	i2c_Wait();
	i2c_write_byte(Data);
	i2c_Wait();
	i2c_Stop();
	Pause();
}

/* 
 * "N" internal registers of the I2C slave
 * SlaveID: address of the slave
 * RegisterAddress: address of the first internal slave register to be read
 * N: Number of registers to be read in sequence
 * Result stored in the pointer * r
 */
void I2CReadMultiRegisters(char SlaveID, char RegisterAddress, char n, char * r)
{
	char i;

	IIC_StartTransmission(SlaveID,MWSR);
	i2c_Wait();
	i2c_write_byte(RegisterAddress);
	i2c_Wait();
	i2c_RepeatedStart();
	i2c_write_byte((SlaveID << 1) | 0x01);
	i2c_Wait();
	i2c_EnterRxMode();
	i2c_EnableAck();
	i = I2C0_D ; // Dummy read
	i2c_Wait();

	for(i=0;i<n-2;i++) // le n-1 bytes
	{
		*r = I2C0_D;
		r++;
		i2c_Wait();
	}
	i2c_DisableAck(); // Disable ACK for being the penultimate byte to read
	*r = I2C0_D; // The penultimate byte
	r++;
	i2c_Wait();
	i2c_Stop(); // Send STOP to be the last byte
	*r = I2C0_D; // The last byte
	Pause();
}

void ReadAccelData(char SlaveID, char n, char * r)
{
	char i;

	IIC_StartTransmission(SlaveID,MWSR);
	i2c_Wait();
	i2c_write_byte(OUT_X_MSB);
	i2c_Wait();
	i2c_RepeatedStart();
	i2c_write_byte((SlaveID << 1) | 0x01);
	i2c_Wait();
	i2c_EnterRxMode();
	i2c_EnableAck();
	i = I2C0_D ; // Dummy read
	i2c_Wait();

	for(i=0;i<(n-1);i++) // le n-1 bytes
	{
		*(r+1) = I2C0_D;
		i2c_Wait();
		*r = I2C0_D;
		i2c_Wait();
		r += 2;
	}
	i2c_DisableAck(); // Disable ACK for being the penultimate byte to read
	*(r+1) = I2C0_D; // The penultimate byte
	i2c_Wait();
	i2c_Stop(); // Send STOP to be the last byte
	*r = I2C0_D; // The last byte
	Pause();
}

signed int ReadShort(char * r) 
{
	signed short result;
	result = r[0] << 8;
	result |= r[1];
	result = result >> 2;
	result = result*9.81/4.096; //2.395
	return result;
}
void Init_Accel() 
{
	uint16_t i;
	int32_t x = 0;
	int32_t y = 0;
	int32_t z = 0;
	int8_t nx = 0;
	int8_t ny = 0;
	int8_t nz = 0;
	char stat = 0;
	Reset_Accel();
	TFC_Ticker[0] = 0;
	while (TFC_Ticker[0] < 2000) {}
	//enable_irq(INT_PORTC - 16);

	//Reset_Accel();

	I2CWriteRegister(MMA8451_I2C_ADDRESS, OFF_X, 0x00); 	//calibrate X offset
	I2CWriteRegister(MMA8451_I2C_ADDRESS, OFF_Y, 0x00); 	//calibrate Y offset
	I2CWriteRegister(MMA8451_I2C_ADDRESS, OFF_Z, 0x00); 	//calibrate Z offset

	I2CWriteRegister(MMA8451_I2C_ADDRESS, XYZ_DATA_CFG, 0x01); 	// (+-4g) (0.488 mg/lsb)
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, 0x05); 	//low noise + 800 ODR setup and start accel

	//I2CWriteRegister(MMA8451_I2C_ADDRESS, XYZ_DATA_CFG, 0x00); 	// (+-2g) (0.244 mg/lsb)
	//I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, 0x11); 	//200 ODR setup and start accel

	for (i = 0; i < calibration_samples; i++) {
		//I2CReadMultiRegisters(MMA8451_I2C_ADDRESS, OUT_X_MSB, 6, (char *)accel_data);

		ReadAccelData(MMA8451_I2C_ADDRESS, AXES, (char *)accel_data);
		x += (accel_data[0]>>2);
		y += (accel_data[1]>>2);
		z += ((accel_data[2]>>2) - cg);
		do {
			stat = I2CReadRegister(MMA8451_I2C_ADDRESS, STATUS);
			if (stat == 0xA0) { 
				Reset_Accel();
				return Init_Accel();
			}
		}
		while ((stat & 0x0F) != 0x0F);
	}
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, 0x00); 	//stop accel

	x = x/(4*calibration_samples);
	y = y/(4*calibration_samples);
	z = z/(4*calibration_samples);

	if ((x > 127) || (x < -128) || (y > 127) || (y < -128) || (z > 127) || (z < -128)) {
		RED_LED_ON;
	} else {
		GREEN_LED_ON;
	}

	nx = x;
	ny = y;
	nz = z;

	I2CWriteRegister(MMA8451_I2C_ADDRESS, OFF_X, (-nx)); 	//calibrate X offset
	I2CWriteRegister(MMA8451_I2C_ADDRESS, OFF_Y, (-ny)); 	//calibrate Y offset
	I2CWriteRegister(MMA8451_I2C_ADDRESS, OFF_Z, (-nz)); 	//calibrate Z offset
	/*
	//I2CWriteRegister(MMA8451_I2C_ADDRESS, XYZ_DATA_CFG, 0x00); 	// (+-2g) (0.244 mg/lsb)
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, 0x11); 	//200 ODR setup and start accel
	//I2CReadMultiRegisters(MMA8451_I2C_ADDRESS, 6, OUT_X_MSB, (char *)accel_data);

	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, 0x00); 	//stop accel
	 */

	I2CWriteRegister(MMA8451_I2C_ADDRESS, F_SETUP, 0x40); 		//FIFO enable with no watermark count
	//I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG3, 0x02); 	//make interrupt polarity active HIGH | not working?
	//I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG4, 0x01); 	//enable data-ready interrupts
	//I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG5, 0x01); 	//map data-ready interrupt to pin INT1 (PTC6)
	I2CWriteRegister(MMA8451_I2C_ADDRESS, XYZ_DATA_CFG, 0x01); 	// (+-4g) (0.488 mg/lsb)
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, 0x05); 	//low noise + 800 ODR setup and start accel

	vx = 0x0000;
	vy = 0x0000;
	vz = 0x0000;

	px = 0x0000;
	py = 0x0000;
	pz = 0x0000;
}

void FIFO_Dump() {
	uint8_t stat;
	uint8_t count;
	uint8_t i;

	stat = (I2CReadRegister(MMA8451_I2C_ADDRESS, STATUS) & 0x3F);
	count = stat*AXES;
	ReadAccelData(MMA8451_I2C_ADDRESS, (char)count, (char *)accel_data);
	for (i = 0 ; i < count ; i++) {
		accel_data[i] = (accel_data[i] >> 3); //2048 = 1g
		if ((i % 3) == 2) {
			accel_data[i] -= g/2;
		}
		if ((accel_data[i] < 5) && (accel_data[i] > -5)) {
			accel_data[i] = 0x0;
		}

	}
	//filtering mumble
	/*
	do{

	accelerationx[1]=accelerationx[1] + Sample_X;   //filtering routine for noise attenuation
	accelerationy[1]=accelerationy[1] + Sample_Y;   //64 samples are averaged. The resulting 
	count2++;                                       // average represents the acceleration of 
	       	   	   	   	   	   	   	   	   	   	   	// an instant.
	}while (count2!=0x40);                          // 64 sums of the acceleration sample

	accelerationx[1]= accelerationx[1]>>6;          // division by 64
	accelerationy[1]= accelerationy[1]>>6;
	 */

	//mechanical filtering mumble
	/*
	if ((accelerationx[1] <=3)&&(accelerationx[1] >= -3)) //Discrimination window applied to
	{accelerationx[1] = 0;}                   // the X axis acceleration variable
	 */

	//integration mumble


	if (TFC_Ticker[0] > 200) {
		GREEN_LED_ON;
	}
	if (TFC_Ticker[0] > 400) {
		TFC_Ticker[0] = 0;
		GREEN_LED_OFF;
	}


accavgx = 0x0000;
accavgy = 0x0000;
accavgz = 0x0000;

for (i = AXES ; i < count ; i = i + AXES) {

	//first integration
	//x
	prev_vx = vx;
	vx = prev_vx + accel_data[i-3] + T*((accel_data[i] - accel_data[i-3])<<2);

	//y
	prev_vy = vy;
	vy = prev_vy + accel_data[i-2] + T*((accel_data[i+1] - accel_data[i-2])<<2);

	//z
	prev_vz = vz;
	vz = prev_vz + accel_data[i-1] + T*((accel_data[i+2] - accel_data[i-1])<<2);

	//second integration
	//x
	prev_px = px;
	px = prev_px + prev_vx + T*((vx - prev_vx) << 2);

	//y
	prev_py = py;
	py = prev_py + prev_vy + T*((vy - prev_vy) << 2);

	//z
	prev_pz = pz;
	pz = prev_pz + prev_vz + T*((vz - prev_vz) << 2);

	accavgx += accel_data[i-3];
	accavgy += accel_data[i-2];
	accavgz += accel_data[i-1];
}
/*
accavgx += accel_data[i-3];
accavgy += accel_data[i-2];
accavgz += accel_data[i-1];

accavgx = accavgx/stat;
accavgy = accavgy/stat;
accavgz = accavgz/stat;
*/
accavgx = accel_data[i-3];
accavgy = accel_data[i-2];
accavgz = accel_data[i-1];

//TERMINAL_PRINTF("%6d%6d%6d%6d%6d%6d%6d%6d%6d\r\n",accavgx, accavgy, accavgz, vx, vy, vz, px, py, pz);

}
void Reset_Accel() {
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG2, 0x40); 	//reset
	TFC_Ticker[0] = 0;
	while (TFC_Ticker[0] < 100) {}

}
