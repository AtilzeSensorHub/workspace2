/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 radio.c
 This is the library file for radio.

 Version   : 1.03
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : radio.h

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 HW 23-Nov-17 Send battery level first
   1.02 HW 23-Nov-17 Added in debug message
   1.03 BK 08-May-18 Added downlink
 */

#ifndef SENSOR_HUB_LIB_SYSTEM_RADIO_H_
#define SENSOR_HUB_LIB_SYSTEM_RADIO_H_

void radio_send_data(uint16_t *send_data, uint8_t token[1200]);

uint8_t * radio_receive_data(uint8_t status[], uint8_t token[1200]);

#endif /* SENSOR_HUB_LIB_SYSTEM_RADIO_H_ */
