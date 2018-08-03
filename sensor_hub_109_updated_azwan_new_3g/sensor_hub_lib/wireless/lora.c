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
#include "string.h"

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

// Receive data thru LoRa (Class A)
uint8_t * lora_class_a_receive(const qm_uart_t uart, uint8_t data[])
{
	uint8_t uart_buffer=0;
	uint8_t uart_rx[1000];
	memset(uart_rx,0,sizeof(uart_rx));

	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;

	clk_sys_udelay(2000000); 	// Create 2 sec delay

	// Clear UART buffer
	int count = 0;
	for(int jj=0; jj<16; jj++)
	{
		qm_uart_get_status(uart, &uart0_status);
		if (uart0_status != QM_UART_IDLE)
		{
		    qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
		    uart_rx[count] = uart_buffer;
		    count++;
		}
		else
		{
			clk_sys_udelay(10000); 	// Create 10 us delay

		}
	}

	// Send the AT command via UART (AT+ECHO=1)
	uint8_t at_command[] = "AT+ECHO=1\r\n";
	qm_uart_write_buffer(uart, at_command, (sizeof(at_command)-1));

	clk_sys_udelay(1000000); 	// Create 10 us delay

	// Send the AT command via UART (AT+DTTX)
	uint8_t at_command0[] = "AT+DTX=4,1234\r\n";
	qm_uart_write_buffer(uart, at_command0, (sizeof(at_command0)-1));

	// Read UART strings
	count = 0;
	for(int jj=0; jj<5000; jj++)
	{
		qm_uart_get_status(uart, &uart0_status);
		if (uart0_status != QM_UART_IDLE)
		{
		    qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
		    uart_rx[count] = uart_buffer;
		    count++;
		}
		else
		{
			clk_sys_udelay(5000); 	// Create 10 us delay

		}
	}

	// Check if there is RX data
	uint8_t rx_done = 0;
	for(int jj=0; jj<1000; jj++)
	{
        if (uart_rx[jj  ] == 'R' && uart_rx[jj+1] == 'x' && uart_rx[jj+2] == ' ' && uart_rx[jj+3] == 'D' &&
        	uart_rx[jj+4] == 'o' && uart_rx[jj+5] == 'n' && uart_rx[jj+6] == 'e')
        {
            rx_done = 1;
            break;
        }
	}

	clk_sys_udelay(61000000); 	// Create 61 s delay
	// Send the AT command via UART (AT+DRX?)
	//uint8_t at_command1[] = "AT+DRX?\r\n";
	//qm_uart_write_buffer(uart, at_command1, (sizeof(at_command1)-1));

	if (rx_done == 1)
	{
		memset(uart_rx,0,sizeof(uart_rx));

		// Send the AT command via UART (AT+DRX?)
		uint8_t at_command1[] = "AT+DRX?\r\n";
		qm_uart_write_buffer(uart, at_command1, (sizeof(at_command1)-1));

		// Read UART strings
		count = 0;
		for(int jj=0; jj<1000; jj++)
		{
			qm_uart_get_status(uart, &uart0_status);
			if (uart0_status != QM_UART_IDLE)
			{
				qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
				uart_rx[count] = uart_buffer;
				count++;
			}
			else
			{
				clk_sys_udelay(1000); 	// Create 1 us delay

			}
		}

		// Check if there is RX data
		for(int jj=0; jj<1000; jj++)
		{
	        if (uart_rx[jj  ] == 'D' && uart_rx[jj+1] == 'R' && uart_rx[jj+2] == 'X' && uart_rx[jj+3] == ':')
	        {
	        	int data_len;
	        	if (uart_rx[jj+5]==',')
	        	{
		            int data_len_s;

		            data_len_s = uart_rx[jj+4];
	        	    data_len   = 2*(data_len_s - '0');
	        	}
	        	else
	        	{
                     int data_len_s[2];

                     data_len_s[0] = uart_rx[jj+4];
                     data_len_s[1] = uart_rx[jj+5];

                     data_len      = 2*((data_len_s[0]-'0')*10 + (data_len_s[1]-'0'));

	        	}
	        	//uint8_t data[data_len];
	        	//memset(data,0,sizeof(data));

                //data_size[0] = data_len;

	            // TODOLIST!!!!!!! Change to fit different length
	        	if (uart_rx[jj+5]==',')
	        	{
					for(int ii=0; ii<data_len; ii++)
					{
						data[ii] = uart_rx[jj+6+ii];
					}

	        	}
	        	else
	        	{
					for(int ii=0; ii<data_len; ii++)
					{
						data[ii] = uart_rx[jj+7+ii];
					}
	        	}

	        	data[data_len] = '\0';

	            break;
	        }
		}

	}

	return data;
}

// Receive data thru LoRa (Class C)
uint8_t * lora_class_c_receive(const qm_uart_t uart, uint8_t data[])
{
    uint8_t uart_buffer=0;
	uint8_t uart_rx[1000];
	memset(uart_rx,0,sizeof(uart_rx));

	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//            Clear UART buffer            //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	int count = 0;
	for(int jj=0; jj<16; jj++)
	{
		qm_uart_get_status(uart, &uart0_status);
		if (uart0_status != QM_UART_IDLE)
		{
			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
			uart_rx[count] = uart_buffer;
			count++;
		}
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//            Wait for RX data             //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	// Send the AT command via UART (AT+DTTX)
	uint8_t at_command0[] = "AT+DTX=4,1234\r\n";
	qm_uart_write_buffer(uart, at_command0, (sizeof(at_command0)-1));

	for(int jj=0; jj<60000; jj++)
	{
		qm_uart_get_status(uart, &uart0_status);
		if (uart0_status != QM_UART_IDLE)
		{
			qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
			uart_rx[count] = uart_buffer;
			count++;
		}
		clk_sys_udelay(1000); 	// Create 1us delay

	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//       Check if there is RX data         //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	uint8_t rx_done = 0;
	for(int jj=0; jj<1000; jj++)
	{
		if (uart_rx[jj  ] == 'R' && uart_rx[jj+1] == 'x' && uart_rx[jj+2] == ' ' && uart_rx[jj+3] == 'D' &&
			uart_rx[jj+4] == 'o' && uart_rx[jj+5] == 'n' && uart_rx[jj+6] == 'e')
		{
			rx_done = 1;
			break;
		}
	}


	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	//      Extract data if there is RX        //
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
	if (rx_done == 1)
	{
		memset(uart_rx,0,sizeof(uart_rx));

		// Send the AT command via UART (AT+DRX?)
		uint8_t at_command1[] = "AT+DRX?\r\n";
		qm_uart_write_buffer(uart, at_command1, (sizeof(at_command1)-1));

		// Read UART strings
		count = 0;
		for(int jj=0; jj<1000; jj++)
		{
			qm_uart_get_status(uart, &uart0_status);
			if (uart0_status != QM_UART_IDLE)
			{
				qm_uart_read(QM_UART_0, &uart_buffer, &uart0_status);
				uart_rx[count] = uart_buffer;
				count++;
			}
			else
			{
				clk_sys_udelay(1000); 	// Create 1 us delay

			}
		}

		// Check if there is RX data
		for(int jj=0; jj<1000; jj++)
		{
			if (uart_rx[jj  ] == 'D' && uart_rx[jj+1] == 'R' && uart_rx[jj+2] == 'X' && uart_rx[jj+3] == ':')
			{
				int data_len;
				if (uart_rx[jj+5]==',')
				{
					int data_len_s;

					data_len_s = uart_rx[jj+4];
					data_len   = 2*(data_len_s - '0');
				}
				else
				{
					 int data_len_s[2];

					 data_len_s[0] = uart_rx[jj+4];
					 data_len_s[1] = uart_rx[jj+5];

					 data_len      = 2*((data_len_s[0]-'0')*10 + (data_len_s[1]-'0'));

				}

				// TODOLIST!!!!!!! Change to fit different length
				if (uart_rx[jj+5]==',')
				{
					for(int ii=0; ii<data_len; ii++)
					{
						data[ii] = uart_rx[jj+6+ii];
					}

				}
				else
				{
					for(int ii=0; ii<data_len; ii++)
					{
						data[ii] = uart_rx[jj+7+ii];
					}
				}

				data[data_len] = '\0';

				break;
			}
		}

	}


    return data;
}

// Receive data thru LoRa
uint8_t * lora_receive(const qm_uart_t uart, uint8_t data[])
{
#if (LORA_CLASS == CLASS_A)
	// Class A
	lora_class_a_receive(uart, data);
#elif (LORA_CLASS == CLASS_B)
	// Class B
#elif (LORA_CLASS == CLASS_C)
    // Class C
	lora_class_c_receive(uart, data);
#endif

	return data;
}
