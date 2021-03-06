/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 gpio.c
 This is the library file for GPIO.

 Version   : 1.01
 Author    : Khoo Boon Kien / Tan Han Wei / Shahrul Azwan
 File Name : gpio.c

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 AZ 18-May-18 Add cfg bit to enable relay function
 */

#include "sensors/pm25_hpma115.h"
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

// Initialise GPIO
void gpio_init(qm_gpio_port_config_t cfg, qm_gpio_port_config_t cfg0)
{
	// Init. sensor enable and 3.3V/5V pins
	//static qm_gpio_port_config_t cfg;

    qm_pmux_select(QM_PIN_ID_54, QM_PIN_54_FN_GPIO_20);  // XBee enable
	qm_pmux_select(QM_PIN_ID_58, QM_PIN_58_FN_GPIO_24);  // Init. ADC2 enable
	qm_pmux_select(QM_PIN_ID_59, QM_PIN_59_FN_GPIO_25);  // Init. ADC1 enable
	qm_pmux_select(QM_PIN_ID_60, QM_PIN_60_FN_GPIO_26);  // Init. ADC0 enable
	qm_pmux_select(QM_PIN_ID_61, QM_PIN_61_FN_GPIO_27);  // Init. 5V enable
	qm_pmux_select(QM_PIN_ID_62, QM_PIN_62_FN_GPIO_28);  // Init. 3.3V enable
	qm_pmux_select(QM_PIN_ID_33, QM_PIN_33_FN_GPIO_29);  // uart1 control

	qm_pmux_select(QM_PIN_ID_56, QM_PIN_56_FN_GPIO_22);  // arduino enable


	//cfg.direction =  BIT(20) |BIT(22) |  BIT(24) | BIT(25) | BIT(26) | BIT(27) | BIT(28) | BIT(29) ;  // Output

	cfg.direction =  BIT(14) | BIT(16) | BIT(17) | BIT(20) |BIT(22) |  BIT(24) | BIT(25) | BIT(26) | BIT(27) | BIT(28) | BIT(29) ;

	qm_gpio_set_config(QM_GPIO_0, &cfg); // Configure

	clk_sys_udelay(1000000);  // Delay 1 second

	qm_gpio_clear_pin(QM_GPIO_0, 29); // Select Arduino
	qm_gpio_set_pin(QM_GPIO_0,22);	//enable arduino

	// Init. LED0
	//static qm_gpio_port_config_t cfg0;

	cfg0.direction = BIT(0) | BIT(1) | BIT(2);  // Output

	qm_gpio_set_config(QM_AON_GPIO_0, &cfg0); // Configure
}

// Flash Blue LED
void led_b_flash()
{
	// Flash Blue LED
	qm_gpio_set_pin(QM_AON_GPIO_0, 0);
	clk_sys_udelay(1000000);
	qm_gpio_clear_pin(QM_AON_GPIO_0, 0);
}

// Flash Red LED
void led_r_flash()
{
	// Flash Red LED
	qm_gpio_set_pin(QM_AON_GPIO_0, 2);
    clk_sys_udelay(1000000);
    qm_gpio_clear_pin(QM_AON_GPIO_0, 2);
}

// Turn Blue LED On
void led_b_on()
{
	// Turn Blue LED On
	qm_gpio_set_pin(QM_AON_GPIO_0, 0);
}

// Turn Blue LED Off
void led_b_off()
{
	// Turn Blue LED Off
	qm_gpio_clear_pin(QM_AON_GPIO_0, 0);
}

// Turn Red LED On
void led_r_on()
{
	// Turn Red LED On
	qm_gpio_set_pin(QM_AON_GPIO_0, 2);
}

// Turn Red LED Off
void led_r_off()
{
	// Turn Red LED Off
	qm_gpio_clear_pin(QM_AON_GPIO_0, 2);
}
