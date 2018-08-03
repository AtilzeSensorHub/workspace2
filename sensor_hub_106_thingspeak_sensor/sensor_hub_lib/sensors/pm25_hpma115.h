/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 pm25_hm32322550.h
 This is the library file for PM25 sensor.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name :  pm25_hpma115.h

 History:
   1.00 HW 8-11-17 Initial
 */

#ifndef PM25_UART_H_
#define PM25_UART_H_

#include "qm_uart.h"

int get_pm25_hpma115();
void pm25_hpma115_init(qm_uart_config_t *cfg);
void init_pm25_control();

typedef enum {
	PM01_TYPE,
	PM25_TYPE,
	PM10_TYPE,
} pm25_sensor_type;


#endif /* PM25_UART_H_ */
