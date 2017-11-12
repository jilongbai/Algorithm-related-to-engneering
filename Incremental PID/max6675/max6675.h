#ifndef _6675_
#define _6675_

 #include"stm32f10x_conf.h"
 extern float temper;
extern void delay_us(u16 us);
u16 read_max6675(void);  //????
void Max6675_Init(void);
void read_temper(void);//??????

#endif



