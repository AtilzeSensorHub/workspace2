/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 Atilze Sensor Hub
 This is the main file for Atilze Sensor Hub.

 Version   : 1.06
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : main.c

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
#define SENSOR_TYPE         SMART_CITY0   // Applications

// Debug
#define DEBUG_MODE           0   // 0: Debug 1: Normal

#endif /* MAIN_H_ */
