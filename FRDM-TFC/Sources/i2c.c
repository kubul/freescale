#include "derivative.h"
#include "i2c.h"

void Init_I2C(void)
{
	//SIM_SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
	
	PORTE_PCR24 = PORT_PCR_MUX(5);
	PORTE_PCR25 = PORT_PCR_MUX(5);
	I2C0_F  = 0x14;
	I2C0_C1 = I2C_C1_IICEN_MASK;
}

/* Inicia Transmissao I2C
 * SlaveID: endereco do slave
 * "Mode" define modo Read (1) ou Write (0)
 * 
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

/* Le um registrador do slave I2C
 * SlaveID: endereco do slave
 * RegisterAddress: endereco do registrador interno do slave
 * Retorna o valor do registrador
 * 
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
	i2c_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler /// Disable ACK for being the penultimate byte to read
	result = I2C0_D ; // Dummy read
	i2c_Wait();
	i2c_Stop(); // Envia STOP por ser o ultimo byte /// Send STOP to be the last byte
	result = I2C0_D ; // Le o byte /// Byte number 0
	Pause();
	return result;
}

/* Escreve um byte no registrador interno do slave I2C
 * SlaveID: endereco do slave
 * RegisterAddress: endereco do registrador interno do slave
 * Data: Valor a escrever
 * 
 * 
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

/* Le "N" registradores internos do slave I2C
 * SlaveID: endereco do slave
 * RegisterAddress: endereco do primeiro registrador interno do slave a ser lido
 * n: Numero de registradores a serem lidos em sequencia
 * Resultado armazenado no ponteiro *r
 * 
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
	i2c_DisableAck(); // Desabilita ACK por ser o penultimo byte a ler /// Disable ACK for being the penultimate byte to read
	*r = I2C0_D; // Le penultimo byte /// The penultimate byte
	r++;
	i2c_Wait();
	i2c_Stop(); // Envia STOP por ser o ultimo byte /// Send STOP to be the last byte
	*r = I2C0_D; // Le ultimo byte /// The last byte
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
	
	I2CWriteRegister(MMA8451_I2C_ADDRESS, PULSE_CFG, 0x44); //enable single tap detection on Y axis
	I2CWriteRegister(MMA8451_I2C_ADDRESS, PULSE_THSY, 0x40); //set Y axis tap threshold (x of 127)
	I2CWriteRegister(MMA8451_I2C_ADDRESS, HP_FILTER_CUTOFF, 0x10); //enable LPF
	//I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG3, 0x02); //make interrupt polarity active HIGH
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG4, 0x08); //enable pulse interrupts
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG5, 0x08); //map pulse interrupt to pin INT1 (PTA14)
	I2CWriteRegister(MMA8451_I2C_ADDRESS, PULSE_TMLT, 0x0); //time window to register pulse
	I2CWriteRegister(MMA8451_I2C_ADDRESS, CTRL_REG1, 0x01); //start accel
	
	
	
}
