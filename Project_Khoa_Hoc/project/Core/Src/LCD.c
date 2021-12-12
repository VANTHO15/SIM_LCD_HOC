/******************************************************************************************************************
@File:  	CLCD I2C Chip PCF8574
@Author:  Huy Dang
@Website: 
@Youtube: 
Huong dan su dung:
- Su dung thu vien HAL
- Khoi tao bien LCD: 
LCD1;
- Khoi tao LCD do: CLCD_I2C_Init(&LCD1,&hi2c1,(0x27<<1),20,4);
- Su dung cac ham phai truyen dia chi cua LCD do: 
CLCD_I2C_SetCursor(&LCD1, 0, 1);
CLCD_I2C_WriteString(&LCD1,"Hello Deviot");
******************************************************************************************************************/
#include "LCD.h"
//************************** Low Level Function ****************************************************************//
uint8_t currentline;
uint8_t Xcursor,Ycursor,countWriteChar,CurrentWriteChar;
CLCD_I2C_Name LCD1;
static void CLCD_Delay(uint16_t Time)
{
	HAL_Delay(Time);
}
static void CLCD_WriteI2C(CLCD_I2C_Name* LCD, uint8_t Data, uint8_t Mode)
{
	char Data_H;
	char Data_L;
	uint8_t Data_I2C[4];
	Data_H = Data&0xF0;
	Data_L = (Data<<4)&0xF0;
	if(LCD->BACKLIGHT)
	{
		Data_H |= LCD_BACKLIGHT; 
		Data_L |= LCD_BACKLIGHT; 
	}
	if(Mode == CLCD_DATA)
	{
		Data_H |= LCD_RS;
		Data_L |= LCD_RS;
	}
	else if(Mode == CLCD_COMMAND)
	{
		Data_H &= ~LCD_RS;
		Data_L &= ~LCD_RS;
	}
	Data_I2C[0] = Data_H|LCD_EN;
	CLCD_Delay(1);
	Data_I2C[1] = Data_H;
	Data_I2C[2] = Data_L|LCD_EN;
	CLCD_Delay(1);
	Data_I2C[3] = Data_L;
	HAL_I2C_Master_Transmit(LCD->I2C, LCD->ADDRESS, (uint8_t *)Data_I2C, sizeof(Data_I2C), 1000);
	
}
//************************** High Level Function ****************************************************************//
void CLCD_I2C_Init(CLCD_I2C_Name* LCD, I2C_HandleTypeDef* hi2c_CLCD, uint8_t Address, uint8_t Colums, uint8_t Rows)
{
	LCD->I2C = hi2c_CLCD;
	LCD->ADDRESS = Address;
	LCD->COLUMS = Colums;
	LCD->ROWS = Rows;
	
	LCD->FUNCTIONSET = LCD_FUNCTIONSET|LCD_4BITMODE|LCD_2LINE|LCD_5x8DOTS;
	LCD->ENTRYMODE = LCD_ENTRYMODESET|LCD_ENTRYLEFT|LCD_ENTRYSHIFTDECREMENT;
	LCD->DISPLAYCTRL = LCD_DISPLAYCONTROL|LCD_DISPLAYON|LCD_CURSOROFF|LCD_BLINKOFF;
	LCD->CURSORSHIFT = LCD_CURSORSHIFT|LCD_CURSORMOVE|LCD_MOVERIGHT;
	LCD->BACKLIGHT = LCD_BACKLIGHT;

	CLCD_Delay(50);
	CLCD_WriteI2C(LCD, 0x33, CLCD_COMMAND);
//	CLCD_Delay(5);
	CLCD_WriteI2C(LCD, 0x33, CLCD_COMMAND);
	CLCD_Delay(5);
	CLCD_WriteI2C(LCD, 0x32, CLCD_COMMAND);
	CLCD_Delay(5);
	CLCD_WriteI2C(LCD, 0x20, CLCD_COMMAND);
	CLCD_Delay(5);
	
	CLCD_WriteI2C(LCD, LCD->ENTRYMODE,CLCD_COMMAND);
	CLCD_WriteI2C(LCD, LCD->DISPLAYCTRL,CLCD_COMMAND);
	CLCD_WriteI2C(LCD, LCD->CURSORSHIFT,CLCD_COMMAND);
	CLCD_WriteI2C(LCD, LCD->FUNCTIONSET,CLCD_COMMAND);
	
	CLCD_WriteI2C(LCD, LCD_CLEARDISPLAY,CLCD_COMMAND);
	CLCD_WriteI2C(LCD, LCD_RETURNHOME,CLCD_COMMAND);
}
void CLCD_I2C_SetCursor(CLCD_I2C_Name* LCD, uint8_t Xpos, uint8_t Ypos)
{
	uint8_t DRAM_ADDRESS = 0x00;
	if(Xpos >= LCD->COLUMS)
	{
		Xpos = LCD->COLUMS - 1;
	}
	if(Ypos >= LCD->ROWS)
	{
		Ypos = LCD->ROWS -1;
	}
	if(Ypos == 0)
	{
		DRAM_ADDRESS = 0x00 + Xpos;
	}
	else if(Ypos == 1)
	{
		DRAM_ADDRESS = 0x40 + Xpos;
	}
	else if(Ypos == 2)
	{
		DRAM_ADDRESS = 0x14 + Xpos;
	}
	else if(Ypos == 3)
	{
		DRAM_ADDRESS = 0x54 + Xpos;
	}
	Xcursor = Xpos;
	
	Ycursor = Ypos;
	currentline = Ycursor;
	countWriteChar = Xcursor;
	CLCD_WriteI2C(LCD, LCD_SETDDRAMADDR|DRAM_ADDRESS, CLCD_COMMAND);
}
void CLCD_I2C_WriteChar(CLCD_I2C_Name* LCD, char character)
{
	CLCD_WriteI2C(LCD, character, CLCD_DATA);
	countWriteChar++;
	if (CurrentWriteChar)
	{
	if (countWriteChar ==20)
	{		
	switch(currentline)
			{
				case 0:	CLCD_I2C_SetCursor(&LCD1, 0, 1);
								break;
				case 1:	CLCD_I2C_SetCursor(&LCD1, 0, 2);
								break;
				case 2:	CLCD_I2C_SetCursor(&LCD1, 0, 3);
								break;
				case 3:	CLCD_I2C_SetCursor(&LCD1, 0, 0);
								break;
				default: 
								break;
			}
		}
	}
}
void CLCD_I2C_WriteString(CLCD_I2C_Name* LCD, char *String)
{
	while (*String)
	{
	CurrentWriteChar=0;	
	for (uint8_t i=0;i<19-Xcursor+1;i++)
		{
			if (*String)
			{
				CLCD_I2C_WriteChar(LCD, *String++);
			}
			else break;
		}
		if (*String) 
		{
			switch(currentline)
			{
				case 0:	CLCD_I2C_SetCursor(&LCD1, 0, 1);
								break;
				case 1:	CLCD_I2C_SetCursor(&LCD1, 0, 2);
								break;
				case 2:	CLCD_I2C_SetCursor(&LCD1, 0, 3);
								break;
				case 3:	CLCD_I2C_SetCursor(&LCD1, 0, 0);
								break;
				default: 
								break;
			}
		}
		else break;
				
		}
	CurrentWriteChar=1;
}
void CLCD_I2C_Clear(CLCD_I2C_Name* LCD)
{
	CLCD_WriteI2C(LCD, LCD_CLEARDISPLAY, CLCD_COMMAND);
	CLCD_Delay(5);
}
void CLCD_I2C_ReturnHome(CLCD_I2C_Name* LCD)
{
	CLCD_WriteI2C(LCD, LCD_RETURNHOME, CLCD_COMMAND);
	CLCD_Delay(5);
}
void CLCD_I2C_CursorOn(CLCD_I2C_Name* LCD)
{
	LCD->DISPLAYCTRL |= LCD_CURSORON;
	CLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
}
void CLCD_I2C_CursorOff(CLCD_I2C_Name* LCD)
{
	LCD->DISPLAYCTRL &= ~LCD_CURSORON;
	CLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
}
void CLCD_I2C_BlinkOn(CLCD_I2C_Name* LCD)
{
	LCD->DISPLAYCTRL |= LCD_BLINKON;
	CLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
}
void CLCD_I2C_BlinkOff(CLCD_I2C_Name* LCD)
{
	LCD->DISPLAYCTRL &= ~LCD_BLINKON;
	CLCD_WriteI2C(LCD, LCD->DISPLAYCTRL, CLCD_COMMAND);
}
