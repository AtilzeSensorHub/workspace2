/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

battery_level.h
Detect battery level.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : battery_level.h

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#ifndef BATTERY_LEVEL_H_
#define BATTERY_LEVEL_H_

void batt_level_adc_init();

void batt_level_adc_deinit();

int get_battery_level ();

#endif /* BATTERY_LEVEL_H_ */
