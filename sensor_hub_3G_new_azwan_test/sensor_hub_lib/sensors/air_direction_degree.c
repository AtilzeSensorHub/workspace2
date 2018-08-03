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
void init_airdirectiondegree_control(qm_gpio_port_config_t *cfg)
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
void airdirectiondegree_init(qm_uart_config_t *cfg)
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

int get_airdirectiondegree()
//void get_airdirectiondegree(int airdirectiondegree_int[3])
{
	uint8_t uart_buffer;
	int x[3];
	//char uart1_raw[1];
	int airdirectiondegree_int;


	static qm_uart_status_t uart1_status __attribute__((unused)) = 0;

	qm_uart_get_status(QM_UART_1, &uart1_status);

	//int j=0;					//set counter to zero before start

	//for(int j=0;j<3;j++)			//try 100 times to search for pattern
	//{
		if (uart1_status != QM_UART_IDLE)// && uart1_read_done ==0)
		{
			qm_uart_read(QM_UART_1, &uart_buffer, &uart1_status);
			clk_sys_udelay(100);

			//airdirectiondegree_int=uart1_raw[1];
			//QM_PUTS("not idle \n");

				//search for "c"
				if(uart_buffer == 'c')
				{
					for (int k=0;k<3;k++)
					{
						//uart1_raw[0] = 0x73;
						qm_uart_read(QM_UART_1,&uart_buffer,&uart1_status);
						//uart1_raw[k] = uart_buffer;
						x[k]=uart_buffer;
						//QM_PRINTF("Value k: %d\n", k);
						//QM_PRINTF("Data_buffer (air direction): %d\n", uart_buffer);
					}
					//exit while loop once read 32 characters
					//break;
				}
		}

//		else
//		{
//			QM_PRINTF("idle");
//		}
	//}

	//QM_PRINTF("Data_raws: %d\n", uart1_raw[0]);

	airdirectiondegree_int=(x[0]*100);
	//QM_PRINTF("Data_x: %d\n", x[0]);
	QM_PRINTF("airdirectiondegree_int: %d\n", airdirectiondegree_int);

//	QM_PRINTF("Data_x: %d\n", x[1]);
//	QM_PRINTF("Data_x: %d\n", x[2]);

	//int res[3];

//	res[0]=x[0];
//	res[1]=x[1];
//	res[2]=x[2];

	//int haha [3]={x[0], x[1], x[2]};

	//QM_PRINTF("haha: %d\n", haha);
//	int w;
//
//	for (w = 0; w < 3; w++)
//	{
//		QM_PRINTF("%d ", res[w]);
//
//	}

	//int haha[3]={x[0], x[1], x[2]};
//	QM_PRINTF("haha: %d\n", haha);

	airdirectiondegree_int=x[0];

	return airdirectiondegree_int;

}
