#include "dht.h"

extern uint8_t Rh_byte1,Rh_byte2, Temp_byte1,Temp_byte2;
extern uint8_t SUM,RH,TEMP;
extern float Temperature ;
extern float Humidity;
uint8_t Presence=0;

void delay(uint16_t time)
{
	__HAL_TIM_SET_COUNTER(&htim2,0);
	while ((__HAL_TIM_GET_COUNTER(&htim2))<time);
}

void Set_Pin_Output(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct ={0};
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
}
void Set_Pin_Input(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStruct ={0};
	GPIO_InitStruct.Pin = DHT11_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Speed = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOx,&GPIO_InitStruct);
}
void DHT11_Start(void)
{
	Set_Pin_Output(DHT11_PORT, DHT11_PIN);
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN,0);
	delay(18000);
	HAL_GPIO_WritePin (DHT11_PORT, DHT11_PIN,1);
	delay(20);
	Set_Pin_Input(DHT11_PORT,DHT11_PIN);
}
uint8_t DHT11_CheckResponse(void)
{
	uint8_t Response =0;
	delay(40);
	if (!(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)))
	{
		delay(80);
		if(HAL_GPIO_ReadPin (DHT11_PORT, DHT11_PIN)) 
			Response=1;
		else Response=-1;
	}
	while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN));
	return Response;
}
uint8_t DHT11_Read(void)
{
	uint8_t i,j;
	for (j=0; j<8;j++)
	{
		while (!(HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)));
		delay(40);
		if(!(HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)))
		{
			i&=~(1<<(7-j)); 
		}
		else i|=(1<<(7-j));
		while ((HAL_GPIO_ReadPin(DHT11_PORT,DHT11_PIN)));
	}
	return i;
}
//void Display_Temp(uint8_t Temp_byte1,uint8_t Temp_byte2)
//{
//	char str[10];
//	CLCD_I2C_SetCursor(&LCD1,5,0);
//	
//	sprintf(str,"%d.%d%cC",Temp_byte1,Temp_byte2,0xDF);
//	CLCD_I2C_WriteString(&LCD1,str);
//}
//void Display_Rh(float Hum)
//{
//	char str[10];
//	CLCD_I2C_SetCursor(&LCD1,16,0);
//	
//	sprintf (str, "%.0f%c",Hum,0x25);
//	CLCD_I2C_WriteString(&LCD1,str);
//}
void DHT11(void)
{
//	CLCD_I2C_BlinkOff(&LCD1);
//	Display_Temp(Temp_byte1, Temp_byte2);
//	Display_Rh(Humidity);
				
	DHT11_Start();
	Presence = DHT11_CheckResponse();

	Rh_byte1 = DHT11_Read();

	Rh_byte2 = DHT11_Read();
	Temp_byte1 = DHT11_Read();
	Temp_byte2 = DHT11_Read();
	SUM = DHT11_Read();

	TEMP = Temp_byte1;
	RH = Rh_byte1;
		
	Temperature = (float) TEMP;
	Humidity = (float) RH;
}