/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 aqua_dissolved_oxygen.c
 This is the library file for DO sensor.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name : aqua_dissolved_oxygen.c

 History:
   1.00 BK 18-Aug-17 Initial
 */

#include <stdlib.h>
#include "clk.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "qm_uart.h"

char sensorstring;
int DO_val = 0;

// Concatenation of two values
unsigned concatenate(unsigned x, unsigned y) {
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;
}

// Convert char to int
int char2int(char *array, size_t n)
{
    int number = 0;
    int mult = 1;

    n = (int)n < 0 ? -n : n;       /* quick absolute value check  */

    /* for each character in array */
    while (n--)
    {
        /* if not digit or '-', check if number > 0, break or continue */
        if ((array[n] < '0' || array[n] > '9') && array[n] != '-') {
            if (number)
                break;
            else
                continue;
        }

        if (array[n] == '-') {      /* if '-' if number, negate, break */
            if (number) {
                number = -number;
                break;
            }
        }
        else {                      /* convert digit to numeric value   */
            number += (array[n] - '0') * mult;
            mult *= 10;
        }
    }

    return number;
}

// Initialise DO sensor
void do_kit103d_init(qm_uart_config_t *cfg){
	qm_pmux_select(QM_PIN_ID_16, QM_PIN_16_FN_UART1_TXD);  // UART1 TXD
	qm_pmux_select(QM_PIN_ID_17, QM_PIN_17_FN_UART1_RXD);  // UART1 RXD
	qm_pmux_select(QM_PIN_ID_9, QM_PIN_9_FN_UART1_RTS);  // UART1 RTS
	qm_pmux_select(QM_PIN_ID_8, QM_PIN_8_FN_UART1_CTS);  // UART1 CTS
	qm_pmux_input_en(QM_PIN_ID_17, true);
	qm_pmux_input_en(QM_PIN_ID_9, true);

	cfg->baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 208, 5);// Configures UART_0 for 9600 bps
	cfg->line_control = QM_UART_LC_8N1;
	cfg->hw_fc = false;

	qm_uart_set_config(QM_UART_1, cfg);

	    // enable clock for UART_1
	clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTA_REGISTER);
	return;
}

// Get DO value
int get_do_kit103d(){
	uint8_t uart_buffer;
	char buf[10];
	char DO_read[5];
	int i, j;
	int do_val = 0;
	char do_val_char[5];
	int do_val_int[5];

	//qm_uart_read_non_block(QM_UART_1, &uart_buffer);

	clk_sys_udelay(1000);

	qm_uart_read_non_block(QM_UART_1, &uart_buffer);
	clk_sys_udelay(2000);		//delay is critical for measuring this UART
	for (i = 0; i < 10; i++) {
		qm_uart_read_non_block(QM_UART_1, &uart_buffer);
		buf[i] = uart_buffer;
		//printf("%c",buf[i]);
		clk_sys_udelay(2000);		//delay is critical for measuring this UART

		DO_read[i] = buf[i];
		if (buf[i] == 13 && i >= 5) {//find "CR" and make sure more 5 characters
			i = i - 5;

			do_val_char[0] = DO_read[i];
			do_val_char[1] = DO_read[i + 1];
			do_val_char[2] = DO_read[i + 2];
			do_val_char[3] = DO_read[i + 3];
			do_val_char[4] = DO_read[i + 4];

			printf("%c", do_val_char[0]);
			printf("%c", do_val_char[1]);
			printf("%c", do_val_char[2]);
			printf("%c", do_val_char[3]);
			printf("%c\n", do_val_char[4]);

			for (j = 0; j < 5; j++) {
				if (j == 2) {

				} else {
					do_val_int[j] = char2int(&do_val_char[j], 1);

				}

			}
			do_val = (do_val_int[0] * 1000) + (do_val_int[1] * 100)
					+ (do_val_int[3] * 10) + do_val_int[4];

			//QM_PRINTF("do_val  %d\n",do_val);

		}

	}

	return do_val;

}



