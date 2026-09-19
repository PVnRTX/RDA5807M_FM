#ifndef BASICCONFIG_H
#define BASICCONFIG_H

#include <STC8G.H>

#define VolUp_Pin   P55
#define VolDown_Pin P30
#define Seek_Pin    P54
#define Speaker_Pin P31

#define seek_action      0x80
#define vol_up_action    0x40
#define vol_down_action  0x20
#define speaker_action   0x10

enum state
{
    blank = 0x00,
    seek_waiting,
    seek_auto,
    seek_up,
    seek_down,
};

// enum
// {
//    7 seek_action,
//    6 vol_up_action,
//    5 vol_down_action,
//    4 speaker_action,
//    3 none,
//    2 none,
//    1 none,
//    0 none
// } key_action = blank_action;
extern enum state seek_state;
extern volatile unsigned char key_action;
extern unsigned char seek_trigger;
extern volatile unsigned char sleep_counter; // 71ms + 1
extern unsigned char seek_auto_flag;
extern volatile unsigned char seek_waiting_counter; // 71ms + 1

void INT2_P54_Seek(void);
void INT3_P55_VolUp(void);
void INT4_P30_VolDown(void);
void CCP02_P31_Speaker(void);
void Timer0_Sleep_ISR(void);
void Timer1_SeekWaiting_ISR(void);

#endif