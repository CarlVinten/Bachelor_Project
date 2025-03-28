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


void uart_print(uint8_t *buf, char *text, UART_HandleTypeDef * husart);
void get_HAL_error(HAL_StatusTypeDef return_value, UART_HandleTypeDef * husart);

#endif /* INC_MY_HAL_H_ */
