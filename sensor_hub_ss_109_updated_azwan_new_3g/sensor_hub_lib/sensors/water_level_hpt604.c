/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

aqua_water_level.c
Water level sensor.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : aqua_water_level.c

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
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

// Initialise aqua water level
void water_level_hpt604_init()
{
	if(DEBUG_MODE)
	{
		QM_PUTS("Aqua Water Level Starting: ADC_CH0");
	}

	qm_ss_adc_channel_t channel_adc[] = {QM_SS_ADC_CH_0};
	adc_init(QM_PIN_ID_0, QM_PIN_0_FN_AIN_0, channel_adc);
}

// Deinitialise aqua water level
void water_level_hpt604_deinit()
{
    adc_deinit();

	if(DEBUG_MODE)
	{
		QM_PUTS("Aqua Water Level Finished: ADC_CH0");
		QM_PUTS("");
	}
}

// Get water level
int get_water_level_hpt604()
{
	int water_level_dec;
	float water_level_val;
	float adc_output;

	// Initialise water level sensor
	water_level_hpt604_init();

	// Wait for ADC samples
	adc_output = wait_for_samples();

	// Convert ADC data to water level
	water_level_val=  (adc_output/4096)*3.3; // Electrical Conductivity (us/cm)= voltage * 5000
	water_level_dec = (int)(water_level_val *100);

	// Deinitialise water level sensor
	water_level_hpt604_deinit();

	return water_level_dec;
}

