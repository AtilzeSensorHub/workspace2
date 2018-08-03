/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

soi_ec.h
Soil EC sensor.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : soil_ec.h

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#ifndef SOIL_EC_H_
#define SOIL_EC_H_

#include <math.h>

void soil_ec_ae0001_init();

void soil_ec_ae0001_deinit();

int get_soil_ec_ae0001();

#endif /* SOIL_EC_H_ */
