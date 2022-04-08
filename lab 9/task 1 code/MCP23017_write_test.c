/*
 * MCP23017_write_test.c
 *
 * Created: 3/29/2022 8:12:51 AM
 * Author : jason
 */ 

#include <avr/io.h>
#define F_CPU 4000000     // 4MHz default clock
#include <util/delay.h>

//Defines for GIO, b1 means when 8-bit mode, BANK = 1
#define IOCONaddr_b0 0x0A   //address at reset, default 16-bit mode
#define IOCONaddr_b1 0x05	//Control registers
#define IODIRAaddr_b1 0x00	//Directional register for PORTA
#define IODIRBaddr_b1 0x10	//Directional register for PORTB
#define GPPUAaddr_b1 0x06
#define GPIOAaddr_b1 0x09	//GPIOA I/O PORT register
#define OLATBaddr_b1 0x1A	//GPIO output registers
#define WRITE_opcode 0x40
#define READ_opcode 0x41

void I2C0_MCP23017_init();
void MCP23017_I2C_init();
void MCP23017_I2C_write(uint8_t, uint8_t, uint8_t);

int main(void)
{	
	I2C0_MCP23017_init();   //Initializes  the AVR128DB48 I2C0 to communicate with MCP23017
	
	//Initializes the MCP23017. Port A of the GPIO (GPA) must be configured as
	//an input with pull ups enabled. Port B must be configured as an output.
	MCP23017_I2C_init();	
	while (1)
	{
		for(uint8_t i = 0; i <= 0xFF; i++){
			//Write that incremented value to the GPB to display the LEDs
			MCP23017_I2C_write(WRITE_opcode, OLATBaddr_b1, ~i);
			_delay_ms(500);  //Wait half a second between each increment of a number for the LED
		}		
	}
}

void I2C0_MCP23017_init()
{
	//Baud rate for the I2C which set to 0 assuming that is the fastest you can get to
	TWI0.MBAUD = 0;


	//Enable for the I2C Master
	TWI0.MCTRLA = TWI_ENABLE_bm;
	
	//Force the I2C to the idle state
	TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

//Seems like this function is what initializes or configures the MCP23017
//port pins with indirect access to it
void MCP23017_I2C_init(){
	//https://blackboard.stonybrook.edu/bbcswebdav/pid-1709660-dt-content-rid-13115758_1/courses/1224-ESE-381-SEC01-48354/MCP23017_MCP23S17%2016_Bit%20IO%20Expander%20with%20Serial%20Interface%2020001952C.pdf
	
	MCP23017_I2C_write(WRITE_opcode, IOCONaddr_b0, 0xA0);
	
	//Configure PORTA of the GPIO MCP23017 as the input
	MCP23017_I2C_write(WRITE_opcode, IODIRAaddr_b1, 0xFF);
	
	//Enable the pull up resistor for PORTA of the GPIO MCP23017
	MCP23017_I2C_write(WRITE_opcode, GPPUAaddr_b1, 0xFF);
	
	//Configure PORTB of the GPIO MCP23017 as the output
	MCP23017_I2C_write(WRITE_opcode, IODIRBaddr_b1, 0x00);
}


void MCP23017_I2C_write(uint8_t opcode, uint8_t address, uint8_t data){
	
	
	TWI0_MADDR = opcode;   //Read the opcode to the address
	
	//Poll until master transmit address of byte write operation is complete regardless
	while(!(TWI0.MSTATUS & TWI_WIF_bm));	
	
	TWI0_MDATA = address;
	
	//Poll until master transmit address of byte write operation is complete regardless
	while(!(TWI0.MSTATUS & TWI_WIF_bm));
	
	
	
	TWI0_MDATA = data;
	
	//Poll until master transmit address of byte write operation is complete regardless
	while(!(TWI0.MSTATUS & TWI_WIF_bm));
	
	
	//Execute acknowledge action followed by issuing a stop condition
	TWI0_MCTRLB = TWI_MCMD_STOP_gc;
	
	
	
}
 



