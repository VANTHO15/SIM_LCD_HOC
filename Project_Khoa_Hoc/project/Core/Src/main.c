/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "matrix4x4.h"
#include "LCD.h"
#include "Sim800L.h"
#include "dht.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ADJUST 'A'
#define RESET 'B'
#define CORRECT 'C'
#define DELETE 'D'
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern CLCD_I2C_Name LCD1;
unsigned char button;

uint8_t Rh_byte1,Rh_byte2, Temp_byte1,Temp_byte2;
uint8_t SUM,RH,TEMP;
float Temperature ;
float Humidity;

uint16_t Rx_Data[1]; // bien chua du lieu tu flash
FLASH_EraseInitTypeDef EraseInitStruct; // struct cau hình xóa du lieu
uint32_t PAGEError;  // bien chua dia chi loi

uint8_t StateLED;

uint8_t TempWarn =100;
char TempSet[4];
uint8_t count=0;
uint8_t SettingFlag= 0;

uint8_t StatusFlag =0;
uint8_t hold=0;


uint8_t KnewFlag=0;
uint16_t KnewCount =0;




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
char  Transfer_cplt2;
char Rx_Buffer2[SIMCOM_DATA_IN_MAX_SIZE];
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart-> Instance == USART1) // UART hien tai la uart1 SIM
	{ 
		simcom_receive_data_it();
	}
}
void DelLineLCD( char y)
{
	CLCD_I2C_SetCursor(&LCD1, 0, y);
	for (uint8_t i=0;i<20;i++) 
	{
		CLCD_I2C_SetCursor(&LCD1, i, y);
		CLCD_I2C_WriteString(&LCD1," ");
	}
}
void SimWarn()
{
		if (Sim_init()==1)
		{
			if		(Sim_SendCommand("ATD0964121702;\r\n","OK",3000) == 1)
			{
					DelLineLCD(2);
					CLCD_I2C_SetCursor(&LCD1,0,2);
					CLCD_I2C_WriteString(&LCD1,"Status: Warning...");
					uint32_t time = HAL_GetTick();
					while (HAL_GetTick() - time < 30000) 
					{
						if(strstr((char*)Rx_Buffer2,"BUSY")!=NULL)
						{
							DelLineLCD(2);
							CLCD_I2C_SetCursor(&LCD1,0,2);
							CLCD_I2C_WriteString(&LCD1,"Status: BUSY");
							break;
						}
						if(strstr((char*)Rx_Buffer2,"NO CARRIER")!=NULL)
						{
							DelLineLCD(2);
							CLCD_I2C_SetCursor(&LCD1,0,2);
							CLCD_I2C_WriteString(&LCD1,"Status: NO CARRIER");
							break;
						}
						if(strstr((char*)Rx_Buffer2,"NO ANSWER")!=NULL)
						{
							DelLineLCD(2);
							CLCD_I2C_SetCursor(&LCD1,0,2);
							CLCD_I2C_WriteString(&LCD1,"Status: NO ANSWER");
							break;
						}
						if(strstr((char*)Rx_Buffer2,"NO DIALTONE")!=NULL)
						{
							DelLineLCD(2);
							CLCD_I2C_SetCursor(&LCD1,0,2);
							CLCD_I2C_WriteString(&LCD1,"Status:NO DIALTONE");
							break;
						}
					}		
					HAL_Delay(1000);
			}
		}	
}
void ProcessButton()
{
	if (button==ADJUST)
	{	
		CLCD_I2C_SetCursor(&LCD1,8,1);
		CLCD_I2C_BlinkOn(&LCD1);
		SettingFlag =1; 
	}	
	else if (button==DELETE)
	{
		if (count >0) 
		{
			count--;
			TempSet[count]='\0';
			DelLineLCD(1);
			CLCD_I2C_SetCursor(&LCD1,0,1);
			CLCD_I2C_WriteString(&LCD1,"TempSet:"); 
			char *k=TempSet;
			CLCD_I2C_WriteString(&LCD1,k);
		}
	}
	else if (button==RESET)
	{
		count=0;
		for (uint8_t i=0; i<4;i++) TempSet[i]=0;
		DelLineLCD(1);
		CLCD_I2C_SetCursor(&LCD1,0,1);
		CLCD_I2C_WriteString(&LCD1,"TempSet:"); 
		DeleDatain();
	}
	else if (button==CORRECT)
	{
		if (count>0)
		{
			CLCD_I2C_BlinkOff(&LCD1);
			switch(count)
			{
				case 1: TempWarn = TempSet[0]-48; break;
				case 2: TempWarn = (TempSet[0]-48)*10+(TempSet[1]-48); break;
				case 3: TempWarn = (TempSet[0]-48)*100+(TempSet[1]-48)*10+TempSet[2]-48; break;
				default:	break;
			} 
			CLCD_I2C_BlinkOff(&LCD1);
			SettingFlag=0;
		}
	}
	else 
	{
		if ((SettingFlag==1)&&(count<=2))
		{
			CLCD_I2C_WriteChar(&LCD1,button);
			TempSet[count++]=button;
		}
	}
}
int SavetoFlash(void)
{
	HAL_FLASH_Unlock();
	EraseInitStruct.Banks = 1;
	EraseInitStruct.TypeErase  = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = 0x08000000+1024*64; // dia chi page 64
	EraseInitStruct.NbPages    = 1;
	//xoa page
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
 {
   return HAL_FLASH_GetError (); // tr? v? l?i n?u không xóa thành công
 }
	//ghi page
 if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, 0x08000000+1024*64,StateLED) != HAL_OK)
 {
   return HAL_FLASH_GetError ();
 }
	HAL_FLASH_Lock();
	return HAL_OK;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	CLCD_I2C_Init(&LCD1, &hi2c1, (0x27<<1),20,4);
	
	
	CLCD_I2C_Clear(&LCD1);
	CLCD_I2C_SetCursor(&LCD1, 0, 0);
	CLCD_I2C_WriteString(&LCD1, "Temp:");
	
	CLCD_I2C_SetCursor(&LCD1, 12, 0);
	CLCD_I2C_WriteString(&LCD1, "Hum:");
	
	CLCD_I2C_SetCursor(&LCD1, 0, 1);
	CLCD_I2C_WriteString(&LCD1, "TempSet:"); 
	
	
	CLCD_I2C_SetCursor(&LCD1, 0, 2);
	CLCD_I2C_WriteString(&LCD1,"Status: SAFE"); 
	Rx_Data[0] = *(__IO uint32_t *)(0x08000000+1024*64);
	if (Rx_Data[0]==1) 
	{
		HAL_GPIO_WritePin(GPIOC,Led_Pin,0); 
		StateLED=1;
		CLCD_I2C_SetCursor(&LCD1, 0, 3);
		CLCD_I2C_WriteString(&LCD1,"Led: On");
	}
	else if (Rx_Data[0]==0) 
	{
		HAL_GPIO_WritePin(GPIOC,Led_Pin,1); 
		StateLED=0;
		CLCD_I2C_SetCursor(&LCD1, 0, 3);
		CLCD_I2C_WriteString(&LCD1,"Led: Off");
	}
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	TurnUart();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		DHT11();
		if (SettingFlag==1)
		{
			CLCD_I2C_BlinkOn(&LCD1);
			CLCD_I2C_SetCursor(&LCD1,8+count,1);
		}
		uint32_t time = HAL_GetTick();
		while (HAL_GetTick() - time <1500)
		{
			if (Temperature > TempWarn)
			{
				StatusFlag =1;
				if (KnewFlag==0)
					{
						hold =1;
						DelLineLCD(2);
						CLCD_I2C_SetCursor(&LCD1,0,2);
						CLCD_I2C_WriteString(&LCD1,"Status: TOO HOT");
						CLCD_I2C_SetCursor(&LCD1,8+count,1);
						SimWarn();
					}
					else 
					{
						if (hold ==1)
						{
							DelLineLCD(2);
							CLCD_I2C_SetCursor(&LCD1,0,2);
							CLCD_I2C_WriteString(&LCD1,"Status: Knew");
							CLCD_I2C_SetCursor(&LCD1,8+count,1);
							hold=0;
						}
					}
			}
			else
			{
				 if (StatusFlag==1)
					 {
						 DelLineLCD(2);
						 CLCD_I2C_SetCursor(&LCD1, 0, 2);
						 CLCD_I2C_WriteString(&LCD1,"Status: SAFE"); 
						 CLCD_I2C_SetCursor(&LCD1,8+count,1);
						 StatusFlag=0;
					 }
		 }
			button = get_key();
		 if (button)
		 {
			 ProcessButton();
		 }
			
		}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
if (htim->Instance==htim2.Instance)
	{
		if(get_key()=='*')
		{
				SavetoFlash();
				NVIC_SystemReset();
		}
		if(strstr((char*)Rx_Buffer2,"LedOn")!=NULL)
		{
			HAL_GPIO_WritePin(GPIOC,Led_Pin,0);
			StateLED=1;
			DelLineLCD(3);
			CLCD_I2C_SetCursor(&LCD1,0,3);
			CLCD_I2C_WriteString(&LCD1,"Led: On");
			DeleDatain();
			CLCD_I2C_SetCursor(&LCD1,8+count,1);
			SavetoFlash();
		}
		if(strstr((char*)Rx_Buffer2,"LedOff")!=NULL)
		{
			HAL_GPIO_WritePin(GPIOC,Led_Pin,1);
			StateLED=0;
			DelLineLCD(3);
			CLCD_I2C_SetCursor(&LCD1,0,3);
			CLCD_I2C_WriteString(&LCD1,"Led: Off");
			DeleDatain();
			CLCD_I2C_SetCursor(&LCD1,8+count,1);
			SavetoFlash();
		}
		if(strstr((char*)Rx_Buffer2,"Knew")!=NULL)
		{
			KnewFlag = 1;	
			DelLineLCD(2);
			CLCD_I2C_SetCursor(&LCD1,0,2);
			CLCD_I2C_WriteString(&LCD1,"Status: Knew");
			DeleDatain();
			CLCD_I2C_SetCursor(&LCD1,8+count,1);
		}
		if (KnewFlag==1) KnewCount++;
		if (KnewCount==360) { KnewFlag=0; KnewCount=0;}
		
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
