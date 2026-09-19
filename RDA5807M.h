#ifndef RDA5807M_H
#define RDA5807M_H
// RDA5807M 寄存器速查（纯文本）
// ========================================
// 可写寄存器：0x02H ~ 0x07H
// 可读寄存器：0x0AH ~ 0x0BH
// I2C 写操作固定从 0x02H 开始，地址自动递增。
// ========================================

// 【0x02H】控制寄存器1
// bit15 DHIZ                音频输出高阻禁用: 0=高阻, 1=正常 (默认0)
// bit14 DMUTE               静音禁用: 0=静音, 1=正常 (默认0)
// bit13 MONO                单声道: 0=立体声, 1=强制单声道 (默认0)
// bit12 BASS                低音增强: 1=启用 (默认0)
// bit11 RCLK NON-CALIBRATE  RCLK校准模式 (默认0)
// bit10 RCLK DIRECT INPUT   RCLK直接输入 (默认0)
// bit9  SEEKUP              搜台方向: 0=向下, 1=向上 (默认0)
// bit8  SEEK                搜台使能: 1=启动 (默认0)
// bit7  SKMODE              搜台模式: 0=回绕, 1=边界停止 (默认0)
// bit6:4 CLK_MODE[2:0]      时钟模式:
//                           000=32.768kHz, 001=12MHz, 010=13MHz,
//                           011=19.2MHz, 101=24MHz, 110=26MHz, 111=38.4MHz
//                           (默认000)
// bit3  RDS_EN              RDS/RBDS使能: 1=启用 (默认0)
// bit2  NEW_METHOD          新解调方法: 1=启用, 提升约1dB灵敏度 (默认0)
// bit1  SOFT_RESET          软复位: 1=复位 (默认0)
// bit0  ENABLE              电源使能: 1=启用 (默认0)

// 【0x03H】频道选择
// bit15:6 CHAN[9:0]         频道选择值 (默认0x00)
// bit5    DIRECT MODE       直接控制模式(测试用) (默认0)
// bit4    TUNE              调谐使能: 1=启动, 完成后自动清零 (默认0)
// bit3:2  BAND[1:0]         频段:
//                           00=87-108MHz, 01=76-91MHz,
//                           10=76-108MHz, 11=65-76MHz或50-76MHz
//                           (默认00)
// bit1:0  SPACE[1:0]        步进:
//                           00=100kHz, 01=200kHz,
//                           10=50kHz,  11=25kHz
//                           (默认00)

// 【0x04H】调谐控制/音频
// bit15   RSVD              保留 (默认0)
// bit14:12 RSVD             保留 (默认000)
// bit11   DE                去加重: 0=75us, 1=50us (默认0)
// bit10   RSVD              保留 (默认0)
// bit9    SOFTMUTE_EN       软静音使能: 1=启用 (默认1)
// bit8    AFCD              AFC禁用: 0=工作, 1=禁用 (默认0)
// bit7:0  RSVD              保留 (默认0)

// 【0x05H】音量与搜台阈值
// bit15   INT_MODE          中断模式:
//                           0=5ms脉冲,
//                           1=保持至读取0x0CH
//                           (默认0)
// bit14:12 RSVD             保留 (默认000)
// bit11:8 SEEKTH[3:0]       搜台SNR阈值 (默认1000, 约32dB)
// bit7:4  RSVD              保留 (默认00)
// bit3:0  VOLUME[3:0]       音量: 0000=静音, 1111=最大 (默认1011)

// 【0x06H】I2S配置与写权限
// bit15   RSVD              保留 (默认0)
// bit14:13 OPEN_MODE[1:0]   后续寄存器写权限:
//                           11=开放写入,
//                           其他=只读
//                           (默认00)
// bit12   slave_master      I2S主从: 1=从机, 0=主机 (默认0)
// bit11   ws_lr             WS与左右声道关系:
//                           0=WS0->右, WS1->左;
//                           1=WS0->左, WS1->右
//                           (默认0)
// bit10   sclk_i_edge       内部SCLK边沿: 1=反相 (默认0)
// bit9    data_signed       I2S数据格式:
//                           0=无符号16位,
//                           1=有符号16位
//                           (默认0)
// bit8    WS_I_EDGE         内部WS边沿: 1=反相 (默认0)
// bit7:4  I2S_SW_CNT[3:0]   主机模式采样率:
//                           1000=48k,   0111=44.1k,
//                           0110=32k,   0101=24k,
//                           0100=22.05k,0011=16k,
//                           0010=12k,   0001=11.025k,
//                           0000=8k
//                           (默认0000)
// bit3    SW_O_EDGE         主机WS输出反相: 1=反相 (默认0)
// bit2    SCLK_O_EDGE       主机SCLK输出反相: 1=反相 (默认0)
// bit1    L_DELY            左声道数据延迟1T: 1=延迟 (默认0)
// bit0    R_DELY            右声道数据延迟1T: 1=延迟 (默认0)

// 【0x07H】软混合与直接频率模式
// bit15   RSVD              保留 (默认0)
// bit14:10 TH_SOFRBLEND[4:0] 噪声软混合阈值, 单位2dB (默认10000)
// bit9    65M_50M MODE      BAND=11时有效:
//                           1=65-76MHz,
//                           0=50-76MHz
//                           (默认1)
// bit8    RSVD              保留 (默认0)
// bit7:2  SEEK_TH_OLD       旧搜台模式阈值 (默认000000)
// bit1    SOFTBLEND_EN      软混合使能 (默认1)
// bit0    FREQ_MODE         直接频率模式:
//                           1=启用,
//                           频率=76000(或87000)kHz+freq_direct
//                           (默认0)

// 【0x0AH】状态寄存器（只读）
// bit15   RDSR              RDS就绪: 1=新RDS/RBDS组就绪
// bit14   STC               搜台/调谐完成: 1=完成
// bit13   SF                搜台失败: 1=失败
// bit12   RDSS              RDS同步: 1=已同步
// bit11   BLK_E             RDS块错误标志
// bit10:0 保留

// 【0x0BH】信号强度与状态（只读）
// bit15:9 RSSI[6:0]         接收信号强度指示
// bit8    STEREO            立体声指示: 1=立体声
// bit7:0  保留

// ========================================
// 常用配置
// ========================================
// 上电初始化：0x02H = 0xC001  (DHIZ=1, DMUTE=1, ENABLE=1)
// 调台：设置 0x03H 的 CHAN/BAND/SPACE，并置 TUNE=1
// 调音量：修改 0x05H 低4位 VOLUME[3:0]
// 搜台：0x02H 的 SEEK=1，方向由 SEEKUP 决定
// 写 0x07H：需先将 0x06H 的 OPEN_MODE[1:0] 设为 11
// extern const unsigned char rda5807m_init_config[];
extern unsigned char rda5807m_current_config[];
extern const unsigned char rda5807m_addr;
extern unsigned char rda5807m_state[];

enum direction
{
    down = 0x00,
    up
};

void RDA5807M_Init(void);
void RDA5807M_Seek(enum direction dir);
void RDA5807M_ChangeVolume(enum direction dir);
void RDA5807M_UpdateCurrentChan(void);
void RDA5807M_ChangeMonoStereo(enum direction dir);
void RDA5807M_GetState(void);

#endif