#include "Sim800L.h"
extern char Transfer_cplt2;
extern char Rx_Buffer2[SIMCOM_DATA_IN_MAX_SIZE];
Sms_TypeDef hsms;

Simcom_TypeDef hsimcom;

extern CLCD_I2C_Name LCD1;
/////////////////////////////////////////////////////////////////////////
extern  void ShowTEST(char * abc);
extern void BlinkLed(void);
////////////////////////////////////////////////////////////////////////
void Init_Sim_Parameter(void )
{
	hsimcom.data_in_index=0;
	hsimcom.sim_inited=-1;
	hsimcom.retry=RETRY_NUM;
	hsimcom.sim_stat=SIM_UNKOWN;
	char data_in[SIMCOM_DATA_IN_MAX_SIZE];
	hsms.sms_inited=-1;
}
void simcom_puts(uint8_t* pData)
{
	HAL_UART_Transmit(&huart1, pData, strlen((char*)pData), 3000);
}
void TurnUart(void)
{
	HAL_UART_Receive_IT(&huart1, &hsimcom.data_in_tmp, 1);
}
void simcom_receive_data_it(void)
{
	if (hsimcom.data_in_index > SIMCOM_DATA_IN_MAX_SIZE - 1)
	{
		hsimcom.data_in_index = 0;
	}
	hsimcom.data_in[hsimcom.data_in_index] = hsimcom.data_in_tmp;
	Rx_Buffer2[hsimcom.data_in_index] = hsimcom.data_in_tmp;
	
	hsimcom.data_in[hsimcom.data_in_index + 1] = 0;
	Rx_Buffer2[hsimcom.data_in_index + 1] = 0;
	
	hsimcom.data_in_index ++;
	
	Transfer_cplt2 = 1;
	HAL_UART_Receive_IT(&huart1, &hsimcom.data_in_tmp, 1);
}
void deleteBuffer(char * buf)
{
		for (int i =0; i< SIMCOM_DATA_IN_MAX_SIZE ; i++)
	{
		buf[i] =0;
	}
}
void DeleDatain(void)
{
	
	hsimcom.data_in_index = 0;

	deleteBuffer((char*)hsimcom.data_in);
	
	deleteBuffer((char*)Rx_Buffer2);
}
int8_t Sim_SendCommand(char * command,char * response, uint32_t timeout)
{
	int8_t answer = -1;
	while (hsimcom.retry > 0)
	{
	
	DeleDatain();
	
	uint32_t time = HAL_GetTick();
	uint32_t time1 = HAL_GetTick();
	simcom_puts((uint8_t*)command);
	do	
		{ 
			while(Transfer_cplt2 ==0)
			{
				if(HAL_GetTick() - time > timeout) 
				{
					return -1;
				}
			}
				time = HAL_GetTick();
				// HAL_Delay(200);
				while((HAL_GetTick() - time < timeout))
				{
					if (HAL_UART_Receive_IT(&huart1, &hsimcom.data_in_tmp, 1) == HAL_OK)
					{
						time1 = HAL_GetTick();
					}
					else 
					{
						if ( HAL_GetTick() - time1 > 100)
						{
							Transfer_cplt2 =0;
							if( strstr((char*)hsimcom.data_in,response)!=NULL)
							{
								answer =1;
								if (DEBUG) { ShowTEST(hsimcom.data_in);HAL_Delay(1000); }		
							}
							else answer =-1;
							break;
						}
					}
				}
		}	
	while(answer == -1);
	if (answer==1) break;
	HAL_Delay(1000);
	hsimcom.retry --;	
	}	
	return answer;
}
int8_t Sim_init(void)
{
	Init_Sim_Parameter();
	hsimcom.retry = RETRY_NUM;
	//////////////////////////////////////////
	if (DEBUG) ShowTEST("Sim_init");
	HAL_Delay(1000);
	if (Sim_SendCommand("AT\r\n","OK",3000)==-1)
	{
		return -1;
	}
	hsimcom.retry = RETRY_NUM;
////////////////////////////////////////////	
	if (Sim_SendCommand("AT+CPIN?\r\n","+CPIN: READY",5000) == -1 )
	{
		hsimcom.sim_inited = -1;
		return -1 ;
	}
	HAL_Delay(2000);
	hsimcom.sim_wait_time = HAL_GetTick() + 50000;
//////////////////////////////////////////////
	while (1)
	{
		if (strstr((char*)hsimcom.data_in,"SMS Ready")!=NULL)
		{
			hsimcom.sim_stat = SIM_INSERTED;
			if (DEBUG) {  ShowTEST("SIM_INSERTED"); HAL_Delay(2000);}
			break;
		}
		else if (strstr((char*)hsimcom.data_in, "NOT INSERTED")!=NULL)
		{
			hsimcom.sim_inited = -1;
			if (DEBUG) { ShowTEST("SIM_NOT_INSERT");HAL_Delay(1000); }

			hsimcom.sim_stat = SIM_NOT_INSERT;
			return -1;
		}
		else if (hsimcom.sim_wait_time < HAL_GetTick())
		{
			break;
		}
	}
////////////////////////////////////////////////////////
	hsimcom.retry = RETRY_NUM;
	if (Sim_SendCommand("AT+CMGF=1\r\n","OK",3000) == 1)
	{
		hsms.sms_inited = 1;   
		//return;
	}
///////Set Echo Mode
//	hsimcom.retry = RETRY_NUM;
//		if (Sim_SendCommand("ATE0\r\n","OK",3000) ==1)
//	{
//		hsms.sms_inited = 1;
//		//return;
//	}

//	if (Sim_SendCommand("AT&W\r\n","OK",3000) == 1)
//	{
//		hsms.sms_inited = 1;
//		//return;
//	}
	return 1;
}


