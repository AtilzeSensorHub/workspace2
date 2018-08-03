/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 floatswitch_lv600.c
 This is the library file for floatswitch_lv600.c.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name :  floatswitch_lv600.c

 History:
   1.00 HW 17-Dec-17 Initial

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

//User button define
#define pin_tankA_status  16			// GPIO_AON_3
#define pin_tankB_status  17			// GPIO_AON_5 // for rev2 board

//Init User Buttons
qm_gpio_state_t tank_A_status;
qm_gpio_state_t tank_B_status;

uint8_t get_tank_A_level()
{
	int tank_A_level;

	//Read  status that are pulling high
	qm_gpio_read_pin(QM_GPIO_0, pin_tankA_status, &tank_A_status);

	// Check status
	if (tank_A_status == QM_GPIO_LOW)
	{
		tank_A_level = 0x00;
	}

	else
	{
		tank_A_level = 0x01;
	}

	//delay 1 second
	clk_sys_udelay(100000);

	return tank_A_level;
}


uint8_t get_tank_B_level()
{
	int tank_B_level;

	//Read status that are pulling high
	qm_gpio_read_pin(QM_GPIO_0, pin_tankB_status, &tank_B_status);

	// Check status
	if (tank_B_status == QM_GPIO_LOW)
	{
		tank_B_level = 0x00;
	}

	else
	{
		tank_B_level = 0x01;
	}

	//delay 1 second
	clk_sys_udelay(100000);

	return tank_B_level;
}
