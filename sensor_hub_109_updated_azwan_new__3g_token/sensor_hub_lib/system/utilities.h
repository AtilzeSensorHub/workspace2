/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 utilities.h
 This is the library file for utilities.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : utilities.h

 History:
   1.00 BK 18-Aug-17 Initial
 */

#ifndef SENSOR_HUB_INCLUDE_UTILITIES_H_
#define SENSOR_HUB_INCLUDE_UTILITIES_H_


uint8_t* conv_to_2B_hex(uint16_t data_int, uint8_t data_hex[]);

uint8_t* conv_to_1B_hex(uint8_t data_int, uint8_t data_hex[]);

//uint8_t* concat(uint8_t data_left[], uint8_t data_right[], uint8_t data_new[]);
uint8_t* concat(uint8_t data_left[], uint8_t data_right[]);

//uint8_t* resize_array(uint8_t resized_data[], uint8_t data[]);
uint8_t* int2char(int data_int, uint8_t data_char[]);

#endif /* SENSOR_HUB_INCLUDE_UTILITIES_H_ */
