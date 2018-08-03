/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 cellular_3g.c
 This is the library file for cellular 3G.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : cellular_3g.h

 History:
   1.00 BK 27-Nov-17 Initial
 */

#ifndef SENSOR_HUB_LIB_UART_H_
#define SENSOR_HUB_LIB_UART_H_

void uart0_init(qm_uart_config_t *cfg);

uint8_t* uart0_get_token(const qm_uart_t uart, uint8_t token[]);

void uart0_send(const qm_uart_t uart, uint16_t device_id, uint8_t frame_cntr, uint8_t data_ch, uint16_t data_type, uint8_t data[], uint8_t data_len);

void uart0_connect(const qm_uart_t uart);


#endif /* SENSOR_HUB_LIB_WIRELESS_CELLULAR_3G_H_ */
