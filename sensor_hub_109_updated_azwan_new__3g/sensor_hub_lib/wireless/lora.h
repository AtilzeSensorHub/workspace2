/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 lora.h
 This is the library file for LoRa.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : lora.h

 History:
   1.00 BK 18-Aug-17 Initial
 */

#ifndef LORA_H_
#define LORA_H_

void lora_send(const qm_uart_t uart, uint16_t device_id, uint8_t frame_cntr, uint8_t data_ch, uint16_t data_type, uint8_t data[], uint8_t data_len);

void lora_init(qm_uart_config_t *cfg);

uint8_t * lora_class_a_receive(const qm_uart_t uart, uint8_t data[]);

uint8_t * lora_class_c_receive(const qm_uart_t uart, uint8_t data[]);

uint8_t * lora_receive(const qm_uart_t uart, uint8_t data[]);

#endif /* LORA_H_ */
