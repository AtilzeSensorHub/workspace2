/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

ard_relayshield_v3.c
4 Channels Arduino Relay Shield V3 from Cytron

 Version   : 1.00
 Author    : Tan Han Wei

 History:
   1.00 HW 20-Oct-17 Initial

 */
#include "shields/relayshield_v3.h"
#include "qm_gpio.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "clk.h"


#define pin_relay1	14
#define pin_relay2	15
#define pin_relay3	16
#define pin_relay4	17


void relayshield_init(qm_gpio_port_config_t cfg1)
{
	qm_pmux_select(QM_PIN_ID_55, QM_PIN_55_FN_GPIO_21);  // Init RS232 enable

	qm_pmux_select(QM_PIN_ID_48, QM_PIN_48_FN_GPIO_14);  // Relay 1
	qm_pmux_select(QM_PIN_ID_49, QM_PIN_49_FN_GPIO_15);  // Relay 2 //I2S RXD
	qm_pmux_select(QM_PIN_ID_50, QM_PIN_50_FN_GPIO_16);  // Relay 3
	qm_pmux_select(QM_PIN_ID_51, QM_PIN_51_FN_GPIO_17);  // Relay 4

}

void on_relay1 ()					//Turn On Relay 1
{
	qm_gpio_set_pin(QM_GPIO_0, pin_relay1);		//Trigger GPIO 14
	QM_PUTS("on1");
	return;
}


void off_relay1 ()					//Turn Off Relay 1
{
	qm_gpio_clear_pin(QM_GPIO_0, pin_relay1);	//Clear GPIO 14
	QM_PUTS("off1");
	return;
}


void on_relay2 ()					//Turn On Relay 2
{
	qm_gpio_set_pin(QM_GPIO_0, pin_relay2);		// Trigger GPIO 15
	QM_PUTS("on2");
	return;
}


void off_relay2 ()					//Turn Off Relay 2
{
	qm_gpio_clear_pin(QM_GPIO_0, pin_relay2);	// Clear GPIO 15
	QM_PUTS("off2");
	return;
}


void on_relay3 ()					// Turn On Relay 3
{
	qm_gpio_set_pin(QM_GPIO_0, pin_relay3);		// Trigger GPIO 16
	QM_PUTS("on3");
	return;
}


void off_relay3 ()					// Turn Off Relay 3
{
	qm_gpio_clear_pin(QM_GPIO_0, pin_relay3);	// Clear GPIO 16
	QM_PUTS("off3");
	return;
}


void on_relay4 ()					// Turn On Relay 4
{
	qm_gpio_set_pin(QM_GPIO_0, pin_relay4);		// Trigger GPIO 17
	QM_PUTS("on4");
	return;
}


void off_relay4 ()					// Turn Off Relay 4
{
	qm_gpio_clear_pin(QM_GPIO_0, pin_relay4);	//	Clear GPIO 17
	QM_PUTS("off4");
	return;
}



