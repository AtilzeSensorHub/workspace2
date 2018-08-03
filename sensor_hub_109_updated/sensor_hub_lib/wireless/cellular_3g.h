/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 cellular_3g.c
 This is the library file for cellular 3G.

 Version   : 1.01
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : cellular_3g.h

 History:
   1.00 BK 27-Nov-17 Initial
   1.01 BK 08-May-18 Added 3G Tx, Rx, get token, update status
 */

#ifndef SENSOR_HUB_LIB_WIRELESS_CELLULAR_3G_H_
#define SENSOR_HUB_LIB_WIRELESS_CELLULAR_3G_H_

void cellular_3g_init(qm_uart_config_t *cfg);

uint8_t* cellular_3g_get_token(const qm_uart_t uart, uint8_t token[]);

void cellular_3g_send(const qm_uart_t uart, uint16_t device_id, uint8_t frame_cntr, uint8_t data_ch, uint16_t data_type, uint8_t data[], uint8_t data_len);

void cellular_3g_connect(const qm_uart_t uart);

uint8_t* cellular_3g_recv(const qm_uart_t uart, uint8_t payload_data[], uint8_t downlink_channel[], uint8_t token[1200]);

void cellular_3g_update_dl_status(const qm_uart_t uart, uint8_t downlink_channel[], uint8_t status[], uint8_t token[1200]);

#endif /* SENSOR_HUB_LIB_WIRELESS_CELLULAR_3G_H_ */
