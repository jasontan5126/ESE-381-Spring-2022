/*
 * SCD41_LCD_USART3_Lab11Task1.c
 *
 * Created: 4/26/2022 6:13:25 PM
 * Author : jason
 */ 

#include <avr/io.h>
#include <math.h>
#define F_CPU 4000000
#include "lcd_dog_AVR128_driver.h"
#include "SCD41_AVR128_driver.h"
#include "USART3_asynch_transmit.h"
#include <util/delay.h>

#define MAX_INPUT_DISPLAY 5


uint16_t CO2;
uint16_t Temp;
uint16_t Rh;

uint16_t baudRate = 9600;   //For the baud rate of USART3
uint8_t dataBits = USART_CHSIZE_8BIT_gc;	//For the (character size)  CHSIZE[2:0]
unsigned char parity = 0x00;	//PMODE[1:0]



int main(void)
{
	
	init_lcd_dog();		 //Initialize the buffer of the LCD
	I2C0_SCD41_init();   //Initializes  the AVR128DB48 I2C0 to communicate with SCD41
	
	//Function to initialize the USART3 baud rate, data bit and parity
	USART3_init(baudRate, dataBits, parity);
	
	while (1)
	{
		
		SCD41_start_periodic_measurement(I2CSLAVE_ADDR_WRITE, ADDRESS_STARTPERIODIC_MSB, ADDRESS_STARTPERIODIC_LSB);
		
		//Keep polling until data is ready which can be measured
		while(!SCD41_get_data_ready_status(I2CSLAVE_ADDR_WRITE, ADDRESS_GETDATAREADY_MSB, ADDRESS_GETDATAREADY_LSB));
		
		SCD41_read_measurement(I2CSLAVE_ADDR_WRITE, ADDRESS_READMEASUR_MSB, ADDRESS_READMEASUR_LSB);
		
		CO2 = getParseCO2;
		Temp = -45 + ( (175 * getParseTemp) / (pow(2, 16)));
		Rh = 100 * (((float)getParseRh) / (pow(2, 16)));
		
		
		
		//Print the CO2 value into LCD buffer
		sprintf(dsp_buff1, "CO2: %d", CO2);
		sprintf(dsp_buff2, "Temp: %d", Temp);
		//Print the humidity value into LCD buffer
		sprintf(dsp_buff3, "Relative Hum: %d", Rh);
		//Update the 3 line messages into the LCD buffer
		update_lcd_dog();
		
		//Where it will transmit the entire array of strings to display in TeraTerm or Termite
		char * inputUSART3DataDisplay[] = {dsp_buff1, "    ", dsp_buff2, "    ", dsp_buff3};
		
		//Loop through all the strings in the array
		for(int i = 0; i < MAX_INPUT_DISPLAY; i++){
			USART3_sendString(inputUSART3DataDisplay[i]);	
		}
		_delay_ms(1000);  //Transmit the readings once every 1 second
	}
}

