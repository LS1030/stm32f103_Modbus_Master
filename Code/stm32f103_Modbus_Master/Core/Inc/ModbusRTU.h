/*
 * ModbusRTU.h
 *
 *  
 *      Author: C
 */

#ifndef INC_MODBUSRTU_H_
#define INC_MODBUSRTU_H_

#include "main.h"
#include "tim.h"
#include "usart.h"

typedef enum
{
  Read_Coils = 0x01,
  Read_Discrete_Inputs = 0x02,
  Read_Multiple_Holding_Registers = 0x03,
  Read_Input_Registers = 0x04,
  Write_Single_Coil = 0x05,
  Write_Single_Holding_Register = 0x06,
  Read_Exception_Status = 0x07,
  Diagnostic = 0x08,
  Get_Com_Event_Counter = 0x11,
  Get_Com_Event_Log = 0x12,
  Write_Multiple_Holding_Registers = 0x16,
  Read_File_Record = 0x20,
  Write_File_Record = 0x21
} Modbus_Function_Code;

typedef enum
{
  Modbus_Ready = 0x01, Modbus_Busy
} Modbus_StatusTypeDef;

typedef enum
{
  Modbus_1 = 0x01,
  Modbus_2,
  Modbus_3,
  Modbus_4,
  Modbus_5,
  Modbus_6,
  Modbus_7,
  Modbus_8,
  Modbus_9,
  Modbus_10
} Modbus_NumberTypeDef;

typedef struct _ModbusRTU_HandleTypeDef
{
  Modbus_NumberTypeDef Modbus_number;
  uint16_t Modbus_BaudRate;
  uint32_t Modbus_Delaytime;
  Modbus_StatusTypeDef Modbus_Status;
} ModbusRTU_HandleTypeDef;

extern ModbusRTU_HandleTypeDef Modbus1;

void ModbusRTU_init(void);
void ModbusRTU_Transmit(ModbusRTU_HandleTypeDef *Modbus,
	uint8_t Station_Address, Modbus_Function_Code Function_Code,
	uint8_t Register_Address, uint8_t Date_Len);
void ModbusRTU_Receive(uint8_t Station_Address,
	Modbus_Function_Code Function_Code, uint8_t *Date, uint8_t Size);

#endif /* INC_MODBUSRTU_H_ */
