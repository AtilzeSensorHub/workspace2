/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 Atilze Sensor Hub
 This is the main file for Atilze Sensor Hub.

 Version   : 1.08
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : main.h

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 BK 02-Nov-17 Reorganise the entire structure
   1.02 BK 13-Nov-17 Rename the sensors
   1.03 HW 22-Nov-17 Added in battery monitoring "if statement". If below 3.2v, go to sleep.
   1.04 HW 16-Dec-17 To adapt Rev2 PCB. Changes on temp_rh_sht10.c. Added arduino enable pin 22
   1.05 HW 31-Dec-17 Added re-initialize gpio_init after sht10. Created e-ladang0,float switches,
   	   	   	   	   	 measure input solar voltage, reset charging timer, pump A&B status input.
   1.06 BK 09-Jan-18 Added 3G library.
   1.07 BK 02-Mar-18 Added authentication for 3G HTTP request/response.
   1.08 BK 08-May-18 Added 3G downlink and relay shield.
 */


#ifndef MAIN_H_
#define MAIN_H_

// Applications
#define SMART_AGRI0          0
#define SMART_AQUA0          1
#define SMART_CITY0          2
#define SMART_CITY1          3
#define E_LADANG0          	 4

// Sleep or Wait Mode
#define SLEEP_MODE           0
#define WAIT_MODE            1

// Applications
#define SENSOR_TYPE         E_LADANG0     // Applications
#define SLEEP_TIME          10            // Deep sleep time in seconds
#define XBEE_MODE            1            // 0: LoRa 	1:3G
#define RX_DURATION_TIME    60            // RX duration time in seconds
#define SLEEP_OR_WAIT        WAIT_MODE    // Sleep or wait mode
#define WAIT_TIME            30           // Wait time in seconds

// LoRa Class
#define CLASS_A              0         // LoRa Class A
#define CLASS_B              1         // LoRa Class B
#define CLASS_C				 2         // LoRa Class C
#define LORA_CLASS           CLASS_C   // LoRa Class

// State Diagram
#define STATE_INIT           0
#define STATE_TX             1
#define STATE_RX             2
#define STATE_SLEEP          3
#define STATE_WAIT           4
#define STATE_ENG            5

// POST/GET RESTAPI (PLS CLEAN THE PROJECT UPON CHANGES)
#define POST_URL             "/api/device/data/inbound"
#define POST_HOST            "staging.atilze.com"
#define GET_URL              "/api/downlinkStatus/FFFFFF37/"
#define GET_HOST             "staging.atilze.com"
//#define STATUS_URL           "/api/updateDownlinkStatus/FFFFFFF4/"
#define STATUS_URL           "/api/downlinkStatus/FFFFFF37/"
#define STATUS_HOST          "staging.atilze.com"
#define MAC_ADDR             "00000000FFFFFF37"
#define TOKEN_URL            "/oauth/token"
#define TOKEN_HOST           "staging.atilze.com"
#define TOKEN_CLIENT_ID      "19"
#define TOKEN_CLIENT_SECRET  "qB2dR48BWzBmFr4tVV76UoZBLBcgJPVjCHw0yYcW"
#define TOKEN_USER_NAME      "hanwei.tan@atilze.com"
#define TOKEN_USER_PWD       "@Packet-1"

// Debug
#define DEBUG_MODE           1

#define get_ticks() _rdtsc()

#endif /* MAIN_H_ */