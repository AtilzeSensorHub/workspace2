/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 sensor.c
 This is the library file for sensor.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : sensor.c

 History:
   1.00 BK 18-Aug-17 Initial
 */

#include "sensors/pm25_hpma115.h"
#include "sensors/do_kit103d.h"
#include "sensors/ultrasonic_mb7060.h"
#include "../main.h"
#include "qm_pinmux.h"
#include "qm_gpio.h"
#include "qm_pin_functions.h"
#include "clk.h"
#include "qm_uart.h"
#include "system/xbee.h"
#include "qm_mailbox.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "ss_init.h"
#include "qm_isr.h"

// Initialise sensors
void sensors_init(qm_uart_config_t uart1_cfg)
{
	if (SENSOR_TYPE == 1) // Smart Aqua0
	{
		do_kit103d_init(&uart1_cfg);

		clk_sys_udelay(1000);
	}
	else if (SENSOR_TYPE == 2) // Smart City0
	{
		pm25_hpma115_init(&uart1_cfg);

		clk_sys_udelay(10000);

	}
	else if (SENSOR_TYPE == 3) // Smart City1
	{
		ultrasonic_mb7060_init(&uart1_cfg);

		clk_sys_udelay(10000);

	}
}
