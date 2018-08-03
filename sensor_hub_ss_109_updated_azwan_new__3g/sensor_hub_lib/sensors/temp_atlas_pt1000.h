/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 temperature_pt1000.h
 temperature sensor pt1000.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : temperature_pt1000.h

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#ifndef TEMPERATURE_PT1000_H_
#define TEMPERATURE_PT1000_H_

void temp_atlas_pt1000_init();

void temp_atlas_pt1000_deinit();

int get_temp_atlas_pt1000();

#endif /* TEMPERATURE_PT1000_H_ */
