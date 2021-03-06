/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 power.c
 This is the library file for power modes.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : power.c

 History:
   1.00 BK 18-Aug-17 Initial
 */

#include "../main.h"
#include "power_states.h"
#include "qm_common.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_rtc.h"
#include "qm_uart.h"
#include "soc_watch.h"
#include "qm_interrupt.h"
#include "qm_isr.h"

#define QM_AC_COMPARATORS_MASK (0x7FFFF)
#define RTC_SYNC_CLK_COUNT (5)

// Setup RTC alarm
void setup_rtc_alarm(void)
{
	qm_rtc_config_t rtc_cfg;
	uint32_t rtc_start;

	// Configure the RTC and request the IRQ.
	rtc_cfg.init_val = 0;
	rtc_cfg.alarm_en = true;
	rtc_cfg.alarm_val = SLEEP_TIME*QM_RTC_ALARM_SECOND(CLK_RTC_DIV_1);
	rtc_cfg.callback = NULL;
	rtc_cfg.callback_data = NULL;
	rtc_cfg.prescaler = CLK_RTC_DIV_1;

	qm_rtc_set_config(QM_RTC_0, &rtc_cfg);

	QM_IR_UNMASK_INT(QM_IRQ_RTC_0_INT);
	QM_IRQ_REQUEST(QM_IRQ_RTC_0_INT, qm_rtc_0_isr);

	// The RTC clock resides in a different clock domain to the
	// system clock. It takes 3-4 RTC ticks for a system clock
	// write to propagate to the RTC domain. If an entry to sleep
	// is initiated without waiting for the transaction to complete
	// the SOC will not wake from sleep.
	//
	rtc_start = QM_RTC[QM_RTC_0]->rtc_ccvr;
	while (QM_RTC[QM_RTC_0]->rtc_ccvr - rtc_start < RTC_SYNC_CLK_COUNT)
		;
}

// Deep sleep mode
static void app_deep_sleep()
{
	//qm_uart_context_t uart_ctx;
	//qm_uart_context_t uart1_ctx;
	//qm_rtc_context_t rtc_ctx;
	//qm_irq_context_t irq_ctx;

	// Setup RTC alarm
	setup_rtc_alarm();

	// Save the context
	//qm_uart_save_context(QM_UART_0, &uart1_ctx);
	//qm_uart_save_context(STDOUT_UART, &uart_ctx);
	//qm_rtc_save_context(QM_RTC_0, &rtc_ctx);
	//qm_irq_save_context(&irq_ctx);

	// Go to sleep, RTC will wake me up.
	//qm_power_soc_deep_sleep_restore();
	qm_power_soc_deep_sleep();

	// Restore the context
	//qm_irq_restore_context(&irq_ctx);
	//qm_rtc_restore_context(QM_RTC_0, &rtc_ctx);
	//qm_uart_restore_context(STDOUT_UART, &uart_ctx);
	//qm_uart_restore_context(QM_UART_0, &uart1_ctx);
}

// Enter low power mode
void enter_low_power(void)
{
	if(DEBUG_MODE)
	{
	QM_PUTS("Go to sleep");
	}

	clk_sys_udelay(1000000); // 1 sec delay
	app_deep_sleep();

	// soc_watch instrumentation to log wakeup.
	// soc_sleep entry is logged in qmsi driver.
	QM_PUTS("Wake up");
	SOC_WATCH_LOG_EVENT(SOCW_EVENT_INTERRUPT, QM_IRQ_RTC_0_INT_VECTOR);
}


