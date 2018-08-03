/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 lora.c
 This is the library file for LoRa.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : lora.c

 History:
   1.00 BK 18-Aug-17 Initial
 */

#include "qm_common.h"
#include "system/utilities.h"
#include "qm_uart.h"
#include "../main.h"
#include "qm_common.h"
//#include "qm_uart.h"
#include "qm_pinmux.h"
#include "clk.h"
//#include "wireless/lora.h"

// Initialise the LoRa module
void lora_init(qm_uart_config_t *cfg)
{
	qm_pmux_select(QM_PIN_ID_19, QM_PMUX_FN_0);  // UART0 TXD
	qm_pmux_select(QM_PIN_ID_18, QM_PMUX_FN_0);  // UART0 RXD
	qm_pmux_select(QM_PIN_ID_41, QM_PMUX_FN_1);  // UART0 RTS
	qm_pmux_select(QM_PIN_ID_40, QM_PMUX_FN_1);  // UART0 CTS
	qm_pmux_input_en(QM_PIN_ID_18, true);
	qm_pmux_input_en(QM_PIN_ID_41, true);

    // Configures UART_0 for 9600 bps
    cfg->baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 208, 5);
    cfg->line_control = QM_UART_LC_8N1;
    cfg->hw_fc = false;

    qm_uart_set_config(QM_UART_0, cfg);

    // enable clock for UART_0
    clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTA_REGISTER);
}

// Send data thru LoRa
void lora_send(const qm_uart_t uart, uint16_t device_id, uint8_t frame_cntr, uint8_t data_ch, uint16_t data_type, uint8_t data[], uint8_t data_len)
{
	//QM_PRINTF("Send AT-command\n");

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//      Conv. to respective HEX size       //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Device ID (2 bytes)
	uint8_t tx_device_id[5];
	conv_to_2B_hex(device_id, tx_device_id);

    // Frame Counter (1 byte)
	uint8_t tx_frame_cntr[3];
	conv_to_1B_hex(frame_cntr, tx_frame_cntr);

	// Data Channel (1 byte)
	uint8_t tx_data_ch[3];
	conv_to_1B_hex(data_ch, tx_data_ch);

	// Data Type (1 byte)
	uint8_t tx_data_type[3];
	conv_to_1B_hex(data_type-3200, tx_data_type);

	// Data (N bytes payload)
	uint8_t tx_data[data_len];
	for(uint8_t ii=0;ii<data_len;ii++) tx_data[ii] = data[ii];

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//               Tx. packet                //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Length of Tx Packet
	uint8_t tx_packet_len = data_len + 14;

	uint8_t tx_packet[tx_packet_len];

	// Arrange Tx packet
	tx_packet[0]  = tx_device_id[0];
	tx_packet[1]  = tx_device_id[1];
	tx_packet[2]  = tx_device_id[2];
	tx_packet[3]  = tx_device_id[3];
	tx_packet[4]  = '0';                // TODO: PLS CORRECT IT
	tx_packet[5]  = '0';                // TODO: PLS CORRECT IT
	tx_packet[6]  = '0';                // TODO: PLS CORRECT IT
	tx_packet[7]  = '0';                // TODO: PLS CORRECT IT
	tx_packet[8]  = tx_frame_cntr[0];
	tx_packet[9]  = tx_frame_cntr[1];
	tx_packet[10] = tx_data_ch[0];
	tx_packet[11] = tx_data_ch[1];
	tx_packet[12] = tx_data_type[0];
	tx_packet[13] = tx_data_type[1];
	for (uint8_t ii = 0; ii < tx_packet_len; ii++)
	{
		tx_packet[14+ii] = tx_data[ii];
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//               AT commands               //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t at_command[tx_packet_len+12];
	uint8_t tx_packet_len_c[2];

	// Convert the packet length to ASCII
	if (tx_packet_len<10)
	{
		tx_packet_len_c[0] = tx_packet_len + '0';
		tx_packet_len_c[1] = '0';
	}
	else if (tx_packet_len >= 10 && tx_packet_len < 20)
	{
		tx_packet_len_c[0] = (tx_packet_len - 10) + '0';
		tx_packet_len_c[1] = '1';
	}
	else
	{
		tx_packet_len_c[0] = (tx_packet_len - 20) + '0';
		tx_packet_len_c[1] = '2';
	}

    // Arrange AT command
	at_command[0] = 'A';
	at_command[1] = 'T';
	at_command[2] = '+';
	at_command[3] = 'D';
	at_command[4] = 'T';
	at_command[5] = 'X';
	at_command[6] = '=';
    at_command[7] = tx_packet_len_c[1];
    at_command[8] = tx_packet_len_c[0];
    at_command[9] = ',';
	for (uint8_t ii = 0; ii < tx_packet_len; ii++)
	{
		at_command[10+ii] = tx_packet[ii];
	}
    at_command[10+tx_packet_len  ] = '\r';
    at_command[10+tx_packet_len+1] = '\n';

	// Send the AT command via UART
	qm_uart_write_buffer(uart, at_command, (sizeof(at_command)));

}
