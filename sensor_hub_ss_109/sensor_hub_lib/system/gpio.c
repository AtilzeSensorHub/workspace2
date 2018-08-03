/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 gpio.c
 GPIO library file.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : GPIO.c

 History:
   1.00 HW 18-Aug-17 Initial
 */

#include "qm_ss_gpio.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "ss_clk.h"

#define CHARGE_EN	3
#define LED_GREEN	4

void gpio_init()
{
	qm_ss_gpio_port_config_t cfg;

    // Initialise SS GPIO
	qm_pmux_select(QM_PIN_ID_63, QM_PIN_63_FN_GPIO_SS_10);
	qm_pmux_select(QM_PIN_ID_64, QM_PIN_64_FN_GPIO_SS_11);
	qm_pmux_select(QM_PIN_ID_65, QM_PIN_65_FN_GPIO_SS_12);
	qm_pmux_select(QM_PIN_ID_66, QM_PIN_66_FN_GPIO_SS_13);
	qm_pmux_select(QM_PIN_ID_67, QM_PIN_67_FN_GPIO_SS_14);
	qm_pmux_select(QM_PIN_ID_68, QM_PIN_68_FN_GPIO_SS_15);

	cfg.direction =  BIT(2) | BIT(3) | BIT(4) | BIT(5) | BIT(6) | BIT(7) ;  // Output

	ss_clk_gpio_enable(QM_SS_GPIO_1);
	qm_ss_gpio_set_config(QM_SS_GPIO_1, &cfg);

	//qm_ss_gpio_set_pin(QM_SS_GPIO_1, 2);  // Turn off LED2 B
	qm_ss_gpio_clear_pin(QM_SS_GPIO_1, CHARGE_EN);  // Turn off LED2 G
	qm_ss_gpio_clear_pin(QM_SS_GPIO_1, LED_GREEN);  // Turn off LED2 R
	//qm_ss_gpio_set_pin(QM_SS_GPIO_1, 5);  // Turn off LED1 B
	//qm_ss_gpio_set_pin(QM_SS_GPIO_1, 6);  // Turn off LED1 G
	//qm_ss_gpio_set_pin(QM_SS_GPIO_1, 7);  // Turn off LED1 R
}

void charge_en()
{
	qm_ss_gpio_set_pin(QM_SS_GPIO_1, CHARGE_EN);
	qm_ss_gpio_set_pin(QM_SS_GPIO_1, LED_GREEN);  // Turn off LED2 R

	clk_sys_udelay(2000000);
	qm_ss_gpio_clear_pin(QM_SS_GPIO_1, CHARGE_EN);
	qm_ss_gpio_clear_pin(QM_SS_GPIO_1, LED_GREEN);  // Turn off LED2 R


}
