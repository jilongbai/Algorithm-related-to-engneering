#ifndef _led_
#define _led_
#include "stm32f10x_conf.h"
extern void LED_Init(void);	  //LED≥ı ºªØ
#define led_on    GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define led_off   GPIO_SetBits(GPIOC, GPIO_Pin_13)

#endif



