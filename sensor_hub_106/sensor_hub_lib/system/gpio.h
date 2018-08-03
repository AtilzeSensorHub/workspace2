/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 gpio.h
 This is the library file for GPIO.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : gpio.h

 History:
   1.00 BK 18-Aug-17 Initial
 */


#ifndef SENSOR_HUB_LIB_SYSTEM_GPIO_H_
#define SENSOR_HUB_LIB_SYSTEM_GPIO_H_

#include "qm_gpio.h"

void gpio_init(qm_gpio_port_config_t cfg, qm_gpio_port_config_t cfg0);

void led_b_flash();

void led_r_flash();

void led_b_on();

void led_r_on();

#endif /* SENSOR_HUB_LIB_SYSTEM_GPIO_H_ */
