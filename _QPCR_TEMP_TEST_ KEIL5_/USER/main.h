/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/main.h 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    13-April-2012
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include <stdio.h>
#include "System_Init.h"
#include "ADS8832.h"
#include "IIC_EEprom.h"
//#include "temp_correct.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

float Calculate_Temp_SZ(u16 AD_Res , float UP_Res , float V_ref );
float Calculate_Temp_Less_32(float ResVal);
float Successive_Linear_Interpolation_Temp_Correction(float ResVal);
float Linear_Interpolation(float x0, float y0, float x1, float y1, float x);
float Temp_Correction(float temprature);
u16 Mid_Data_Filter(u16 CHx_Value , u16 Mid_Num , u8 Chx);//Mid_Data_Num ÎªÆæÊý
void New_Order(void);
void New_Comm(void);
void Dispaly(void);
u16 Corr_K_B(u8 Channel,u16 AD_Temp);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
