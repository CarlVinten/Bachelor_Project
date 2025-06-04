/*
 * my_spi.h
 *
 *  Created on: Mar 14, 2025
 *      Author: carlv
 */

#ifndef INC_MY_SPI_H_
#define INC_MY_SPI_H_



#include "main.h"
#include "my_hal.h"


//Functions
HAL_StatusTypeDef read_device_id(uint8_t *read_buffer,XSPI_HandleTypeDef *octo_spi_handle);
HAL_StatusTypeDef read_electronic_signature(uint8_t *read_buffer,XSPI_HandleTypeDef *octo_spi_handle);
HAL_StatusTypeDef check_status_register(uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint8_t QPI_on);

HAL_StatusTypeDef read_page(uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);
HAL_StatusTypeDef write_page(uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);

HAL_StatusTypeDef dual_read(uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);

HAL_StatusTypeDef quad_read(uint8_t * read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);
HAL_StatusTypeDef quad_write(uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);


HAL_StatusTypeDef erase_sector(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);
HAL_StatusTypeDef chip_erase(XSPI_HandleTypeDef *octo_spi_handle);
HAL_StatusTypeDef check_config_register(uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint8_t QPI_on, UART_HandleTypeDef *huart);

HAL_StatusTypeDef flush_buffer(XSPI_HandleTypeDef *octo_spi_handle);
HAL_StatusTypeDef reset(XSPI_HandleTypeDef *octo_spi_handle);
HAL_StatusTypeDef set_QE(XSPI_HandleTypeDef *octo_spi_handle);
HAL_StatusTypeDef check_QE(XSPI_HandleTypeDef *octo_spi_handle, UART_HandleTypeDef *huart);
HAL_StatusTypeDef set_temp(XSPI_HandleTypeDef *octo_spi_handle);
// Read in different modes - Five repetitions.
HAL_StatusTypeDef test_read_n_bytes_single(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart);
HAL_StatusTypeDef test_read_n_bytes_dual_1(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart); // Address on one line
HAL_StatusTypeDef test_read_n_bytes_dual_2(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart); // Address on two lines
HAL_StatusTypeDef test_read_n_bytes_quad_1(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart); // Inst on one line
HAL_StatusTypeDef test_read_n_bytes_quad_2(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart); // Inst on four lines
HAL_StatusTypeDef test_read_n_bytes_quad_3(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart); // Inst on four lines
HAL_StatusTypeDef test_read_n_bytes_quad_4(uint32_t n, uint8_t *read_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart); // Inst on four lines

// Write in different modes - five repetitions.
HAL_StatusTypeDef test_write_n_bytes_single(uint32_t n, uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart);
HAL_StatusTypeDef test_write_n_bytes_quad_1(uint32_t n, uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart); // 1 Wire.
HAL_StatusTypeDef test_write_n_bytes_quad_2(uint32_t n, uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart); // 4 Wires.

// Erase tests
HAL_StatusTypeDef test_sector_erase(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart);
HAL_StatusTypeDef test_block_erase_32K(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart);
HAL_StatusTypeDef test_block_erase_64K(XSPI_HandleTypeDef *octo_spi_handle, uint32_t address, UART_HandleTypeDef *huart);
HAL_StatusTypeDef test_chip_erase(XSPI_HandleTypeDef *octo_spi_handle, UART_HandleTypeDef *huart);

HAL_StatusTypeDef write_256_bytes_single(uint8_t *write_buffer, XSPI_HandleTypeDef *octo_spi_handle, uint32_t address);
// 256 byte write - performance mode??


// Write many times.









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
#define DUMMY_6 6

#define QPI_ON  1
#define QPI_OFF 0

// Time
#define ONE_MINUTE 60000








#endif /* INC_MY_SPI_H_ */
