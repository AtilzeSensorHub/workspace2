/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 radio.c
 This is the library file for radio.

 Version   : 1.05
 Author    : Khoo Boon Kien / Tan Han Wei / Shahrul Azwan
 File Name : radio.c

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 HW 23-Nov-17 Send battery level first
   1.02 HW 23-Nov-17 Added in debug message
   1.03 BK 08-May-18 Added downlink
   1.04 AZ 22-May-18 Call Pump A and Pump B downlink function during Tx with if else condition
   1.05 AZ 23-May-18 Add function to check irrigation pump status (pumpB)
 */

#include "sensors/pm25_hpma115.h"
#include "sensors/do_kit103d.h"
#include "sensors/ultrasonic_mb7060.h"
#include "sensors/temp_rh_sht10.h"
#include "sensors/floatswitch_lv600.h"
#include "sensors/ferti_pump_status.h"
#include "../main.h"
#include <stdint.h>
#include "system/utilities.h"
#include "system/xbee.h"
#include "wireless/lora.h"
#include "clk.h"
#include "system/gpio.h"
#include "string.h"
#include "system/mailbox.h"

void radio_send_data(uint16_t *send_data)
{
	//if (SENSOR_TYPE == 0)

	uint16_t adc_data[6];
	get_ss_data(adc_data);

	uint16_t battery_level= (adc_data[0]*2);
	uint8_t kkk[4];

	for(int x=0;x<5;x++)
	{
		kkk[x]=battery_level[x];
	}

	QM_PRINTF("Battery level: %d\n", battery_level);


	QM_PRINTF("Battery level: %d\n", 2*adc_data[0]);
	QM_PRINTF("Battery level: %d\n", adc_data[1]);
	QM_PRINTF("Battery level: %d\n", adc_data[2]);
	QM_PRINTF("Battery level: %d\n", adc_data[3]);
	QM_PRINTF("Battery level: %d\n", adc_data[4]);
	QM_PRINTF("Battery level: %d\n", adc_data[5]);

	//xbee_tx(QM_UART_0, battery_level);

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PUTS("done");
	}

	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(2000000);


}
