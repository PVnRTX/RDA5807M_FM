#include "STC8G.H"
#include "I2C.h"

void I2C_WaitCMD(void)
{
    while (!(I2CMSST & 0x40));
    I2CMSST &= ~0x40;
}

void I2C_Init(void)
{
    P_SW2 |= 0x80;
    P_SW2 &= ~0x30;

	P3M0 |= 0x04; P3M1 |= 0x04; // P3.2 开漏 (M0=1, M1=1)
	P3M0 |= 0x08; P3M1 |= 0x08; // P3.3 开漏 (M0=1, M1=1)
	
    I2CCFG = 0xDA;  // 使能 I2C, 主机模式, 11.0592MHz/2/(26 * 2 + 4) = 98.74kHz
    I2CMSST = 0x00;
}

void I2C_Start(void)
{
    I2CMSCR = 0x01;
    I2C_WaitCMD();
}

void I2C_Stop(void)
{
    I2CMSCR = 0x06;
    I2C_WaitCMD();
}

unsigned char I2C_SendByte_AckAuto(unsigned char dat)
{
    I2CTXD = dat;
    I2CMSCR = 0x0A;
    I2C_WaitCMD();
    return (I2CMSST & 0x02);     // 返回 ACK(0)/NAK(1)
}

unsigned char I2C_WaitAck(void)
{
    I2CMSCR = 0x03;
    I2C_WaitCMD();
    return (I2CMSST & 0x02);     // 返回 ACK/NAK
}

void I2C_SendACK(void)
{
    I2CMSST &= ~0x01;
    I2CMSCR = 0x05;
    I2C_WaitCMD();
}

void I2C_SendNAK(void)
{
    I2CMSST |= 0x01;
    I2CMSCR = 0x05;
    I2C_WaitCMD();
}

void I2C_GetBytes(unsigned char len, unsigned char* read_buffer)
{
    unsigned char i = 0;
    for(i = 0; i < len - 1; i++)
    {
        I2CMSCR = 0x0B;
        I2C_WaitCMD();
        read_buffer[i] = I2CRXD;
    }
    I2CMSCR = 0x0C;
    I2C_WaitCMD();
    read_buffer[i] = I2CRXD;
}
