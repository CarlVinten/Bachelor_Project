/*
 * my_uart.c
 *
 *  Created on: Mar 14, 2025
 *      Author: carlv
 */


#include "my_hal.h"



void uart_print(uint8_t *buf, char *text, UART_HandleTypeDef * husart){
	strcpy((char*)buf, text);
	HAL_UART_Transmit(husart, buf, strlen((char*)buf), HAL_MAX_DELAY);
}


void get_HAL_error(HAL_StatusTypeDef return_value, UART_HandleTypeDef * husart){
#ifdef HUART_DEBUG
	uint8_t buf[16];
	 switch(return_value){
		  case HAL_OK:
			  uart_print(buf, "HAL_OK\r\n", husart);
			  break;
		  case HAL_ERROR:
			  uart_print(buf, "HAL ERROR\r\n", husart);
			  break;
		  case HAL_TIMEOUT:
			  uart_print(buf, "HAL TIMEOUT\r\n", husart);
			  break;
		  case HAL_BUSY:
			  uart_print(buf, "HAL BUSY\r\n", husart);
			  break;
		  }
#endif
#ifdef LED_DEBUG
	 	if(return_value != HAL_OK){
	 		while(1){
	 			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	 			HAL_Delay(500);
	 		}
	 	}
#endif
	 return;
}

void fill_page_buffer_1(char character, char *buffer){
	for(int i = 0; i < PAGE_SIZE; i++){
		buffer[i] = character;
	}
	buffer[256] = '\0';
	return;
}
void fill_page_buffer_2(char *buffer){
	for(int i = 0; i < PAGE_SIZE; i++){
		buffer[i] = i;
	}
	buffer[256] = '\0';
	return;
}
