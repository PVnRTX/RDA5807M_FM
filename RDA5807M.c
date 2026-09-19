// This is about RDA5807M configs, including register defination and basic functions
#include <I2C.h>
#include <RDA5807M.h>
#include <Delay.h>
#include "BasicConfig.h"

// const unsigned char rda5807m_init_config[] = 
// {
//     0xC0,
//     0x01,  // 0x02H: DHIZ=1, DMUTE=1, ENABLE=1 (开启音频输出、取消静音、使能芯片)
//     0x00,
//     0x00,  // 0x03H: CHAN=0, BAND=00, SPACE=00 (默认 87.0MHz, 100kHz 步进)
//     0x00,
//     0x60,  // 0x04H: 调谐控制
//     0x88,
//     0x88,  // 0x05H: SEEKTH=1000 (二进制), VOLUME=1001 (中音量)
//     // 0x00,
//     // 0x00,  // 0x06H: OPEN_MODE=00 (默认，仅开放读功能), 保留
// };

unsigned char rda5807m_current_config[] = 
{
    0xC0,
    0x01,
    0x00,
    0x00,
    0x00,
    0x60,
    0x88,
    0x88,  // Init config
    // 0x00,
    // 0x00,
};

unsigned char rda5807m_state[] = 
{
    0x00,
    0x00  // 0x0AH: 状态位
};

const unsigned char rda5807m_addr = 0x10;

void RDA5807M_Init(void)
{
    unsigned char i = 0;

    rda5807m_current_config[0] &= ~0x20;  // 清除mono位
    if(Speaker_Pin == 0)
    {
        rda5807m_current_config[0] &= ~0x20; // mono = 0, 立体声
    }
    else
    {
        rda5807m_current_config[0] |= 0x20; // mono = 1, 单声道
    }

    I2C_Start();
    I2C_SendByte_AckAuto((rda5807m_addr << 1) | 0x00);
    for(i = 0; i < 8; i++)
    {
        I2C_SendByte_AckAuto(rda5807m_current_config[i]);
    }
    I2C_Stop();

    RDA5807M_GetState();
}

void RDA5807M_Seek(enum direction dir)
{
    unsigned char i = 0;

    rda5807m_current_config[0] &= ~0x02;
    if(dir == down)
    {    
        rda5807m_current_config[0] |= 0x00;  // SEEKMODE=down
    }
    else
    {
        rda5807m_current_config[0] |= 0x02;  // SEEKMODE=up
    }
    rda5807m_current_config[0] |= 0x01;  // SEEK=1, 开始搜索

    I2C_Start();
    I2C_SendByte_AckAuto((rda5807m_addr << 1) | 0x00);
    for(i = 0; i < 2; i++)
    {
        I2C_SendByte_AckAuto(rda5807m_current_config[i]);
    }
    I2C_Stop();

    rda5807m_current_config[0] &= ~0x01;

    _nop_();
    _nop_();
    _nop_();
}

void RDA5807M_ChangeVolume(enum direction dir)
{
    unsigned char i = 0;
    unsigned char volume = rda5807m_current_config[7] & 0x0F;  // 获取当前音量  
    if(dir == up && volume < 15)
    {
        volume++;
    }
    else if(dir == down && volume > 0)
    {
        volume--;
    }

    rda5807m_current_config[7] &= ~0x0F;  // 清除低4位
    rda5807m_current_config[7] |= (volume & 0x0F);  // 设置音量

    I2C_Start();
    I2C_SendByte_AckAuto((rda5807m_addr << 1) | 0x00);
    for(i = 0; i < 8; i++)
    {
        I2C_SendByte_AckAuto(rda5807m_current_config[i]);
    }
    I2C_Stop();

    _nop_();
    _nop_();
    _nop_();
}

void RDA5807M_ChangeMonoStereo(enum direction dir)
{
    unsigned char i = 0;

    rda5807m_current_config[0] &= ~0x20;  // 清除mono位
    if(dir == up)
    {
        rda5807m_current_config[0] |= 0x20; // mono = 1, 单声道
    }
    else
    {
        rda5807m_current_config[0] &= ~0x20; // mono = 0, 立体声
    }

    I2C_Start();
    I2C_SendByte_AckAuto((rda5807m_addr << 1) | 0x00);
    for(i = 0; i < 2; i++)
    {
        I2C_SendByte_AckAuto(rda5807m_current_config[i]);
    }
    I2C_Stop();

    _nop_();
}

void RDA5807M_GetState(void)
{
    I2C_Start();
    I2C_SendByte_AckAuto((rda5807m_addr << 1) | 0x01);
    I2C_GetBytes(2, rda5807m_state);
    I2C_Stop();

    _nop_();
}

void RDA5807M_UpdateCurrentChan(void)
{
    rda5807m_current_config[2] = rda5807m_state[0] << 6;
    rda5807m_current_config[2] |= rda5807m_state[1] >> 2;
    rda5807m_current_config[3] = (rda5807m_current_config[3] & 0x3F) | (rda5807m_state[1] << 6);
}
