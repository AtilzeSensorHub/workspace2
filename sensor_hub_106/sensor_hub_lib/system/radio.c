/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 radio.c
 This is the library file for radio.

 Version   : 1.00
 Author    : Khoo Boon Kien / Tan Han Wei
 File Name : radio.c

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 HW 23-Nov-17 Send battery level first
   1.02 HW 23-Nov-17 Added in debug message
 */

#include "sensors/pm25_hpma115.h"
#include "sensors/do_kit103d.h"
#include "sensors/ultrasonic_mb7060.h"
#include "sensors/temp_rh_sht10.h"
#include "sensors/floatswitch_lv600.h"
#include "sensors/ferti_pump_status.h"
#include "../main.h"
#include <stdint.h>
#include "system/utilities.h"
#include "system/xbee.h"
#include "wireless/lora.h"
#include "clk.h"
#include "system/gpio.h"

static qm_gpio_port_config_t cfg2;   // GPIO
static qm_gpio_port_config_t cfg02;  // AON GPIO

void radio_send_data(uint16_t *send_data)
{
#if (SENSOR_TYPE == 0)
	//============================================================================//
    //                              SMART FARMING0                                //
	//============================================================================//

	// Battery level===============================================================
	uint8_t battery_level[5];
	conv_to_2B_hex(2 * send_data[0], battery_level);

	// Send battery level data via LoRa
	//QM_PRINTF("Send battery level message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3316, battery_level, (sizeof(battery_level) - 1));

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(1/6) Battery Level: %d\n",2 * send_data[0]);
	}

	// Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);


    // Soil EC=====================================================================
    uint8_t soil_ec[5];
    conv_to_2B_hex(send_data[0], soil_ec);

    // Send soil EC data via LoRa
    //QM_PRINTF("Send soil EC message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3327, soil_ec, (sizeof(soil_ec) - 1));

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(2/6) Soil EC: %d\n", send_data[1]);
    }

	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);

	// Soil Moisture===============================================================
	uint8_t soil_moisture[5];
	conv_to_2B_hex(send_data[2], soil_moisture);

	// Send soil moisture data via LoRa
	//QM_PRINTF("Send soil moisture message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3401, soil_moisture, (sizeof(soil_moisture) - 1));

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
	    QM_PRINTF("(3/6) Soil moisture: %d\n", send_data[2]);
	}

	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);

	// Soil pH=====================================================================
	uint8_t soil_ph[5];
	conv_to_2B_hex(send_data[3], soil_ph);

	// Send soil pH data via LoRa
	//QM_PRINTF("Send soil pH message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3326, soil_ph, (sizeof(soil_ph) - 1));

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		 QM_PRINTF("(4/6) Soil pH: %d\n", send_data[3]);
	}

	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);


	// Temperature(sht10)=========================================================
	int temp_degree=0;
	uint8_t temp[5];

	temp_degree = get_sht10_temp();
	gpio_init(cfg2, cfg02);               // Initialise back the GPIO

	conv_to_2B_hex(temp_degree, temp);

	// Send temperature data via LoRa
	//QM_PRINTF("Send temperature message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3303, temp, (sizeof(temp)-1));

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		 QM_PRINTF("(5/6) Temperature: %d\n", temp_degree);
	}


	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);


	// Humidity (sht10)=========================================================

	int humidity_percent;
	uint8_t humidity[5];

	humidity_percent = get_sht10_rh_linear();
	gpio_init(cfg2, cfg02);               // Initialise back the GPIO

	conv_to_2B_hex(humidity_percent, humidity);

	// Send humidity data via LoRa
	//QM_PRINTF("Send humidity message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3304, humidity, (sizeof(humidity)-1));

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		 QM_PRINTF("(6/6) Humidity: %d\n\n", humidity_percent);
	}

	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);




#elif (SENSOR_TYPE == 1)

	//============================================================================//
    //                              SMART AQUACULTURE0                            //
	//============================================================================//

	// Battery level===============================================================
	uint8_t battery_level[5];
	conv_to_2B_hex(2 * send_data[3], battery_level);

	// Send battery level data via LoRa
	//QM_PRINTF("Send battery level message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3316, battery_level, (sizeof(battery_level) - 1));

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(1/4) Battery Level: %d\n",2 * send_data[0]);
	}

	// Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);

#elif (SENSOR_TYPE == 1)

	//============================================================================//
    //                              SMART AQUACULTURE0                            //
	//============================================================================//
    // DO==========================================================================
	uint16_t doxy_val;
    uint8_t doxy[5];
    doxy_val = get_dissolved_oxy();
    conv_to_2B_hex(doxy_val, doxy);

    // Send DO data via LoRa
    //QM_PRINTF("Send DO message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3403, doxy, (sizeof(doxy)-1));

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(2/4) Dissolved Oxy: %d\n",doxy_val);
	}

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);

    // Water level=================================================================
    uint8_t water_level[5];
    conv_to_2B_hex(send_data[1], water_level);

    // Send water level data via LoRa
    //QM_PRINTF("Send water level message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3319, water_level, (sizeof(water_level)-1));

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(3/4) Water level: %d\n",send_data[1]);
	}

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);

    // Temperature(PT1000)=========================================================
    uint8_t temp[5];
    conv_to_2B_hex(send_data[2], temp);

    // Send water level data via LoRa
    //QM_PRINTF("Send temperature message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3303, temp, (sizeof(temp)-1));

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(4/4) Temperature(PT1000): %d\n\n",send_data[2]);
	}

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);

#elif (SENSOR_TYPE == 2)
	//============================================================================//
    //                              SMART CITY0                                   //
	//============================================================================//
    // Battery level===============================================================
    uint8_t battery_level[5];
    conv_to_2B_hex(2*send_data[0], battery_level);

    // Flash Red LED
    led_r_flash();

    // Send battery level data via LoRa
    //QM_PRINTF("Send battery level message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3316, battery_level, (sizeof(battery_level)-1));

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
    {
      clk_sys_udelay(10000000);
      led_r_flash();
    }

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);

    // PM2.5=======================================================================
	uint16_t pm25_val=0;
    uint8_t pm25[5];
    //pm25_val = get_pm25_hpma115();    // EDITED!!!!!!!!!!!!!!!!!!!!!!!!
    //conv_to_2B_hex(send_data[4], pm25);
    conv_to_2B_hex(pm25_val, pm25);

    // Flash Red LED
    led_r_flash();

    // Send PM2.5 data via LoRa
    //QM_PRINTF("Send PM2.5 message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3402, pm25, (sizeof(pm25)-1));

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
    {
      clk_sys_udelay(10000000);
      led_r_flash();
    }

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);

#elif (SENSOR_TYPE == 3)
	//============================================================================//
    //                              SMART CITY1                                   //
	//============================================================================//
    // Battery level===============================================================
    uint8_t battery_level[5];
    conv_to_2B_hex(2*send_data[0], battery_level);

    // Send battery level data via LoRa
    //QM_PRINTF("Send battery level message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3316, battery_level, (sizeof(battery_level)-1));

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(1/2) Battery Level: %d\n",2 * send_data[0]);
	}

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);

    // Ultrasonic mb7060=======================================================================
	uint16_t ultrasonic_mb7060_val;
    uint8_t ultrasonic_mb7060[5];
    ultrasonic_mb7060_val = get_ultrasonic_mb7060();
    conv_to_2B_hex(ultrasonic_mb7060_val, ultrasonic_mb7060);

    // Send range data via LoRa
    //QM_PRINTF("Send ultrasonic message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3405, ultrasonic_mb7060, (sizeof(ultrasonic_mb7060)-1));

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(2/2) Ultrasonic mb7060: %d\n\n",ultrasonic_mb7060_val);
	}

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);

#elif (SENSOR_TYPE == 4)
	//============================================================================//
    //                              E_LADANG0                                //
	//============================================================================//

    // Battery level===============================================================
    uint8_t battery_level[5];
    conv_to_2B_hex(2*send_data[0], battery_level);

    // Send battery level data via LoRa
    //QM_PRINTF("Send battery level message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3316, battery_level, (sizeof(battery_level)-1));

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(1/8) Battery Level: %d\n",2 * send_data[0]);
	}

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);


    // Fertilizer EC=====================================================================
	uint8_t fertilizer_ec[5];
    conv_to_2B_hex(send_data[1], fertilizer_ec);

    // Send soil EC data via LoRa
    //QM_PRINTF("Send soil EC message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3327, fertilizer_ec, (sizeof(fertilizer_ec) - 1));

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(2/8) Fertilizer EC: %d\n", send_data[2]);
    }
	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);


    // Tank A Water Level=====================================================================
	uint8_t tank_A_waterlevel;
	uint8_t tank_A_level[5];

	tank_A_waterlevel = get_tank_A_level();

	conv_to_2B_hex(tank_A_waterlevel, tank_A_level);
    // Send soil EC data via LoRa
    //QM_PRINTF("Send soil EC message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3406, tank_A_level, (sizeof(tank_A_level) - 1));

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(3/8) Tank A Water Level: %d\n", tank_A_waterlevel);		// 0:No Water 1:Water
    }
	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);


    // Tank B Water Level=====================================================================
	uint8_t tank_B_waterlevel;
	uint8_t tank_B_level[5];

	tank_B_waterlevel = get_tank_B_level();

	conv_to_2B_hex(tank_B_waterlevel, tank_B_level);
    // Send soil EC data via LoRa
    //QM_PRINTF("Send soil EC message\n");
    xbee_tx(QM_UART_0, 0, 1, 1, 3406, tank_B_level, (sizeof(tank_B_level) - 1));

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(4/8) Tank B Water Level: %d\n", tank_B_waterlevel);		// 0:No Water 1:Water

    }
	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);



    // Pump A Status=====================================================================
	uint8_t pump_A_status;
	uint8_t pumpA_status[5];

	pump_A_status = get_fertipumpA_status();

	conv_to_2B_hex(pump_A_status, pumpA_status);

    xbee_tx(QM_UART_0, 0, 1, 0, 3407, pumpA_status, (sizeof(pumpA_status) - 1));

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(5/8) Pump A status: %d\n", pump_A_status);		// 0:OFF 1:ON
    }
	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);


    // Pump B Status=====================================================================
	uint8_t pump_B_status;
	uint8_t pumpB_status[5];

	pump_B_status = get_fertipumpB_status();

	conv_to_2B_hex(pump_B_status, pumpB_status);

    xbee_tx(QM_UART_0, 0, 1, 1, 3407, pumpB_status, (sizeof(pumpB_status) - 1));

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(6/8) Pump B Status: %d\n", pump_B_status);		// 0:OFF 1:ON
    }
	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);


	// Temperature(sht10)=========================================================

	int temp_degree=0;
	uint8_t temp[5];

	temp_degree = get_sht10_temp();
	gpio_init(cfg2, cfg02);               // Initialise back the GPIO

	conv_to_2B_hex(temp_degree, temp);

	// Send temperature data via LoRa
	//QM_PRINTF("Send temperature message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3303, temp, (sizeof(temp)-1));

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		 QM_PRINTF("(7/8) Temperature: %d\n", temp_degree);
	}


	// Create 62 sec delay for duty cycle limit
	clk_sys_udelay(62000000);


	// Humidity (sht10)=========================================================

	int humidity_percent;
	uint8_t humidity[5];

	humidity_percent = get_sht10_rh_linear();
	gpio_init(cfg2, cfg02);               // Initialise back the GPIO

	conv_to_2B_hex(humidity_percent, humidity);

	// Send humidity data via LoRa
	//QM_PRINTF("Send humidity message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3304, humidity, (sizeof(humidity)-1));

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		 QM_PRINTF("(8/8) Humidity: %d\n\n", humidity_percent);
	}

    // Create 62 sec delay for duty cycle limit
    clk_sys_udelay(62000000);


#else
#endif
}
