/*
 * MCP23017_inout_test.c
 *
 * Created: 3/30/2022 9:44:07 PM
 * Author : jason
 */ 

#define F_CPU 4000000     // 4MHz default clock
#include <avr/io.h>
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
uint8_t MCP23017_I2C_read(uint8_t, uint8_t);
uint8_t readData;

int main(void)
{
	//uint8_t readData;
	I2C0_MCP23017_init();   //Initializes  the AVR128DB48 I2C0 to communicate with MCP23017
	
	//Initializes the MCP23017. Port A of the GPIO (GPA) must be configured as
	//an input with pull ups enabled. Port B must be configured as an output.
	MCP23017_I2C_init();
	while (1)
	{
			//To read the value of the switches
			readData = MCP23017_I2C_read(READ_opcode, GPIOAaddr_b1);  
			asm volatile("nop");
			
			//To write to the LEDs output
			MCP23017_I2C_write(WRITE_opcode, OLATBaddr_b1, readData);
	}
}


void I2C0_MCP23017_init(){
	//Baud rate for the I2C
	TWI0.MBAUD = 0;
	
	TWI0.MCTRLA = 1 << TWI_ENABLE_bp; // Enable TWI Master: enabled
	
	//TWI0.DBGCTRL = TWI_DBGRUN_bm;
	
	//To force the I2C bus to its idle state
	TWI0_MSTATUS |= TWI_BUSSTATE_IDLE_gc;      
	
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

//Reads the address and its data which in this case from DIP switches
uint8_t MCP23017_I2C_read(uint8_t opcode, uint8_t address){
	TWI0_MADDR = WRITE_opcode;   //Read the opcode to the address
	
	//Poll until master transmit address of byte write operation is complete regardless
	while(!(TWI0.MSTATUS & TWI_WIF_bm));
	
	
	//Write the address to MADDR which would find the address of whatever register name of MCP23017
	TWI0_MDATA = address;
	
	//Poll until master transmit address of byte write operation is complete regardless
	while(!(TWI0.MSTATUS & TWI_WIF_bm));
	
	//Pass the opcode to the address
	TWI0_MADDR = opcode;
	
	
	//Poll until there is something to read from the slave
	while(!(TWI0.MSTATUS & TWI_RIF_bm));
	

	
	//Acknowledge set meaning slave can send data to the master
	
	//Issue the stop condition meaning done with this action of reading switches
	TWI0_MCTRLB = TWI_MCMD_STOP_gc | TWI_ACKACT_bm;
	
	return TWI0_MDATA;
}


//Write function to write to the MCP23017
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


//Do reverse of bits
