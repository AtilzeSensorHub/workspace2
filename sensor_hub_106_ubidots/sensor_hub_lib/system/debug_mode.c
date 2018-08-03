/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 debug_mode.c
 This is the library file for PM25 sensor.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name : debug_mode.c

 History:
   1.00 HW 22-Nov-17 Initial

 */

#include "clk.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "qm_uart.h"
#include "debug_mode.h"
#include "qm_gpio.h"

void debug_init(qm_uart_config_t *cfg)
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
