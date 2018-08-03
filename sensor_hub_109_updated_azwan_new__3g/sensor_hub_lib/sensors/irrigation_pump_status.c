/*
 Copyright (c) 2018, Atilze Digital Sdn. Bhd.

 irrigation_pump_status.c
 This is the library file for irrigation_pump_status.c.

 Version   : 1.00
 Author    : Shahrul Azwan
 File Name :  irrigation_pump_status.c

 History:
   1.00 AZ 23-May-18 Initial

 */

#include "system/gpio.h"
#include "qm_common.h"
#include "qm_gpio.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "clk.h"
#include "../main.h"

qm_gpio_state_t tank_B_status;

uint8_t get_irrigationpumpB_status()
{
	int irrigationpumpB_status;

	if (RELAY_PUMP_B==1)
	{
		qm_gpio_read_pin(QM_GPIO_0, 14, &tank_B_status);
	}
	else if (RELAY_PUMP_B==3)
	{
		qm_gpio_read_pin(QM_GPIO_0, 16, &tank_B_status);
	}

	else if (RELAY_PUMP_B==4)
	{
		qm_gpio_read_pin(QM_GPIO_0, 17, &tank_B_status);
	}


	// Check pumpB (irrigation pump) status
	if (tank_B_status == QM_GPIO_LOW)
	{
		irrigationpumpB_status = 0x00;
		QM_PUTS("low");
	}

	else
	{
		irrigationpumpB_status = 0x01;
		QM_PUTS("high");
	}

	//delay 1 second
	clk_sys_udelay(100000);

	return irrigationpumpB_status;
}
