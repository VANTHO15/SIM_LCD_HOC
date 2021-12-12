#ifndef __DHT_H
#define __DHT_H

//#########################################################################################################################
#include "stm32f1xx_hal.h"
#include "tim.h"

#define DHT11_PORT DHT11_GPIO_Port
#define DHT11_PIN DHT11_Pin

//#########################################################################################################################


//#########################################################################################################################
void delay(uint16_t time);
void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void DHT11_Start(void);
uint8_t DHT11_CheckResponse(void);
uint8_t DHT11_Read(void);
void DHT11(void);
//void Display_Temp(uint8_t Temp_byte1,uint8_t Temp_byte2);
//void Display_Rh(float Hum);
//#########################################################################################################################
#endif 