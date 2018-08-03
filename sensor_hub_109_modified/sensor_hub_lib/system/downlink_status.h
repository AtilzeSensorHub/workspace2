/*
 Copyright (c) 2018, Atilze Digital Sdn. Bhd.

 radio.c
 This is the library file for downlink_status.

 Version   : 1.01
 Author    : Shahrul Azwan
 File Name : downlink_status.h

 History:
   1.00 AZ 21-May-18 Initial
   1.01 AZ 22-May-18 Separate Pump A and Pump B to two different class
 */

#ifndef SENSOR_HUB_LIB_SYSTEM_DOWNLINK_STATUS_H_
#define SENSOR_HUB_LIB_SYSTEM_DOWNLINK_STATUS_H_

uint8_t * get_downlink_status_pumpA(uint8_t status[]);
uint8_t * get_downlink_status_pumpB(uint8_t status[]);

#endif /* SENSOR_HUB_LIB_SYSTEM_DOWNLINK_STATUS_H_ */
