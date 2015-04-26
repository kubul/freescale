


#define MMA8451_I2C_ADDRESS (0x1d)              // address for accelerometer

//Register address map

#define STATUS				0x00 // R ; (FMODE = 0) real time status ; (FMODE > 0) fifo status
#define OUT_X_MSB			0x01 // R ; (FMODE = 0) 7:0 are 8 MSB of 14 bit real time X axis ; (FMODE > 0) FIFO data
#define OUT_X_LSB			0x02 // R ; 7:2 are 6 LSB of 14 bit real time X axis 
#define OUT_Y_MSB			0x03 // R ; 7:0 are 8 MSB of 14 bit real time Y axis
#define OUT_Y_LSB			0x04 // R ; 7:2 are 6 LSB of 14 bit real time Y axis 
#define OUT_Z_MSB 			0x05 // R ; 7:0 are 8 MSB of 14 bit real time Z axis
#define OUT_Z_LSB			0x06 // R ; 7:2 are 6 LSB of 14 bit real time Z axis 
#define RESERVED1			0x07 // R ; Reserved, read returns 0x00
#define RESERVED2			0x08 // R ; Reserved, read returns 0x00
#define F_SETUP				0x09 // R/W ; FIFO Setup
#define TRIG_CFG			0x0A // R/W ; Map of FIFO data capture events
#define SYSMOD				0x0B // R ; Current System Mode
#define INT_SOURCE			0x0C // R ; Interrupt status
#define WHO_AM_I			0x0D // R ; Device ID (0x1A)
#define XYZ_DATA_CFG		0x0E // R/W ; Dynamic Range Settings
#define HP_FILTER_CUTOFF	0x0F // R/W ; High pass cutoff setting 16-800 Hz
#define PL_STATUS			0x10 // R ; Landscape/Portrait status
#define PL_CFG				0x11 // R/W ; Landscape/Portrait c
#define PL_COUNT			0x12 // R/W ; Landscape/Portrait debounce counter
#define PL_BF_ZCOMP			0x13 // R/W ; Back/Front, Z-Lock Trip threshold
#define P_L_THSREG			0x14 // R/W ; Portrait to Landscape Trip Angle is 29 degrees
#define FF_MT_CFG			0x15 // R/W ; Freefall/Motion configuration
#define FF_MT_SRC			0x16 // R ; Freefall/Motion event source
#define FF_MT_THS			0x17 // R/W ; Freefall/Motion threshold
#define FF_MT_COUNT			0x18 // R/W ; Freefall/Motion debounce counter
#define RESERVED3			0x19 // R ; Reserved, read returns 0x00
#define RESERVED4			0x1A // R ; Reserved, read returns 0x00
#define RESERVED5			0x1B // R ; Reserved, read returns 0x00
#define RESERVED6			0x1C // R ; Reserved, read returns 0x00
#define TRANSIENT_CFG		0x1D // R/W ; Transient cfg
#define TRANSIENT_SCR		0x1E // R ; Transient event status
#define TRANSIENT_THS		0x1F // R/W ; Transient event threshold
#define TRANSIENT_COUNT		0x20 // R/W ; Transient debounce counter
#define PULSE_CFG			0x21 // R/W ; ELE, Double XYZ or Single XYZ
#define PULSE_SRC			0x22 // R ; EA, Double XYZ or Single XYZ
#define PULSE_THSX			0x23 // R/W ; X pulse threshold
#define PULSE_THSY			0x24 // R/W ; Y pulse threshold
#define PULSE_THSZ			0x25 // R/W ; Z pulse threshold
#define PULSE_TMLT			0x26 // R/W ; Pulse time limit
#define PULSE_LTCY			0x27 // R/W ; Latency for 2nd pulse
#define PULSE_WIND			0x28 // R/W ; Window time for 2nd pulse
#define ASLP_COUNT			0x29 // R/W ; Counter for Auto-sleep
#define CTRL_REG1	 		0x2A // R/W ; ODR = 800Hz, STANDBY mode
#define CTRL_REG2	 		0x2B // R/W ; Sleep enable, OS Modes, RST, ST
#define CTRL_REG3			0x2C // R/W ; Wake from Sleep, IPOL, PP_OD
#define CTRL_REG4			0x2D // R/W ; Interrupt enable
#define CTRL_REG5			0x2E // R/W ; Interrupt pin map (INT1/INT2)
#define OFF_X				0x2F // R/W ; X axis offset
#define OFF_Y				0x30 // R/W ; Y axis offset
#define OFF_Z				0x31 // R/W ; Z axis offset



#define MWSR                   0x00  /* Master write  */
#define MRSW                   0x01  /* Master read */
#define i2c_DisableAck()       I2C0_C1 |= I2C_C1_TXAK_MASK
#define i2c_EnableAck()        I2C0_C1 &= ~I2C_C1_TXAK_MASK
#define i2c_RepeatedStart()    I2C0_C1 |= I2C_C1_RSTA_MASK
#define i2c_EnterRxMode()      I2C0_C1 &= ~I2C_C1_TX_MASK
#define i2c_write_byte(data)   I2C0_D = data

#define i2c_Start()            I2C0_C1 |= I2C_C1_TX_MASK;\
                               I2C0_C1 |= I2C_C1_MST_MASK

#define i2c_Stop()             I2C0_C1 &= ~I2C_C1_MST_MASK;\
                               I2C0_C1 &= ~I2C_C1_TX_MASK

#define i2c_Wait()               while((I2C0_S & I2C_S_IICIF_MASK)==0) {} \
                                  I2C0_S |= I2C_S_IICIF_MASK;

void init_I2C(void);
void IIC_StartTransmission (char SlaveID, char Mode);
void I2CWriteRegister(char SlaveID, char u8RegisterAddress, char u8Data);
char I2CReadRegister(char SlaveID, char u8RegisterAddress);
void I2CReadMultiRegisters(char SlaveID, char u8RegisterAddress, char n, char * r);
