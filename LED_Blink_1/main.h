/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 Atilze Sensor Hub
 This is the main file for Atilze Sensor Hub.

 Version   : 1.09
 Author    : Khoo Boon Kien / Tan Han Wei / Shahrul Azwan
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
   1.09 AZ 22-May-18 Add Pump A and Pump B declaration
 */


#ifndef MAIN_H_
#define MAIN_H_

#define UART	1	// 0: Using UART0	1: Using UART1
#define BPS		0	// 0: 9600bps		1: 115200bps

#endif /* MAIN_H_ */
