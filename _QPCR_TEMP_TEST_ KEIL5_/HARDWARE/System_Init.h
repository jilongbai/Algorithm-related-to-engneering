#ifndef __SYSTEM_INIT_H
#define __SYSTEM_INIT_H
#include "stm32f2xx.h"

#define TIM_CLK_FREQ      30000000
#define START_BYTE1 0xA5
#define START_BYTE2 0x5A
#define BUF_SIZE 256

#define ADC_0    0
#define ADC_1    1
#define ADC_2    2

#define ADC0_CS_H()       GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define ADC0_CS_L()       GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define ADC1_CS_H()       GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define ADC1_CS_L()       GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define ADC2_CS_H()       GPIO_SetBits(GPIOG, GPIO_Pin_1)
#define ADC2_CS_L()       GPIO_ResetBits(GPIOG, GPIO_Pin_1)

#define ADC0_CONV_H()     GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define ADC0_CONV_L()     GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define ADC1_CONV_H()     GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define ADC1_CONV_L()     GPIO_ResetBits(GPIOB, GPIO_Pin_2)
#define ADC2_CONV_H()     GPIO_SetBits(GPIOF, GPIO_Pin_15)
#define ADC2_CONV_L()     GPIO_ResetBits(GPIOF, GPIO_Pin_15)

#define ADC0_EOC()        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define ADC1_EOC()        GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11)
#define ADC2_EOC()        GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_0)
typedef struct{
	u8 rx_buf[BUF_SIZE];
	u8 rx_index;
	u8 rx_data_size;
	u8 tx_buf[BUF_SIZE];
	u8 tx_index;
	u8 tx_data_size;
}struct_uart;

void* INT_To_ASCII(u16 data,char* lptr);
void* TwoPoint_To_ASCII(float data,char* lptr);
void* TreePoint_To_ASCII(float data,char* lptr);
void Sys_GPIO_Init(void);
void Sys_TIMx_Init(void);
void Sys_NVIC_Init(void);
void USART1_Init(void);
void USART2_Init(void);
void USART2_Tx(u16 Data_count);
void USART1_Tx(u16 Data_count);
void Sys_SPI1_Init(void);
void Sys_ADC_Init(void);
void Sys_DAC_Init(void);
void SET_DAC_VALUE(float DAC1_VALUE,float DAC2_VALUE);
void TIM3_Init(void);
#endif 
