/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

soi_ec.c
Soil EC sensor.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : soil_ec.c

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#include <unistd.h>
#include <math.h>
#include "sensors/soil_ec_ae0001.h"
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
#include "qm_uart.h"
#include "ss_clk.h"
#include "ring_buffer.h"
#include "system/adc.h"

int ec_offset=430;

// Initialise soil EC sensor
void soil_ec_ae0001_init()
{
	if(DEBUG_MODE)
	{
		QM_PUTS("Soil EC Starting: ADC_CH0");
	}

	qm_ss_adc_channel_t channel_adc[] = {QM_SS_ADC_CH_0};
	adc_init(QM_PIN_ID_0, QM_PIN_0_FN_AIN_0, channel_adc);
}

// Deinitialise soil EC sensor
void soil_ec_ae0001_deinit()
{
    adc_deinit();

	if(DEBUG_MODE)
	{
		QM_PUTS("Soil EC Finished: ADC_CH0 ");
		QM_PUTS("");
	}
}

// Get the soil EC data
int get_soil_ec_ae0001()
{
	int soil_ec_dec;
	float soil_ec_val;
	float adc_output;

	// Initialise EC sensor
	soil_ec_ae0001_init();

	// Wait for ADC samples
	adc_output = wait_for_samples();

	// Convert ADC data to soil EC
	soil_ec_val=  (adc_output/4096)*5*5000;
    soil_ec_dec = (int)(soil_ec_val) +ec_offset;

    // Deinitialise EC sensor
	soil_ec_ae0001_deinit();

	return soil_ec_dec;
}
