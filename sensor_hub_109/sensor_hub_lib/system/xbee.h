/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 xbee.c
 This is the library file for xbee.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : xbee.h

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 BK 08-May-18 Added xbee_rx & xbee_update_dl_status
 */

#ifndef XBEE_H_
#define XBEE_H_

#include "qm_uart.h"

void xbee_init(qm_uart_config_t uart0_cfg);

void xbee_tx(const qm_uart_t uart, uint16_t device_id, uint8_t frame_cntr, uint8_t data_ch, uint16_t data_type, uint8_t data[], uint8_t data_len);

uint8_t * xbee_rx(const qm_uart_t uart, uint8_t downlink_data[], uint8_t downlink_channel[]);

void xbee_update_dl_status(const qm_uart_t uart, uint8_t downlink_channel[], uint8_t status[]);

#endif /* XBEE_H_ */
