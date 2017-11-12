#ifndef __TEMP_CORRECT_H
#define __TEMP_CORRECT_H

extern const float Temp_Comp[20];
//该表格记录校准设备在校准点的数据
extern const float Temp_Correct[20];
//该表格记录工装在个校准点的数据
extern const float Temp_Gongzhc[20];

float Temp_Correction(float temprature);

#endif











