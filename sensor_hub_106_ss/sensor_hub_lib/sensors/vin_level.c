/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

battery_level.c
Detect battery level.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : vin_level.c

 History:
   1.00 HW 17-Dec-17 Initial

 */

#include <unistd.h>
#include <math.h>
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
#include "qm_uart.h"
#include "ss_clk.h"
#include "ring_buffer.h"
#include "system/adc.h"

// Initialise vin level ADC
void vin_level_adc_init()
{
	if(DEBUG_MODE)
	{
		QM_PUTS("Vin Level ADC Starting: ADC_CH10");
	}

	qm_ss_adc_channel_t channel_adc[] = {QM_SS_ADC_CH_10};
	adc_init(QM_PIN_ID_10, QM_PIN_10_FN_AIN_10, channel_adc);
}

// Deinitialise vin level ADC
void vin_level_adc_deinit()
{
    adc_deinit();

	if(DEBUG_MODE)
	{
		QM_PUTS("Battery Level ADC Finished: ADC_CH10");
		QM_PUTS("");
	}
}

// Get vin level
int get_vin_level()
{
	int vin_level_dec;
	float vin_level_val;
	float adc_output;

	// Init. vin level ADC
	vin_level_adc_init();

    // Get ADC data
	adc_output = wait_for_samples();

	// Convert to Volt.
	vin_level_val =  (adc_output)* (3.33/4096) *((120+820)/120);

	vin_level_dec = (int)(vin_level_val *100);

	// Deinit. vin level ADC
	vin_level_adc_deinit();

	return vin_level_dec;
}



