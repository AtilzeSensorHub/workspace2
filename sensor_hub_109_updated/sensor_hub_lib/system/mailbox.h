/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 mailbox.h
 This is the library file for mailbox.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : mailbox.h

 History:
   1.00 BK 18-Aug-17 Initial
 */

#ifndef SENSOR_HUB_LIB_SYSTEM_MAILBOX_H_
#define SENSOR_HUB_LIB_SYSTEM_MAILBOX_H_

#include "qm_gpio.h"

int get_ss_data(uint16_t *adc_data);

void mailbox_init();

#endif /* SENSOR_HUB_LIB_SYSTEM_MAILBOX_H_ */
