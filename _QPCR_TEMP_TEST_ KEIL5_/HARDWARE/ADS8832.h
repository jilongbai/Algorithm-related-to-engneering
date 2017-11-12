#ifndef		__ADS8832_H
#define		__ADS8832_H

#include "stm32f2xx.h"
#include "stm32f2xx_spi.h"
#include "stm32f2xx_gpio.h"


//ADS8332 register and command defines
#define ADS8332_Channel_0				((uint16_t)0x0000)//Select analog input channel 0
#define ADS8332_Channel_1				((uint16_t)0x1000)//Select analog input channel 1
#define ADS8332_Channel_2				((uint16_t)0x2000)//Select analog input channel 2
#define ADS8332_Channel_3				((uint16_t)0x3000)//Select analog input channel 3
#define ADS8332_Channel_4				((uint16_t)0x4000)//Select analog input channel 4
#define ADS8332_Channel_5				((uint16_t)0x5000)//Select analog input channel 5
#define ADS8332_Channel_6				((uint16_t)0x6000)//Select analog input channel 6
#define ADS8332_Channel_7				((uint16_t)0x7000)//Select analog input channel 7
#define ADS8332_WAKE_UP					((uint16_t)0xB000)//Wake up
#define ADS8332_READ_CFR				((uint16_t)0xC000)//Read CFR
#define ADS8332_READ_DATA				((uint16_t)0xD000)//Read data
#define ADS8332_WRITE_CFR				((uint16_t)0xE000)//Write CFR
#define ADS8332_DEFAULT_CFR				((uint16_t)0xF000)//Default mode

#define ADS8332_CH_AUTO					((uint16_t)0x0800)//Channel select mode
#define ADS8332_INT_CLK					((uint16_t)0x0400)//Conversion clock (CCLK) source select
#define ADS8332_TR_MANUL				((uint16_t)0x0200)//Trigger (conversion start) select
#define ADS8332_SMPL_250K				((uint16_t)0x0100)//Sample rate for Auto-Trigger moden
#define ADS8332_SMPL_500K				((uint16_t)0x0000)//Sample rate for Auto-Trigger moden
#define ADS8332_EOC_LOW					((uint16_t)0x0080)//Pin 10 polarity select when used as an
#define ADS8332_P10_EOC					((uint16_t)0x0040)//Pin 10 polarity select when used as an
#define ADS8332_P10_OUT					((uint16_t)0x0020)//Pin 10 I/O select for daisy-chain mode operation
#define ADS8332_AUTONAP_DIS				((uint16_t)0x0010)//Auto-Nap Power-Down enable/disable
#define ADS8332_NAP_DIS					((uint16_t)0x0008)//Nap Power-Down.
#define ADS8332_PD_DIS					((uint16_t)0x0004)//Deep Power-Down
#define ADS8332_TAG_OUT					((uint16_t)0x0002)//TAG bit output enable
#define ADS8332_NOSW_RST				((uint16_t)0x0001)//Software reset
void Qpcr_Delay(unsigned int n);
void ADS8832_Init(void);
void ADS8832_PrepareSpi(void);
void ADS8832_Reset(void);
void ADS8832_Convst(u8 ADC_x);
void ADS8832_WriteCfr(u16 CfrData,u8 ADC_x);
u16 ADS8832_ReadCfr(u8 ADC_x);
void ADS8832_SetChannel(u8 ChannelNum);
void ADS8832_16Channel_Sample(void);
void ADS8832_16Channel_10TimesSample(void);
void ADS8832_Cont2SampleCfg(void);
void ADS8832_TwoChanSample(u16* AdcDataBuf);
u16 ADS8832_Chan7Sample(void);
#endif
