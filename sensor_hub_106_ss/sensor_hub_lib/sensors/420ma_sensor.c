/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

420ma_sensor.c
4-20mA sensor.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name : 420ma_sensor.c

 History:
   1.00 HW 9-NOV-17 Initial

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
#include "sensors/420ma_sensor.h"

// Initialise 4-20mA sensor
void sensor_420ma_init()
{
	if(DEBUG_MODE)
	{
		QM_PUTS("4-20mA Generator Starting: ADC_CH3");
	}

	qm_ss_adc_channel_t channel_adc[] = {QM_SS_ADC_CH_3};
	adc_init(QM_PIN_ID_3, QM_PIN_3_FN_AIN_3, channel_adc);
}

// Deinitialise 4-20ma sensor
void sensor_420ma_deinit()
{
    adc_deinit();

	if(DEBUG_MODE)
	{
		QM_PUTS("4-20mA Generator Finished: ADC_CH3");
		QM_PUTS("");
	}
}

// Get 4-20ma sensor
int get_420ma_sensor ()
{
	int sensor_420ma_dec;
	float sensor_420ma_val;
	float adc_output;

	// Initialise 420ma sensor
	sensor_420ma_init();

	// Wait for ADC samples
	adc_output = wait_for_samples();

	// Convert ADC data to a value
	sensor_420ma_val=  (adc_output/4096)*3.3;
	sensor_420ma_dec = (int)(sensor_420ma_val *100);

	// Deinitialise 420ma sensor
	sensor_420ma_deinit();

	return sensor_420ma_dec;
}



