/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 Atilze Sensor Hub
 This is the main file for Atilze Sensor Hub.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : main.h

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 BK 02-Nov-17 Reorganise the entire structure
 */

#ifndef MAIN_H_
#define MAIN_H_

// Applications
#define SMART_AGRI0          0
#define SMART_AQUA0          1
#define SMART_CITY0          2
#define SMART_CITY1          3
#define E_LADANG0          	 4

// Applications
#define SENSOR_TYPE         E_LADANG0   // Applications
#define SLEEP_TIME          70            // Deep sleep time in seconds
#define XBEE_MODE            1            // 0: LoRa 	1:3G

// State Diagram
#define STATE_INIT           0
#define STATE_TX             1
#define STATE_RX             2
#define STATE_SLEEP          3
#define STATE_ENG            4

// POST/GET RESTAPI
#define POST_URL             "/api/temp/device/data/inbound"   // PLS CLEAN THE PROJECT
#define POST_HOST            "staging.atilze.com"              // PLS CLEAN THE PROJECT
#define POST_URL_UBIDOTS	 "/api/v1.6/devices/datax/variablex/values?token=BBFF-20ea4cf25b88c8df62c76b9fb6522a1ec25"
#define POST_HOST_UBIDOTS	 "industrial.api.ubidots.com"

// Debug
#define DEBUG_MODE           1

#endif /* MAIN_H_ */
