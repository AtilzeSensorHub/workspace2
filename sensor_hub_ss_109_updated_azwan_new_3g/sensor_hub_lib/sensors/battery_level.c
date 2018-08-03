/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

battery_level.c
Detect battery level.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : battery_level.c

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#include <unistd.h>
#include <math.h>
#include "../main.h"
//#include "soil_ph.h"
#include "qm_common.h"
#include "qm_interrupt.h"
#include "qm_interrupt_router.h"
#include "qm_isr.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "qm_ss_adc.h"
#include "qm_ss_interrupt.h"
#include "qm_ss_isr.h"
#include "qm_uart.h"
#include "ss_clk.h"
#include "ring_buffer.h"
#include "system/adc.h"

// Initialise battery level ADC
void batt_level_adc_init()
{
	if(DEBUG_MODE)
	{
		QM_PUTS("Battery Level ADC Starting: ADC_CH15");
	}

	qm_ss_adc_channel_t channel_adc[] = {QM_SS_ADC_CH_15};
	adc_init(QM_PIN_ID_15, QM_PIN_15_FN_AIN_15, channel_adc);
}

// Deinitialise battery level ADC
void batt_level_adc_deinit()
{
    adc_deinit();

	if(DEBUG_MODE)
	{
		QM_PUTS("Battery Level ADC Finished: ADC_CH15");
		QM_PUTS("");
	}
}

// Get battery level
int get_battery_level()
{
	int battery_level_dec;
	float battery_level_val;
	float adc_output;

	// Init. battery level ADC
	batt_level_adc_init();

    // Get ADC data
	adc_output = wait_for_samples();

	// Convert to Volt.
	battery_level_val =  (adc_output/4096)*3.3; //THis part still required voltage divider
	battery_level_dec = (int)(battery_level_val *100);

	// Deinit. battery level ADC
	batt_level_adc_deinit();

	return battery_level_dec;
}



