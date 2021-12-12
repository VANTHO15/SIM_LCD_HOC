#ifndef __MATRIX4x4_H
#define __MATRIX4x4_H

#include "gpio.h"

#define ROW1_0 HAL_GPIO_WritePin(GPIOB,R1_Pin,0)
#define ROW2_0 HAL_GPIO_WritePin(GPIOB,R2_Pin,0)
#define ROW3_0 HAL_GPIO_WritePin(GPIOB,R3_Pin,0)
#define ROW4_0 HAL_GPIO_WritePin(GPIOB,R4_Pin,0)

#define ROW1_1 HAL_GPIO_WritePin(GPIOB,R1_Pin,1)
#define ROW2_1 HAL_GPIO_WritePin(GPIOB,R2_Pin,1)
#define ROW3_1 HAL_GPIO_WritePin(GPIOB,R3_Pin,1)
#define ROW4_1 HAL_GPIO_WritePin(GPIOB,R4_Pin,1)

#define COL1 HAL_GPIO_ReadPin(GPIOA,C1_Pin)
#define COL2 HAL_GPIO_ReadPin(GPIOA,C2_Pin)
#define COL3 HAL_GPIO_ReadPin(GPIOA,C3_Pin)
#define COL4 HAL_GPIO_ReadPin(GPIOA,C4_Pin)

static unsigned char KEYS_PAD[4][4] ={'1', '2' , '3','A',
																			'4' ,'5' ,'6','B',
                                       '7' ,'8' ,'9', 'C',
                                       '*' ,'0','#','D'};
unsigned char check_but( void );
void scan_row(unsigned char );
unsigned char check_col();
unsigned char get_key();
#endif