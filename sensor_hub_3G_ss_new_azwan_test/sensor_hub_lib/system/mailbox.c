/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 mailbox.c
 Mailbox library file.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : mailbox.c

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 HW 22-Nov-17 Change all battery level arrary to [0]

 */

#include "clk.h"
#include "get_ticks.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_mailbox.h"
#include "ss_init.h"
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include "../main.h"
#include "qm_common.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "qm_ss_adc.h"
#include "qm_ss_interrupt.h"
#include "qm_ss_isr.h"
#include "qm_ss_gpio.h"
#include "qm_uart.h"
#include "ss_clk.h"
#include "ring_buffer.h"
#include "sensors/battery_level.h"
#include "sensors/water_level_hpt604.h"
#include "sensors/soil_ec_ae0001.h"
#include "sensors/soil_moisture_ae0001.h"
#include "sensors/temp_atlas_pt1000.h"
#include "sensors/soil_ph_sen0169.h"
#include "sensors/420ma_sensor.h"
#include "sensors/vin_level.h"

volatile int transfer_count = 0;
qm_mbox_msg_t tx_data, rx_data;
qm_mbox_ch_t mbox_tx, mbox_rx;

// Check received data and send a new message.
int send_quark_data(qm_mbox_ch_t mbox_rx, qm_mbox_ch_t mbox_tx)
{
	int soil_ec_val;
	int soil_moisture_val;
	int soil_ph_val;
	int battery_level_val;
	int aqua_water_level;
	int temp_pt1000_val;
	int fertilizer_ec_val;
//	int vin_level_val;

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
		if (SENSOR_TYPE == 0)
		{
			//==============================//
			//          Smart Agri. 0       //
			//==============================//
			// Get the sensor data
			soil_ec_val       = get_soil_ec_ae0001();
			soil_moisture_val = get_soil_moisture_ae0001();
			soil_ph_val       = get_soil_ph_sen0169();
			battery_level_val = get_battery_level();

			// Assign sensor data to mailbox packet
			tx_data.ctrl = 0;
			tx_data.data[0] = battery_level_val;
			tx_data.data[1] = soil_ec_val;
			tx_data.data[2] = soil_moisture_val;
			tx_data.data[3] = soil_ph_val;

		}
		else if (SENSOR_TYPE == 1)
		{
			//==============================//
			//          Smart Aqua. 0       //
			//==============================//
			// Get the sensor data
			temp_pt1000_val   = get_temp_atlas_pt1000();
			battery_level_val = get_battery_level();
			aqua_water_level  = get_water_level_hpt604();

			// Assign sensor data to mailbox packet
			tx_data.ctrl = 0;
			tx_data.data[0] = battery_level_val;
			tx_data.data[1] = aqua_water_level;
			tx_data.data[2] = temp_pt1000_val;

		}
		else if (SENSOR_TYPE == 2)
		{
			//==============================//
			//          Smart City 0        //
			//==============================//
			// Get the sensor data
			battery_level_val = get_battery_level();

			// Assign sensor data to mailbox packet
			tx_data.ctrl = 0;
			tx_data.data[0] = battery_level_val;
		}
		else if (SENSOR_TYPE == 3)
		{
			//==============================//
			//          Smart City 1        //
			//==============================//
			// Get the sensor data
			battery_level_val = get_battery_level();

			// Assign sensor data to mailbox packet
			tx_data.ctrl = 0;
			tx_data.data[0] = battery_level_val;
		}


		else if (SENSOR_TYPE == 4)
		{
			//==============================//
			//          E Ladang 0        //
			//==============================//

			// Get the sensor data
			battery_level_val = get_battery_level();
//			vin_level_val = get_vin_level();
			fertilizer_ec_val = get_420ma_sensor();

			// Assign sensor data to mailbox packet
			tx_data.ctrl = 0;
			tx_data.data[0] = battery_level_val;
//			tx_data.data[1] = vin_level_val;
			tx_data.data[1] = fertilizer_ec_val;

		}

		// Send data to Quark
		if (0 != qm_mbox_ch_write(mbox_tx, &tx_data))
		{
			QM_PRINTF("Error: mbox %d write\n", mbox_tx);
			return 1;
		}
	}

	return 0;
}

