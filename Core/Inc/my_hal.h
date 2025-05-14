/*
 * my_uart.h
 *
 *  Created on: Mar 14, 2025
 *      Author: carlv
 */

#ifndef INC_MY_HAL_H_
#define INC_MY_HAL_H_
#include <string.h>
#include "stm32h5xx_hal.h"
#include "my_spi.h"

#define DELAY_ON 1
#if DELAY_ON
#define HAL_DELAY_2500 HAL_Delay(2500)
#else
#define HAL_DELAY_2500
#endif

#define check_spi_return(return_value) if(return_value!=HAL_OK){return return_value;}

void uart_print(uint8_t *text, UART_HandleTypeDef * husart);
void get_HAL_error(HAL_StatusTypeDef return_value, UART_HandleTypeDef * husart);
void fill_page_buffer_1(uint8_t character, uint8_t *buffer);
void fill_page_buffer_2(uint8_t *buffer);
void HAL_error_led(HAL_StatusTypeDef return_value);
void wait_for_button(UART_HandleTypeDef *huart);

#endif /* INC_MY_HAL_H_ */
