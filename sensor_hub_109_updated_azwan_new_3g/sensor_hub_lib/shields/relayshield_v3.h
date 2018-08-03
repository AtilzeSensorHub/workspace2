/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

ard_relayshield_v3.h
4 Channels Arduino Relay Shield V3 from Cytron

 Version   : 1.00
 Author    : Tan Han Wei

 History:
   1.00 HW 20-Oct-17 Initial

 */

#ifndef ARD_RELAYSHIELD_V3_H_
#define ARD_RELAYSHIELD_V3_H_
#include "qm_gpio.h"

void relayshield_init(qm_gpio_port_config_t cfg1);

void on_relay1 ();
void off_relay1 ();

void on_relay2 ();
void off_relay2 ();

void on_relay3 ();
void off_relay3 ();

void on_relay4 ();
void off_relay4 ();

#endif /* ARD_RELAYSHIELD_V3_H_ */
