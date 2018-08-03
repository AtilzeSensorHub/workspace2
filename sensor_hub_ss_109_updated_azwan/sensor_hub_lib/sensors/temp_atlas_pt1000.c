/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 temperature_pt1000.c
 temperature sensor PT1000.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : temperature_pt1000.c

 History:
   1.00 HW 18-Aug-17 Initial
   1.01 BK 20-Oct-17 Moved the ADC to a library
 */

#include <stdio.h>
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

// Initialise PT1000
void temp_atlas_pt1000_init()
{
	if(DEBUG_MODE)
	{
		QM_PUTS("PT1000 Starting: ADC_CH1");
	}

	qm_ss_adc_channel_t channel_adc[] = {QM_SS_ADC_CH_1};
	adc_init(QM_PIN_ID_1, QM_PIN_1_FN_AIN_1, channel_adc);
}

// Deinitialise PT1000
void temp_atlas_pt1000_deinit()
{
    adc_deinit();

	if(DEBUG_MODE)
	{
		QM_PUTS("PT1000 Finished: ADC_CH1 ");
		QM_PUTS("");
	}
}

// Get PT1000 data
int get_temp_atlas_pt1000 ()
{
	int temp_pt1000_dec;
	float temp_pt1000_val;
	float adc_output;
	float Vout, R2;

	// Init. PT1000
	temp_atlas_pt1000_init();

	// Get ADC data
	adc_output = wait_for_samples();

	// Convert ADC data to temperature data
	Vout = (adc_output/4096)*3.3; // This part still required voltage divider
	R2   = (Vout * 1000)/(3.3-Vout);	//1076 is R1 resistor. Please use 1k ohm resistor divider
	temp_pt1000_val = -(((sqrt((-0.00232*R2) + 17.59246)) - 3.908)/0.00116);
	temp_pt1000_dec = (int)(temp_pt1000_val *100);

	// Deinit. PT1000
	temp_atlas_pt1000_deinit();

	return temp_pt1000_dec;
}



