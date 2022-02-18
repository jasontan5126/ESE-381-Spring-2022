/*
 * data.h
 *
 * Created: 2/6/2022 4:46:15 PM
 *  Author: Jason
 */ 


#ifndef DATA_H_
#define DATA_H_

typedef union {
	
	uint8_t byte;        //member used for register access
	
	struct{
		uint8_t bit0: 1;  //bit0
		uint8_t bit1: 1;    //bit1
		uint8_t bit2: 1;    //bit2
		uint8_t bit3: 1;//bit3
		uint8_t bit4: 1;//bit4
		uint8_t bit5: 1;//bit5
		uint8_t bit6: 1;//bit6
		uint8_t bit7: 1;//bit7
		

		}bvals;
	
	} Named_bits;



#endif /* DATA_H_ */