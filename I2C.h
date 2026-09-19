#ifndef I2C_H
#define I2C_H

void I2C_WaitCMD(void);
void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
unsigned char I2C_SendByte_AckAuto(unsigned char dat);
unsigned char I2C_WaitAck(void);
void I2C_SendACK(void);
void I2C_SendNAK(void);
void I2C_GetBytes(unsigned char len, unsigned char* read_buffer);

#endif
