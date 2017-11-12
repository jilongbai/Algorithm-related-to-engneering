#ifndef _24c02_
#define _24c02_
#include"stm32f10x_conf.h"

typedef  union 
{
  float t;
  u8 s[4];
}PING_Float;

typedef  union 
{
  u16 t;
  u8 s[4];
}PING_u16;

typedef  union 
{
  unsigned long t;
  u8 s[4];
}PING_u32;





extern void IIC_Init(void);
extern void write_byte(u8 add,u8 d);
u8 I2Cread_byte(u8 add);
extern void save_float(u8 add,float t);
extern float read_float(u8 add);
void save_u16(u8 add,u16 t);
u16 read_u16(u8 add);
 void save_ulong(u8 add,unsigned long t);
unsigned long read_ulong(u8 add);

#endif

