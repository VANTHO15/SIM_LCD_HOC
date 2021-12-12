#ifndef __SIM800L_H__
#define __SIM800L_H__

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "string.h"
#include "usart.h"
#include "LCD.h"

#define SIMCOM_DATA_IN_MAX_SIZE 256
#define RETRY_NUM 4

#define DEBUG 0
///////////////////
typedef enum {SIM_UNKOWN, SIM_NOT_INSERT, SIM_INSERTED, SIM_INITED, SIM_CONNECTED} Enum_Simcom_Stat;
////////////////////////////////////////////////////////
typedef struct
{
	uint8_t phone_eep[12];
	uint8_t data_in_tmp;
	uint16_t data_in_index;
	uint32_t sim_wait_time;
	int8_t sim_inited;
	int8_t pwrkey;
	uint8_t retry;
	Enum_Simcom_Stat sim_stat;
	char data_in[SIMCOM_DATA_IN_MAX_SIZE];
} Simcom_TypeDef;
typedef struct
{
	uint8_t sms_inited;      // Su dung de kiem tra trang thai khi gui tin
}
Sms_TypeDef;

/////////////////////////////////////////////
void Init_Sim_Parameter(void );   
void simcom_puts(uint8_t* pData); 
void deleteBuffer(char * buf);
void simcom_receive_data_it(void);
void DeleDatain(void);
void TurnUart(void);
int8_t Sim_sendCommand(char* ,char* ,uint32_t ); // -1: ko phan hoi || phan hoi sai  1: phan hoi dung
int8_t Sim_init(void); // -1 : Khoi tao khong thanh cong
//////////////////////////////////////////


#endif /* __SIM800L_H__ */