/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 radio.c
 This is the library file for radio.

 Version   : 1.05
 Author    : Khoo Boon Kien / Tan Han Wei / Shahrul Azwan
 File Name : radio.c

 History:
   1.00 BK 18-Aug-17 Initial
   1.01 HW 23-Nov-17 Send battery level first
   1.02 HW 23-Nov-17 Added in debug message
   1.03 BK 08-May-18 Added downlink
   1.04 AZ 22-May-18 Call Pump A and Pump B downlink function during Tx with if else condition
   1.05 AZ 23-May-18 Add function to check irrigation pump status (pumpB) & add if else state in Xbee Mode sleep time
 */

#include "sensors/pm25_hpma115.h"
#include "sensors/do_kit103d.h"
#include "sensors/ultrasonic_mb7060.h"
#include "sensors/temp_rh_sht10.h"
#include "sensors/floatswitch_lv600.h"
#include "sensors/ferti_pump_status.h"
#include "sensors/irrigation_pump_status.h"
#include "shields/relayshield_v3.h"
#include "system/downlink_status.h"
#include "../main.h"
#include <stdint.h>
#include "system/utilities.h"
#include "system/xbee.h"
#include "wireless/lora.h"
#include "clk.h"
#include "system/gpio.h"
#include "string.h"

static qm_gpio_port_config_t cfg2;   // GPIO
static qm_gpio_port_config_t cfg02;  // AON GPIO

void radio_send_data(uint16_t *send_data, uint8_t token[1200])
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

	xbee_tx(QM_UART_0, 0, 1, 0, 3316, battery_level, (sizeof(battery_level) - 1), token);

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
    conv_to_2B_hex(send_data[1], soil_ec);

    // Send soil EC data via LoRa
    //QM_PRINTF("Send soil EC message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3327, soil_ec, (sizeof(soil_ec) - 1), token);

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
	xbee_tx(QM_UART_0, 0, 1, 0, 3401, soil_moisture, (sizeof(soil_moisture) - 1), token);

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
	xbee_tx(QM_UART_0, 0, 1, 0, 3326, soil_ph, (sizeof(soil_ph) - 1), token);

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
	xbee_tx(QM_UART_0, 0, 1, 0, 3303, temp, (sizeof(temp)-1), token);

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
	xbee_tx(QM_UART_0, 0, 1, 0, 3304, humidity, (sizeof(humidity)-1), token);

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
	conv_to_2B_hex(2 * send_data[0], battery_level);

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
		  QM_PRINTF("(1/2) Battery Level: %d\n",2 * send_data[0]);//azwan added
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
		  QM_PRINTF("(1/2) PM2.5: %d\n",2 * send_data[1]);//azwan added
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
    //                              E_LADANG0                                     //
	//============================================================================//
    // Battery level===============================================================

    //azwan added starts
    if(RELAY_SHIELD!=0)
    {
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB0[100];
    		uint8_t * downlink_pump_statusB0;
    		downlink_pump_statusB0=get_downlink_status_pumpB(statusB0, token);
    		QM_PRINTF("%s\n", downlink_pump_statusB0);
    		QM_PUTS("Downlink ends");
		}
    }
    //ends


	uint8_t battery_level[5];
    conv_to_2B_hex(2*send_data[0], battery_level);


    // Send battery level data via LoRa
    //QM_PRINTF("Send battery level message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3316, battery_level, (sizeof(battery_level)-1), token);

    // Flash Red LED
    led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		QM_PRINTF("(1/8) Battery Level: %d\n",2 * send_data[0]);
	}

    // Create 62 sec delay for duty cycle limit for lora. others 2 sec.
    if(XBEE_MODE!=0)
    {
    	clk_sys_udelay(2000000);
    }

    else
    	clk_sys_udelay(62000000);

//	//azwan added starts
    if(RELAY_SHIELD!=0)
	{
    	if(RELAY_PUMP_B!=0)
		{
			QM_PUTS("Downlink starts");
    		uint8_t statusB1[100];
			uint8_t * downlink_pump_statusB1;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB1=get_downlink_status_pumpB(statusB1, token);
			QM_PRINTF("%s\n", downlink_pump_statusB1);
			QM_PUTS("Downlink ends");
		}
	}
	//ends
//

    // Fertilizer EC=====================================================================
	//azwan added starts
    if(RELAY_SHIELD!=0)
    {
    	if(RELAY_PUMP_B!=0)
    	{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB2[100];
			uint8_t * downlink_pump_statusB2;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB2=get_downlink_status_pumpB(statusB2, token);
			QM_PRINTF("%s\n", downlink_pump_statusB2);
			QM_PUTS("Downlink ends");
    	}

    }
	//ends

	uint8_t fertilizer_ec[5];
    conv_to_2B_hex(send_data[1], fertilizer_ec);

    // Send soil EC data via LoRa
    //QM_PRINTF("Send soil EC message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3327, fertilizer_ec, (sizeof(fertilizer_ec) - 1), token);

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(2/8) Fertilizer EC: %d\n", send_data[1]);
    }

	if(XBEE_MODE!=0)
	{
		clk_sys_udelay(2000000);
	}

	else
		clk_sys_udelay(62000000);

	 //azwan added starts
    if(RELAY_SHIELD!=0)
    {
    	if(RELAY_PUMP_B!=0)
        {
    		QM_PUTS("Downlink starts");
    		uint8_t statusB3[100];
			uint8_t * downlink_pump_statusB3;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB3=get_downlink_status_pumpB(statusB3, token);
			QM_PRINTF("%s\n", downlink_pump_statusB3);
			QM_PUTS("Downlink ends");
        }
    }
	//ends


    // Tank A Water Level=====================================================================
	 //azwan added starts
    if(RELAY_SHIELD!=0)
    {
    	if(RELAY_PUMP_B!=0)
        {
    		QM_PUTS("Downlink starts");
    		uint8_t statusB4[100];
			uint8_t * downlink_pump_statusB4;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB4=get_downlink_status_pumpB(statusB4, token);
			QM_PRINTF("%s\n", downlink_pump_statusB4);
			QM_PUTS("Downlink ends");
        }
    }
	//ends

	uint8_t tank_A_waterlevel;
	uint8_t tank_A_level[5];

	tank_A_waterlevel = get_tank_A_level();

	conv_to_2B_hex(tank_A_waterlevel, tank_A_level);
    // Send soil EC data via LoRa
    //QM_PRINTF("Send soil EC message\n");
    xbee_tx(QM_UART_0, 0, 1, 0, 3406, tank_A_level, (sizeof(tank_A_level) - 1), token);

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(3/8) Tank A Water Level: %d\n", tank_A_waterlevel);		// 0:No Water 1:Water
    }

	// Create 62 sec delay for duty cycle limit for lora. others 2 sec.
    if(XBEE_MODE!=0)
	{
		clk_sys_udelay(2000000);
	}

	else
		clk_sys_udelay(62000000);

//    //azwan added starts
    if(RELAY_SHIELD!=0)
	{
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB5[100];
			uint8_t * downlink_pump_statusB5;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB5=get_downlink_status_pumpB(statusB5, token);
			QM_PRINTF("%s\n", downlink_pump_statusB5);
			QM_PUTS("Downlink ends");
		}

	}
   	//ends


    // Tank B Water Level=====================================================================
	//azwan added starts

    if(RELAY_SHIELD!=0)
	{
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB6[100];
			uint8_t * downlink_pump_statusB6;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB6=get_downlink_status_pumpB(statusB6, token);
			QM_PRINTF("%s\n", downlink_pump_statusB6);
			QM_PUTS("Downlink ends");
		}

	}
   	//ends


	uint8_t tank_B_waterlevel;
	uint8_t tank_B_level[5];

	tank_B_waterlevel = get_tank_B_level();

	conv_to_2B_hex(tank_B_waterlevel, tank_B_level);
    // Send soil EC data via LoRa
    //QM_PRINTF("Send soil EC message\n");
    xbee_tx(QM_UART_0, 0, 1, 1, 3406, tank_B_level, (sizeof(tank_B_level) - 1), token);

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(4/8) Tank B Water Level: %d\n", tank_B_waterlevel);		// 0:No Water 1:Water

    }
	// Create 62 sec delay for duty cycle limit for lora. others 2 sec.
    if(XBEE_MODE!=0)
	{
		clk_sys_udelay(2000000);
	}

	else
		clk_sys_udelay(62000000);

    //azwan added starts
    if(RELAY_SHIELD!=0)
	{
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB7[100];
			uint8_t * downlink_pump_statusB7;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB7=get_downlink_status_pumpB(statusB7, token);
			QM_PRINTF("%s\n", downlink_pump_statusB7);
			QM_PUTS("Downlink ends");
		}

	}
	//ends



    // Pump A Status=====================================================================
	//azwan added starts
    if(RELAY_SHIELD!=0)
	{
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB8[100];
			uint8_t * downlink_pump_statusB8;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB8=get_downlink_status_pumpB(statusB8, token);
			QM_PRINTF("%s\n", downlink_pump_statusB8);
			QM_PUTS("Downlink ends");
		}

	}
   	//ends

	uint8_t pump_A_status;
	uint8_t pumpA_status[5];

	pump_A_status = get_fertipumpA_status();

	conv_to_2B_hex(pump_A_status, pumpA_status);

    xbee_tx(QM_UART_0, 0, 1, 0, 3407, pumpA_status, (sizeof(pumpA_status) - 1), token);

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(5/8) Pump A status: %d\n", pump_A_status);		// 0:OFF 1:ON
    }
	// Create 62 sec delay for duty cycle limit for lora. others 2 sec.
    if(XBEE_MODE!=0)
	{
		clk_sys_udelay(2000000);
	}

	else
		clk_sys_udelay(62000000);

    //azwan added starts
    if(RELAY_SHIELD!=0)
	{
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB9[100];
			uint8_t * downlink_pump_statusB9;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB9=get_downlink_status_pumpB(statusB9, token);
			QM_PRINTF("%s\n", downlink_pump_statusB9);
			QM_PUTS("Downlink ends");
		}

	}
	//ends


    // Pump B Status=====================================================================
	//azwan added starts
    if(RELAY_SHIELD!=0)
	{
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB10[100];
			uint8_t * downlink_pump_statusB10;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB10=get_downlink_status_pumpB(statusB10, token);
			QM_PRINTF("%s\n", downlink_pump_statusB10);
			QM_PUTS("Downlink ends");
		}

	}
   	//ends

	uint8_t pump_B_status;
	uint8_t pumpB_status[5];

	pump_B_status = get_irrigationpumpB_status();

	conv_to_2B_hex(pump_B_status, pumpB_status);

    xbee_tx(QM_UART_0, 0, 1, 1, 3407, pumpB_status, (sizeof(pumpB_status) - 1), token);

    // Flash Red LED
    led_r_flash();

    //Debug mode
    if (DEBUG_MODE)
    {
    	QM_PRINTF("(6/8) Pump B Status: %d\n", pump_B_status);		// 0:OFF 1:ON
    }
	// Create 62 sec delay for duty cycle limit for lora. others 2 sec.
    if(XBEE_MODE!=0)
	{
		clk_sys_udelay(2000000);
	}

	else
		clk_sys_udelay(62000000);

    //azwan added starts
    if(RELAY_SHIELD!=0)
    {
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB11[100];
			uint8_t * downlink_pump_statusB11;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB11=get_downlink_status_pumpB(statusB11, token);
			QM_PRINTF("%s\n", downlink_pump_statusB11);
			QM_PUTS("Downlink ends");
		}

     }
	//ends


	// Temperature(sht10)=========================================================
	 //azwan added starts
    if(RELAY_SHIELD!=0)
	{
    	if(RELAY_PUMP_B!=0)
		{
    		QM_PUTS("Downlink starts");
    		uint8_t statusB12[100];
			uint8_t * downlink_pump_statusB12;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB12=get_downlink_status_pumpB(statusB12, token);
			QM_PRINTF("%s\n", downlink_pump_statusB12);
			QM_PUTS("Downlink ends");

		}

	}
   	//ends

	int temp_degree=0;
	uint8_t temp[5];

	temp_degree = get_sht10_temp();
	gpio_init(cfg2, cfg02);               // Initialise back the GPIO

	conv_to_2B_hex(temp_degree, temp);

	// Send temperature data via LoRa
	//QM_PRINTF("Send temperature message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3303, temp, (sizeof(temp)-1), token);

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		 QM_PRINTF("(7/8) Temperature: %d\n", temp_degree);
	}


	// Create 62 sec delay for duty cycle limit for lora. others 2 sec.
	if(XBEE_MODE!=0)
	{
		clk_sys_udelay(2000000);
	}

	else
		clk_sys_udelay(62000000);

	//azwan added starts
	if(RELAY_SHIELD!=0)
	{
		if(RELAY_PUMP_B!=0)
		{
			QM_PUTS("Downlink starts");
			uint8_t statusB13[100];
			uint8_t * downlink_pump_statusB13;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB13=get_downlink_status_pumpB(statusB13, token);
			QM_PRINTF("%s\n", downlink_pump_statusB13);
			QM_PUTS("Downlink ends");
		}
	}

	//ends

	// Humidity (sht10)=========================================================

	 //azwan added starts
	if(RELAY_SHIELD!=0)
	{
		if(RELAY_PUMP_B!=0)
		{
			QM_PUTS("Downlink starts");
			uint8_t statusB14[100];
			uint8_t * downlink_pump_statusB14;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB14=get_downlink_status_pumpB(statusB14, token);
			QM_PRINTF("%s\n", downlink_pump_statusB14);
			QM_PUTS("Downlink ends");
		}

	}
	//ends

	int humidity_percent;
	uint8_t humidity[5];

	humidity_percent = get_sht10_rh_linear();
	gpio_init(cfg2, cfg02);               // Initialise back the GPIO

	conv_to_2B_hex(humidity_percent, humidity);

	// Send humidity data via LoRa
	//QM_PRINTF("Send humidity message\n");
	xbee_tx(QM_UART_0, 0, 1, 0, 3304, humidity, (sizeof(humidity)-1), token);

	// Flash Red LED
	led_r_flash();

	//Debug mode
	if (DEBUG_MODE)
	{
		 QM_PRINTF("(8/8) Humidity: %d\n\n", humidity_percent);
	}

    if(XBEE_MODE!=0)
	{
		clk_sys_udelay(2000000);
	}

	else
		clk_sys_udelay(62000000);

	 //azwan added starts
	if(RELAY_SHIELD!=0)
	{
		if(RELAY_PUMP_B!=0)
		{
			QM_PUTS("Downlink starts");
			uint8_t statusB15[100];
			uint8_t * downlink_pump_statusB15;
			QM_PRINTF("Downlink token: %s\n",token);
			downlink_pump_statusB15=get_downlink_status_pumpB(statusB15,token);
			QM_PRINTF("%s\n", downlink_pump_statusB15);
			QM_PUTS("Downlink ends");

		}

	}
	//ends



#else
#endif
}

uint8_t * radio_receive_data(uint8_t status[], uint8_t token[1200])
{

#if (SENSOR_TYPE == 4)
	//============================================================================//
    //                              E_LADANG0                                     //
	//============================================================================//

    // Initialisation for Pumps=====================================================
	uint8_t channel_pump_a[] = "00";
	uint8_t channel_pump_b[] = "01";

    // Initialisation for DL Data===================================================
	uint8_t downlink_data_a[100];
	uint8_t downlink_data_b[100];
	memset(downlink_data_a, 0, sizeof(downlink_data_a));
	memset(downlink_data_b, 0, sizeof(downlink_data_b));

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
		}
		else
		{
			//led_r_off();
		}

		// Update Pump A status
		clk_sys_udelay(5000000); // Delay 5 seconds
		xbee_update_dl_status(QM_UART_0, channel_pump_a, pump_a_cmd, token);
	}

	// Pump B=======================================================================
	clk_sys_udelay(5000000); // Delay 5 seconds
	xbee_rx(QM_UART_0, downlink_data_b, channel_pump_b, token);

	// Downlink data for Pump B
	uint8_t pump_b_cmd[] = "00";
	pump_b_cmd[0] = downlink_data_b[14];
	pump_b_cmd[1] = downlink_data_b[15];

	if (pump_b_cmd[1] != status[1])
	{
		status[1] = pump_b_cmd[1];

		// Take action
		if (downlink_data_b[15]=='1')
		{
			on_relay1();
		}
		else
		{
			off_relay1();
		}

		// Update Pump B status
		clk_sys_udelay(5000000); // Delay 5 seconds
		xbee_update_dl_status(QM_UART_0, channel_pump_b, pump_b_cmd, token);
	}

	clk_sys_udelay(10000000); // Delay 10 seconds
#endif
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


