/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

soi_moisture.c
Soil moistures sensor.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : soil_moisture.c

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#include <unistd.h>
#include <math.h>
#include "sensors/soil_moisture_ae0001.h"
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

// Initialise soil moisture sensor
void soil_moisture_ae0001_init()
{
	if(DEBUG_MODE)
	{
		QM_PUTS("Soil Moisture Starting: ADC_CH1");
	}

	qm_ss_adc_channel_t channel_adc[] = {QM_SS_ADC_CH_1};
	adc_init(QM_PIN_ID_1, QM_PIN_1_FN_AIN_1, channel_adc);
}

// Deintialise soil moisture sensor
void soil_moisture_ae0001_deinit()
{
    adc_deinit();

	if(DEBUG_MODE)
	{
		QM_PUTS("Soil Moisture Finished: ADC_CH1 ");
		QM_PUTS("");
	}
}

// Get soil moisture sensor data
int get_soil_moisture_ae0001()
{
	int soil_moisture_dec;
	float soil_moisture_val;
	float adc_output;

	// Init. soil moisture sensor
	soil_moisture_ae0001_init();

	// Wait for samples
	adc_output = wait_for_samples();

	// Convert ADC data to soil moisture data
	soil_moisture_val=  (adc_output/4096)*3.3*50;
	soil_moisture_dec = (int)(soil_moisture_val*100);

	// Deinit. soil moisture sensor
	soil_moisture_ae0001_deinit();

	return soil_moisture_dec;
}

