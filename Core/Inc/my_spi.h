/*
 * my_spi.h
 *
 *  Created on: Mar 14, 2025
 *      Author: carlv
 */

#ifndef INC_MY_SPI_H_
#define INC_MY_SPI_H_



#include "main.h"


//Functions
HAL_StatusTypeDef read_device_id(char *read_buffer,XSPI_HandleTypeDef *octo_spi_handle);
HAL_StatusTypeDef read_electronic_signature(char *read_buffer,XSPI_HandleTypeDef *octo_spi_handle);

HAL_StatusTypeDef read_page(char *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);
HAL_StatusTypeDef write_page(char *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);

HAL_StatusTypeDef dual_read(char *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);
HAL_StatusTypeDef dual_write(char *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);

HAL_StatusTypeDef erase_sector(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);

//Commands
//Array access
#define MY_READ 			(0x03U) // Normal read op.
#define MY_FAST_READ 		(0x0B) // fast read op.
#define MY_2READ			(0xBB) // 2 I/O read.
#define MY_DREAD			(0x3B) // 1I 2O read.
#define MY_4READ			(0xEB) // 4 IO read
#define MY_QREAD			(0x6B) // 1I 4O read
#define MY_W4READ			(0xE7) // 4 IO read with 4 dummy cycles.
#define MY_PP				(0x02) // Page program/ Write a page
#define MY_4PP				(0x38) // 4 IO page write
#define MY_SE				(0x20) // Sector Erase
#define MY_BE_32K			(0x52) // Block erase 32 KB
#define MY_BE_64K			(0xD8) // Block erase 64 KB
#define MY_CE				(0x60) // Chip erase

//Device operation
#define MY_WREN 			(0x06) //(write enable) 06 (hex)
#define MY_WRDI 			(0x04) //(write disable) 04 (hex)
#define MY_WPSEL 			(0x68) //(Write Protect Selection) 68 (hex)
#define MY_EQIO 			(0x35) //(Enable QPI) 35 (hex)
#define MY_RSTQIO 			(0xF5) //(Reset QPI) F5 (hex)
#define MY_PGM_ERS_SUSPEND  (0x75) //(Suspends Program/ Erase) 75 or B0 (hex)
#define MY_PGM_ERS_RESUME 	(0x7A) //(Resumes Program/ Erase) 7A or 30 (hex)
#define MY_DP 				(0xB9) //(Deep power down) B9 (hex)
#define MY_NOP 				(0x00) //(No Operation) 00 (hex)
#define MY_RSTEN 			(0x66) //(Reset Enable) 66 (hex)
#define MY_RST 				(0x99) //(Reset Memory) 99 (hex)
#define MY_GBLK 			(0x7E) //(gang block lock) 7E (hex)
#define MY_GBULK  			(0x98) //(gang block unlock) 98 (hex)
#define MY_FMEN 			(0x41) //(factory mode enable) 41 (hex)

//Register access
#define MY_RDID				(0x9F) //(read identification) 9F (hex)
#define MY_RES				(0xAB) //(read electronic ID) AB (hex) 10101011 11010101
#define MY_REMS				(0x90) //(read electronic manufacturer & device ID) 90 (hex)
#define MY_QPIID			(0xAF) //(QPI ID Read) AF (hex)
#define MY_RDSFDP 			(0x5A) //(Read SFDP Table) 5A (hex)
#define MY_RDSR				(0x05) //(read status register) 05 (hex)
#define MY_RDCR 			(0x15) //(read configuration register) 15 (hex)
#define MY_RDFMSR			(0x44) //(Read Factory Mode Status Register) 44 (hex)
#define MY_WRSR				(0x01) //(write status/configuration register) 01 (hex)
#define MY_RDSCUR			(0x2B) //(read security register) 2B (hex)
#define MY_WRSCUR			(0x2F) //(write security register) 2F (hex)
#define MY_SBL				(0xC0) //(Set Burst Length) C0 (hex)
#define MY_ENSO				(0xB1) //(enter secured OTP) B1 (hex)
#define MY_EXSO 			(0xC1) //(exit secured OTP) C1 (hex)
#define MY_WRLR				(0x2C) //(write Lock register) 2C (hex)
#define MY_RDLR				(0x2D) //(read Lock register) 2D (hex)
#define MY_WRSPB			(0xE3) //(SPB bit program) E3 (hex)
#define MY_ESSPB			(0xE4) //(all SPB bit erase) E4 (hex)
#define MY_RDSPB			(0xE2) //(read SPB status) E2 (hex)
#define MY_WRDPB			(0xE1) //(write DPB register) E1 (hex)
#define MY_RDDPB			(0xE0) //(read DPB register) E0 (hex)

// Defines for shorter variables, so I can remember them.
// Number of instruction wires.
#define INST_NONE    HAL_XSPI_INSTRUCTION_NONE
#define INST_1_WIRE  HAL_XSPI_INSTRUCTION_1_LINE
#define INST_2_WIRES HAL_XSPI_INSTRUCTION_2_LINES
#define INST_4_WIRES HAL_XSPI_INSTRUCTION_4_LINES

//Instruction width
#define INST_LEN_1_BYTE  HAL_XSPI_INSTRUCTION_8_BITS
#define INST_LEN_2_BYTES HAL_XSPI_INSTRUCTION_16_BITS
// Number of address wires.
#define ADDR_NONE    HAL_XSPI_ADDRESS_NONE
#define ADDR_1_WIRE  HAL_XSPI_ADDRESS_1_LINE
#define ADDR_2_WIRES HAL_XSPI_ADDRESS_2_LINES
#define ADDR_4_WIRES HAL_XSPI_ADDRESS_4_LINES

// Number of data wires.
#define DATA_NONE    HAL_XSPI_DATA_NONE
#define DATA_1_WIRE  HAL_XSPI_DATA_1_LINE
#define DATA_2_WIRES HAL_XSPI_DATA_2_LINES
#define DATA_4_WIRES HAL_XSPI_DATA_4_LINES

// Bytes.
#define ONE_BYTE  		1
#define TWO_BYTES 		2
#define THREE_BYTES 	3
#define FOUR_BYTES  	4
#define PAGE_SIZE   	256
#define HALF_BLOCK_SIZE 32000
#define BLOCK_SIZE		64000

// Dummy Bytes.
#define DUMMY_0 0
#define DUMMY_1 8
#define DUMMY_2 16
#define DUMMY_3 24

// Time
#define ONE_MINUTE 60000








#endif /* INC_MY_SPI_H_ */
