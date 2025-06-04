/*
 * my_spi.c
 *
 *  Created on: Mar 14, 2025
 *      Author: carlv
 *      Here simple commands will be programmed. Fx read, write or erase.
 */



#include "my_spi.h"
#include "stm32h5xx_hal_xspi.h"
#include <string.h>

void set_spi_command(XSPI_RegularCmdTypeDef *command_struct, uint32_t instruction, //
					 uint32_t instruction_wires, uint32_t address, uint32_t address_wires, //
					 uint32_t data_wires, uint32_t data_length, uint32_t dummy_cycles){
	command_struct->OperationType         = HAL_XSPI_OPTYPE_COMMON_CFG; 		// Constant.
	command_struct->IOSelect 		      = HAL_XSPI_SELECT_IO_3_0;				// Constant. Always the same pins
	command_struct->Instruction	          = instruction;						// Not constant.
	// Use defined instructions
	command_struct->InstructionMode	      = instruction_wires;
	// Not constant. Number of wires for transferring instructions.
	command_struct->InstructionWidth 	  = HAL_XSPI_INSTRUCTION_8_BITS;		// Constant - always 1 byte
	command_struct->InstructionDTRMode    = HAL_XSPI_INSTRUCTION_DTR_DISABLE;	// Constant - DTR = Double transfer rate
	command_struct->Address 			  = address;							// Not constant
	// Between 0x00 and 0xFFFFFFFF.
	command_struct->AddressMode           = address_wires;						// Not constant
	// Number of wires for sending the address.
	command_struct->AddressWidth          = HAL_XSPI_ADDRESS_24_BITS;		// constant
	command_struct->AddressDTRMode        = HAL_XSPI_ADDRESS_DTR_DISABLE;		// Constant - IDK
	command_struct->AlternateBytes        = 0x00;							    // I don't what alternate bytes are but it isn't used in the flash
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
	command_struct->DQSMode 			  = HAL_XSPI_DQS_DISABLE;				// Constant - IDK
	command_struct->SIOOMode 			  = HAL_XSPI_SIOO_INST_EVERY_CMD;		// Constant - IDK


}


HAL_StatusTypeDef read_device_id(uint8_t *read_buffer,XSPI_HandleTypeDef *octo_spi_handle){
	// Set command.
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	set_spi_command(&spi_command, MY_RDID, INST_1_WIRE, 0, ADDR_NONE, DATA_1_WIRE, THREE_BYTES, 0);
	// Set XSPI ready for read
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	// Read
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle, read_buffer, ONE_MINUTE);
	read_buffer[3] = '\0';
	return octo_spi_return;
}

HAL_StatusTypeDef read_electronic_signature(uint8_t *read_buffer,XSPI_HandleTypeDef *octo_spi_handle){
	// Set command.
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	set_spi_command(&spi_command, MY_RES, INST_1_WIRE, 0x00, ADDR_NONE, DATA_1_WIRE, ONE_BYTE, DUMMY_3);
	// Set XSPI ready for read
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);

	// Read
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle, read_buffer, ONE_MINUTE);
	read_buffer[1] = '\0';
	return octo_spi_return;
}

///////////////////////// Other functions /////////////////////////
HAL_StatusTypeDef write_enable(XSPI_HandleTypeDef *octo_spi_handle, XSPI_RegularCmdTypeDef *spi_command, uint8_t QPI_on){
	if(QPI_on){
		set_spi_command(spi_command, MY_WREN, INST_4_WIRES, ADDR_NONE, 0, DATA_NONE, 0, DUMMY_0);
	}else{
		set_spi_command(spi_command, MY_WREN, INST_1_WIRE, ADDR_NONE, 0, DATA_NONE, 0, DUMMY_0);
	}
	return HAL_XSPI_Command(octo_spi_handle, spi_command, ONE_MINUTE);
}

HAL_StatusTypeDef check_status_register(uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint8_t QPI_on){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	
	if(QPI_on){
		set_spi_command(&spi_command, MY_RDSR, INST_4_WIRES, 0x00, ADDR_NONE, DATA_4_WIRES, ONE_BYTE, DUMMY_0);
	}else{
		set_spi_command(&spi_command, MY_RDSR, INST_1_WIRE, 0x00, ADDR_NONE, DATA_1_WIRE, ONE_BYTE, DUMMY_0);
	}
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	return HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
}

HAL_StatusTypeDef check_config_register(uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint8_t QPI_on, UART_HandleTypeDef *huart){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;

	if(QPI_on){
		set_spi_command(&spi_command, MY_RDSR, INST_4_WIRES, 0x00, ADDR_NONE, DATA_4_WIRES, ONE_BYTE, DUMMY_0);
	}else{
		set_spi_command(&spi_command, MY_RDCR, INST_1_WIRE, 0x00, ADDR_NONE, DATA_1_WIRE, ONE_BYTE, DUMMY_0);
	}
	uart_print((uint8_t *)"config reg\r\n", huart);
	wait_for_button(huart);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	return HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
}

HAL_StatusTypeDef write_enable_with_check(XSPI_HandleTypeDef *octo_spi_handle, uint8_t QPI_on){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	uint8_t status_reg_buffer;
	do{
		octo_spi_return = write_enable(octo_spi_handle, &spi_command, QPI_on);
		check_spi_return(octo_spi_return);
		
		//HAL_DELAY_2500;
		octo_spi_return = check_status_register(&status_reg_buffer, octo_spi_handle, QPI_on);
		check_spi_return(octo_spi_return);
		
		//HAL_DELAY_2500;
	}while(!(status_reg_buffer & 0b10));
	return octo_spi_return;
}

HAL_StatusTypeDef write_in_progress(XSPI_HandleTypeDef *octo_spi_handle, uint8_t QPI_on){
	HAL_StatusTypeDef octo_spi_return;
	uint8_t status_reg_buffer;
	do{
		octo_spi_return = check_status_register(&status_reg_buffer, octo_spi_handle, QPI_on);
		check_spi_return(octo_spi_return);
	}while((status_reg_buffer & 0b1));
	return octo_spi_return;
}

HAL_StatusTypeDef set_QPI(XSPI_HandleTypeDef *octo_spi_handle){
	XSPI_RegularCmdTypeDef spi_command;

	set_spi_command(&spi_command, MY_EQIO, INST_1_WIRE, 0x00, ADDR_NONE, DATA_NONE, DATA_NONE, DUMMY_0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

	return HAL_OK;
}

HAL_StatusTypeDef reset_QPI(XSPI_HandleTypeDef *octo_spi_handle){
	XSPI_RegularCmdTypeDef spi_command;

	set_spi_command(&spi_command, MY_RSTQIO, INST_4_WIRES, 0x00, ADDR_NONE, DATA_NONE, DATA_NONE, 0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

	return HAL_OK;
}

HAL_StatusTypeDef set_QE(XSPI_HandleTypeDef *octo_spi_handle){
	XSPI_RegularCmdTypeDef spi_command;
	uint8_t status_reg_buffer;
	uint8_t quad_enable = 0b01000000;

	do{
		check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

		set_spi_command(&spi_command, MY_WRSR, INST_1_WIRE, 0x00, ADDR_NONE, DATA_1_WIRE, 1, DUMMY_0);
		check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));
		check_spi_return(HAL_XSPI_Transmit(octo_spi_handle, &quad_enable, ONE_MINUTE));

		check_spi_return(check_status_register(&status_reg_buffer, octo_spi_handle, QPI_OFF));
	}while(!(status_reg_buffer & 0b01000000));

	return HAL_OK;
}
HAL_StatusTypeDef set_temp(XSPI_HandleTypeDef *octo_spi_handle){
	XSPI_RegularCmdTypeDef spi_command;
	uint8_t status_reg_buffer;
	uint8_t quad_enable[2] = {0b00000000,0b00000001};

	do{
		check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));
		HAL_DELAY_2500;
		set_spi_command(&spi_command, MY_WRSR, INST_1_WIRE, 0x00, ADDR_NONE, DATA_1_WIRE, 2, DUMMY_0);
		check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));
		check_spi_return(HAL_XSPI_Transmit(octo_spi_handle, quad_enable, ONE_MINUTE));

		HAL_DELAY_2500;
		set_spi_command(&spi_command, MY_RDCR, INST_1_WIRE, 0x00, ADDR_NONE, DATA_1_WIRE, 1, DUMMY_0);

		check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));
		HAL_XSPI_Receive(octo_spi_handle,  &status_reg_buffer, ONE_MINUTE);
	}while(!(status_reg_buffer & 0b00000110));

	return HAL_OK;
}

HAL_StatusTypeDef check_QE(XSPI_HandleTypeDef *octo_spi_handle, UART_HandleTypeDef *huart){
	uint8_t status_reg_buffer;
	for(int i = 0; i < 3; i++){
		check_spi_return(check_status_register(&status_reg_buffer, octo_spi_handle, QPI_OFF));
		if(status_reg_buffer & 0b01000000){
			uart_print((uint8_t *) "QE on\r\n", huart);
		}else{
			uart_print((uint8_t *) "QE off\r\n", huart);
		}
	}
	return HAL_OK;
}

HAL_StatusTypeDef flush_buffer(XSPI_HandleTypeDef *octo_spi_handle){
	XSPI_RegularCmdTypeDef spi_command;
	uint8_t empty_buffer[256] = {0};
	set_spi_command(&spi_command, MY_PP, INST_1_WIRE, 0x00, ADDR_1_WIRE, DATA_1_WIRE, PAGE_SIZE - 1, 0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));
	return HAL_XSPI_Transmit(octo_spi_handle,  empty_buffer, ONE_MINUTE);
}

HAL_StatusTypeDef reset(XSPI_HandleTypeDef *octo_spi_handle){
	XSPI_RegularCmdTypeDef spi_command;
	set_spi_command(&spi_command, MY_RSTEN, INST_1_WIRE, ADDR_NONE, ADDR_NONE, DATA_NONE, DATA_NONE, DUMMY_0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));
	set_spi_command(&spi_command, MY_RST, INST_1_WIRE, ADDR_NONE, ADDR_NONE, DATA_NONE, DATA_NONE, DUMMY_0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));
	set_spi_command(&spi_command, MY_RSTEN, INST_4_WIRES, ADDR_NONE, ADDR_NONE, DATA_NONE, DATA_NONE, DUMMY_0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));
	set_spi_command(&spi_command, MY_RST, INST_4_WIRES, ADDR_NONE, ADDR_NONE, DATA_NONE, DATA_NONE, DUMMY_0);
	return HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
}
///////////////////////// Single SPI /////////////////////////
// Read page
HAL_StatusTypeDef read_page(uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	//Set spi command
	set_spi_command(&spi_command, MY_READ, INST_1_WIRE, address, ADDR_1_WIRE, DATA_1_WIRE, 24, DUMMY_0);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
	read_buffer[24] = '\0';
	return octo_spi_return;
}

// Write page
HAL_StatusTypeDef write_page(uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	uint8_t transmit_size = PAGE_SIZE - 1;
	uint8_t status_reg_buffer;
	do{
		octo_spi_return = write_enable(octo_spi_handle, &spi_command, 0);
		check_spi_return(octo_spi_return);

		HAL_DELAY_2500;
		octo_spi_return = check_status_register(&status_reg_buffer, octo_spi_handle, 0);
		check_spi_return(octo_spi_return);
		HAL_DELAY_2500;
	}while(!(status_reg_buffer & 0b10));
	
	HAL_DELAY_2500;
	
	set_spi_command(&spi_command, MY_PP, INST_1_WIRE, address, ADDR_1_WIRE, DATA_1_WIRE, transmit_size, DUMMY_0);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	
	octo_spi_return = HAL_XSPI_Transmit(octo_spi_handle,  write_buffer, ONE_MINUTE);
	//memset(octo_spi_handle->pBuffPtr, 0, transmit_size);
	do{
		HAL_DELAY_2500;
		octo_spi_return = check_status_register(&status_reg_buffer, octo_spi_handle, 0);
		check_spi_return(octo_spi_return);
	}while((status_reg_buffer & 0b1));
	
	return octo_spi_return;
}


// Erase page
HAL_StatusTypeDef erase_sector(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	
	uint8_t status_reg_buffer;
	do{
		octo_spi_return = write_enable(octo_spi_handle, &spi_command, 0);
		check_spi_return(octo_spi_return);
		HAL_DELAY_2500;
		octo_spi_return = check_status_register(&status_reg_buffer, octo_spi_handle, 0);
		check_spi_return(octo_spi_return);
		HAL_DELAY_2500;
	}while(!(status_reg_buffer & 0b10));
	
	HAL_DELAY_2500;
	set_spi_command(&spi_command, MY_SE, INST_1_WIRE, address, ADDR_1_WIRE, DATA_NONE, DATA_NONE, DUMMY_0);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	do{
		HAL_DELAY_2500;
		octo_spi_return = check_status_register(&status_reg_buffer, octo_spi_handle, 0);
		check_spi_return(octo_spi_return);
	}while((status_reg_buffer & 0b1));
	return octo_spi_return;
}



// Simple read tests
HAL_StatusTypeDef test_read_n_bytes_single(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;


    set_spi_command(&spi_command, MY_READ, INST_1_WIRE, address, ADDR_1_WIRE, DATA_1_WIRE, n, DUMMY_0);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	
	uart_print((uint8_t *)"READ\r\n", huart);
	wait_for_button(huart);
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
	read_buffer[n] = 0;
	return octo_spi_return;
}

HAL_StatusTypeDef test_read_n_bytes_dual_1(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){ // Address on one line
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;

    set_spi_command(&spi_command, MY_DREAD, INST_1_WIRE, address, ADDR_1_WIRE, DATA_2_WIRES, n, 8);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	
	uart_print((uint8_t *)"DREAD\r\n", huart);
	wait_for_button(huart);
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
	read_buffer[n] = 0;
	return octo_spi_return;
}

HAL_StatusTypeDef test_read_n_bytes_dual_2(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){ // Address on two lines
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;

    set_spi_command(&spi_command, MY_2READ, INST_1_WIRE, address, ADDR_2_WIRES, DATA_2_WIRES, n, 0);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	
	uart_print((uint8_t *)"2READ\r\n", huart);
	wait_for_button(huart);
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle, read_buffer, ONE_MINUTE);
	read_buffer[n] = 0;
	return octo_spi_return;
}

HAL_StatusTypeDef test_read_n_bytes_quad_1(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){ // SPI mode
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;

    set_spi_command(&spi_command, MY_4READ, INST_1_WIRE, address, ADDR_4_WIRES, DATA_4_WIRES, n, 6);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	
	uart_print((uint8_t *)"4READ - SPI\r\n", huart);
	wait_for_button(huart);
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
	read_buffer[n] = 0;
	return octo_spi_return;
}

HAL_StatusTypeDef test_read_n_bytes_quad_2(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){ // QPI mode
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;

    octo_spi_return = set_QPI(octo_spi_handle);
	check_spi_return(octo_spi_return);
	
    set_spi_command(&spi_command, MY_4READ, INST_4_WIRES, address, ADDR_4_WIRES, DATA_4_WIRES, n, 6);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	
	uart_print((uint8_t *)"4READ - QPI\r\n", huart);
	wait_for_button(huart);
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	read_buffer[n] = 0;
	

	return reset_QPI(octo_spi_handle);
}

HAL_StatusTypeDef test_read_n_bytes_quad_3(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){ // Inst on four lines
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;



	set_spi_command(&spi_command, MY_QREAD, INST_1_WIRE, address, ADDR_1_WIRE, DATA_4_WIRES, n, 8);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	
	uart_print((uint8_t *)"QREAD\r\n", huart);
	wait_for_button(huart);
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
	read_buffer[n] = 0;
	return octo_spi_return;
}

HAL_StatusTypeDef test_read_n_bytes_quad_4(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){
	HAL_StatusTypeDef octo_spi_return;
	XSPI_RegularCmdTypeDef spi_command;
	


	set_spi_command(&spi_command, MY_W4READ, INST_1_WIRE, address, ADDR_4_WIRES, DATA_4_WIRES, n, 4);
	octo_spi_return = HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE);
	check_spi_return(octo_spi_return);
	
	uart_print((uint8_t *)"W4READ\r\n", huart);
	wait_for_button(huart);
	octo_spi_return = HAL_XSPI_Receive(octo_spi_handle,  read_buffer, ONE_MINUTE);
	read_buffer[n] = 0;

	return octo_spi_return;
}

//////// WRITING ////////
HAL_StatusTypeDef test_write_n_bytes_single(uint32_t n, uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){
    XSPI_RegularCmdTypeDef spi_command;

    uart_print((uint8_t *)"WREN - SPI\r\n", huart);
    check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

    set_spi_command(&spi_command, MY_PP, INST_1_WIRE, address, ADDR_1_WIRE, DATA_1_WIRE, n, DUMMY_0);
    check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

    uart_print((uint8_t *)"Page Program - SPI\r\n", huart);
    wait_for_button(huart);
    check_spi_return(HAL_XSPI_Transmit(octo_spi_handle,  write_buffer, ONE_MINUTE));

    check_spi_return(write_in_progress(octo_spi_handle, QPI_OFF));
    return HAL_OK;
}

HAL_StatusTypeDef test_write_n_bytes_quad_1(uint32_t n, uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){ // 1 Wire.
	XSPI_RegularCmdTypeDef spi_command;

	uart_print((uint8_t *)"WREN - SPI\r\n", huart);
	check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

	set_spi_command(&spi_command, MY_4PP, INST_1_WIRE, address, ADDR_4_WIRES, DATA_4_WIRES, n, DUMMY_0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

	uart_print((uint8_t *)"4 Page Program - SPI\r\n", huart);
	wait_for_button(huart);
	check_spi_return(HAL_XSPI_Transmit(octo_spi_handle,  write_buffer, ONE_MINUTE));

	check_spi_return(write_in_progress(octo_spi_handle, QPI_OFF));
	return HAL_OK;
}


HAL_StatusTypeDef test_sector_erase(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){
	XSPI_RegularCmdTypeDef spi_command;

	check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

	uart_print((uint8_t *)"SE - SPI\r\n", huart);
		wait_for_button(huart);
	set_spi_command(&spi_command, MY_SE, INST_1_WIRE, address, ADDR_1_WIRE, DATA_NONE, 0, DUMMY_0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

	check_spi_return(write_in_progress(octo_spi_handle, QPI_OFF));
	uart_print((uint8_t *)"SE DONE\r\n", huart);
	return HAL_OK;
}

HAL_StatusTypeDef test_block_erase_32K(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){
	XSPI_RegularCmdTypeDef spi_command;

	uart_print((uint8_t *)"BE 32K - WREN - SPI\r\n", huart);
	wait_for_button(huart);
	check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

	set_spi_command(&spi_command, MY_BE_32K, INST_1_WIRE, address, ADDR_1_WIRE, DATA_NONE, 0, DUMMY_0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

	check_spi_return(write_in_progress(octo_spi_handle, QPI_OFF));
	uart_print((uint8_t *)"BE 32K DONE\r\n", huart);
	return HAL_OK;
}

HAL_StatusTypeDef test_block_erase_64K(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart){
	XSPI_RegularCmdTypeDef spi_command;

	uart_print((uint8_t *)"BE 64K - WREN - SPI\r\n", huart);
	wait_for_button(huart);
	check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

	set_spi_command(&spi_command, MY_BE_64K, INST_1_WIRE, address, ADDR_1_WIRE, DATA_NONE, 0, DUMMY_0);
	check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

	check_spi_return(write_in_progress(octo_spi_handle, QPI_OFF));
	uart_print((uint8_t *)"BE 64K DONE\r\n", huart);
	return HAL_OK;
}

HAL_StatusTypeDef test_chip_erase(XSPI_HandleTypeDef *octo_spi_handle, UART_HandleTypeDef *huart){
    XSPI_RegularCmdTypeDef spi_command;

    uart_print((uint8_t *)"CHIP ERASE - WREN - SPI\r\n", huart);
    wait_for_button(huart);
    check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

    set_spi_command(&spi_command, MY_CE, INST_1_WIRE, 0x00, ADDR_NONE, DATA_NONE, 0, DUMMY_0);
    check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

    check_spi_return(write_in_progress(octo_spi_handle, QPI_OFF));
    uart_print((uint8_t *)"CHIP ERASED DONE\r\n", huart);
    return HAL_OK;
}

//

HAL_StatusTypeDef write_256_bytes_single(uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address){
    XSPI_RegularCmdTypeDef spi_command;

    check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

    set_spi_command(&spi_command, MY_PP, INST_1_WIRE, address, ADDR_1_WIRE, DATA_1_WIRE, 256, DUMMY_0);
    check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

    check_spi_return(HAL_XSPI_Transmit(octo_spi_handle,  write_buffer, ONE_MINUTE));

    check_spi_return(write_in_progress(octo_spi_handle, QPI_OFF));
    return HAL_OK;
}

HAL_StatusTypeDef chip_erase(XSPI_HandleTypeDef *octo_spi_handle){
    XSPI_RegularCmdTypeDef spi_command;

    check_spi_return(write_enable_with_check(octo_spi_handle, QPI_OFF));

    set_spi_command(&spi_command, MY_CE, INST_1_WIRE, 0x00, ADDR_NONE, DATA_NONE, 0, DUMMY_0);
    check_spi_return(HAL_XSPI_Command(octo_spi_handle, &spi_command, ONE_MINUTE));

    check_spi_return(write_in_progress(octo_spi_handle, QPI_OFF));
    return HAL_OK;
}


