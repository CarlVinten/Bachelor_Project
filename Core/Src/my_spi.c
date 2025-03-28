/*
 * my_spi.c
 *
 *  Created on: Mar 14, 2025
 *      Author: carlv
 *      Here simple commands will be programmed. Fx read, write or erase.
 */



#include "my_spi.h"
#include "stm32h5xx_hal_xspi.h"

void set_spi_command(XSPI_RegularCmdTypeDef *command_struct, uint32_t instruction, uint32_t instruction_wires, uint32_t address, uint32_t address_wires, //
		uint32_t data_wires, uint8_t data_length, uint32_t dummy_cycles){
	command_struct->OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG; 		// Constant.
	command_struct->IOSelect 		      = HAL_XSPI_SELECT_IO_3_0;				// Constant. Always the same pins
	command_struct->Instruction	          = instruction;						// Not constant.
	// Use defined instructions
	command_struct->InstructionMode	      = instruction_wires;
	// Not constant. Number of wires for transferring instructions.
	command_struct->InstructionWidth 	  = HAL_XSPI_INSTRUCTION_8_BITS;		// Constant - always 1 byte
	command_struct->InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;	// Constant - I don't know what this is.
	command_struct->Address 			  = address;							// Not constant
	// Between 0x00 and 0xFFFFFFFF.
	command_struct->AddressMode           = address_wires;						// Not constant
	// Number of wires for sending the address.
	command_struct->AddressWidth          = HAL_XSPI_INSTRUCTION_32_BITS;		// constant
	command_struct->AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;		// Constant - IDK
	command_struct->AlternateBytes        = 0xFFFF0000;							// I don't what alternate bytes are but it isn't used in the flash
	command_struct->AlternateBytesMode    = HAL_XSPI_ALT_BYTES_NONE;			// ---
	command_struct->AlternateBytesWidth   = HAL_XSPI_ALT_BYTES_32_BITS;			// ---
	command_struct->AlternateBytesDTRMode = HAL_XSPI_ALT_BYTES_DTR_DISABLE;		// ---
	command_struct->DataMode 			  = data_wires;
	// Number of wires for sending and receiving data.
	command_struct->DataLength 		      = data_length;
	// Length in bytes
	command_struct->DataDTRMode 		  = HAL_XSPI_DATA_DTR_DISABLE;			// Constant
	command_struct->DummyCycles 		  = dummy_cycles;						// Not constant
	// Enter number of dummy cycles.
	command_struct->DQSMode 			  = HAL_XSPI_DQS_ENABLE;				// Constant - IDK
	command_struct->SIOOMode 			  = HAL_XSPI_SIOO_INST_EVERY_CMD;		// Constant - IDK
}

/*HAL_StatusTypeDef write_page_SPI(uint8_t* write_buffer,XSPI_HandleTypeDef octo_handle){
	// set spi command
	XSPI_RegularCmdTypeDef spi_command;
	set_spi_command(&spi_command, )
	//write enable

	//Write command

	//write
}*/


HAL_StatusTypeDef write_data(char *read_buffer,XSPI_HandleTypeDef *octo_spi_handle){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	set_spi_command(&spi_command, MY_WREN, INST_1_WIRE, 0, ADDR_NONE, DATA_NONE, 0, 0);

}
HAL_StatusTypeDef read_device_id(char *read_buffer,XSPI_HandleTypeDef *octo_spi_handle){
	// Set command.
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	set_spi_command(&spi_command, MY_RDID, INST_1_WIRE, 0, ADDR_NONE, DATA_1_WIRE, THREE_BYTES, 0);
	// Set XSPI ready for read
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	if(octo_spi_return != HAL_OK){
		return octo_spi_return;
	}
	// Read
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle, read_buffer, ONE_MINUTE);
	read_buffer[3] = '\0';
	return octo_spi_return;
}

HAL_StatusTypeDef read_electronic_signature(char *read_buffer,XSPI_HandleTypeDef *octo_spi_handle){
	// Set command.
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	set_spi_command(&spi_command, MY_RES, INST_1_WIRE, 0, ADDR_NONE, DATA_1_WIRE, ONE_BYTE, 3);
	// Set XSPI ready for read
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	if(octo_spi_return != HAL_OK){
		return octo_spi_return;
	}
	// Read
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle, read_buffer, ONE_MINUTE);
	read_buffer[1] = '\0';
	return octo_spi_return;
}





















