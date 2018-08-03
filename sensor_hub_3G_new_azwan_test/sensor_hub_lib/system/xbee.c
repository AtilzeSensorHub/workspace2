/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 xbee.c
 This is the library file for xbee.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : xbee.c

 History:
   1.00 BK 18-Aug-17 Initial
 */

#include "../main.h"
#include "qm_common.h"
#include "qm_uart.h"
#include "qm_pinmux.h"
#include "clk.h"
#include "string.h"
#include "wireless/lora.h"
#include "wireless/cellular_3g.h"
#include "wireless/wifi.h"
#include "wireless/uart.h"


// Initialise XBee
void xbee_init(qm_uart_config_t uart0_cfg)
{
	// Initialise XBee Wireless Module (Cellular 3G)
	//cellular_3g_init(&uart0_cfg);

#if (XBEE_MODE == 0)
	QM_PUTS("LoRa Mode");
	// Initialise XBee Wireless Module (LoRa)
	lora_init(&uart0_cfg);

	// Create 5 sec delay
	clk_sys_udelay(5000000);

	// AT Command
	uint8_t at_echo[] = "AT+ECHO=1\r\n";

	// Send AT Command (Turn on LoRa echo for debugging)
	qm_uart_write_buffer(QM_UART_0, at_echo, (sizeof(at_echo) - 1));

#elif (XBEE_MODE == 1)

	QM_PUTS("3G Mode");
	// Initialise XBee Wireless Module (Cellular 3G)
	cellular_3g_init(&uart0_cfg);
	// Create 5 sec delay
	clk_sys_udelay(5000000);
	// Connect to cellular base station
	cellular_3g_connect(QM_UART_0);

	//azwan added starts
#elif (XBEE_MODE == 2)
	QM_PUTS("WiFi Mode");
	wifi_init(&uart0_cfg);
	clk_sys_udelay(5000000);
	wifi_connect(QM_UART_0);

#elif (XBEE_MODE == 3)
	QM_PUTS("UART Mode");
	uart0_init(&uart0_cfg);
	clk_sys_udelay(5000000);
	uart0_connect(QM_UART_0);
	//ends
#endif
}

void xbee_tx(const qm_uart_t uart, uint16_t device_id, uint8_t frame_cntr, uint8_t data_ch, uint16_t data_type, uint8_t data[], uint8_t data_len)
{
	//cellular_3g_send( uart );
#if (XBEE_MODE == 0)
	lora_send( uart, device_id, frame_cntr, data_ch, data_type, data, data_len);

#elif (XBEE_MODE == 1)
    cellular_3g_send( uart, device_id, frame_cntr, data_ch, data_type, data, data_len);

    //azwan added starts
#elif (XBEE_MODE == 2)
    wifi_send( uart, device_id, frame_cntr, data_ch, data_type, data, data_len);
    //ends

#endif
}
