/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 cellular_3g.c
 This is the library file for cellular 3G.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : cellular_3g.c

 History:
   1.00 BK 27-Nov-17 Initial
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
void wifi_init(qm_uart_config_t *cfg)
{
    // Enable XBee Vcc
	qm_gpio_set_pin(QM_GPIO_0, 20);

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

    clk_sys_udelay(5000000);    // Create 5 sec delay

    // Send +++ (Enter AT command mode)
    uint8_t at0[] = "AT\r\n";
    qm_uart_write_buffer(QM_UART_0, at0, (sizeof(at0) - 1));
    clk_sys_udelay(5000000);    // Create 5 sec delay

//    //added starts
//    uint8_t uart_rx[16];
//    memset(uart_rx,0,sizeof(uart_rx));
//
//    static qm_uart_status_t uart0_status __attribute__((unused)) = 0;
//
//    uint8_t uart_buffer=0;
//    qm_uart_get_status(QM_UART_0, &uart0_status);
//    if (uart0_status != QM_UART_IDLE)
//	{
//		for(int jj=0; jj<16; jj++)
//		{
//			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//			uart_rx[jj] = uart_buffer;
//			QM_PRINTF("++: %d\n", uart_buffer);
//		}
//	}
//
//    //ends
//
//
//    // Send ATDE 50
//    uint8_t at1[] = "AT";
//    qm_uart_write_buffer(QM_UART_0, at1, (sizeof(at1) - 1));
//    clk_sys_udelay(3000000); 	// Create 3 sec delay
//
//    //added
//    if (uart0_status != QM_UART_IDLE)
//	{
//		for(int jj=0; jj<16; jj++)
//		{
//			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//			uart_rx[jj] = uart_buffer;
//			QM_PRINTF("ATDE: %d\n", uart_buffer);
//		}
//	}
//    //ends
//
//    // Send ATWR
//    uint8_t at2[] = "ATWR\r\n";
//    qm_uart_write_buffer(QM_UART_0, at2, (sizeof(at2) - 1));
//    clk_sys_udelay(3000000); 	// Create 3 sec delay
//
//    //added
//    if (uart0_status != QM_UART_IDLE)
//   	{
//   		for(int jj=0; jj<16; jj++)
//   		{
//   			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//   			uart_rx[jj] = uart_buffer;
//   			QM_PRINTF("ATWR: %d\n", uart_buffer);
//   		}
//   	}
//    //ends
//
//    // Send ATAC
//    uint8_t at3[] = "ATAC\r\n";
//    qm_uart_write_buffer(QM_UART_0, at3, (sizeof(at3) - 1));
//    clk_sys_udelay(3000000); 	// Create 3 sec delay
//
//    //added
//    if (uart0_status != QM_UART_IDLE)
//   	{
//   		for(int jj=0; jj<16; jj++)
//   		{
//   			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//   			uart_rx[jj] = uart_buffer;
//   			QM_PRINTF("ATAC: %d\n", uart_buffer);
//   		}
//   	}
//    //ends
//
//    // Send ATCN (Exit AT command mode)
//    uint8_t at4[] = "ATCN\r\n";
//    qm_uart_write_buffer(QM_UART_0, at4, (sizeof(at4) - 1));
//    clk_sys_udelay(5000000); 	// Create 5 sec delay
//
//    //added
//	if (uart0_status != QM_UART_IDLE)
//	{
//		for(int jj=0; jj<16; jj++)
//		{
//			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//			uart_rx[jj] = uart_buffer;
//			QM_PRINTF("ATCN: %d\n", uart_buffer);
//		}
//	}
//	//ends
//
//    // Send +++ (Enter AT command mode)
//    uint8_t at5[] = "+++";
//    qm_uart_write_buffer(QM_UART_0, at5, (sizeof(at5) - 1));
//    clk_sys_udelay(5000000);    // Create 5 sec delay
//
//    //added
//	if (uart0_status != QM_UART_IDLE)
//	{
//		for(int jj=0; jj<16; jj++)
//		{
//			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//			uart_rx[jj] = uart_buffer;
//			QM_PRINTF("+++: %d\n", uart_buffer);
//		}
//	}
//	//ends
//
//    // Send ATDL putsreq.com
//	uint8_t at6[] = "ATDL fabrick.atilze.com\r\n";
//    //uint8_t at6[] = "ATDL httpbin.org\r\n";
//    qm_uart_write_buffer(QM_UART_0, at6, (sizeof(at6) - 1));
//    clk_sys_udelay(3000000); 	// Create 3 sec delay
//
//    //added
//	if (uart0_status != QM_UART_IDLE)
//	{
//		for(int jj=0; jj<16; jj++)
//		{
//			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//			uart_rx[jj] = uart_buffer;
//			QM_PRINTF("ATDL: %d\n", uart_buffer);
//		}
//	}
//	//ends
//
//    // Send ATWR
//    uint8_t at7[] = "ATWR\r\n";
//    qm_uart_write_buffer(QM_UART_0, at7, (sizeof(at7) - 1));
//    clk_sys_udelay(3000000); 	// Create 3 sec delay
//
//    //added
//	if (uart0_status != QM_UART_IDLE)
//	{
//		for(int jj=0; jj<16; jj++)
//		{
//			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//			uart_rx[jj] = uart_buffer;
//			QM_PRINTF("ATWR: %d\n", uart_buffer);
//		}
//	}
//	//ends
//
//    // Send ATAC
//    uint8_t at8[] = "ATAC\r\n";
//    qm_uart_write_buffer(QM_UART_0, at8, (sizeof(at8) - 1));
//    clk_sys_udelay(3000000); 	// Create 3 sec delay
//
//    //added
//	if (uart0_status != QM_UART_IDLE)
//	{
//		for(int jj=0; jj<16; jj++)
//		{
//			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//			uart_rx[jj] = uart_buffer;
//			QM_PRINTF("ATAC: %d\n", uart_buffer);
//		}
//	}
//	//ends
//
//    // Send ATCN (Exit AT command mode)
//    uint8_t at9[] = "ATCN\r\n";
//    qm_uart_write_buffer(QM_UART_0, at9, (sizeof(at9) - 1));
//    clk_sys_udelay(5000000); 	// Create 5 sec delay
//
//    //added
//	if (uart0_status != QM_UART_IDLE)
//	{
//		for(int jj=0; jj<16; jj++)
//		{
//			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
//			uart_rx[jj] = uart_buffer;
//			QM_PRINTF("ATCN: %d\n", uart_buffer);
//		}
//	}
//	//ends
}

// Get authentication token
uint8_t* wifi_get_token(const qm_uart_t uart, uint8_t token[])
{
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

	clk_sys_udelay(5000000); // Delay 0.5 second

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Flush UART Buffer                                           //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t uart_buffer=0;
	uint8_t uart_rx[2000];
	memset(uart_rx,0,sizeof(uart_rx));
	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;

	//while (uart0_status == QM_UART_IDLE)
	//{
	//	qm_uart_get_status(uart, &uart0_status);
	//	clk_sys_udelay(10000); 	// Create 10 milisec delay
	//}

	//for(int jj=0; jj<16; jj++)
	//{
	//	qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
	//	uart_rx[jj] = uart_buffer;
	//}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Send HTTP Request Via UART                                  //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	qm_uart_write_buffer(uart, cmd0, (size_of_cmd0- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (size_of_cmd1- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (size_of_cmd2- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd3, (size_of_cmd3- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, body, (size_of_body- 1));     clk_sys_udelay(500000); // Delay 0.5 second

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Find And Grab The Token From the HTTP Response              //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	qm_uart_get_status(uart, &uart0_status);
	clk_sys_udelay(10000); 	// Create 10 milisec delay

	while (uart0_status == QM_UART_IDLE)
	{
		qm_uart_get_status(uart, &uart0_status);
		clk_sys_udelay(10000); 	// Create 10 milisec delay
	}

	// Find the characters "access"
	uint8_t found_token = 0;
	uint8_t tmp_uart[12];
	while (found_token == 0)
	{
		qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
		tmp_uart[0] = uart_buffer;

		if (tmp_uart[0] == 'a')
		{
			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
			tmp_uart[1] = uart_buffer;

			if (tmp_uart[1] == 'c')
			{
				qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
				tmp_uart[2] = uart_buffer;

				if (tmp_uart[2] == 'c')
				{
					qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
					tmp_uart[3] = uart_buffer;

					if (tmp_uart[3] == 'e')
					{
						qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
						tmp_uart[4] = uart_buffer;

						if (tmp_uart[4] == 's')
						{
							qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
							tmp_uart[5] = uart_buffer;

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
	}

	// Get the token
	for(int jj=0; jj<1500; jj++)
	{
		qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
		uart_rx[jj] = uart_buffer;
	}

	// Store the token
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

	return token;

}

// Send data via 3G (HTTP POST)
void wifi_send(const qm_uart_t uart, uint16_t device_id, uint8_t frame_cntr, uint8_t data_ch, uint16_t data_type, uint8_t data[], uint8_t data_len)
{
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
	//int content_len = 194 + sizeof(tx_packet)-1;
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
	uint8_t size_of_cmd3 = sizeof(cmd3_a)+sizeof(cmd3_b)-1;

	concat(cmd3, cmd3_a);
	concat(cmd3, cmd3_b);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// 5th POST command ("Authorization:Bearer XXX")               //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t token[1200];
	memset(token,0,sizeof(token));
	wifi_get_token(QM_UART_0, token);
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

	concat(cmd4, cmd4_a);
	concat(cmd4, cmd4_b);
	concat(cmd4, cmd4_c);

	//uint8_t cmd4[] = "Authorization:Bearer eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6Ijg3YzQxZGZiOTZiODljYTFlZTlhZDMyNDRmMDM4ODNlYmQ2Mzg3OTk4NTg5MWY0Y2U4OTk0MjE0ZmEwM2VjNWI5MDE4YTE3YjY5OTM0YWM0In0.eyJhdWQiOiIxMSIsImp0aSI6Ijg3YzQxZGZiOTZiODljYTFlZTlhZDMyNDRmMDM4ODNlYmQ2Mzg3OTk4NTg5MWY0Y2U4OTk0MjE0ZmEwM2VjNWI5MDE4YTE3YjY5OTM0YWM0IiwiaWF0IjoxNTE2OTM1ODQyLCJuYmYiOjE1MTY5MzU4NDIsImV4cCI6MTUxOTUyNzg0MSwic3ViIjoiMjMiLCJzY29wZXMiOltdfQ.ctkwTLEphjvHlnDaV0jA4UAmNu-YU_C5yCUAO297rjSnY4xNfENnhXW99r22b8WyVHqMT2xuTB3hoihm6FaU0nmbRrV7QdrtVzNb89SW-0_JT7EpMLAqc-oOjMNvLg1-lIl1ThgZu9rCU4ANuEYAeiRLkZig_8xrqluPpzfC2FDvUhHPmYhJQYKT3UFgFXKWZSSRk98iNguziGUqrdp-1KqlhQLN9jydh066Jt0SAiSeufX5UAG7sKA7S6iAWshzYn7D3ujndmDnf_xj2vFUwYVTVsmMFoxk0QN6b-KcyeOEB2171Q6G3B1B0f7I9ZubnS2Q5uMk6D9WrPIKF3s9PRKymAUHp2fJ7qp_jz9H7TaGcA4zfYWdE-wAkLZcNCqbzSpkwrfFRePERM_y8S2AmBgCtFZH3Kh9VNNf7mon2VW_8vgPs8w4uByNf-b7vpgbjZe-vaSFTW2s277Jty9tpRi75WzuZZX02YrVgYeROHHGi-O6-l5XrJPJm_akab1hvNNnX1GyW6G3VFjm23j4DVFN2jJ2ddhJtRE9Q_3scTWnHYXaDjRigowRsj-jG1299S46bIiQymdWEBq1CIofL59fbAjiMuyNyqYz1ns3lwB4yzXA6GM_wEVROYXARFkb-bKCAmEs1k60nxMD7ZIMMC3uqL0A0qPqkbfepwqpOmM\r\n";

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
	uint8_t size_of_body = sizeof(body_a)+sizeof(body_b)+sizeof(body_c)+sizeof(body_d)+sizeof(body_e)-4;

	concat(body, body_a);
	concat(body, body_b);
	concat(body, body_c);
	concat(body, body_d);
	concat(body, body_e);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Send the POST msgs over HTTP                                //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	qm_uart_write_buffer(uart, cmd0, (size_of_cmd0- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (size_of_cmd1- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd2, (size_of_cmd2- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd4, (size_of_cmd4- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd4, (sizeof(cmd4)- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd3, (size_of_cmd3- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	//qm_uart_write_buffer(uart, cmd4, (size_of_cmd4- 1));     clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, body, (size_of_body- 1));     clk_sys_udelay(500000); // Delay 0.5 second

	memset(cmd0,0,sizeof(cmd0));
	memset(cmd1,0,sizeof(cmd1));
	memset(cmd2,0,sizeof(cmd2));
	memset(cmd3,0,sizeof(cmd3));
	memset(cmd4,0,sizeof(cmd4));
	memset(body,0,sizeof(body));
	memset(token,0,sizeof(token));

}

// Check 3G connectivity
void wifi_connect(const qm_uart_t uart)
{
	uint8_t uart_buffer=0;
	uint8_t uart_rx[16];
	memset(uart_rx,0,sizeof(uart_rx));

	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;

	// Clear buffer and flush all the data
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

	// Make sure it is connected
	while (1)
	{
        // Send +++ (Enter AT command mode)
		uint8_t at0[] = "AT\r\n";
		qm_uart_write_buffer(uart, at0, (sizeof(at0) - 1));

		// Total of 6 seconds delay with blinking LED
		for(int jj=0; jj<3; jj++)
		{
		  led_b_flash();             // Flash LED2 (RED)
		  clk_sys_udelay(1000000); 	// Create 1 sec delay
		}


//		// Send ATAI (Reads the Association status)
//		uint8_t at1[] = "ATAI\r\n";
//		qm_uart_write_buffer(uart, at1, (sizeof(at1) - 1));
//
//		// Total of 2 seconds delay with blinking LED
//		led_b_flash();               // Flash LED2 (RED)
//		clk_sys_udelay(1000000); 	// Create 1 sec delay

		qm_uart_get_status(uart, &uart0_status);
		clk_sys_udelay(10000); 	// Create 10 milisec delay

		if (uart0_status != QM_UART_IDLE)
		{
			for(int jj=0; jj<4; jj++)
			{
				qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
                uart_rx[jj] = uart_buffer;
                QM_PRINTF("dd: %d\n", uart_buffer);
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

}

// Receive data via 3G (HTTP GET) [DO NOT USE. NEED TO TEST]
uint8_t* wifi_recv(const qm_uart_t uart, uint8_t payload_data[])
{
    // Commands to GET
	uint8_t cmd0[] = "GET /get HTTP/1.1\r\n";
	uint8_t cmd1[] = "Host: httpbin.org\r\n\r\n";

	qm_uart_write_buffer(uart, cmd0, (sizeof(cmd0)- 1)); 	 clk_sys_udelay(500000); // Delay 0.5 second
	qm_uart_write_buffer(uart, cmd1, (sizeof(cmd1)- 1));     clk_sys_udelay(500000); // Delay 0.5 second

	// Get the HTTP response data
	uint8_t uart_buffer=0;
	uint8_t uart_rx[450];
	memset(uart_rx,0,sizeof(uart_rx));
	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;

	qm_uart_get_status(uart, &uart0_status);
	clk_sys_udelay(10000); 	// Create 10 milisec delay
	while (uart0_status == QM_UART_IDLE)
	{
		qm_uart_get_status(uart, &uart0_status);
		clk_sys_udelay(10000); 	// Create 10 milisec delay
	}

	for(int jj=0; jj<450; jj++)
	{
		qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
		uart_rx[jj] = uart_buffer;
	}

	// Find the payload
	for(int jj=0; jj<450; jj++)
	{
        if (uart_rx[jj  ] == 'o' && uart_rx[jj+1] == 'r' && uart_rx[jj+2] == 'i' && uart_rx[jj+3] == 'g' && \
        	uart_rx[jj+4] == 'i' && uart_rx[jj+5] == 'n' )
        {
        	clk_sys_udelay(10000); 	         // Create 10 milisec delay

        	uint8_t flag_exit = 0;

        	for (int ii=0; ii<30; ii++)
        	{
        	   if (uart_rx[jj+10+ii] != '"')
        	   {
                   payload_data[ii] = uart_rx[jj+10+ii];
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

	return payload_data;
}
