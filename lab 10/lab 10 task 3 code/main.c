/*
 * SCD41_LCD_Multifile.c
 *
 * Created: 4/18/2022 12:56:05 AM
 * Author : jason
 */ 

#include <avr/io.h>
#include <math.h>
#include "lcd_dog_AVR128_driver.h"
#include "SCD41_AVR128_driver.h"


uint16_t CO2;
uint16_t Temp;
uint16_t Rh;

int main(void)
{
	init_lcd_dog();		 //Initialize the buffer of the LCD
	I2C0_SCD41_init();   //Initializes  the AVR128DB48 I2C0 to communicate with SCD41
						
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
			
    }
}

