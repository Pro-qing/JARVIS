#ifndef __ADC_H
#define __ADC_H	
#include "sys.h"
#include "system.h"
#define Battery_Ch    8 //Battery voltage, ADC channel 14 //电池电压，ADC通道14
#define SLIDE_POSITION_ADC 11
#define CAR_MODE_ADC 12  //Potentiometer, ADC channel 8 //电位器，ADC通道8
#define SERVO_BALANCE_ADC  13

void Adc_Init(void);
u16 Get_Adc(u8 ch);
float Get_battery_volt(void) ;
u16 Get_adc_Average(u8 chn, u8 times);
extern float Voltage,Voltage_Count,Voltage_All; 
float Get_Current1_volt(u8 ch);

#endif 


