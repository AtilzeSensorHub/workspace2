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
#include "sensors/air_direction_degree.h"
#include "sensors/air_speed_1min.h"
#include "sensors/air_temperature.h"

//static qm_gpio_port_config_t cfg2;   // GPIO //comment by Azwan
//static qm_gpio_port_config_t cfg02;  // AON GPIO //comment by Azwan

void radio_send_data(uint16_t *send_data)
{
#if (SENSOR_TYPE == 2)
//	//============================================================================//
//    //                              SMART CITY0                                   //
//	//============================================================================//
//    // Battery level===============================================================
//    uint8_t battery_level[5];
//    conv_to_2B_hex(2*send_data[0], battery_level);
//
//    // Flash Red LED
//    led_r_flash();
//
//    // Send battery level data via LoRa
//    //QM_PRINTF("Send battery level message\n");
//    xbee_tx(QM_UART_0, 0, 1, 0, 3316, battery_level, (sizeof(battery_level)-1));
//
//    // Flash Red LED
//    led_r_flash();
//
//	//Debug mode
//	if (DEBUG_MODE)
//    {
//		QM_PRINTF("(1/2) Battery Level: %d\n",2 * send_data[0]);//azwan added
//		clk_sys_udelay(10000000);
//      led_r_flash();
//    }
//
//    // Create 62 sec delay for duty cycle limit
//    clk_sys_udelay(10000000);


//	  // Uart Data=======================================================================
//		int uartdata_val;
//		uint8_t uart_data[5];
//
//		uartdata_val = get_uartdata();    // EDITED!!!!!!!!!!!!!!!!!!!!!!!!
//		conv_to_2B_hex(uartdata_val, uart_data);
//		if (DEBUG_MODE)
//		{
//			clk_sys_udelay(10000000);
//			led_r_flash();
//		}
//		clk_sys_udelay(10000000);

    // Air Direction=======================================================================

		int air_direction_val;
        //uint8_t air_direction[5];
        QM_PUTS("check");

        //for (int g=0;g<3;g++)
       // {
        	air_direction_val = get_pm25_hpma115();    // EDITED!!!!!!!!!!!!!!!!!!!!!!!!
        	//air_d[g]=air_direction_val;
        	QM_PRINTF("gg: %d\n", air_direction_val);
     //   }
        //conv_to_2B_hex(send_data[4], pm25);
        //conv_to_2B_hex(air_direction_val, air_direction);

        // Flash Red LED
        //led_r_flash();

        // Send PM2.5 data via LoRa
        //QM_PRINTF("Send PM2.5 message\n");
        //xbee_tx(QM_UART_0, 0, 1, 0, 3402, pm25, (sizeof(pm25)-1));

        // Flash Red LED
        //led_r_flash();

//    	//Debug mode
//    	if (DEBUG_MODE)
//        {
//    		//QM_PRINTF("(2/2) Air Direction Degree: %d\n", air_direction_val);//azwan added
//    		clk_sys_udelay(10000000);
//          led_r_flash();
//        }

        // Create 62 sec delay for duty cycle limit
        //clk_sys_udelay(10000000);


        // Air Speed 1 Min==========================================================

//		int air_speed1min_val;
//		uint8_t air_speed1min[5];
//
//		  //for (int g=0;g<3;g++)
//		 // {
//			air_speed1min_val = get_airspeed1min();    // EDITED!!!!!!!!!!!!!!!!!!!!!!!!
//			//air_d[g]=air_direction_val;
//			//QM_PRINTF("Air Speed 1min (radio): %d\n", air_speed1min_val);
//	   //   }
//		  //conv_to_2B_hex(send_data[4], pm25);
//		  conv_to_2B_hex(air_speed1min_val, air_speed1min);
//
//		  // Flash Red LED
//		  led_r_flash();
//
//		  // Send PM2.5 data via LoRa
//		  //QM_PRINTF("Send PM2.5 message\n");
//		  //xbee_tx(QM_UART_0, 0, 1, 0, 3402, pm25, (sizeof(pm25)-1));
//
//		  // Flash Red LED
//		  led_r_flash();
//
//		//Debug mode
//		if (DEBUG_MODE)
//		  {
//			//QM_PRINTF("(2/2) Air Direction Degree: %d\n", air_direction_val);//azwan added
//			clk_sys_udelay(10000000);
//			led_r_flash();
//		  }
//
//		  // Create 62 sec delay for duty cycle limit
//		  clk_sys_udelay(10000000);



//              // Air Temperature==========================================================
//
//		int air_temperature_val;
//		uint8_t air_temperature[5];
//		air_temperature_val = get_airdirectiondegree();    // EDITED!!!!!!!!!!!!!!!!!!!!!!!!
//
//		conv_to_2B_hex(air_temperature_val, air_temperature);
//		//QM_PRINTF("Data: %d", air_temperature_val);
//
//		led_r_flash();
//
//		//QM_PRINTF("Send Air Temperature\n");
//		xbee_tx(QM_UART_0, 0, 1, 0, 3303, air_temperature, (sizeof(air_temperature)-1));
//
//		led_r_flash();
//
//		if (DEBUG_MODE)
//		{
//			clk_sys_udelay(10000000);
//			led_r_flash();
//		}
//
//		clk_sys_udelay(10000000);



//    // PM2.5=======================================================================
//	uint16_t pm25_val=0;
//    uint8_t pm25[5];
//    //pm25_val = get_pm25_hpma115();    // EDITED!!!!!!!!!!!!!!!!!!!!!!!!
//    //conv_to_2B_hex(send_data[4], pm25);
//    conv_to_2B_hex(pm25_val, pm25);
//
//    // Flash Red LED
//    led_r_flash();
//
//    // Send PM2.5 data via LoRa
//    //QM_PRINTF("Send PM2.5 message\n");
//    xbee_tx(QM_UART_0, 0, 1, 0, 3402, pm25, (sizeof(pm25)-1));
//
//    // Flash Red LED
//    led_r_flash();
//
//	//Debug mode
//	if (DEBUG_MODE)
//    {
//		QM_PRINTF("(1/2) PM2.5: %d\n",2 * send_data[1]);//azwan added
//		clk_sys_udelay(10000000);
//      led_r_flash();
//    }
//
//    // Create 62 sec delay for duty cycle limit
//    clk_sys_udelay(62000000);

#else
#endif
}
