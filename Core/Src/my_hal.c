/*
 * my_uart.c
 *
 *  Created on: Mar 14, 2025
 *      Author: carlv
 */


#include "my_hal.h"



void uart_print(uint8_t *text, UART_HandleTypeDef * husart){
	uint32_t n = strlen((char *)text) > 31 ? 31 : strlen((char *)text);
	HAL_UART_Transmit(husart, text, n, HAL_MAX_DELAY);
	return;
}


void get_HAL_error(HAL_StatusTypeDef return_value, UART_HandleTypeDef * husart){
	 switch(return_value){
		  case HAL_OK:
			  uart_print((uint8_t *)"HAL_OK\r\n", husart);
			  break;
		  case HAL_ERROR:
			  uart_print((uint8_t *)"HAL ERROR\r\n", husart);
			  break;
		  case HAL_TIMEOUT:
			  uart_print((uint8_t *)"HAL TIMEOUT\r\n", husart);
			  break;
		  case HAL_BUSY:
			  uart_print((uint8_t *)"HAL BUSY\r\n", husart);
			  break;
		  }
	 return;
}

void fill_page_buffer_1(uint8_t character, uint8_t *buffer){
	for(int i = 0; i < PAGE_SIZE; i++){
		buffer[i] = character;
	}
	buffer[256] = '\0';
	return;
}
void fill_page_buffer_2(uint8_t *buffer){
	for(int i = 0; i < PAGE_SIZE; i++){
		buffer[i] = i;
	}
	buffer[256] = '\0';
	return;
}

void wait_for_button(UART_HandleTypeDef * huart){
    while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) != GPIO_PIN_SET){}
    while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET){}
	return;
}









