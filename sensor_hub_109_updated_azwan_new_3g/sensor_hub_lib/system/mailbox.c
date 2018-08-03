/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 mailbox.c
 This is the library file for mailbox.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : mailbox.c

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 BK  9-Nov-17 Fixed bug, i.e. 6 mailbox data.
 */

//#include <sensor_hub_lib/sensors/do_kit103d.h>
#include "../main.h"
#include "qm_pinmux.h"
#include "qm_gpio.h"
#include "qm_pin_functions.h"
#include "clk.h"
#include "qm_uart.h"
#include "system/xbee.h"
#include "qm_mailbox.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "ss_init.h"
#include "qm_isr.h"

// Initialisations of Mailbox.
volatile bool cb_fired = false;
volatile int transfer_count = 0;
qm_mbox_msg_t tx_data, rx_data;
qm_mbox_ch_t mbox_pair[2] = {QM_MBOX_CH_0, QM_MBOX_CH_1};
qm_mbox_config_t mbox_rx_config;
qm_mbox_ch_t mbox_tx, mbox_rx;

// Mailbox callback on available data.
void mailbox_cb(void *callback_data)
{
	cb_fired = true;
}

// Request data from Sensor Sub-System.
int request_ss_data(uint32_t value)
{
	// Ctrl register and data
	tx_data.ctrl = value;
	tx_data.data[0] = value + 1;
	tx_data.data[1] = value + 2;
	tx_data.data[2] = value + 3;
	tx_data.data[3] = value + 4;

	// Send request
	if (0 != qm_mbox_ch_write(mbox_tx, &tx_data)) {
		QM_PRINTF("Error: mbox %d write\n", mbox_tx);
		return 1;
	}
	return 0;
}

// Read Mailbox data.
int read_mailbox_data(uint16_t *adc_data)
{
	// Read the mailbox.
	if (0 != qm_mbox_ch_read(mbox_rx, &rx_data))
	{
		QM_PRINTF("Error: Reading failed on mbox=%d, "
			  "ctrl=%d.\n",
			  mbox_rx, (int)rx_data.ctrl);
		return 1;
	}
	else
	{
		adc_data[0] = rx_data.data[0];
		adc_data[1] = rx_data.data[1];
		adc_data[2] = rx_data.data[2];
		adc_data[3] = rx_data.data[3];
		adc_data[4] = rx_data.data[4];
		adc_data[5] = rx_data.data[5];

	    ++(transfer_count);
	}
	return 0;
}

// Get the data from Sensor Sub-System.
int get_ss_data(uint16_t *adc_data)
{
	// Request data from sensor sub-system
	if (0 != request_ss_data(transfer_count)) {
		return 1;
	}

	while(1)
	{
		if (cb_fired)
		{
			cb_fired = false;

			if (read_mailbox_data(adc_data))
			{
				return 1;
			}

			clk_sys_udelay(5000000);

			break;
		}
	}
	return 0;
}

// Initialise mailbox.
void mailbox_init()
{
	//QM_PUTS("Starting: Mailbox");

	// Choose mailbox channels for RX and TX.
	// If one mailbox is chosen as a RX or TX on sensor, they must
	// be configured respectively as TX or RX on x86 core.
	mbox_rx = mbox_pair[1];
	mbox_tx = mbox_pair[0];
	//QM_PRINTF("Mailbox: TX channel %d -> RX channel %d\n", mbox_tx,
	//		mbox_rx);
	mbox_rx_config.dest = QM_MBOX_TO_LMT;
	mbox_rx_config.mode = QM_MBOX_INTERRUPT_MODE;
	mbox_rx_config.callback = mailbox_cb;
	mbox_rx_config.callback_data = NULL;

	// Register the interrupt handler.
	QM_IRQ_REQUEST(QM_IRQ_MAILBOX_0_INT, qm_mailbox_0_isr);

	// Configure RX channel.
	qm_mbox_ch_set_config(mbox_rx, &mbox_rx_config);

	// Start ARC.
	sensor_activation();

}

