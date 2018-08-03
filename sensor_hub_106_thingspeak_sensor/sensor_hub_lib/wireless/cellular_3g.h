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

#ifndef SENSOR_HUB_LIB_WIRELESS_CELLULAR_3G_H_
#define SENSOR_HUB_LIB_WIRELESS_CELLULAR_3G_H_

void cellular_3g_init(qm_uart_config_t *cfg);

void cellular_3g_send(const qm_uart_t uart, uint8_t battery_level[]);

void cellular_3g_connect(const qm_uart_t uart);

#endif /* SENSOR_HUB_LIB_WIRELESS_CELLULAR_3G_H_ */
