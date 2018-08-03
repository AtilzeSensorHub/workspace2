/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 Atilze Sensor Hub
 This is the main file for Atilze Sensor Hub.

 Version   : 1.06
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : main.c

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 BK 02-Nov-17 Reorganise the entire structure
   1.02 BK 13-Nov-17 Rename the sensors
   1.03 HW 22-Nov-17 Added in battery monitoring "if statement". If below 3.2v, go to sleep.
   1.04 HW 16-Dec-17 To adapt Rev2 PCB. Changes on temp_rh_sht10.c. Added arduino enable pin 22
   1.05 HW 31-Dec-17 Added re-initialize gpio_init after sht10. Created e-ladang0,float switches,
   	   	   	   	   	 measure input solar voltage, reset charging timer, pump A&B status input.
   1.06 BK 09-Jan-18 Added 3G library.
   1.07 BK 02-Mar-18 Added authentication for 3G HTTP request/response.
 */

#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_mailbox.h"
#include "sensor_hub_lib/sensors/battery_level.h"
#include "sensor_hub_lib/system/mailbox.h"
#include "sensor_hub_lib/system/gpio.h"
#include "sensor_hub_lib/sensors/water_level_hpt604.h"
#include "sensor_hub_lib/sensors/soil_ec_ae0001.h"
#include "sensor_hub_lib/sensors/soil_moisture_ae0001.h"
#include "sensor_hub_lib/sensors/soil_ph_sen0169.h"
#include "sensor_hub_lib/sensors/temp_atlas_pt1000.h"

volatile bool cb_fired = false;

// Mailbox callback on available data.
void mailbox_cb(void *callback_data)
{
	cb_fired = true;
}

void main(void)
{
	//==============================//
	//        Mailbox Init.         //
	//==============================//
	// Initialise mailbox.
	qm_mbox_ch_t     mbox_tx, mbox_rx;
	qm_mbox_config_t mbox_rx_config;
	qm_mbox_ch_t     mbox_pair[2] = {QM_MBOX_CH_0, QM_MBOX_CH_1};

	// Choose mailbox channels for RX and TX.
	mbox_rx = mbox_pair[0];
	mbox_tx = mbox_pair[1];

	mbox_rx_config.dest = QM_MBOX_TO_SS;
	mbox_rx_config.mode = QM_MBOX_INTERRUPT_MODE;
	mbox_rx_config.callback = mailbox_cb;
	mbox_rx_config.callback_data = NULL;

	// Register the interrupt handler.
	QM_IRQ_REQUEST(QM_IRQ_MAILBOX_0_INT, qm_mailbox_0_isr);

	// Configure RX channel.
	qm_mbox_ch_set_config(mbox_rx, &mbox_rx_config);

	gpio_init();

	//==============================//
	//          Main Loop           //
	//==============================//
	while (1)
	{
		// Wait for request from Quark
		if (cb_fired)
		{
			cb_fired = false;

			send_quark_data(mbox_rx, mbox_tx);

			//trigger charge enable to reset charging timer
			charge_en();

		}


	}

}
