/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 adc.c
 ADC library file.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : adc.c

 History:
   1.00 HW 18-Aug-17 Initial
 */

#include <unistd.h>
#include <math.h>
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

//
#define QM_ADC_CMD_STOP_CONT (5)
#define NUM_CHANNELS (1)
#define NUM_SAMPLES (NUM_CHANNELS * 5)
#define ADC_BUFFER_SIZE (256)
#define MAX_SAMPLES (10)
#define ADC_SAMPLE_SHIFT (11)

//
static uint16_t samples[NUM_SAMPLES] = {0};
static ring_buffer_t rb;
static uint16_t adc_buffer[ADC_BUFFER_SIZE];
static volatile bool acq_error = false;

// ISR for ADC 0 continuous conversion.
static QM_ISR_DECLARE(adc_0_continuous_isr)
{
	uint32_t i, controller = QM_SS_ADC_BASE;

	// Read the samples into the array.
	for (i = 0; i < NUM_SAMPLES; ++i)
	{
		// Pop one sample into the sample register.
		QM_SS_REG_AUX_OR(controller + QM_SS_ADC_SET,
				 QM_SS_ADC_SET_POP_RX);

		// Read the sample in the array.
		rb_add(&rb, __builtin_arc_lr(controller + QM_SS_ADC_SAMPLE) >>
				(ADC_SAMPLE_SHIFT - QM_SS_ADC_RES_12_BITS));
	}

	// Clear the data available status register.
	QM_SS_REG_AUX_OR(controller + QM_SS_ADC_CTRL,
			 QM_SS_ADC_CTRL_CLR_DATA_A);
}

// Transfer callback.
void transfer_cb(void *data, int error, qm_ss_adc_status_t status,
		 qm_ss_adc_cb_source_t source)
{
	if (error) {
		acq_error = true;
	}
}

// Wait for samples
float wait_for_samples()
{
	uint16_t val;
	uint32_t count = 0;
	int i;
	float sum =0, average=0;

	// Loop until an error occurs or MAX_SAMPLES conversions done.
	while ((!acq_error) && (count < MAX_SAMPLES))
	{
		if (rb.overflow)
		{
			acq_error = true;
		}

		// Print one data from the ring buffer.
		if (!rb_is_empty(&rb))
		{

			rb_get(&rb, &val);

			// Make sure the ADC is enabled as the driver disables
		    // it after a successful conversion.
			QM_SS_REG_AUX_OR(QM_SS_ADC_BASE + QM_SS_ADC_CTRL, QM_SS_ADC_CTRL_ADC_ENA);

			++count;

		}
	}

	// Get average adc value from buffer
	for (i=0; i<10;++i)
	{
		sum += adc_buffer[i];
	}

	average= sum / 10;

	// Transfer error
	if (acq_error && !rb.overflow)
	{
		if (DEBUG_MODE)
		{
			QM_PUTS("ADC conversion failed.");
		}
	}

	//soil_ec_val=  (average/4096)*3.3*5000; // Electrical Conductivity (us/cm)= voltage * 5000
	//int soil_ec_dec = (int)(soil_ec_val); // to move the 2 decimal front

	if (DEBUG_MODE)
	{
		printf("ADC average: %3.2f \n", average);
	}


	//return soil_ec_dec;
	return average;
}

// Start IRQ conversion
int start_irq_conversion(qm_ss_adc_channel_t channel_adc[])
{
	qm_ss_adc_xfer_t xfer;

	// Set up xfer.
	xfer.ch = channel_adc;
	xfer.ch_len = NUM_CHANNELS;
	xfer.samples = samples;
	xfer.samples_len = NUM_SAMPLES;
	xfer.callback = transfer_cb;

	if (qm_ss_adc_irq_convert(QM_SS_ADC_0, &xfer))
	{
		if (DEBUG_MODE)
		{
			QM_PUTS("Soil EC Error:convert failed");
		}
		return 1;
	}
	return 0;
}

// Pin MUX setup
void pin_mux_setup(const qm_pin_id_t pin, const qm_pmux_fn_t fn)
{
	qm_pmux_select(pin, fn);
	qm_pmux_input_en(pin, true);
}

// Setup ADC
void setup_adc(const qm_pin_id_t pin, const qm_pmux_fn_t fn)
{
	qm_ss_adc_config_t cfg;

	// Enable the adc and set the clock divisor.
	ss_clk_adc_enable();
	ss_clk_adc_set_div(100);

	// Set up pinmux.
	pin_mux_setup(pin, fn);

	// Request the necessary IRQs.
	QM_IR_UNMASK_INTERRUPTS(QM_INTERRUPT_ROUTER->ss_adc_0_int_mask);
	QM_IR_UNMASK_INTERRUPTS(QM_INTERRUPT_ROUTER->ss_adc_0_error_int_mask);

	qm_ss_irq_request(QM_SS_IRQ_ADC_0_INT, adc_0_continuous_isr);
	qm_ss_irq_request(QM_SS_IRQ_ADC_0_ERROR_INT, qm_ss_adc_0_error_isr);

	// Set the mode and calibrate.
	qm_ss_adc_set_mode(QM_SS_ADC_0, QM_SS_ADC_MODE_NORM_CAL);
	qm_ss_adc_calibrate(QM_SS_ADC_0);

	// Set up config.
	cfg.window = 50; // Clock cycles between the start of each sample.
	cfg.resolution = QM_SS_ADC_RES_12_BITS;
	qm_ss_adc_set_config(QM_SS_ADC_0, &cfg);
}

// Initialise ADC
void adc_init(const qm_pin_id_t pin, const qm_pmux_fn_t fn, qm_ss_adc_channel_t channel_adc[])
{
	setup_rb(&rb, adc_buffer, ADC_BUFFER_SIZE);

	setup_adc(pin, fn);

	start_irq_conversion(channel_adc);
}


// Deinitialise ADC
void adc_deinit()
{
	QM_SS_REG_AUX_NAND(QM_SS_ADC_BASE + QM_SS_ADC_CTRL,
			   QM_SS_ADC_CTRL_SEQ_START);

	QM_SS_REG_AUX_OR(QM_SS_ADC_BASE + QM_SS_ADC_CTRL,
			 QM_SS_ADC_CTRL_MSK_ALL_INT);
	qm_ss_adc_set_mode(QM_SS_ADC_0, QM_SS_ADC_MODE_PWR_DOWN);
}
