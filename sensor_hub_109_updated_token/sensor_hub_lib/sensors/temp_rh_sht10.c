/*
 Copyright (c) 2017, Atilze Digital Sdn. Bhd.

 Atilze Sensor Hub
 This is the main file for Atilze Sensor Hub.

 Version   : 1.00
 Author    : Tan Han Wei
 File Name : sht11.c

 History:
   1.00 HW 16-Nov-17 Initial

 */
#include "qm_common.h"
#include "qm_gpio.h"
#include "qm_pinmux.h"
#include "qm_pin_functions.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "clk.h"
#include "qm_uart.h"

#define dataPin  18
#define clockPin 19
#define send_cmd_done 		1
#define send_cmd_failed 	0
#define wait_complete_done 		1
#define wait_complete_failed 	0


//initialize voltage,RS232 enable and UART1 control pins
void init_voltages_enable(qm_gpio_port_config_t *cfg)
{
	qm_pmux_select(QM_PIN_ID_61, QM_PIN_61_FN_GPIO_27);  // Init. 5V enable
	qm_pmux_select(QM_PIN_ID_62, QM_PIN_62_FN_GPIO_28);  // Init. 3.3V enable
	qm_pmux_select(QM_PIN_ID_55, QM_PIN_55_FN_GPIO_21);  // Init RS232 enable
	qm_pmux_select(QM_PIN_ID_33, QM_PIN_33_FN_GPIO_29);  // uart1 control

	cfg->direction =   BIT(21)|BIT(27)|BIT(28) |BIT(29) ;
	qm_gpio_set_config(QM_GPIO_0, cfg); // Configure
	qm_gpio_set_pin(QM_GPIO_0, 29); // Select Arduino

	return;

}

//initialize UART1
void init_uart(qm_uart_config_t *cfg)
{
	qm_pmux_select(QM_PIN_ID_16, QM_PIN_16_FN_UART1_TXD);  // UART1 TXD
	qm_pmux_select(QM_PIN_ID_17, QM_PIN_17_FN_UART1_RXD);  // UART1 RXD
	qm_pmux_select(QM_PIN_ID_9, QM_PIN_9_FN_UART1_RTS);  // UART1 RTS
	qm_pmux_select(QM_PIN_ID_8, QM_PIN_8_FN_UART1_CTS);  // UART1 CTS
	qm_pmux_input_en(QM_PIN_ID_17, true);
	qm_pmux_input_en(QM_PIN_ID_9, true);

	cfg->baud_divisor = QM_UART_CFG_BAUD_DL_PACK(0, 208, 5);// Configures UART_0 for 9600 bps
	cfg->line_control = QM_UART_LC_8N1;
	cfg->hw_fc = false;

	qm_uart_set_config(QM_UART_1, cfg);

	// enable clock for UART_1
	clk_periph_enable(CLK_PERIPH_CLK | CLK_PERIPH_UARTA_REGISTER);
	qm_gpio_clear_pin(QM_GPIO_0, 21);  // disable RS232
	qm_gpio_clear_pin(QM_GPIO_0, 29);  //UART1 Control: Clear ->Select Arduino UART

	return;
}

//pinmode(dataPin,input)
void data_input()
{
	static qm_gpio_port_config_t input_cfg;
	qm_pmux_select(QM_PIN_ID_52, QM_PIN_52_FN_GPIO_18);
	input_cfg.direction = 0x38680000;	//18 as input. The rest output
	qm_gpio_set_config(QM_GPIO_0, &input_cfg);
}

// pinmode(dataPin,output)
void data_output()
{
	static qm_gpio_port_config_t cfg;
	qm_pmux_select(QM_PIN_ID_52, QM_PIN_52_FN_GPIO_18); //data pin
	qm_pmux_select(QM_PIN_ID_53, QM_PIN_53_FN_GPIO_19);	//clock pin
	qm_pmux_select(QM_PIN_ID_56, QM_PIN_56_FN_GPIO_22);  // arduino enable
	qm_pmux_select(QM_PIN_ID_61, QM_PIN_61_FN_GPIO_27);  // Init. 5V enable
	qm_pmux_select(QM_PIN_ID_62, QM_PIN_62_FN_GPIO_28);  // Init. 3.3V enable
	qm_pmux_select(QM_PIN_ID_33, QM_PIN_33_FN_GPIO_29);  // uart1 control


	cfg.direction =   BIT(18)|BIT(19)|BIT(22)|BIT(27)|BIT(28) |BIT(29) ;

	qm_gpio_set_config(QM_GPIO_0, &cfg);
	qm_gpio_set_pin(QM_GPIO_0,22);	//enable arduino

}

//send acknowledge to notify we received
void send_ack()
{
	data_output();
	qm_gpio_set_pin(QM_GPIO_0,dataPin);	//digitalWrite(dataPin, HIGH);
	clk_sys_udelay(2);
	qm_gpio_clear_pin(QM_GPIO_0,dataPin);	//digitalWrite(dataPin, LOW);
	clk_sys_udelay(2);

    qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, LOW);
    qm_gpio_set_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, HIGH);
	clk_sys_udelay(2);
    qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, LOW);
	clk_sys_udelay(2);

}

//skip acknowledge due to no CRC
void skip_ack()
{
	data_output();
	qm_gpio_clear_pin(QM_GPIO_0,dataPin);	//digitalWrite(dataPin, LOW);
	clk_sys_udelay(2);
	qm_gpio_set_pin(QM_GPIO_0,dataPin);	//digitalWrite(dataPin, HIGH);
	clk_sys_udelay(2);


    qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, LOW);
    qm_gpio_set_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, HIGH);
	clk_sys_udelay(2);
    qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, LOW);
	clk_sys_udelay(2);
}

//refresh connection
void connection_reset()
{
	int j;

	for (j=0;j<10;j++ )
	{

		qm_gpio_set_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, HIGH);
		clk_sys_udelay(2);

		qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, LOW);
		clk_sys_udelay(2);
	}
}

//receiving SHT11 byte
uint8_t sht11_recv_byte()
{
	qm_gpio_state_t data_pin_status = 0;
    int k;
    uint8_t res = 0;

    for (k = 0; k < 8; ++k)
    {
        res <<= 1;
        qm_gpio_set_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, HIGH);
		clk_sys_udelay(2);
		data_input();
        qm_gpio_read_pin(QM_GPIO_0,dataPin, &data_pin_status);
		clk_sys_udelay(2);
        res |= data_pin_status;
        qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, LOW);
		clk_sys_udelay(2);

    }

    return res;

}


//start transmission pattern
void start_transmission()
{
	data_output();
	qm_gpio_set_pin(QM_GPIO_0,dataPin);	//digitalWrite(dataPin, HIGH);

	qm_gpio_clear_pin(QM_GPIO_0,clockPin); //digitalWrite(clockPin, LOW);
	clk_sys_udelay(2);

	qm_gpio_set_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, HIGH);
	clk_sys_udelay(2);

	qm_gpio_clear_pin(QM_GPIO_0,dataPin);	//digitalWrite(dataPin, LOW);
	clk_sys_udelay(2);

	qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, Low);
	clk_sys_udelay(2);

	qm_gpio_set_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, HIGH);
	clk_sys_udelay(2);

	qm_gpio_set_pin(QM_GPIO_0,dataPin);	//digitalWrite(dataPin, HIGH);
	clk_sys_udelay(2);

	qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, LOW);
	clk_sys_udelay(2);


}

//sending command to SHT11 either temperature or humidity data request
int send_command(int command)
{
	qm_gpio_state_t data_pin_status = 0;
	int ack;
	int send_cmd_complete = 0;
	uint32_t i;

	for(i=0;i<8;i++)
	{
	  if(command & 0x80)
	  {
		  qm_gpio_set_pin(QM_GPIO_0,dataPin);
	  }
	  else
	  {
		  qm_gpio_clear_pin(QM_GPIO_0,dataPin);
	  }

	  clk_sys_udelay(2);
	  qm_gpio_set_pin(QM_GPIO_0,clockPin);
	  clk_sys_udelay(2);
	  qm_gpio_clear_pin(QM_GPIO_0,clockPin);
	  command <<=1;

	}

	qm_gpio_set_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, HIGH);
	data_input();
	qm_gpio_read_pin(QM_GPIO_0,dataPin, &data_pin_status);

	ack  = data_pin_status;
	if (ack == QM_GPIO_LOW)
	{
		qm_gpio_clear_pin(QM_GPIO_0,clockPin);	//digitalWrite(clockPin, LOW);
		qm_gpio_read_pin(QM_GPIO_0,dataPin, &data_pin_status);
		ack  = data_pin_status;
		if (ack == QM_GPIO_HIGH)
		{
			send_cmd_complete = send_cmd_done;

			  //      QM_PUTS("Ack Error 1");
		}
	}

	else
	{
		return send_cmd_complete = send_cmd_failed;
	}

	return send_cmd_complete;

}

int wait_for_result()
{
	int i;
	int wait_complete =0;
	qm_gpio_state_t pinstatus;
	data_input();

	//wait until SHT11 finished reading
	for (i=0; i<100; i++)
	{
		clk_sys_udelay(80000);
		qm_gpio_read_pin(QM_GPIO_0,dataPin,&pinstatus);
		if (pinstatus == QM_GPIO_LOW)
		{
			//QM_PUTS("wait_complete_done");
			wait_complete = wait_complete_done;
			break;
		}
		if (pinstatus == QM_GPIO_HIGH)
		{
			wait_complete = wait_complete_failed;

			//QM_PUTS("wait_complete_failed");
		}
	}

	return wait_complete;
}



//calculate temperature with coefficients
//Temperature is 14 bits data
float calc_temp()
{
	// Conversion coefficients from SHT11 datasheet
	const float D1 = -40.1;   // for 14 Bit @ 5V
	const float D2 =   0.01; // for 14 Bit DEGC

	uint16_t res_temp;
	int temp_val = 0;       // Temperature derived from raw value

	res_temp = sht11_recv_byte() << 8;
	send_ack();

	res_temp |= sht11_recv_byte();
	skip_ack();

	// Convert raw value to degrees C
	temp_val = ((res_temp *D2) + D1)*100;
	//QM_PRINTF("temp_val:%d\n",temp_val);


	return temp_val;
}


//calculate humidity with coefficients
//humidity is 12 bits data
float calc_rh_linear()
{
	// Conversion coefficients from SHT11 datasheet
	const float C1 = -2.0468;       // for 12 Bit
	const float C2 =  0.0367;    // for 12 Bit
	const float C3 = -0.0000015955; // for 12 Bit

	uint16_t res_rh_linear;
	int rh_linear = 0;       // humidity derived from raw value

	res_rh_linear = sht11_recv_byte() << 8;
	send_ack();

	res_rh_linear |= sht11_recv_byte();
	skip_ack();

	rh_linear = (C1 + (C2 * res_rh_linear) + (C3 * res_rh_linear * res_rh_linear))*10;
	return rh_linear;

}

//calculate true humidity relative to temperature and linear humidity
float calc_rh_true(float temperature, float humidity)
{
	// Conversion coefficients from SHT11 datasheet
	const float t1 = 0.01;       // for 12 Bit
	const float t2 = 0.00008;    // for 12 Bit

	uint16_t res_rh_linear;
	int rh_true = 0;       // humidity derived from raw value

	res_rh_linear = sht11_recv_byte() << 8;
	send_ack();

	res_rh_linear |= sht11_recv_byte();
	skip_ack();

	rh_true = ((((temperature/100) - 25)*(t1 + t2*res_rh_linear)) + (humidity/100))*10;
	//QM_PRINTF("true_humidity%d\n",true_humidity);

	return rh_true;

}

//Connection Reset -> Start Trasmission -> Send Command (temp or humidity) -> wait for measurement -> read buffer
int get_sht10_temp()
{
	int temp;

	//Connection Reset Sequence
	connection_reset();

	//Send transmission start pattern
	start_transmission();

	//Command to request Temperature 0x03

	if (send_command(0x03) == send_cmd_failed)
	{
		return temp = 0;
	}

	//wait for SHT11 to complete the measurement
	if (wait_for_result() == wait_complete_failed)
	{
		return temp = 0;
	}

	//multiple 100 to adapt fabrick
	temp = calc_temp();
	//QM_PRINTF("Temp :%d\n",temp);
	clk_sys_udelay(2);
	return temp;
}

//Connection Reset -> Start Trasmission -> Send Command (temp or humidity) -> wait for measurement -> read buffer
int get_sht10_rh_linear()
{
	int humidity;

	//Connection Reset Sequence
	connection_reset();

	//Send transmission start pattern
	start_transmission();

	//Command to request Humidity 0x05
	if (send_command(0x05) == send_cmd_failed)
	{
		return humidity = 0;
	}

	//wait for SHT11 to complete the measurement
	if (wait_for_result()== wait_complete_failed)
	{
		return humidity = 0;
	}

	//multiple 100 to adapt fabrick
	humidity = calc_rh_linear();

	clk_sys_udelay(2);
	return humidity;
}

//Connection Reset -> Start Trasmission -> Send Command (temp or humidity) -> wait for measurement -> read buffer
int get_sht10_rh_true()
{
	int temp;
	int rh_linear;
	int rh_true;


	temp = get_sht10_temp();
	if (temp == 0)
	{
		return rh_true = 0;
	}

	//QM_PRINTF("Temp :%d\n",temp);


	rh_linear = get_sht10_rh_linear();
	if(rh_linear == 0)
	{
		return rh_true = 0;
	}

	//QM_PRINTF("RH Linear: %d\n",rh_linear);

	rh_true = calc_rh_true(temp, rh_linear);
	//QM_PRINTF("RH True:%d\n",rh_true);

	clk_sys_udelay(2);
	return rh_true ;
}
