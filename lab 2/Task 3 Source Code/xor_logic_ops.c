/*
 * xor3_logic_ops.c
 *
 * Created: 2/6/2022 4:02:10 PM
 * Author : jason
 */ 

#include <avr/io.h>


int main(void)
{
	
	//pointer to PIN5CTRL array of pin configuration registers
	uint8_t* ptr = (uint8_t*)&PORTA.PIN2CTRL;
	
	//DIR is what configure the port pins as inputs or outputs
	VPORTA_DIR = 0x00;	//Configure PORTA pins ( PA7, PA6, PA5) as the inputs
	
	VPORTD_DIR = PIN7_bm;   //Configure (PD7) as output pin
	
	/*
	PA7 is "C"
	PA6 is "B"
	PA5 is "A"
	PD7 is "F"
	*/
	
	//Configure PA7 - PA5 as input buffers with internal pull up resistors
	for(uint8_t i = 3; i < 8; i++){
		*(ptr + i) |= PORT_PULLUPEN_bm;
	}
   
    while (1) 
    {	
		uint8_t C = PORTA_IN & PIN7_bm;
		uint8_t B = (PORTA_IN & PIN6_bm) << 1;   //Shift left by 1 to bit 6 compare the bit 5 position (B) to bit 4 position (A)
		uint8_t A = (PORTA_IN & PIN5_bm) << 2;	//Shift left by 1 to compare the bit 5 position (B) to bit 4 position (A) 
		//Aligns the bit 7, bit 6 and bit 5 positions to compare
		
		
		PORTD_OUT = (((~C) ^ (~B) ^ (A)) ^ 0xFF ) | (((~C) ^ (B) ^ (~A)) ^ 0xFF ) | ( ((C) ^ (~B) ^ (~A)) ^ 0xFF) | (((C) ^ (B) ^ (A)) ^ 0xFF );
		
		
    }
}

