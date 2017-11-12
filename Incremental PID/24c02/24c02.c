		   /*
IO口模拟I2C时序

PE8(44pin)-->SCL
PE9(45PIN)-->SDA
*/	

#include"24c02.h"

#define scl_0 GPIO_ResetBits(GPIOB, GPIO_Pin_6)
#define scl_1 GPIO_SetBits(GPIOB, GPIO_Pin_6)

#define sda  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)

#define sda_0 GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define sda_1 GPIO_SetBits(GPIOB, GPIO_Pin_7)



//void delay_1us()
//{
// 

//}

void i2c_delay(int n)
{u8 i;
  while(n--)
 for(i=0;i<6;i++);
}
void IIC_Init() 
{ GPIO_InitTypeDef io;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  io.GPIO_Pin=GPIO_Pin_6;  //SCL
  io.GPIO_Speed=GPIO_Speed_50MHz;
  io.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB,&io);
}
void sda_out()   //
{
  GPIO_InitTypeDef io;
  io.GPIO_Pin=GPIO_Pin_7;  //
  io.GPIO_Speed=GPIO_Speed_50MHz;
  io.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB,&io);
}
void sda_in()
{
  GPIO_InitTypeDef io;
  io.GPIO_Pin=GPIO_Pin_7;  //
  io.GPIO_Speed=GPIO_Speed_50MHz;
  io.GPIO_Mode=GPIO_Mode_IPU;
  GPIO_Init(GPIOB,&io);
}


void i2c_start()
{
 sda_out();
 sda_1;
 scl_1;
 i2c_delay(5);
 sda_0;
 i2c_delay(5);
 scl_0;
 i2c_delay(5);
}
void i2c_stop()
{
 scl_0;
 sda_out();
 sda_0;
 i2c_delay(5);
 scl_1;
 i2c_delay(5);
 sda_1;
 i2c_delay(5);
}

void i2c_send(u8 d)
{
 u8 i;
 sda_out();
 for(i=0;i<8;i++)
 {
   scl_0;
   if(d&0x80)
     sda_1;
   else
     sda_0;
   i2c_delay(5);
   scl_1;
   i2c_delay(5);
   d<<=1;
 }
  scl_0;
}


u8 i2c_get()    //读位
{
 u8 d,i;
 d=0;
 // scl_0;
 sda_in();
 for(i=0;i<8;i++)
 {
   scl_1;
   d<<=1; 
   i2c_delay(2);
   if(sda)
   d++; 
   scl_0;
   i2c_delay(2);
 }
 return d;
}

/* 
u8 iicwait_ack()
{
 volatile int c;
  c=0; 
 scl_0;
 sda_out();  
  sda_1;
 // scl_0;
 i2c_delay(4);
  scl_1;
 i2c_delay(4);
  sda_in();
  i2c_delay(4);
  while(sda==1)
  {	
   
    if(c++>2500)
	{
	 //i2c_stop();
	 scl_0;
	 return 1;  //等应答失败
	}
  }
  while(sda==0);
  scl_0;
  return 0;//等应答成功
}
*/
u8 iicwait_ack(void)
{
	u8 ucErrTime=0;
	scl_0;
	sda_in();      //SDA设置为输入  
	sda_1;i2c_delay(4);	   
	scl_1;i2c_delay(4);	 
	while(sda==1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
		     scl_0;
			i2c_stop();
			return 1;
		}
	}
	scl_0;//时钟输出0 	   
	return 0;  
}


void write_byte(u8 add,u8 d)
{
 u8 st;
 i2c_start();
 i2c_send(0xa0);
  iicwait_ack();
 //show_data(0x98, st);
 i2c_send(add);
 iicwait_ack();
 i2c_send(d);
 iicwait_ack();
 i2c_stop();
 //i2c_delay(20);
 do
 { i2c_delay(20);
  i2c_start();
  i2c_send(0xa0);
  st=iicwait_ack();
  i2c_stop();
 
 }while(st==1);
 // i2c_delay(200000);

}
u8 I2Cread_byte(u8 add)
{
 u8 d;
  i2c_start();
 i2c_send(0xa0);
 iicwait_ack();
 i2c_send(add);
 iicwait_ack();
 i2c_start();
 i2c_send(0xa1);
 iicwait_ack();
 d=i2c_get() ;
 i2c_stop();
 // i2c_delay(20000);
 return d;
}

void save_float(u8 add,float t)
{
PING_Float AA;
  AA.t=t;
  write_byte(add++,AA.s[0]);
  write_byte(add++,AA.s[1]);
  write_byte(add++,AA.s[2]);
  write_byte(add++,AA.s[3]);
}
float read_float(u8 add)
{
  PING_Float AA;
   AA.s[0]=I2Cread_byte(add++);
   AA.s[1]=I2Cread_byte(add++);
   AA.s[2]=I2Cread_byte(add++);
   AA.s[3]=I2Cread_byte(add++);
   return AA.t;
}

void save_u16(u8 add,u16 t)
{
 PING_u16 AA;
  AA.t=t;
  write_byte(add++,AA.s[0]);
  write_byte(add++,AA.s[1]);
}
u16 read_u16(u8 add)
{
  PING_u16 AA;
   AA.s[0]=I2Cread_byte(add++);
   AA.s[1]=I2Cread_byte(add++);
   return AA.t;
}

void save_ulong(u8 add,unsigned long t)
{
 PING_u32 AA;
  AA.t=t;
  write_byte(add++,AA.s[0]);
  write_byte(add++,AA.s[1]);
  write_byte(add++,AA.s[2]);
  write_byte(add++,AA.s[3]);
}
unsigned long read_ulong(u8 add)
{
  PING_u32 AA;
   AA.s[0]=I2Cread_byte(add++);
   AA.s[1]=I2Cread_byte(add++);
   AA.s[2]=I2Cread_byte(add++);
   AA.s[3]=I2Cread_byte(add++);
   return AA.t;
}
































