/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 radio.c
 This is the library file for downlink_status.

 Version   : 1.01
 Author    : Shahrul Azwan
 File Name : downlink_status.c

 History:
   1.00 AZ 21-May-18 Initial
   1.01 AZ 22-May-18 Separate Pump A and Pump B to two different class. adjust delay to increase respond time.
 */

#include "sensors/pm25_hpma115.h"
#include "sensors/do_kit103d.h"
#include "sensors/ultrasonic_mb7060.h"
#include "sensors/temp_rh_sht10.h"
#include "sensors/floatswitch_lv600.h"
#include "sensors/ferti_pump_status.h"
#include "shields/relayshield_v3.h"
#include "../main.h"
#include <stdint.h>
#include "system/utilities.h"
#include "system/xbee.h"
#include "wireless/lora.h"
#include "clk.h"
#include "system/gpio.h"
#include "string.h"
#include "wireless/cellular_3g.h"


uint8_t * get_downlink_status_pumpA(uint8_t status[], uint8_t token[1200])
{

	//QM_PRINTF("%d", token);
	// Initialisation for Pumps=====================================================
	uint8_t channel_pump_a[] = "00";

    // Initialisation for DL Data===================================================
	uint8_t downlink_data_a[100];
	memset(downlink_data_a, 0, sizeof(downlink_data_a));

	// Pump A=======================================================================
	clk_sys_udelay(5000000); // Delay 5 seconds
	xbee_rx(QM_UART_0, downlink_data_a, channel_pump_a, token);


	// Downlink data for Pump A
	uint8_t pump_a_cmd[] = "00";
	pump_a_cmd[0] = downlink_data_a[14];
	pump_a_cmd[1] = downlink_data_a[15];

	if (pump_a_cmd[1] != status[0])
	{
		status[0] = pump_a_cmd[1];

		// Take action
		if (downlink_data_a[15]=='1')
		{
			//led_r_on();
			if (RELAY_PUMP_A==1)
			{
				on_relay1();
			}

			else if (RELAY_PUMP_A==3)
			{
				on_relay3();
			}

			else if (RELAY_PUMP_A==4)
			{
				on_relay4();
			}

		}
		else
		{
			//led_r_off();
			if (RELAY_PUMP_A==1)
			{
				off_relay1();
			}

			else if (RELAY_PUMP_A==3)
			{
				off_relay3();
			}

			else if (RELAY_PUMP_A==4)
			{
				off_relay4();
			}
		}

		// Update Pump A status
		clk_sys_udelay(5000000); // Delay 5 seconds
		xbee_update_dl_status(QM_UART_0, channel_pump_a, pump_a_cmd, token);
	}
	return status;
}

uint8_t * get_downlink_status_pumpB(uint8_t status[], uint8_t token[1200])
{
	uint8_t channel_pump_b[] = "01";

	// Initialisation for DL Data===================================================
	uint8_t downlink_data_b[100];
	memset(downlink_data_b, 0, sizeof(downlink_data_b));

	// Pump B=======================================================================
	clk_sys_udelay(2000000); // Delay 5 seconds (azwan changed to 2sec. default 5sec)
	xbee_rx(QM_UART_0, downlink_data_b, channel_pump_b, token);
	//QM_PUTS("xbee_rx");

	// Downlink data for Pump B
	uint8_t pump_b_cmd[] = "00";
	pump_b_cmd[0] = downlink_data_b[14];
	pump_b_cmd[1] = downlink_data_b[15];

	if (pump_b_cmd[1] != status[1])
	{
		status[1] = pump_b_cmd[1];
		//QM_PRINTF("%s\n", downlink_data_b);

		// Take action
		if (downlink_data_b[15]=='1')
		{
			//led_b_on();
			if (RELAY_PUMP_B==1)
			{
				on_relay1();
			}

			else if (RELAY_PUMP_B==3)
			{
				on_relay3();
			}

			else if (RELAY_PUMP_B==4)
			{
				on_relay4();
			}
		}
		else
		{
			//led_b_off();
			if (RELAY_PUMP_B==1)
			{
				off_relay1();
			}

			else if (RELAY_PUMP_B==3)
			{
				off_relay3();
			}

			else if (RELAY_PUMP_B==4)
			{
				off_relay4();
			}
		}

		// Update Pump B status
		clk_sys_udelay(5000000); // Delay 5 seconds (azwan changed to 2sec. default 5sec)
		xbee_update_dl_status(QM_UART_0, channel_pump_b, pump_b_cmd, token);
		//QM_PUTS("xbee_update_dl_status");
	}

	//QM_PUTS("!=");
	clk_sys_udelay(10000000); // Delay 10 seconds (azwan changed to 10sec.default 10sec)

	// For LoRa Under Testing (Pls Do Not Del)
	/*
	//uint8_t downlink_data[100];
	//memset(downlink_data,0,sizeof(downlink_data));

	for (int ii=0; ii<RX_DURATION_TIME; ii++)
	{
		xbee_rx(QM_UART_0, downlink_data);
	}
	*/
return status;
}
