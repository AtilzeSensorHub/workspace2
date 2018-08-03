/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 adc.h
 ADC library file.

 Version   : 1.00
 Author    : Tan Han Wei /Khoo Boon Kien
 File Name : adc.h

 History:
   1.00 HW 18-Aug-17 Initial
 */

#ifndef SENSOR_HUB_SS_PERIPHERALS_ADC_H_
#define SENSOR_HUB_SS_PERIPHERALS_ADC_H_

void transfer_cb(void *data, int error, qm_ss_adc_status_t status, qm_ss_adc_cb_source_t source);

float wait_for_samples();

int start_irq_conversion(qm_ss_adc_channel_t channel_adc[]);

void pin_mux_setup(void);

void setup_adc(const qm_pin_id_t pin, const qm_pmux_fn_t fn);

void adc_init(const qm_pin_id_t pin, const qm_pmux_fn_t fn, qm_ss_adc_channel_t channel_adc[]);

void adc_deinit();

#endif /* SENSOR_HUB_SS_PERIPHERALS_ADC_H_ */
