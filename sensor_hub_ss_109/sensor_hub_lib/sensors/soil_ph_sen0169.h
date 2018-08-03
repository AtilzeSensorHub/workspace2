/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

soi_ph.h
Soil pH sensor.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : soil_ph.h

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#ifndef SOIL_PH_H_
#define SOIL_PH_H_

void soil_ph_sen0169_init();

void soil_ph_sen0169_deinit();

int get_soil_ph_sen0169();

#endif /* SOIL_PH_H_ */
