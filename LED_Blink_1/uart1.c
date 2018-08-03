#include "qm_pinmux.h"
#include "qm_uart.h"
#include "qm_interrupt.h"
#include "qm_isr.h"
#include "clk.h"


void xyz()

{
	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;
	static qm_uart_status_t uart1_status __attribute__((unused)) = 0;

	QM_PRINTF("Starting UART.\r\n");
	qm_uart_config_t uart0_cfg;
	qm_uart_config_t uart1_cfg;

	uint8_t uart0_message[] = "This is UART_0. Characters typed here will appear on UART_1.\r\n";

	uint8_t uart1_message[] = "This is UART_1. Characters typed here will appear on UART_0.\r\n";

	uint8_t data;


	qm_pmux_select(QM_PIN_ID_19, QM_PMUX_FN_0);  /*UART0 TXD*/
	qm_pmux_select(QM_PIN_ID_18, QM_PMUX_FN_0);  /*UART0 RXD */
	qm_pmux_select(QM_PIN_ID_41, QM_PMUX_FN_1);  /*UART0 RTS*/
	qm_pmux_select(QM_PIN_ID_40, QM_PMUX_FN_1);  /*UART0 CTS*/
	qm_pmux_input_en(QM_PIN_ID_18, true);
	qm_pmux_input_en(QM_PIN_ID_41, true);


	qm_pmux_select(QM_PIN_ID_16, QM_PMUX_FN_2);  /*UART1 TXD*/
	qm_pmux_select(QM_PIN_ID_17, QM_PMUX_FN_2);  /*UART1 RXD*/
	qm_pmux_select(QM_PIN_ID_9, QM_PMUX_FN_2);   /*UART1 RTS*/
	qm_pmux_select(QM_PIN_ID_8, QM_PMUX_FN_2);   /*UART1 CTS*/
	qm_pmux_input_en(QM_PIN_ID_17, true);
	qm_pmux_input_en(QM_PIN_ID_9, true);


	/* Configures UART_0 for 9600 bps */
	uart0_cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 208, 5);
	//uart0_cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 17, 6);
	uart0_cfg.line_control = QM_UART_LC_8N1;
	uart0_cfg.hw_fc = false;


	/* Configures UART_1 for 115200 bps */

	/* uart1_cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 208, 5); */
	uart1_cfg.baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 17, 6);
	uart1_cfg.line_control = QM_UART_LC_8N1;
	uart1_cfg.hw_fc = false;


	qm_uart_set_config(QM_UART_0, &uart0_cfg);
	qm_uart_set_config(QM_UART_1, &uart1_cfg);

	/* enable clock for UART_0 */
	clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTA_REGISTER);

	/* enable clock for UART_1 */
	clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTB_REGISTER);


	/* this message will only be printed on the default UART */
	QM_PRINTF("Welcome to Intel Quark SE C1000 UART configuration demo.\r\n");

	qm_uart_write_buffer(QM_UART_0, uart0_message, sizeof(uart0_message));
	qm_uart_write_buffer(QM_UART_1, uart1_message, sizeof(uart1_message));



	while(1)
	{

	//send data UART1 starts
		uint8_t at1[] = "AT/r/n";
		qm_uart_write_buffer(QM_UART_0, at1, (sizeof(at1) - 1));
		clk_sys_udelay(3000000);
	//ends
	/* checks if received data is available on UART_0 */

	  qm_uart_get_status(QM_UART_0, &uart0_status);

	  if (uart0_status && QM_UART_RX_BUSY)
	  {
		  qm_uart_read_non_block(QM_UART_0, &data);
		  qm_uart_write(QM_UART_1, data);
	  }

	  /* checks if received data is available on UART_1 */
	  qm_uart_get_status(QM_UART_1, &uart1_status);

	  if (uart1_status && QM_UART_RX_BUSY)
	  {
		  qm_uart_read_non_block(QM_UART_1, &data);
		  qm_uart_write(QM_UART_0, data);
	  }

	}
	//return 0;

}

