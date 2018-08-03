/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 aqua_dissolved_oxygen.h
 This is the library file for DO sensor.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name : aqua_dissolved_oxygen.h

 History:
   1.00 BK 18-Aug-17 Initial
 */

#ifndef AQUA_DISSOLVED_OXYGEN_H_
#define AQUA_DISSOLVED_OXYGEN_H_

#include "qm_uart.h"

int get_do_kit103d();
void do_kit103d_init(qm_uart_config_t *cfg);


#endif /* AQUA_DISSOLVED_OXYGEN_H_ */
