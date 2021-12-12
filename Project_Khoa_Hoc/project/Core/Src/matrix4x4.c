#include "matrix4x4.h"

unsigned char check_but( void )
{
	ROW1_0;
	ROW2_0;
	ROW3_0;
	ROW4_0;
	if (!COL1||!COL2||!COL3||!COL4) return 1;
	return 0;
}
void scan_row(unsigned char r)
{
	ROW1_1;
	ROW2_1;
	ROW3_1;
	ROW4_1;
	if (r==1) ROW1_0;
	else if(r==2) ROW2_0;
	else if(r==3) ROW3_0;
	else if(r==4) ROW4_0;
}
unsigned char check_col(void )
{
	unsigned char c=0;
	if (!COL1) c=1;
	else if (!COL2) c=2;
	else if (!COL3) c=3;
	else if (!COL4) c=4;
	return c; // Tra ve vi tri cot
}
unsigned char get_key(void )
{
	unsigned char row,col;
	if (check_but())
	{
		HAL_Delay(20);
		if (check_but())
		{
			for (row =1; row <=4; row++)
			{
				scan_row(row);
				col=check_col();
				if (col>0) 
				{
					while (check_but());
					return (KEYS_PAD[row-1][col-1]);
				}
			}
		}
	}
	return 0;
}