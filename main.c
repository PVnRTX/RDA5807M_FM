#include <STC8G.H>
#include "I2C.h"
#include "Delay.h"
#include "RDA5807M.h"
#include "BasicConfig.h"

// ============ main ============
void main(void)
{
    unsigned char i = 0;

    P_SW2 |= 0x80; // 使能拓展SFR寄存器地址访问

    P_SW1 &= ~0x20;
    P_SW1 |= 0x10; // PCA_0模块引脚选择P3.1

    // 高阻输入
    P3M1 = 0xFF; P3M0 = 0x00;
    P5M1 = 0xFF; P5M0 = 0x00;

    // 中断初始化
    INTCLKO |= 0x70;     // INT2, INT3, INT4中断使能
    CCAPM0 |= 0X31;      // CCP0双边沿捕获与中断使能
    IE |= (0x80 | 0x02 | 0x08); // 总中断使能, 定时器0/1中断使能

    TMOD = 0x01 | 0x10;    // 定时器0作为16位定时器, 不使用GATE控制, 不自动重载
                           // 定时器1作为16位定时器, 不使用GATE控制, 不自动重载
    AUXR &= ~(0x80 | 0x40);  // 定时器0/1时钟源选择为Fosc/12
    TH0 = 0x00; TL0 = 0x00;
    TCON |= 0x10;   // 开启定时器0

    IRCDB = 0x10;   // IRC振荡器稳定等待时钟数

    I2C_Init();
    RDA5807M_Init();
    Delay_ms(10);
    RDA5807M_Seek(up);

    while (1)
    {   
        if(key_action || (seek_state != blank))
        {
            TH0 = 0x00; TL0 = 0x00;
            sleep_counter = 0;
        }

        if(sleep_counter >= 70) // 约5sMCU休眠
        {
            sleep_counter = 0;
            PCON |= 0x02;
            _nop_();
            _nop_();
            _nop_();
            _nop_();
            _nop_();
        }
        
        switch(seek_state)
        {
            case blank:
                if(key_action & seek_action)
                {
                    key_action &= ~seek_action;
                    Delay_ms(5);
                    if(Seek_Pin == 0)
                    {
                        seek_state = seek_waiting;
                    }
                }
                break;

            case seek_waiting:
                if(Seek_Pin == 0)
                {
                    if(key_action & vol_down_action)
                    {
                        key_action &= ~vol_down_action;
                        Delay_ms(5);
                        if(VolDown_Pin == 0)
                        {
                            seek_trigger = 1;
                            seek_state = seek_down;
                        }
                    }
                    else if(key_action & vol_up_action)
                    {
                        key_action &= ~vol_up_action;
                        Delay_ms(5);
                        if(VolUp_Pin == 0)
                        {
                            seek_trigger = 1;
                            seek_state = seek_up;
                        }
                    }
                }
                else if(seek_trigger)
                {
                    seek_trigger = 0;
                    seek_state = blank;
                }
                else
                {
                    seek_state = seek_auto;
                }
                break;

            case seek_up:
                RDA5807M_Seek(up);
                do
                {
                    RDA5807M_GetState();
                } while(!(rda5807m_state[0] & 0x40));
                RDA5807M_UpdateCurrentChan();

                seek_state = seek_waiting;
                break;

            case seek_down:
                RDA5807M_Seek(down);
                do
                {
                    RDA5807M_GetState();
                } while(!(rda5807m_state[0] & 0x40));
                RDA5807M_UpdateCurrentChan();

                seek_state = seek_waiting;
                break;

            case seek_auto:
                if(!seek_auto_flag)
                {
                    seek_auto_flag = 1;
                    RDA5807M_Seek(up);
                }

                RDA5807M_GetState();
                if(rda5807m_state[0] & 0x40)
                {
                    RDA5807M_UpdateCurrentChan();
                    TCON |= 0x40;
                }

                if(seek_waiting_counter >= 140) // 搜台间隔持续约10s
                {
                    TCON &= ~0x40;
                    TH1 = 0x00; TL1 = 0x00;
                    seek_waiting_counter = 0; 
                    RDA5807M_Seek(up);
                }

                if(key_action & seek_action)
                {
                    key_action &= ~seek_action;
                    seek_auto_flag = 0;
                    TCON &= ~0x40;
                    TH1 = 0x00; TL1 = 0x00;
                    seek_waiting_counter = 0; 
                    seek_state = blank;                
                }   
                break;

            default:
                break;
        }

        if(key_action & speaker_action)
        {
            key_action &= ~speaker_action;
            Delay_ms(5);
            if(Speaker_Pin == 0)
            {
                RDA5807M_ChangeMonoStereo(down);
            }
            else
            {
                RDA5807M_ChangeMonoStereo(up);
            }
        }

        if(key_action & vol_up_action)
        {
            key_action &= ~vol_up_action;
            Delay_ms(5);
            if(VolUp_Pin == 0)
            {
                RDA5807M_ChangeVolume(up);
            }
        }

        if(key_action & vol_down_action)
        {
            key_action &= ~vol_down_action;
            Delay_ms(5);
            if(VolDown_Pin == 0)
            {
                RDA5807M_ChangeVolume(down);
            }
        }
    }
}