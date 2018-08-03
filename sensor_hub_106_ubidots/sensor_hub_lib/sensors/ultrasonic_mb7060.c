/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

ultrasonic_mb7060.c
Ultrasonic range sensor from Maxbotix

 Version   : 1.00
 Author    : Tan Han Wei

 History:
   1.00 HW 22-Oct-17 Initial
*/
#include <stdlib.h>
//#include "main.h"
#include "clk.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "qm_uart.h"
#include "ultrasonic_mb7060.h"
#include "qm_gpio.h"


char sensorstring;
int Range_val = 0;




int char2int_ultrasonic(char *array, size_t n)
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
void init_ultrasonic_control(qm_gpio_port_config_t *cfg)
{
	qm_pmux_select(QM_PIN_ID_61, QM_PIN_61_FN_GPIO_27);  // Init. 5V enable
	qm_pmux_select(QM_PIN_ID_62, QM_PIN_62_FN_GPIO_28);  // Init. 3.3V enable
	qm_pmux_select(QM_PIN_ID_55, QM_PIN_55_FN_GPIO_21);  // Init RS232 enable
	qm_pmux_select(QM_PIN_ID_33, QM_PIN_33_FN_GPIO_29);  // uart1 control

	cfg->direction =  BIT(21) |BIT(27) |BIT(28) | BIT(29);
	qm_gpio_set_config(QM_GPIO_0, cfg); // Configure


	return;

}

void ultrasonic_mb7060_init(qm_uart_config_t *cfg)
{
	qm_pmux_select(QM_PIN_ID_16, QM_PIN_16_FN_UART1_TXD);  // UART1 TXD
	qm_pmux_select(QM_PIN_ID_17, QM_PIN_17_FN_UART1_RXD);  // UART1 RXD
	qm_pmux_select(QM_PIN_ID_9, QM_PIN_9_FN_UART1_RTS);  // UART1 RTS
	qm_pmux_select(QM_PIN_ID_8, QM_PIN_8_FN_UART1_CTS);  // UART1 CTS
	qm_pmux_input_en(QM_PIN_ID_17, true);
	qm_pmux_input_en(QM_PIN_ID_9, true);

	cfg->baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 208, 5);// Configures UART_1 for 9600 bps
	cfg->line_control = QM_UART_LC_8N1;
	cfg->hw_fc = false;

	qm_uart_set_config(QM_UART_1, cfg);

	 // enable clock for UART_1
	clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTA_REGISTER);
	return;
}

int get_ultrasonic_mb7060()
{
	uint8_t uart_buffer;
	char range_read[4];
	int range_val_int;
	int uart1_read_done=0;
	int i,j;

	static qm_uart_status_t uart1_status __attribute__((unused)) = 0;

	qm_uart_get_status(QM_UART_1, &uart1_status);

	j=0;					//set counter to zero before start

	while(j <100)			//try 100 times to search for pattern "Rxxx"
	{
		j++;
		if (uart1_status != QM_UART_IDLE && uart1_read_done ==0)
		{
			qm_uart_read(QM_UART_1, &uart_buffer, &uart1_status);

			// wait for "R"
			if (uart_buffer == 0x52)
			{
				for (i=0;i<4;i++)			//after "R" are XXX
				{
					if (uart1_status != QM_UART_IDLE )		//when uart available to read
					{
						qm_uart_read(QM_UART_1, &uart_buffer,&uart1_status);
						range_read[i] = uart_buffer;

					}

				}

				if (range_read[3]== 0x0D)
				{
					//convert array to int
					range_val_int= atoi(range_read);

					//set a read done flag
					uart1_read_done = 1;
					clk_sys_udelay(100000);

					break;

				}

			}

		}

	}
	return range_val_int;

}




