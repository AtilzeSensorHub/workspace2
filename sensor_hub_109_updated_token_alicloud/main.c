/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 Atilze Sensor Hub
 This is the main file for Atilze Sensor Hub.

 Version   : 1.09
 Author    : Khoo Boon Kien / Tan Han Wei / Shahrul Azwan
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
   1.08 BK 08-May-18 Added 3G downlink and relay shield (by Azwan).
   1.09 AZ 22-May-18 Disable RX mode. After completes TX, it will proceed with TX again.
 */

#include "main.h"
#include "clk.h"
#include "qm_uart.h"
#include <x86intrin.h>
#include "sensor_hub_lib/system/radio.h"
#include "sensor_hub_lib/system/mailbox.h"
#include "sensor_hub_lib/system/xbee.h"
#include "sensor_hub_lib/system/sensor.h"
#include "sensor_hub_lib/system/gpio.h"
#include "sensor_hub_lib/system/power.h"
#include "sensor_hub_lib/system/debug_mode.h"
#include "string.h"
#include "clk.h"
#include "sensor_hub_lib/shields/relayshield_v3.h"
#include "sensor_hub_lib/wireless/cellular_3g.h"
#include "shields/relayshield_v3.h"
#include "system/downlink_status.h"

int main(void)
{
	//==============================//
	//       Initialisations        //
	//==============================//
    // Initialisations
	static qm_uart_status_t uart0_status __attribute__((unused)) = 0;
	static qm_uart_status_t uart1_status __attribute__((unused)) = 0;

	qm_uart_config_t uart0_cfg;         // UART0
	qm_uart_config_t uart1_cfg;         // UART1
	static qm_gpio_port_config_t cfg;   // GPIO
	static qm_gpio_port_config_t cfg0;  // AON GPIO

	sensors_init(uart1_cfg);            // Initialise UART1 (for sensor)
	gpio_init(cfg, cfg0);               // Initialise GPIO
	mailbox_init();                     // Initialise Mailbox
	xbee_init(uart0_cfg);               // Initialise UART0 (for XBee)

	//uint8_t loop=1;


	// Debug mode
	if (DEBUG_MODE)
	{
		debug_init(&uart1_cfg);			// Initialize Debug Mode
		if (SENSOR_TYPE == 0)
		{
			QM_PUTS("Smart Agri0 Debug Mode");
		}

		else if (SENSOR_TYPE == 1)
		{
			QM_PUTS("Smart Aqua0 Debug Mode");
		}

		else if (SENSOR_TYPE == 2)
		{
			QM_PUTS("Smart City0 Debug Mode");
		}

		else if (SENSOR_TYPE == 3)
		{
			QM_PUTS("Smart City1 Debug Mode");
		}

		else if (SENSOR_TYPE == 4)
		{
			QM_PUTS("ELadang0 Debug Mode");
			//on_relay1();
			//on_relay4();
		}
	}

	// Initialise adc_data
	uint16_t adc_data[6];

	// Initial state
    uint8_t state = STATE_INIT;

    // Wait state time
    uint32_t rx_timeout_s = RX_DURATION_TIME;
    unsigned long long tsc_start;
    unsigned long long tsc_current_s;

    // Rx status (8 bits to represent 8 channels)
    uint8_t rx_status[] = "00000000";

    // Token
    //uint8_t token[1200]="eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6Ijg2NjZkOWFiMTllMTI3NmZlNzZkMDlkOTU3MjJmNjAzYmE3ZTlkNTY3NTAzM2Y3YjYzZjNkMjdkNjkwZjllZWIxODIyNTY1ZjYyNzkxMGQxIn0.eyJhdWQiOiI2IiwianRpIjoiODY2NmQ5YWIxOWUxMjc2ZmU3NmQwOWQ5NTcyMmY2MDNiYTdlOWQ1Njc1MDMzZjdiNjNmM2QyN2Q2OTBmOWVlYjE4MjI1NjVmNjI3OTEwZDEiLCJpYXQiOjE1Mjk5MDgxNDksIm5iZiI6MTUyOTkwODE0OSwiZXhwIjoxNTYxNDQ0MTQ5LCJzdWIiOiIxNCIsInNjb3BlcyI6W119.G6SZji8zeLXPvoyxuS2KpcYnUHw3tlmZpe__KPuUSaPdFwzDR6in1Zw_gCZ0uiCX9oyHlIM3JwFpxn0hyhu2LXMUpKNBrjiF3_RrqmvbLn9-liDu9Ow03oU82x5wbaVFf3z0--PZaJ7y02pfPPFQmuG8aWFI8JrhSBva69IxjXUWgCdfBSVuSGGCLYpki483IpO_RnWOjLyIADPpp1Wdy4jQTdjjqc1WPtoRm0LexTbRtyLGY-X4ALS345s3aF-RetrAlEk5ZylU4fZS1eVEj-HEfNco2iBsX2krCeBQe7YtmX73LTYxAoREOAYFDi0X_HEdtNGhBFjqGbydpLfyMAccHGqx9ijFtYuRadayX3v2NkAW7hsDkU6n3bUtkp_DvpfHWwz1E9PSj4h_yxr4_Z-4QFmLbaSZGGWyEBJ_Wcwolr6GxYzq7fQiufozaInrj0D0fvQ3gQCwY6JJZKNkzEIwVBAVFINkLLr3T5iyESF5knTnWhtqbtYM2XXy8YmaryEIRzLctXNeQ5xDVFn1OG98326XWlcX9PkfJoH8OE_3wC9VWdnaEzB0hjC6NtozQwgm8BJbunYmYxvy1C3_VI8lXb96r17Hvz1A4lcSYIp8L4ha154JoU9wHk8pXNeDRxT1bnhXgh2WW9BiGbzCV-MxYiCaTcHaUoqs1rP0j04";
    uint8_t token[1200];
    uint8_t old_token[1200];
    int flag;
	//==============================//
	//          Main Loop           //
	//==============================//
	while (1)
	{

		switch (state) {

		//==============================//
		//        Initial State         //
		//==============================//
		case STATE_INIT:

//			// Enable Power (azwan added)
//			qm_gpio_set_pin(QM_GPIO_0, 20);  // Enable XBee
//			qm_gpio_set_pin(QM_GPIO_0, 24);  // Enable ADC2
//			qm_gpio_set_pin(QM_GPIO_0, 25);  // Enable ADC1
//			qm_gpio_set_pin(QM_GPIO_0, 26);  // Enable ADC0
//			qm_gpio_set_pin(QM_GPIO_0, 27);  // Enable 5V
//			qm_gpio_set_pin(QM_GPIO_0, 28);  // Enable 3.3V
//			//ends


			// Get token for downlink
			memset(token,0,sizeof(token));
			cellular_3g_get_token(QM_UART_0, token);
			int token_size=sizeof(token);
			for (int x=0;x<4;x++)
			{
				if (token[1]==0x00)
				{
					QM_PUTS("req token");
					flag=1;
					cellular_3g_get_token(QM_UART_0, token);
				}
				else
				{
					for (int y=0;y<token_size;y++)
					{
						old_token[y]=token[y];
					}
					flag=0;
					break;
				}
			}

			if (flag==1)
			{
				for (int x=0;x<token_size;x++)
				{
					token[x]=old_token[x];
				}
			}


			QM_PRINTF("Token: %s\n",token);
			QM_PRINTF("Old Token: %s\n",old_token);
			// Next state
			tsc_start = get_ticks();
 			state = STATE_TX;
			//state = STATE_RX;

			break;

	    //==============================//
		//           TX State           //
		//==============================//
		case STATE_TX:

			// Enable Power
			qm_gpio_set_pin(QM_GPIO_0, 20);  // Enable XBee
			qm_gpio_set_pin(QM_GPIO_0, 24);  // Enable ADC2
			qm_gpio_set_pin(QM_GPIO_0, 25);  // Enable ADC1
			qm_gpio_set_pin(QM_GPIO_0, 26);  // Enable ADC0
			qm_gpio_set_pin(QM_GPIO_0, 27);  // Enable 5V
			qm_gpio_set_pin(QM_GPIO_0, 28);  // Enable 3.3V

//			// Create 10 sec delay for each loop
			clk_sys_udelay(10000000);

			// Get sensor data from sensor sub-system
			get_ss_data(adc_data);

			// Check battery level. If below 3.3v, It will go sleep and continue to charge.
			//voltage battery level x2 due to voltage divider
			if (adc_data[0]*2 < 320)
			{
				QM_PRINTF("battery check: %d\n",adc_data[0]);
				state = STATE_SLEEP;
				break;
			}

			// Send sensor data
			led_r_flash_init();
			radio_send_data(adc_data, token);

//			if (loop%20==0)
//			{
//				QM_PRINTF("loop_20: %d\n",loop);
//				radio_send_data(adc_data, token);
//				loop=1;
//				state = STATE_INIT;
//			}
//
//			else
//			{
//				QM_PRINTF("loop!=20: %d\n",loop);
//				if(RELAY_SHIELD!=0)
//				{
//					if(RELAY_PUMP_B!=0)
//					{
//						QM_PUTS("Downlink starts");
//						uint8_t statusB0[100];
//						uint8_t * downlink_pump_statusB0;
//						downlink_pump_statusB0=get_downlink_status_pumpB(statusB0, token);
//						QM_PRINTF("%s\n", downlink_pump_statusB0);
//						QM_PUTS("Downlink ends");
//					}
//				}
//				loop++;
//
//			}

			// Next state
			//state = STATE_TX; //azwan changed to STATE_TX. Default STATE_RX.
			state = STATE_INIT;//azwan add to STATE_INIT
			break;

		//==============================//
		//           RX State           //
		//==============================//
		case STATE_RX:

			// Enable Power
			qm_gpio_set_pin(QM_GPIO_0, 20);  // Enable XBee
			qm_gpio_set_pin(QM_GPIO_0, 24);  // Enable ADC2
			qm_gpio_set_pin(QM_GPIO_0, 25);  // Enable ADC1
			qm_gpio_set_pin(QM_GPIO_0, 26);  // Enable ADC0
			qm_gpio_set_pin(QM_GPIO_0, 27);  // Enable 5V
			qm_gpio_set_pin(QM_GPIO_0, 28);  // Enable 3.3V

			// Receive data or check downlink data
            radio_receive_data(rx_status, token);

            // Next state
            if (SLEEP_OR_WAIT == SLEEP_MODE)
            {
                state = STATE_SLEEP;
            }
            else
            {
                state = STATE_WAIT;
            }

			break;

		//==============================//
		//          Sleep State         //
		//==============================//
		case STATE_SLEEP:
			// NOTE:
            // Sleep state/mode will enter low power mode. It will
			// reset upon wake up.

			// Disable Power
			qm_gpio_clear_pin(QM_GPIO_0, 20);  // Disable XBee
			qm_gpio_clear_pin(QM_GPIO_0, 24);  // Disable ADC2
			qm_gpio_clear_pin(QM_GPIO_0, 25);  // Disable ADC1
			qm_gpio_clear_pin(QM_GPIO_0, 26);  // Disable ADC0
			qm_gpio_clear_pin(QM_GPIO_0, 27);  // Disable 5V
			qm_gpio_clear_pin(QM_GPIO_0, 28);  // Disable 3.3V

			// Deep sleep mode
			enter_low_power();

			break;

		//==============================//
		//          Wait State          //
		//==============================//
		case STATE_WAIT:
			// NOTE:
            // Wait state/mode will NOT enter low power mode.

			// Wait time
			clk_sys_udelay(WAIT_TIME*1000000);

			// Check the current clock
			tsc_current_s = (get_ticks() - tsc_start)/32000000;  // 32000000 = SYS_TICKS_PER_US_32MHZ * 1000000

			// Next state
			if (tsc_current_s < rx_timeout_s)
			{
				state = STATE_RX;
			}
			else
			{
				state = STATE_INIT;
			}

			break;

		//==============================//
		//           ENG State          //
		//==============================//
		case STATE_ENG:

			break;
		}

	}

	SOC_WATCH_TRIGGER_FLUSH();

	return 0;
}

