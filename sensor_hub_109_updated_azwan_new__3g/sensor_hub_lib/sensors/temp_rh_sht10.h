/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 Atilze Sensor Hub
 This is the main file for Atilze Sensor Hub.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name : sht11.h

 History:
   1.00 HW 16-Nov-17 Initial
 */

#ifndef SHT11_H_
#define SHT11_H_


int get_sht10_temp();
int get_sht10_rh_linear();
int get_sht10_rh_true();
void init_voltages_enable();
void init_uart();
#endif /* SHT11_H_ */
