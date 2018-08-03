/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 floatswitch_lv600.c
 This is the library file for floatswitch_lv600.c.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name :  ferti_pump_status.c

 History:
   1.00 HW 31-Dec-17 Initial

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
#define pin_pumpA_status  14			// D7_GPIO_14
#define pin_pumpB_status  13			// D8_GPIO_13

//Init User Buttons
qm_gpio_state_t tank_A_status;
qm_gpio_state_t tank_B_status;

uint8_t get_fertipumpA_status()
{
	int fertipumpA_status;

	//Read user buttons status that are pulling high
	qm_gpio_read_pin(QM_GPIO_0, pin_pumpA_status, &tank_A_status);

	// Check button0 status
	if (tank_A_status == QM_GPIO_LOW)
	{
		fertipumpA_status = 0x00;
	}

	else
	{
		fertipumpA_status = 0x01;
	}

	//delay 1 second
	clk_sys_udelay(100000);

	return fertipumpA_status;
}


uint8_t get_fertipumpB_status()
{
	int fertipumpB_status;

	//Read user buttons status that are pulling high
	qm_gpio_read_pin(QM_GPIO_0, pin_pumpB_status, &tank_B_status);

	// Check button0 status
	if (tank_B_status == QM_GPIO_LOW)
	{
		fertipumpB_status = 0x00;
	}

	else
	{
		fertipumpB_status = 0x01;
	}

	//delay 1 second
	clk_sys_udelay(100000);

	return fertipumpB_status;
}
