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

#define HUART_DEBUG 1
//#define LED_DEBUG   1

void uart_print(uint8_t *buf, char *text, UART_HandleTypeDef * husart);
void get_HAL_error(HAL_StatusTypeDef return_value, UART_HandleTypeDef * husart);
void fill_page_buffer_1(char character, char *buffer);
void fill_page_buffer_2(char *buffer);
void HAL_error_led(HAL_StatusTypeDef return_value);

#endif /* INC_MY_HAL_H_ */
