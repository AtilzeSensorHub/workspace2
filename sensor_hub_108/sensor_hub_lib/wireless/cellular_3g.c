/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 cellular_3g.c
 This is the library file for cellular 3G.

 Version   : 1.01
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : cellular_3g.c

 History:
   1.00 BK 27-Nov-17 Initial
   1.01 BK 08-May-18 Added 3G Tx, Rx, get token, update status
 */

#include "qm_common.h"
#include "system/utilities.h"
#include "qm_uart.h"
#include "../main.h"
#include "qm_common.h"
#include "qm_pinmux.h"
#include "clk.h"
#include "qm_gpio.h"
#include "string.h"
#include "stdlib.h"
#include "system/gpio.h"

// Initialise the XBee 3G module
void cellular_3g_init(qm_uart_config_t *cfg)
{

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Enable XBee Vcc                                             //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	qm_gpio_set_pin(QM_GPIO_0, 20);

	qm_pmux_select(QM_PIN_ID_19, QM_PMUX_FN_0);  // UART0 TXD
	qm_pmux_select(QM_PIN_ID_18, QM_PMUX_FN_0);  // UART0 RXD
	qm_pmux_select(QM_PIN_ID_41, QM_PMUX_FN_1);  // UART0 RTS
	qm_pmux_select(QM_PIN_ID_40, QM_PMUX_FN_1);  // UART0 CTS
	qm_pmux_input_en(QM_PIN_ID_18, true);
	qm_pmux_input_en(QM_PIN_ID_41, true);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Configures UART_0 for 9600 bps                              //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    cfg->baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 208, 5);
    cfg->line_control = QM_UART_LC_8N1;
    cfg->hw_fc = false;

    qm_uart_set_config(QM_UART_0, cfg);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // enable clock for UART_0                                     //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTA_REGISTER);
    clk_sys_udelay(5000000);    // Create 5 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send +++ (Enter AT command mode)                            //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at0[] = "+++";
    qm_uart_write_buffer(QM_UART_0, at0, (sizeof(at0) - 1));
    clk_sys_udelay(5000000);    // Create 5 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send ATDE 50                                                //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at1[] = "ATDE 50\r\n";
    qm_uart_write_buffer(QM_UART_0, at1, (sizeof(at1) - 1));
    clk_sys_udelay(3000000); 	// Create 3 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send ATWR                                                   //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at2[] = "ATWR\r\n";
    qm_uart_write_buffer(QM_UART_0, at2, (sizeof(at2) - 1));
    clk_sys_udelay(3000000); 	// Create 3 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send ATAC                                                   //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at3[] = "ATAC\r\n";
    qm_uart_write_buffer(QM_UART_0, at3, (sizeof(at3) - 1));
    clk_sys_udelay(3000000); 	// Create 3 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send ATCN (Exit AT command mode)                            //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at4[] = "ATCN\r\n";
    qm_uart_write_buffer(QM_UART_0, at4, (sizeof(at4) - 1));
    clk_sys_udelay(5000000); 	// Create 5 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send +++ (Enter AT command mode)                            //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at5[] = "+++";
    qm_uart_write_buffer(QM_UART_0, at5, (sizeof(at5) - 1));
    clk_sys_udelay(5000000);    // Create 5 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send ATDL staging.atilze.com                                //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t at6[] = "ATDL staging.atilze.com\r\n";
    //uint8_t at6[] = "ATDL httpbin.org\r\n";
    qm_uart_write_buffer(QM_UART_0, at6, (sizeof(at6) - 1));
    clk_sys_udelay(3000000); 	// Create 3 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send ATWR                                                   //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at7[] = "ATWR\r\n";
    qm_uart_write_buffer(QM_UART_0, at7, (sizeof(at7) - 1));
    clk_sys_udelay(3000000); 	// Create 3 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send ATAC                                                   //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at8[] = "ATAC\r\n";
    qm_uart_write_buffer(QM_UART_0, at8, (sizeof(at8) - 1));
    clk_sys_udelay(3000000); 	// Create 3 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Send ATCN (Exit AT command mode)                            //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    uint8_t at9[] = "ATCN\r\n";
    qm_uart_write_buffer(QM_UART_0, at9, (sizeof(at9) - 1));
    clk_sys_udelay(5000000); 	// Create 5 sec delay
}

// Get authentication token
uint8_t* cellular_3g_get_token(const qm_uart_t uart, uint8_t token[])
{

	clk_sys_udelay(10000000); 	// Create 10 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// BODY                                                        //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t body_a[] = "{\"grant_type\":\"password\",\"client_id\":\"";
	uint8_t body_b[] = TOKEN_CLIENT_ID;
	uint8_t body_c[] = "\",\"client_secret\":\"";
	uint8_t body_d[] = TOKEN_CLIENT_SECRET;
	uint8_t body_e[] = "\",\"username\":\"";
	uint8_t body_f[] = TOKEN_USER_NAME;
	uint8_t body_g[] = "\",\"password\":\"";
	uint8_t body_h[] = TOKEN_USER_PWD;
	uint8_t body_i[] = "\"}\r\n";
	uint8_t body[300];
	uint8_t size_of_body =  sizeof(body_a)+sizeof(body_b)+sizeof(body_c) \
			               +sizeof(body_d)+sizeof(body_e)+sizeof(body_f) \
						   +sizeof(body_g)+sizeof(body_h)+sizeof(body_i)-8;
	memset(body,0,sizeof(body));

	concat(body, body_a);
	concat(body, body_b);
	concat(body, body_c);
	concat(body, body_d);
	concat(body, body_e);
	concat(body, body_f);
	concat(body, body_g);
	concat(body, body_h);
	concat(body, body_i);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 1st POST command ("POST TOKEN_URL HTTP/1.1\r\n")            //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd0_a[]  = "POST ";
	uint8_t cmd0_b[]  = TOKEN_URL;
	uint8_t cmd0_c[]  = " HTTP/1.1\r\n";
	uint8_t cmd0[100];
	uint8_t size_of_cmd0 = sizeof(cmd0_a)+sizeof(cmd0_b)+sizeof(cmd0_c)-2;
	memset(cmd0,0,sizeof(cmd0));

	concat(cmd0, cmd0_a);
	concat(cmd0, cmd0_b);
	concat(cmd0, cmd0_c);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 2nd POST command ("Host: TOKEN_HOST \r\n")                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd1_a[]  = "Host: ";
	uint8_t cmd1_b[]  = TOKEN_HOST;
	uint8_t cmd1_c[]  = " \r\n";
	uint8_t cmd1[100];
	uint8_t size_of_cmd1 = sizeof(cmd1_a)+sizeof(cmd1_b)+sizeof(cmd1_c)-2;
	memset(cmd1,0,sizeof(cmd1));

	concat(cmd1, cmd1_a);
	concat(cmd1, cmd1_b);
	concat(cmd1, cmd1_c);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 3rd POST command ("Content-Type: application/json\r\n")     //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd2[] = "Content-Type: application/json\r\n";
	uint8_t size_of_cmd2 = sizeof(cmd2);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 4th POST command (Content-Length: 14\r\n\r\n)               //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	int content_len = size_of_body-1;
	uint8_t content_len_c[3];
	int2char(content_len, content_len_c);

	uint8_t cmd3_a[]  = "Content-Length: ";
	uint8_t cmd3_b[sizeof(content_len_c)+4+1];
	cmd3_b[0]  = content_len_c[2];
    cmd3_b[1]  = content_len_c[1];
    cmd3_b[2]  = content_len_c[0];
	cmd3_b[3]  = '\r';
	cmd3_b[4]  = '\n';
	cmd3_b[5]  = '\r';
	cmd3_b[6]  = '\n';

	uint8_t cmd3[100];
	uint8_t size_of_cmd3 = sizeof(cmd3_a)+sizeof(cmd3_b)-1;
	memset(cmd3,0,sizeof(cmd3));

	concat(cmd3, cmd3_a);
	concat(cmd3, cmd3_b);

	clk_sys_udelay(5000000); // Delay 5 second

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Flush UART Buffer                                           //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t uart_buffer=0;
	uint8_t uart_rx[4000];
	memset(uart_rx,0,sizeof(uart_rx));
	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Clear buffer and ensure UART TX is free                     //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	/*
	qm_uart_get_status(uart, &uart0_status);
	clk_sys_udelay(10000); 	// Create 10 milisec delay
	if (uart0_status != QM_UART_IDLE)
	{
		for(int jj=0; jj<16; jj++)
		{
			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
			uart_rx[jj] = uart_buffer;
		}
	}
	clk_sys_udelay(2000000); 	// Create 2 sec delay
	*/

	qm_uart_get_status(uart, &uart0_status);
	while((uart0_status == QM_UART_TX_BUSY))
	{
		//clk_sys_udelay(10000); 	// Create 10 sec delay
		qm_uart_get_status(uart, &uart0_status);
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Send HTTP Request Via UART                                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	qm_uart_write_buffer(uart, cmd0, (size_of_cmd0- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (size_of_cmd1- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (size_of_cmd2- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd3, (size_of_cmd3- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, body, (size_of_body- 1));     clk_sys_udelay(100000); // Delay 1 second

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Find And Grab The Token From the HTTP Response              //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Make sure there are data coming in (while loop need to becareful!)
	uint8_t rx_flag = 0;
	qm_uart_get_status(uart, &uart0_status);
	for (int ii = 0; ii< 200; ii++)
	{
	    if (uart0_status == QM_UART_IDLE)
	    {
	    	rx_flag = 0;
		    qm_uart_get_status(uart, &uart0_status);
		    clk_sys_udelay(10000); 	// Create 10 milisec delay
	    }
	    else
	    {
            rx_flag = 1;
		    qm_uart_get_status(uart, &uart0_status);
		    //clk_sys_udelay(10000); 	// Create 10 milisec delay
            break;
	    }
	}

	clk_sys_udelay(10000); 	// Create 10 milisec delay

	if (rx_flag == 1)
	{
		// Find the characters "access"
		uint8_t found_token = 0;
		uint8_t tmp_uart[12];
		uint16_t while_count = 0;
		while (found_token == 0)
		{
			// Get tmp_uart[0]
			if (uart0_status == QM_UART_RX_BUSY)
			{
			    qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
			    tmp_uart[0] = uart_buffer;
			}
			else
			{
				qm_uart_get_status(uart, &uart0_status);
				clk_sys_udelay(10000); 	// Create 10 milisec delay
			}

            // Make sure it is the 'access'
			if (tmp_uart[0] == 'a')
			{
				// Get tmp_uart[1]
				if (uart0_status == QM_UART_RX_BUSY){qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);tmp_uart[1] = uart_buffer;}
				else {qm_uart_get_status(uart, &uart0_status); clk_sys_udelay(10000);}

				if (tmp_uart[1] == 'c')
				{
					// Get tmp_uart[2]
					if (uart0_status == QM_UART_RX_BUSY){qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);tmp_uart[2] = uart_buffer;}
					else {qm_uart_get_status(uart, &uart0_status); clk_sys_udelay(10000);}

					if (tmp_uart[2] == 'c')
					{
						// Get tmp_uart[3]
						if (uart0_status == QM_UART_RX_BUSY){qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);tmp_uart[3] = uart_buffer;}
						else {qm_uart_get_status(uart, &uart0_status); clk_sys_udelay(10000);}

						if (tmp_uart[3] == 'e')
						{
							// Get tmp_uart[4]
							if (uart0_status == QM_UART_RX_BUSY){qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);tmp_uart[4] = uart_buffer;}
							else {qm_uart_get_status(uart, &uart0_status); clk_sys_udelay(10000);}

							if (tmp_uart[4] == 's')
							{
                                // Get tmp_uart[5]
								if (uart0_status == QM_UART_RX_BUSY){qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);tmp_uart[5] = uart_buffer;}
								else {qm_uart_get_status(uart, &uart0_status); clk_sys_udelay(10000);}

								if (tmp_uart[5] == 's')
								{
									found_token = 1;

									break;
								}
							}
						}
					}
				}
			}


			// Make sure it doesn't stay in the while loop forever!
			while_count++;
			if (while_count  == 3000)  // Tomorrow add to 3000
			{
				break;
			}

		}

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		// Get the token                                               //
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		for(int jj=0; jj<1200; jj++)
		{
			if (uart0_status == QM_UART_RX_BUSY)
			{
			    qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
			    uart_rx[jj] = uart_buffer;
			}
			else
			{
				qm_uart_get_status(uart, &uart0_status);
				clk_sys_udelay(10000); 	// Create 10 milisec delay
			}

			if (uart_rx[jj] == ',')
			{
				break;
			}
			clk_sys_udelay(1000); 	// Create 10 milisec delay
		}
	}
	clk_sys_udelay(10000); 	// Create 10 milisec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Store the token                                             //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	for(int jj=0; jj<2000; jj++)
	{
		if (uart_rx[jj  ] == '_' && uart_rx[jj+1] == 't' && uart_rx[jj+ 2] == 'o' && uart_rx[jj+ 3] == 'k' && \
			uart_rx[jj+4] == 'e' && uart_rx[jj+5] == 'n' )
		{
			clk_sys_udelay(10000); 	         // Create 10 milisec delay

			uint8_t flag_exit = 0;

			for (int ii=0; ii<1200; ii++)
			{
				if (uart_rx[jj+9+ii] != '"')
				{
					token[ii] = uart_rx[jj+9+ii];
				}
				else
				{
					flag_exit = 1;
					break;
				}
			}

			if (flag_exit==1)
			{
				break;
			}

		}

	}

	memset(cmd0,0,sizeof(cmd0));
	memset(cmd1,0,sizeof(cmd1));
	memset(cmd2,0,sizeof(cmd2));
	memset(cmd3,0,sizeof(cmd3));
	memset(body,0,sizeof(body));

	clk_sys_udelay(10000000); 	// Create 10 sec delay

	qm_uart_get_status(uart, &uart0_status);
	while((uart0_status == QM_UART_RX_BUSY))
	{
		clk_sys_udelay(10000); 	// Create 10 sec delay
		qm_uart_get_status(uart, &uart0_status);
	}

	return token;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Test Bench (Do Not Delete)                                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	/*
	//uint8_t cmd0[] = "POST /Ep050DF3iJZcmqlrv9At HTTP/1.1\r\n";
	//uint8_t cmd1[] = "Host: putsreq.com\r\n";
	//uint8_t cmd2[] = "Content-Type: application/json\r\n";
	//uint8_t cmd3[] = "Content-Length: 14\r\n\r\n";
	//uint8_t cmd4[] = "{\"data\":\"123\"}\r\n";

	uint8_t cmd0[] = "POST /oauth/token HTTP/1.1\r\n";
	uint8_t cmd1[] = "Host: staging.atilze.com\r\n";
	uint8_t cmd2[] = "Content-Type: application/json\r\n";
	uint8_t cmd3[] = "Content-Length: 169\r\n\r\n";
	uint8_t cmd4[] = "{\"grant_type\":\"password\",\"client_id\":\"11\",\"client_secret\":\"t7HoGUUmRw77tnLQ9dB1lOuzRAI5veLjG32ckTm9\",\"username\":\"boonkienkhoo@gmail.com\",\"password\":\"secret\",\"scope\":\"*\"}\r\n";

	//uint8_t cmd0[] = "POST /api/temp/device/data/inbound HTTP/1.1\r\n";
	//uint8_t cmd1[] = "Host: staging.atilze.com\r\n";
	//uint8_t cmd2[] = "Content-Type: application/json\r\n";
	//uint8_t cmd3[] = "Content-Length: 333\r\n\r\n";
	//uint8_t cmd4[] = "{\"message\":{\"id\":\"1513759964471-kG3ssDDDpzWdkQytAHXES09P\",\"macAddr\":\"00000000FFFFFFE4\",\"data\":\"000000000100670c1c\",\"recv\":\"2017-12-20T08:52:44.000Z\",\"extra\":{\"commsysType\":\"lora\",\"rssi\":-47,\"snr\":25,\"frameCnt\":1,\"gwid\":\"00001c497b88cf1f\",\"gwip\":\"192.168.1.85\",\"channel\":920000000,\"sf\":10,\"fport\":1},\"pub\":\"2017-12-20T08:52:44.474Z\"}}\r\n";

	//uint8_t cmd0[] = "POST /api/temp/device/data/inbound HTTP/1.1\r\n";
	//uint8_t cmd1[] = "Host: staging.atilze.com\r\n";
	//uint8_t cmd2[] = "Content-Type: application/json\r\n";
	//uint8_t cmd3[] = "Content-Length: 14\r\n\r\n";
	//uint8_t cmd4[] = "{\"data\":\"123\"}\r\n";

	qm_uart_write_buffer(uart, cmd0, (sizeof(cmd0)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (sizeof(cmd1)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (sizeof(cmd2)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd3, (sizeof(cmd3)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd4, (sizeof(cmd4)- 1));     clk_sys_udelay(1500000); // Delay 1.5 second
	//qm_uart_write_buffer(uart, body, (sizeof(body)- 1));     clk_sys_udelay(5000000); // Delay 0.5 second
	*/
}

// Send data via 3G (HTTP POST)
void cellular_3g_send(const qm_uart_t uart, uint16_t device_id, uint8_t frame_cntr, uint8_t data_ch, uint16_t data_type, uint8_t data[], uint8_t data_len)
{
	QM_PUTS("3g_send");
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//      Conv. to respective HEX size                           //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//               Tx. packet                                    //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Length of Tx Packet
	uint8_t tx_packet_len = data_len + 14 + 1;

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
    tx_packet[tx_packet_len-1] = '\0';

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 1st POST command ("POST POST_URL  HTTP/1.1\r\n")            //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd0_a[]  = "POST ";
	uint8_t cmd0_b[]  = POST_URL;
	uint8_t cmd0_c[]  = " HTTP/1.1\r\n";
	uint8_t cmd0[100];
	memset(cmd0,0,sizeof(cmd0));
	uint8_t size_of_cmd0 = sizeof(cmd0_a)+sizeof(cmd0_b)+sizeof(cmd0_c)-2;

	concat(cmd0, cmd0_a);
	concat(cmd0, cmd0_b);
	concat(cmd0, cmd0_c);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 2nd POST command ("Host: POST_HOST\r\n")                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd1_a[]  = "Host: ";
	uint8_t cmd1_b[]  = POST_HOST;
	uint8_t cmd1_c[]  = " \r\n";
	uint8_t cmd1[100];
	memset(cmd1,0,sizeof(cmd1));
	uint8_t size_of_cmd1 = sizeof(cmd1_a)+sizeof(cmd1_b)+sizeof(cmd1_c)-2;

	concat(cmd1, cmd1_a);
	concat(cmd1, cmd1_b);
	concat(cmd1, cmd1_c);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 3rd POST command ("Content-Type: application/json\r\n")     //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd2[] = "Content-Type: application/json\r\n";
	uint8_t size_of_cmd2 = sizeof(cmd2);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 4th POST command ("Content-Length: 14\r\n\r\n")             //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	int content_len = 194 + sizeof(tx_packet)-2;
	uint8_t content_len_c[3];
	int2char(content_len, content_len_c);

	uint8_t cmd3_a[]  = "Content-Length: ";
	uint8_t cmd3_b[sizeof(content_len_c)+4+1];
	cmd3_b[0]  = content_len_c[2];
    cmd3_b[1]  = content_len_c[1];
    cmd3_b[2]  = content_len_c[0];
	cmd3_b[3]  = '\r';
	cmd3_b[4]  = '\n';
	cmd3_b[5]  = '\r';
	cmd3_b[6]  = '\n';

	uint8_t cmd3[100];
	memset(cmd3,0,sizeof(cmd3));
	uint8_t size_of_cmd3 = sizeof(cmd3_a)+sizeof(cmd3_b)-1;

	concat(cmd3, cmd3_a);
	concat(cmd3, cmd3_b);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 5th POST command ("Authorization:Bearer XXX")               //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//uint8_t token[1200]="eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6Ijg2NjZkOWFiMTllMTI3NmZlNzZkMDlkOTU3MjJmNjAzYmE3ZTlkNTY3NTAzM2Y3YjYzZjNkMjdkNjkwZjllZWIxODIyNTY1ZjYyNzkxMGQxIn0.eyJhdWQiOiI2IiwianRpIjoiODY2NmQ5YWIxOWUxMjc2ZmU3NmQwOWQ5NTcyMmY2MDNiYTdlOWQ1Njc1MDMzZjdiNjNmM2QyN2Q2OTBmOWVlYjE4MjI1NjVmNjI3OTEwZDEiLCJpYXQiOjE1Mjk5MDgxNDksIm5iZiI6MTUyOTkwODE0OSwiZXhwIjoxNTYxNDQ0MTQ5LCJzdWIiOiIxNCIsInNjb3BlcyI6W119.G6SZji8zeLXPvoyxuS2KpcYnUHw3tlmZpe__KPuUSaPdFwzDR6in1Zw_gCZ0uiCX9oyHlIM3JwFpxn0hyhu2LXMUpKNBrjiF3_RrqmvbLn9-liDu9Ow03oU82x5wbaVFf3z0--PZaJ7y02pfPPFQmuG8aWFI8JrhSBva69IxjXUWgCdfBSVuSGGCLYpki483IpO_RnWOjLyIADPpp1Wdy4jQTdjjqc1WPtoRm0LexTbRtyLGY-X4ALS345s3aF-RetrAlEk5ZylU4fZS1eVEj-HEfNco2iBsX2krCeBQe7YtmX73LTYxAoREOAYFDi0X_HEdtNGhBFjqGbydpLfyMAccHGqx9ijFtYuRadayX3v2NkAW7hsDkU6n3bUtkp_DvpfHWwz1E9PSj4h_yxr4_Z-4QFmLbaSZGGWyEBJ_Wcwolr6GxYzq7fQiufozaInrj0D0fvQ3gQCwY6JJZKNkzEIwVBAVFINkLLr3T5iyESF5knTnWhtqbtYM2XXy8YmaryEIRzLctXNeQ5xDVFn1OG98326XWlcX9PkfJoH8OE_3wC9VWdnaEzB0hjC6NtozQwgm8BJbunYmYxvy1C3_VI8lXb96r17Hvz1A4lcSYIp8L4ha154JoU9wHk8pXNeDRxT1bnhXgh2WW9BiGbzCV-MxYiCaTcHaUoqs1rP0j04";
	uint8_t token[1200];
	memset(token,0,sizeof(token));
	cellular_3g_get_token(QM_UART_0, token);
	uint8_t cmd4_a[]  = "Authorization:Bearer ";
	int length_token;

	for(int jj=0; jj<1200; jj++)
	{
      if (token[jj] == '\0')
      {
          length_token = jj+1;
          break;
      }
	}

	uint8_t cmd4_b[length_token];
	for(int jj=0; jj<length_token; jj++)
	{
        cmd4_b[jj] = token[jj];
	}

	uint8_t cmd4_c[]  = "\r\n";
	int size_of_cmd4 = sizeof(cmd4_a)+sizeof(cmd4_b)+sizeof(cmd4_c)-2;
    uint8_t cmd4[1200];
	memset(cmd4,0,sizeof(cmd4));

	concat(cmd4, cmd4_a);
	concat(cmd4, cmd4_b);
	concat(cmd4, cmd4_c);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//     Body/Message ("{\"data\":\"PAYLOAD\"}\r\n")             //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t body_a[]  = "{\"message\":{\"id\":\"0\",\"macAddr\":\"";
	uint8_t body_b[]  = MAC_ADDR;
	uint8_t body_c[]  = "\",\"data\":\"";
	uint8_t body_d[sizeof(tx_packet)];
    for (uint8_t ii = 0; ii < sizeof(tx_packet); ii++)
    	body_d[ii] = tx_packet[ii];
    uint8_t body_e[]  = "\",\"recv\":\"0\",\"extra\":{\"commsysType\":\"0\",\"rssi\":-47,\"snr\":0,\"frameCnt\":1,\"gwid\":\"0\",\"gwip\":\"0\",\"channel\":0,\"sf\":0,\"fport\":0},\"pub\":\"0\"}}\r\n";
	uint8_t body[300];
	memset(body,0,sizeof(body));
	uint8_t size_of_body = sizeof(body_a)+sizeof(body_b)+sizeof(body_c)+sizeof(body_d)+sizeof(body_e)-4;

	concat(body, body_a);
	concat(body, body_b);
	concat(body, body_c);
	concat(body, body_d);
	concat(body, body_e);

	clk_sys_udelay(5000000); // Delay 0.5 second

	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;
	qm_uart_get_status(uart, &uart0_status);
	while((uart0_status == QM_UART_TX_BUSY))
	{
		//clk_sys_udelay(10000); 	// Create 10 sec delay
		qm_uart_get_status(uart, &uart0_status);
	}
	clk_sys_udelay(10000); 	// Create 10 milisec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Send the POST msgs over HTTP                                //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	qm_uart_write_buffer(uart, cmd0, (size_of_cmd0- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (size_of_cmd1- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (size_of_cmd2- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd4, (size_of_cmd4- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd3, (size_of_cmd3- 1));     clk_sys_udelay(500000); // Delay 0.5 second  //TODO: to change 3rd to 4th.
	qm_uart_write_buffer(uart, body, (size_of_body- 1));     clk_sys_udelay(500000); // Delay 0.5 second

	QM_PRINTF("Token 3g_sends: %s\n", token);
	memset(cmd0,0,sizeof(cmd0));
	memset(cmd1,0,sizeof(cmd1));
	memset(cmd2,0,sizeof(cmd2));
	memset(cmd3,0,sizeof(cmd3));
	memset(cmd4,0,sizeof(cmd4));
	memset(body,0,sizeof(body));
	memset(token,0,sizeof(token));

	clk_sys_udelay(5000000); 	// Create 5 sec delay

	/*
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Test Bench (Do Not Delete)                                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Test Codes
	//uint8_t cmd0[] = "POST /GHiLtvmGg8D0aFDhKuEu HTTP/1.1\r\n";
	//uint8_t cmd1[] = "Host: putsreq.com\r\n";
	//uint8_t cmd2[] = "Content-Type: application/json\r\n";
	//uint8_t cmd3[] = "Content-Length: 14\r\n\r\n";
	//uint8_t cmd4[] = "{\"data\":\"123\"}\r\n";

	uint8_t cmd0[] = "POST /api/temp/device/data/inbound HTTP/1.1\r\n";
	uint8_t cmd1[] = "Host: staging.atilze.com\r\n";
	uint8_t cmd2[] = "Content-Type: application/json\r\n";
	uint8_t cmd3[] = "Content-Length: 212\r\n\r\n";
	//uint8_t cmd4[] = "{\"data\":\"123\"}\r\n";

	uint8_t cmd4[] = "{\"message\":{\"id\":\"0\",\"macAddr\":\"000000000FFFFFF4\",\"data\":\"000000000100670c1c\",\"recv\":\"0\",\"extra\":{\"commsysType\":\"0\",\"rssi\":-47,\"snr\":0,\"frameCnt\":1,\"gwid\":\"0\",\"gwip\":\"0\",\"channel\":0,\"sf\":0,\"fport\":0},\"pub\":\"0\"}}\r\n";


	//uint8_t tx_packet[] = "000000000100670c1c";

	qm_uart_write_buffer(uart, cmd0, (sizeof(cmd0)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (sizeof(cmd1)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (sizeof(cmd2)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd3, (sizeof(cmd3)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd4, (sizeof(cmd4)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	*/
	QM_PUTS("3g_send ends");
}

// Check 3G connectivity
void cellular_3g_connect(const qm_uart_t uart)
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Initialisations                                             //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	QM_PUTS("3g_connect");
	uint8_t uart_buffer=0;
	uint8_t uart_rx[16];
	memset(uart_rx,0,sizeof(uart_rx));
	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Clear buffer and flush all the data                         //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	qm_uart_get_status(uart, &uart0_status);
	clk_sys_udelay(10000); 	// Create 10 milisec delay
	if (uart0_status != QM_UART_IDLE)
	{
		for(int jj=0; jj<16; jj++)
		{
			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
			uart_rx[jj] = uart_buffer;
		}
	}
	clk_sys_udelay(2000000); 	// Create 2 sec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Make sure it is connected                                   //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	while (1)
	{
        // Send +++ (Enter AT command mode)
		uint8_t at0[] = "+++";
		qm_uart_write_buffer(uart, at0, (sizeof(at0) - 1));

		// Total of 6 seconds delay with blinking LED
		for(int jj=0; jj<3; jj++)
		{
		  led_b_flash();             // Flash LED2 (RED)
		  clk_sys_udelay(1000000); 	// Create 1 sec delay
		}

		// Send ATAI
		uint8_t at1[] = "ATAI\r\n";
		qm_uart_write_buffer(uart, at1, (sizeof(at1) - 1));

		// Total of 2 seconds delay with blinking LED
		led_b_flash();               // Flash LED2 (RED)
		clk_sys_udelay(1000000); 	// Create 1 sec delay

		qm_uart_get_status(uart, &uart0_status);
		clk_sys_udelay(10000); 	// Create 10 milisec delay

		if (uart0_status != QM_UART_IDLE)
		{
			for(int jj=0; jj<4; jj++)
			{
				qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
                uart_rx[jj] = uart_buffer;
			}

			if (uart_rx[3] == 48)// Check for '0'
			{
				led_b_on();
				break;
			}

		}

		// Total 10 seconds delay with blinking LED
		for(int jj=0; jj<5; jj++)
		{
		  led_b_flash();             // Flash LED2 (RED)
		  clk_sys_udelay(1000000); 	// Create 1 sec delay
		}
	}
	QM_PUTS("3g_connect done");

}

// Receive data via 3G (HTTP GET)
uint8_t* cellular_3g_recv(const qm_uart_t uart, uint8_t payload_data[], uint8_t downlink_channel[])
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 1st POST command ("GET GET_URL HTTP/1.1\r\n")              //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	QM_PUTS("3g_recv");
	uint8_t cmd0_a[]  = "GET ";
	uint8_t cmd0_b[]  = GET_URL;
	uint8_t cmd0_c[]  = "00"; cmd0_c[0]= downlink_channel[0]; cmd0_c[1]= downlink_channel[1];
	uint8_t cmd0_d[]  = " HTTP/1.1\r\n";
	uint8_t cmd0[100];
	memset(cmd0,0,sizeof(cmd0));
	uint8_t size_of_cmd0 = sizeof(cmd0_a)+sizeof(cmd0_b)+sizeof(cmd0_c)+sizeof(cmd0_d)-3;

	concat(cmd0, cmd0_a);
	concat(cmd0, cmd0_b);
	concat(cmd0, cmd0_c);
	concat(cmd0, cmd0_d);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 2nd POST command ("Host: GET_HOST\r\n")                     //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd1_a[]  = "Host: ";
	uint8_t cmd1_b[]  = GET_HOST;
	uint8_t cmd1_c[]  = "\r\n";
	uint8_t cmd1[100];
	memset(cmd1,0,sizeof(cmd1));
	uint8_t size_of_cmd1 = sizeof(cmd1_a)+sizeof(cmd1_b)+sizeof(cmd1_c)-2;

	concat(cmd1, cmd1_a);
	concat(cmd1, cmd1_b);
	concat(cmd1, cmd1_c);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 3rd POST command ("Authorization:Bearer XXX")               //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

	//uint8_t token[1200]="eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6Ijg2NjZkOWFiMTllMTI3NmZlNzZkMDlkOTU3MjJmNjAzYmE3ZTlkNTY3NTAzM2Y3YjYzZjNkMjdkNjkwZjllZWIxODIyNTY1ZjYyNzkxMGQxIn0.eyJhdWQiOiI2IiwianRpIjoiODY2NmQ5YWIxOWUxMjc2ZmU3NmQwOWQ5NTcyMmY2MDNiYTdlOWQ1Njc1MDMzZjdiNjNmM2QyN2Q2OTBmOWVlYjE4MjI1NjVmNjI3OTEwZDEiLCJpYXQiOjE1Mjk5MDgxNDksIm5iZiI6MTUyOTkwODE0OSwiZXhwIjoxNTYxNDQ0MTQ5LCJzdWIiOiIxNCIsInNjb3BlcyI6W119.G6SZji8zeLXPvoyxuS2KpcYnUHw3tlmZpe__KPuUSaPdFwzDR6in1Zw_gCZ0uiCX9oyHlIM3JwFpxn0hyhu2LXMUpKNBrjiF3_RrqmvbLn9-liDu9Ow03oU82x5wbaVFf3z0--PZaJ7y02pfPPFQmuG8aWFI8JrhSBva69IxjXUWgCdfBSVuSGGCLYpki483IpO_RnWOjLyIADPpp1Wdy4jQTdjjqc1WPtoRm0LexTbRtyLGY-X4ALS345s3aF-RetrAlEk5ZylU4fZS1eVEj-HEfNco2iBsX2krCeBQe7YtmX73LTYxAoREOAYFDi0X_HEdtNGhBFjqGbydpLfyMAccHGqx9ijFtYuRadayX3v2NkAW7hsDkU6n3bUtkp_DvpfHWwz1E9PSj4h_yxr4_Z-4QFmLbaSZGGWyEBJ_Wcwolr6GxYzq7fQiufozaInrj0D0fvQ3gQCwY6JJZKNkzEIwVBAVFINkLLr3T5iyESF5knTnWhtqbtYM2XXy8YmaryEIRzLctXNeQ5xDVFn1OG98326XWlcX9PkfJoH8OE_3wC9VWdnaEzB0hjC6NtozQwgm8BJbunYmYxvy1C3_VI8lXb96r17Hvz1A4lcSYIp8L4ha154JoU9wHk8pXNeDRxT1bnhXgh2WW9BiGbzCV-MxYiCaTcHaUoqs1rP0j04";
	uint8_t token[1200];
	memset(token,0,sizeof(token));
	cellular_3g_get_token(QM_UART_0, token);
	uint8_t cmd2_a[]  = "Authorization: Bearer ";
	int length_token;

	for(int jj=0; jj<1200; jj++)
	{
      if (token[jj] == '\0')
      {
          length_token = jj+1;
          break;
      }
	}

	uint8_t cmd2_b[length_token];
	for(int jj=0; jj<length_token; jj++)
	{
        cmd2_b[jj] = token[jj];
	}

	uint8_t cmd2_c[]  = "\r\n\r\n";
	int size_of_cmd2 = sizeof(cmd2_a)+sizeof(cmd2_b)+sizeof(cmd2_c)-2;
    uint8_t cmd2[1200];
    memset(cmd2,0,sizeof(cmd2));

	concat(cmd2, cmd2_a);
	concat(cmd2, cmd2_b);
	concat(cmd2, cmd2_c);


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Get data over HTTP                                          //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	clk_sys_udelay(10000000); // Delay 10 seconds

	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;
	qm_uart_get_status(uart, &uart0_status);
	while((uart0_status == QM_UART_TX_BUSY))
	{
		//clk_sys_udelay(10000); 	// Create 10 sec delay
		qm_uart_get_status(uart, &uart0_status);
	}
	clk_sys_udelay(10000); 	// Create 10 milisec delay

	qm_uart_write_buffer(uart, cmd0, (size_of_cmd0- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (size_of_cmd1- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (size_of_cmd2- 1));     clk_sys_udelay(500000); // Delay 0.5 second

    // Commands to GET
	//uint8_t cmd0[] = "GET /api/downlinkStatus/FFFFFFF4/01 HTTP/1.1\r\n";
	//uint8_t cmd1[] = "Host: staging.atilze.com\r\n";
	//uint8_t cmd2[] = "Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6ImU3YTc4OTVkMGVjNzMzYjRjOGExMTczOWUyMWNjZTQwODAwODY1ZjVhOWQwN2Q2MGMyNDFiMmQ4Y2FlZWEzODNmMzM3NDRlMjYyYTY1MDMyIn0.eyJhdWQiOiIxMSIsImp0aSI6ImU3YTc4OTVkMGVjNzMzYjRjOGExMTczOWUyMWNjZTQwODAwODY1ZjVhOWQwN2Q2MGMyNDFiMmQ4Y2FlZWEzODNmMzM3NDRlMjYyYTY1MDMyIiwiaWF0IjoxNTI1MjQ4NTc4LCJuYmYiOjE1MjUyNDg1NzgsImV4cCI6MTUyNzg0MDU3OCwic3ViIjoiMjMiLCJzY29wZXMiOlsiKiJdfQ.KiFn3aQxxH-c3lUWke_MKUSEjMqooD4GiQ8GaKxqhrAb9kxwfO2pYduc_P-AvjTGsOqZdJJGGEncPMOHaG5bEk5m_IElM4lptISmEXdBVXlYCOuvQ7RKvcowpx1JSqerIRaYLxQxd2zZGuU_EGp_YLqTauuOClNTqI0CLInMdKrHASG5zVWJDojKGzq_jgHTlXP8qIg0_LkvypqhKRP48zbz476EL-PtpevWEdgxpaH6Zxhh_QkWMpGeIHVQvjxdO24lpStot2_H3ENa9PL-GAUcAPqi0Z8By9UKRE-dWMcX_KucvzyRaXgR__uX45s6H-WmZeIfu89ovQ1koZ29jVbZlMpg18C2B0W7btCln_v87JGu3Mos4Fgw71GfPu_GZR9oaFA_0m4AXJTDIQBs6vGoL73-wS5oNRl5MRv-GbC0x-OBH_eBS3WmebschA4EWsKpT-OoUOJiQiW0GYs_alvPz01G5Hu5ER4tp0iS2sLqdz62fxZw7u3dbkFTXMOL2j9RShPiIZjvWviA1XwlqOizSTwjmQ2kqPVO4w_BNJ9DHqFWp0hRJT-Frw4BJBFEBXQHFdgZRRb9c6cnJDYwDmJCUYjQY-kWe2fkSEH-nifDwocqmCRFAPXkt80QyAmfJN64-3aGLeJg-dzzPgiALFAgIbZ76gYX7CkIASgLsp0\r\n\r\n";

	//qm_uart_write_buffer(uart, cmd0, (sizeof(cmd0)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd1, (sizeof(cmd1)- 1));   clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd2, (sizeof(cmd2)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Find downlink data                                          //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t uart_buffer=0;
	uint8_t uart_rx[450];
	memset(uart_rx,0,sizeof(uart_rx));
	qm_uart_get_status(uart, &uart0_status);
	clk_sys_udelay(10000); 	// Create 10 milisec delay

    // Make sure there are data coming in (while loop need to becareful!)
	uint8_t rx_flag = 0;
	for (int ii = 0; ii< 1000; ii++)
	{
	    if (uart0_status == QM_UART_IDLE)
	    {
	    	rx_flag = 0;
		    qm_uart_get_status(uart, &uart0_status);
		    clk_sys_udelay(10000); 	// Create 10 milisec delay
	    }
	    else
	    {
            rx_flag = 1;
            break;
	    }
	}

	// Only goes in when there is data coming in
	if (rx_flag==1)
	{
		// Find downlink data
		for(int jj=6; jj<450; jj++)
		{

			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
			uart_rx[jj] = uart_buffer;

			if(uart_rx[jj-6] =='m'   && uart_rx[jj-5] =='e'  && uart_rx[jj-4] =='s' && uart_rx[jj-3]=='s'&& \
			   uart_rx[jj-2] == 'a' && uart_rx[jj-1] == 'g' && uart_rx[jj  ] == 'e')
			{
				break;
			}
		}
	}
	clk_sys_udelay(10000); 	// Create 10 milisec delay

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Get the payload                                             //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	for(int jj=6; jj<450; jj++)
	{
		if(uart_rx[jj-6] =='d'   && uart_rx[jj-5] =='a'  && uart_rx[jj-4] =='t' && uart_rx[jj-3]=='a'&& \
	       uart_rx[jj-2] == '\"' && uart_rx[jj-1] == ':' && uart_rx[jj  ] == '\"')
		{

        	for (int ii=0; ii<16; ii++)
        	{
        	   payload_data[ii] = uart_rx[jj+ii+1];
        	}

		}

	}
	QM_PRINTF("Token 3g_recv: %s\n", token);
	memset(cmd0,0,sizeof(cmd0));
	memset(cmd1,0,sizeof(cmd1));
	memset(cmd2,0,sizeof(cmd2));
	memset(token,0,sizeof(token));
	QM_PUTS("3g_recv done");

	return payload_data;


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Test Bench (Do Not Delete)                                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	/*
    // Commands to GET
	uint8_t cmd0[] = "GET /api/downlinkStatus/FFFFFFF4/01 HTTP/1.1\r\n";
	uint8_t cmd1[] = "Host: staging.atilze.com\r\n";
	uint8_t cmd2[] = "Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6ImU3YTc4OTVkMGVjNzMzYjRjOGExMTczOWUyMWNjZTQwODAwODY1ZjVhOWQwN2Q2MGMyNDFiMmQ4Y2FlZWEzODNmMzM3NDRlMjYyYTY1MDMyIn0.eyJhdWQiOiIxMSIsImp0aSI6ImU3YTc4OTVkMGVjNzMzYjRjOGExMTczOWUyMWNjZTQwODAwODY1ZjVhOWQwN2Q2MGMyNDFiMmQ4Y2FlZWEzODNmMzM3NDRlMjYyYTY1MDMyIiwiaWF0IjoxNTI1MjQ4NTc4LCJuYmYiOjE1MjUyNDg1NzgsImV4cCI6MTUyNzg0MDU3OCwic3ViIjoiMjMiLCJzY29wZXMiOlsiKiJdfQ.KiFn3aQxxH-c3lUWke_MKUSEjMqooD4GiQ8GaKxqhrAb9kxwfO2pYduc_P-AvjTGsOqZdJJGGEncPMOHaG5bEk5m_IElM4lptISmEXdBVXlYCOuvQ7RKvcowpx1JSqerIRaYLxQxd2zZGuU_EGp_YLqTauuOClNTqI0CLInMdKrHASG5zVWJDojKGzq_jgHTlXP8qIg0_LkvypqhKRP48zbz476EL-PtpevWEdgxpaH6Zxhh_QkWMpGeIHVQvjxdO24lpStot2_H3ENa9PL-GAUcAPqi0Z8By9UKRE-dWMcX_KucvzyRaXgR__uX45s6H-WmZeIfu89ovQ1koZ29jVbZlMpg18C2B0W7btCln_v87JGu3Mos4Fgw71GfPu_GZR9oaFA_0m4AXJTDIQBs6vGoL73-wS5oNRl5MRv-GbC0x-OBH_eBS3WmebschA4EWsKpT-OoUOJiQiW0GYs_alvPz01G5Hu5ER4tp0iS2sLqdz62fxZw7u3dbkFTXMOL2j9RShPiIZjvWviA1XwlqOizSTwjmQ2kqPVO4w_BNJ9DHqFWp0hRJT-Frw4BJBFEBXQHFdgZRRb9c6cnJDYwDmJCUYjQY-kWe2fkSEH-nifDwocqmCRFAPXkt80QyAmfJN64-3aGLeJg-dzzPgiALFAgIbZ76gYX7CkIASgLsp0\r\n\r\n";

	qm_uart_write_buffer(uart, cmd0, (sizeof(cmd0)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (sizeof(cmd1)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (sizeof(cmd2)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	*/

}

// Update downlink status
void cellular_3g_update_dl_status(const qm_uart_t uart, uint8_t downlink_channel[], uint8_t status[])
{
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 1st POST command ("GET STATUS_URL HTTP/1.1\r\n")            //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd0_a[]  = "POST ";
	uint8_t cmd0_b[]  = STATUS_URL;
	uint8_t cmd0_c[]  = "00"; cmd0_c[0] = downlink_channel[0]; cmd0_c[1] = downlink_channel[1];
	uint8_t cmd0_d[]  = "/";
	uint8_t cmd0_e[]  = "00"; cmd0_e[0] = status[0]; cmd0_e[1] = status[1];
	uint8_t cmd0_f[]  = " HTTP/1.1\r\n";
	uint8_t cmd0[100];
	memset(cmd0,0,sizeof(cmd0));
	uint8_t size_of_cmd0 = sizeof(cmd0_a) + sizeof(cmd0_b) + sizeof(cmd0_c) + sizeof(cmd0_d) + sizeof(cmd0_e) + sizeof(cmd0_f)-5;

	concat(cmd0, cmd0_a);
	concat(cmd0, cmd0_b);
	concat(cmd0, cmd0_c);
	concat(cmd0, cmd0_d);
	concat(cmd0, cmd0_e);
	concat(cmd0, cmd0_f);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 2nd POST command ("Host: STATUS_HOST\r\n")                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t cmd1_a[]  = "Host: ";
	uint8_t cmd1_b[]  = STATUS_HOST;
	uint8_t cmd1_c[]  = " \r\n";
	uint8_t cmd1[100];
	memset(cmd1,0,sizeof(cmd1));
	uint8_t size_of_cmd1 = sizeof(cmd1_a)+sizeof(cmd1_b)+sizeof(cmd1_c)-2;

	concat(cmd1, cmd1_a);
	concat(cmd1, cmd1_b);
	concat(cmd1, cmd1_c);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 3rd POST command ("Authorization:Bearer XXX")               //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//uint8_t token[1200]="eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6Ijg2NjZkOWFiMTllMTI3NmZlNzZkMDlkOTU3MjJmNjAzYmE3ZTlkNTY3NTAzM2Y3YjYzZjNkMjdkNjkwZjllZWIxODIyNTY1ZjYyNzkxMGQxIn0.eyJhdWQiOiI2IiwianRpIjoiODY2NmQ5YWIxOWUxMjc2ZmU3NmQwOWQ5NTcyMmY2MDNiYTdlOWQ1Njc1MDMzZjdiNjNmM2QyN2Q2OTBmOWVlYjE4MjI1NjVmNjI3OTEwZDEiLCJpYXQiOjE1Mjk5MDgxNDksIm5iZiI6MTUyOTkwODE0OSwiZXhwIjoxNTYxNDQ0MTQ5LCJzdWIiOiIxNCIsInNjb3BlcyI6W119.G6SZji8zeLXPvoyxuS2KpcYnUHw3tlmZpe__KPuUSaPdFwzDR6in1Zw_gCZ0uiCX9oyHlIM3JwFpxn0hyhu2LXMUpKNBrjiF3_RrqmvbLn9-liDu9Ow03oU82x5wbaVFf3z0--PZaJ7y02pfPPFQmuG8aWFI8JrhSBva69IxjXUWgCdfBSVuSGGCLYpki483IpO_RnWOjLyIADPpp1Wdy4jQTdjjqc1WPtoRm0LexTbRtyLGY-X4ALS345s3aF-RetrAlEk5ZylU4fZS1eVEj-HEfNco2iBsX2krCeBQe7YtmX73LTYxAoREOAYFDi0X_HEdtNGhBFjqGbydpLfyMAccHGqx9ijFtYuRadayX3v2NkAW7hsDkU6n3bUtkp_DvpfHWwz1E9PSj4h_yxr4_Z-4QFmLbaSZGGWyEBJ_Wcwolr6GxYzq7fQiufozaInrj0D0fvQ3gQCwY6JJZKNkzEIwVBAVFINkLLr3T5iyESF5knTnWhtqbtYM2XXy8YmaryEIRzLctXNeQ5xDVFn1OG98326XWlcX9PkfJoH8OE_3wC9VWdnaEzB0hjC6NtozQwgm8BJbunYmYxvy1C3_VI8lXb96r17Hvz1A4lcSYIp8L4ha154JoU9wHk8pXNeDRxT1bnhXgh2WW9BiGbzCV-MxYiCaTcHaUoqs1rP0j04";
	uint8_t token[1200];
	memset(token,0,sizeof(token));
	cellular_3g_get_token(QM_UART_0, token);
	uint8_t cmd2_a[]  = "Authorization:Bearer ";
	int length_token;

	for(int jj=0; jj<1200; jj++)
	{
      if (token[jj] == '\0')
      {
          length_token = jj+1;
          break;
      }
	}

	uint8_t cmd2_b[length_token];
	for(int jj=0; jj<length_token; jj++)
	{
        cmd2_b[jj] = token[jj];
	}

	uint8_t cmd2_c[]  = "\r\n\r\n";
	int size_of_cmd2 = sizeof(cmd2_a)+sizeof(cmd2_b)+sizeof(cmd2_c)-2;
    uint8_t cmd2[1200];
    memset(cmd2,0,sizeof(cmd2));

	concat(cmd2, cmd2_a);
	concat(cmd2, cmd2_b);
	concat(cmd2, cmd2_c);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Update status over HTTP                                     //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	clk_sys_udelay(5000000); // Delay 5 seconds

	qm_uart_write_buffer(uart, cmd0, (size_of_cmd0- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (size_of_cmd1- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (size_of_cmd2- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	QM_PRINTF("Token 3g_sends: %s\n", token);
	memset(cmd0,0,sizeof(cmd0));
	memset(cmd1,0,sizeof(cmd1));
	memset(cmd2,0,sizeof(cmd2));
	memset(token,0,sizeof(token));

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Test Bench (Do Not Delete)                                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    // Commands to GET
	//uint8_t cmd0[] = "POST /api/updateDownlinkStatus/FFFFFFF4/00/01 HTTP/1.1\r\n";
	//uint8_t cmd1[] = "Host: staging.atilze.com\r\n";
	//uint8_t cmd2[] = "Authorization: Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6ImU3YTc4OTVkMGVjNzMzYjRjOGExMTczOWUyMWNjZTQwODAwODY1ZjVhOWQwN2Q2MGMyNDFiMmQ4Y2FlZWEzODNmMzM3NDRlMjYyYTY1MDMyIn0.eyJhdWQiOiIxMSIsImp0aSI6ImU3YTc4OTVkMGVjNzMzYjRjOGExMTczOWUyMWNjZTQwODAwODY1ZjVhOWQwN2Q2MGMyNDFiMmQ4Y2FlZWEzODNmMzM3NDRlMjYyYTY1MDMyIiwiaWF0IjoxNTI1MjQ4NTc4LCJuYmYiOjE1MjUyNDg1NzgsImV4cCI6MTUyNzg0MDU3OCwic3ViIjoiMjMiLCJzY29wZXMiOlsiKiJdfQ.KiFn3aQxxH-c3lUWke_MKUSEjMqooD4GiQ8GaKxqhrAb9kxwfO2pYduc_P-AvjTGsOqZdJJGGEncPMOHaG5bEk5m_IElM4lptISmEXdBVXlYCOuvQ7RKvcowpx1JSqerIRaYLxQxd2zZGuU_EGp_YLqTauuOClNTqI0CLInMdKrHASG5zVWJDojKGzq_jgHTlXP8qIg0_LkvypqhKRP48zbz476EL-PtpevWEdgxpaH6Zxhh_QkWMpGeIHVQvjxdO24lpStot2_H3ENa9PL-GAUcAPqi0Z8By9UKRE-dWMcX_KucvzyRaXgR__uX45s6H-WmZeIfu89ovQ1koZ29jVbZlMpg18C2B0W7btCln_v87JGu3Mos4Fgw71GfPu_GZR9oaFA_0m4AXJTDIQBs6vGoL73-wS5oNRl5MRv-GbC0x-OBH_eBS3WmebschA4EWsKpT-OoUOJiQiW0GYs_alvPz01G5Hu5ER4tp0iS2sLqdz62fxZw7u3dbkFTXMOL2j9RShPiIZjvWviA1XwlqOizSTwjmQ2kqPVO4w_BNJ9DHqFWp0hRJT-Frw4BJBFEBXQHFdgZRRb9c6cnJDYwDmJCUYjQY-kWe2fkSEH-nifDwocqmCRFAPXkt80QyAmfJN64-3aGLeJg-dzzPgiALFAgIbZ76gYX7CkIASgLsp0\r\n\r\n";

	//qm_uart_write_buffer(uart, cmd0, (sizeof(cmd0)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd1, (sizeof(cmd1)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd2, (sizeof(cmd2)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd0, (size_of_cmd0- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd1, (size_of_cmd1- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd2, (size_of_cmd2- 1));     clk_sys_udelay(500000); // Delay 0.5 second

	//memset(cmd0,0,sizeof(cmd0));
	//memset(cmd1,0,sizeof(cmd1));
	//memset(cmd2,0,sizeof(cmd2));
	//memset(token,0,sizeof(token));
}
