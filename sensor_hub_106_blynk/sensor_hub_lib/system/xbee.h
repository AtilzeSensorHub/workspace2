/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 xbee.h
 This is the library file for xbee.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : xbee.h

 History:
   1.00 BK 18-Aug-17 Initial
 */

#ifndef XBEE_H_
#define XBEE_H_

#include "qm_uart.h"

void xbee_init(qm_uart_config_t uart0_cfg);

void xbee_tx(const qm_uart_t uart);

#endif /* XBEE_H_ */
