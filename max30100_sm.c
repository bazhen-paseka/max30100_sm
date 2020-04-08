/**
* \file
* \version 1.0
* \author bazhen.levkovets
** \date 2020
*
*************************************************************************************
* \copyright	Bazhen Levkovets
* \copyright	Brovary, Kyiv region
* \copyright	Ukraine
*
*************************************************************************************
*
* \brief
*
*/

/*
**************************************************************************
*							INCLUDE FILES
**************************************************************************
*/
	#include "max30100_sm.h"
/*
**************************************************************************
*							LOCAL DEFINES
**************************************************************************
*/

	//	#define SET_BIT(var, pos) ((var) |= (1UL << (pos)))
		#define CLR_BIT(var, pos) (var &= ~(1UL << (pos)))
		#define CHECK_BIT(var, pos) (((var) & (1UL << (pos))) != 0)

/*
**************************************************************************
*							LOCAL CONSTANTS
**************************************************************************
*/
/*
**************************************************************************
*						    LOCAL DATA TYPES
**************************************************************************
*/
/*
**************************************************************************
*							  LOCAL TABLES
**************************************************************************
*/
/*
**************************************************************************
*								 MACRO'S
**************************************************************************
*/

/*
**************************************************************************
*						    GLOBAL VARIABLES
**************************************************************************
*/
		lcd1602_fc113_struct h1_lcd1602 =		{
			.i2c = &hi2c1,
			.device_i2c_address = LCD1602_I2C_ADDR
		};
		//----------------------------------------------------------

		bh1750_struct h1_bh1750 =				{
			.i2c = &hi2c1,
			.device_i2c_address = BH1750_I2C_ADDR
		};
		//----------------------------------------------------------

		max30100_struct h1_max30100 = 			{
			.i2c = &hi2c1,
			.device_i2c_address = MAX30100_I2C_ADR
		};
		//----------------------------------------------------------

		int test = 0;
/*
**************************************************************************
*                        LOCAL FUNCTION PROTOTYPES
**************************************************************************
*/
	
/*
**************************************************************************
*                           GLOBAL FUNCTIONS
**************************************************************************
*/

void MAX30100_Init(void) {

	int soft_version_arr_int[3];
	soft_version_arr_int[0] = ((SOFT_VERSION) / 100) %10 ;
	soft_version_arr_int[1] = ((SOFT_VERSION) /  10) %10 ;
	soft_version_arr_int[2] = ((SOFT_VERSION)      ) %10 ;

	char DataChar[100];
	sprintf(DataChar,"\r\n\tMAX30100 2020-April-05 v%d.%d.%d \r\n\tUART1 for debug on speed 115200/8-N-1\r\n\r\n",
			soft_version_arr_int[0], soft_version_arr_int[1], soft_version_arr_int[2]);
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);

	LCD1602_Init(&h1_lcd1602);
	LCD1602_scan_I2C_bus(&h1_lcd1602);
	LCD1602_Scan_I2C_to_UART(&h1_lcd1602, &huart1);

	HAL_StatusTypeDef res = BH1750_Init( &h1_bh1750 );
	sprintf(DataChar,"\r\n\tBH1750 init status: %d;\r\n", (int)res);
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
	LCD1602_Clear(&h1_lcd1602);

	 I2Cdev_init(&hi2c1);
	 uint8_t dev_address = MAX30100_I2C_ADR;
	 uint8_t ModeConfiguration = 0b01000010;
	 I2Cdev_writeByte(dev_address, 6, ModeConfiguration);
}
//************************************************************************

void MAX30100_Main(void) {
	HAL_Delay(3000);
	uint8_t dev_address = MAX30100_I2C_ADR;

	char DataChar[100];
	sprintf(DataChar,"%d) \t", test++);
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);

//	uint8_t ModeConfiguration = 0b01001011;
//	I2Cdev_writeByte(dev_address, 6, ModeConfiguration);

	uint8_t max30_u8[10];
	HAL_I2C_Master_Receive(&hi2c1, dev_address<<1, max30_u8, 10, 200);

	sprintf(DataChar,"\r\n" );
	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);

	for (int reg=0; reg<10; reg++) {
		sprintf(DataChar,"%d) ", reg );
		HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
		for (int bit=7; bit>=0; bit--) {
			sprintf(DataChar,"%d ",	CHECK_BIT(max30_u8[reg], bit) );
			HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
		}
		sprintf(DataChar,"\r\n" );
		HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
	}


//	uint32_t lux_u32 = BH1750_Main( &h1_bh1750 );
//	sprintf(DataChar,"lux: %d; ", (int)lux_u32);
//	HAL_UART_Transmit(&huart1, (uint8_t *)DataChar, strlen(DataChar), 100);
//
//	sprintf(DataChar,"%04d) Lux=%05d;", test, (int)lux_u32);
//	LCD1602_Print_Line(&h1_lcd1602, DataChar, strlen(DataChar));
}
//-------------------------------------------------------------------------------------------------


//************************************************************************

/*
**************************************************************************
*                           LOCAL FUNCTIONS
**************************************************************************
*/

//************************************************************************
