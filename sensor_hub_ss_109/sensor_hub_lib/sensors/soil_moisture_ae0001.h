/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

soi_moisture.h
Soil moistures sensor.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : soil_moisture.h

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#ifndef SOIL_MOISTURE_H_
#define SOIL_MOISTURE_H_

void soil_moisture_ae0001_init();

void soil_moisture_ae0001_deinit();

int get_soil_moisture_ae0001();


#endif /* SOIL_MOISTURE_H_ */
