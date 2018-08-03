/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

soi_ph.c
Soil pH sensor.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : soil_ph.c

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#include <unistd.h>
#include <math.h>
#include "sensors/soil_ph_sen0169.h"
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

// Initialise soil pH sensor
void soil_ph_sen0169_init()
{
	if(DEBUG_MODE)
	{
		QM_PUTS("Soil pH Starting: ADC_CH2");
	}

	qm_ss_adc_channel_t channel_adc[] = {QM_SS_ADC_CH_2};
	adc_init(QM_PIN_ID_2, QM_PIN_2_FN_AIN_2, channel_adc);
}

// Deintialise soil pH sensor
void soil_ph_sen0169_deinit()
{
    adc_deinit();

	if(DEBUG_MODE)
	{
		QM_PUTS("Soil pH Finished: ADC_CH2 ");
		QM_PUTS("");
	}
}

// Get soil pH sensor data
int get_soil_ph_sen0169()
{
	int soil_ph_dec;
	float soil_ph_val;
	float adc_output;

	// Init. soil pH sensor
	soil_ph_sen0169_init();

	// Get ADC data
	adc_output = wait_for_samples();

	// Convert ADC data to pH value
	soil_ph_val=  (adc_output/4096)*14;
	soil_ph_dec = (int)(soil_ph_val *100);

	// Deint. soil pH sensor
	soil_ph_sen0169_deinit();

	return soil_ph_dec;
}

