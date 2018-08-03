/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 pm25_hm32322550.c
 This is the library file for PM25 sensor.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name :  pm25_hm32322550.c

 History:
   1.00 HW 8-Nov-17 Initial
   1.01 HW 8-Nov-17 Replaced the pm25_uart to pm_25hpma115.c. No longer using non-blocking uart.
 */

#include "clk.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "qm_uart.h"
#include "pm25_hpma115.h"
#include "qm_gpio.h"

// Initalise PM25
void init_pm25_control(qm_gpio_port_config_t *cfg)
{
	qm_pmux_select(QM_PIN_ID_61, QM_PIN_61_FN_GPIO_27);  // Init. 5V enable
	qm_pmux_select(QM_PIN_ID_62, QM_PIN_62_FN_GPIO_28);  // Init. 3.3V enable
	qm_pmux_select(QM_PIN_ID_55, QM_PIN_55_FN_GPIO_21);  // Init RS232 enable
	qm_pmux_select(QM_PIN_ID_33, QM_PIN_33_FN_GPIO_29);  // uart1 control

	cfg->direction =  BIT(21) |BIT(27) |BIT(28) | BIT(29);
	qm_gpio_set_config(QM_GPIO_0, cfg); // Configure


	return;

}

// Initialise UART
void pm25_hpma115_init(qm_uart_config_t *cfg)
{
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

// Get PM25 data
int get_pm25_hpma115()
{
	uint8_t uart_buffer;
	char uart1_raw[32];
	int pm25_hpma115_int;
	int j;
	int k;

	static qm_uart_status_t uart1_status __attribute__((unused)) = 0;

	qm_uart_get_status(QM_UART_1, &uart1_status);

	j=0;					//set counter to zero before start

	while(j <100)			//try 100 times to search for pattern
	{
		j++;
		if (uart1_status != QM_UART_IDLE)// && uart1_read_done ==0)
		{
			qm_uart_read(QM_UART_1, &uart_buffer, &uart1_status);
			clk_sys_udelay(100);


				//search for "B"
				while(uart_buffer == 'B')   // TODO: Change it to if()
				{
					for (k=0;k<32;k++)
					{
						uart1_raw[0] = 0x42;
						qm_uart_read(QM_UART_1,&uart_buffer,&uart1_status);
						uart1_raw[k+1] = uart_buffer;
					}
					//exit while loop once read 32 characters
					break;
				}

/***********************************************************************************************************************************/
/* B   M  [00] [1C][00][00][00][05][00][06][00][00][00][00][00][00][00][00][00][00][00][00][00][00][00][00][00][00] Q [00] [01][07]*/
/***********************************************************************************************************************************/

			if(uart1_raw[0] == 'B' && uart1_raw[1] == 'M' && uart1_raw [28] == 'Q')
			{
				pm25_hpma115_int = uart1_raw [6] << 8 | uart1_raw [7];
				break;
			}

		}

	}
	return pm25_hpma115_int;

}
