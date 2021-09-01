/*
 * ModbusRTU.c
 *
 *
 *      Author: C
 */

#include "ModbusRTU.h"

static uint32_t ModbusRTU_Delaytime_Calculate(uint16_t BaudRate);
static uint16_t ModbusRTU_CRC16_Calculate(uint8_t *Date, uint8_t len);

ModbusRTU_HandleTypeDef Modbus1;

/**
 * @brief Init ModbusRTU by configing the delaytime value in tim3 interrupt callback
 *
 * @note  tim3 interrupt by 50us
 *
 * @retval Delaytime
 */
void ModbusRTU_init(void)
{
  Modbus1.Modbus_number = Modbus_1;
  Modbus1.Modbus_BaudRate = 9600;
  Modbus1.Modbus_Delaytime =
	  ModbusRTU_Delaytime_Calculate(Modbus1.Modbus_BaudRate);
  Modbus1.Modbus_Status = Modbus_Ready;
}

void ModbusRTU_Transmit(ModbusRTU_HandleTypeDef *Modbus,
	uint8_t Station_Address, Modbus_Function_Code Function_Code,
	uint8_t Register_Address, uint8_t Data_Len)
{
  uint8_t ModbusRTU_Transmit_Buff[256] = { 0 };
  uint16_t Modbus_CRC;

  ModbusRTU_Transmit_Buff[0] = Station_Address;
  ModbusRTU_Transmit_Buff[1] = Function_Code;
  ModbusRTU_Transmit_Buff[2] = (uint8_t) ((Register_Address >> 8) & 0x0F); //Register_Address MSB
  ModbusRTU_Transmit_Buff[3] = (uint8_t) ((Register_Address) & 0x0F); //Register_Address LSB
  ModbusRTU_Transmit_Buff[4] = 0x00;
  ModbusRTU_Transmit_Buff[5] = Data_Len;

  Modbus_CRC =
	  ModbusRTU_CRC16_Calculate((uint8_t*) ModbusRTU_Transmit_Buff, Data_Len);

  ModbusRTU_Transmit_Buff[7] = (uint8_t) ((Modbus_CRC) & 0x0F);
  ModbusRTU_Transmit_Buff[8] = (uint8_t) ((Modbus_CRC >> 8) & 0x0F);

  if (Modbus->Modbus_number == Modbus_1)
  {
	HAL_UART_Transmit_IT(&huart1, (uint8_t*) ModbusRTU_Transmit_Buff, Data_Len
		+ 2);
  }
}

/**
 * @brief Calculate the delay time with 3.5T
 *
 * @param BaudRate, always 9600 or will be 115200
 *
 * @retval Delaytime
 */
uint32_t ModbusRTU_Delaytime_Calculate(uint16_t BaudRate)
{
  uint16_t Delaytime;

  Delaytime = 7 * 220000UL / (BaudRate * 2);

  return Delaytime;
}

/**
 * @brief Calculate the delay time with 3.5T
 *
 * @param Data, a array saved the data to be calculate
 *
 * @param len, array len
 *
 * @retval CRC16 value
 */
uint16_t ModbusRTU_CRC16_Calculate(uint8_t *Data, uint8_t len)
{
  uint16_t crc = 0xFFFF;
  uint8_t i, j;

  for (j = 0; j < len; j++)
  {
	crc = crc ^ *Data++;
	for (i = 0; i < 8; i++)
	{
	  if ((crc & 0x0001) > 0)
	  {
		crc = crc >> 1;
		crc = crc ^ 0xa001;
	  }
	  else
	  {
		crc = crc >> 1;
	  }
	}
  }
  return (crc);
}
