/*
 * MCP23017_inout_interrupt_test.c
 *
 * Created: 3/30/2022 10:34:43 PM
 * Author : jason
 */ 

#include <avr/io.h>
#define F_CPU 4000000
#include <util/delay.h>
#include <avr/interrupt.h>

//Defines for GIO, b1 means when 8-bit mode, BANK = 1
#define IOCONaddr_b0 0x0A   //address at reset, default 16-bit mode
#define IOCONaddr_b1 0x05	//Control registers
#define IODIRAaddr_b1 0x00	//Directional register for PORTA
#define IODIRBaddr_b1 0x10	//Directional register for PORTB
#define GPPUAaddr_b1 0x06   //What is responsible for enabling pull ups at GPA inputs
#define GPIOAaddr_b1 0x09	//to get switch values at GPA port
#define OLATBaddr_b1 0x1A	// control bargraph at GPB port.
#define WRITE_opcode 0x40
#define READ_opcode 0x41
#define GPINTENA 0x02		//For the INTA interrupt of the GPIO pin

//Functional Prototypes
void I2C0_MCP23017_init();
void MCP23017_I2C_init();
void MCP23017_I2C_write(uint8_t, uint8_t, uint8_t);
uint8_t MCP23017_I2C_read(uint8_t, uint8_t);

//Global variable to get the data read from the DIP switches
uint8_t readData;

//Interrupt service routine for if PF3 is triggered for the interrupt 
//to occur
ISR(PORTF_PORT_vect){
	
	//To go start reading from the DIP switches
	readData = MCP23017_I2C_read(READ_opcode, GPIOAaddr_b1);
	
	//To output what is read from the DIP switches to the LEDs
	MCP23017_I2C_write(WRITE_opcode, OLATBaddr_b1, readData);
	
	//Clear the interrupt flag
	VPORTF_INTFLAGS |= PIN3_bm;
}


int main(void)
{
	//I guess configure PF3 as input but I guess not necessary
	PORTF_DIR &= ~PIN3_bm;
	
    	I2C0_MCP23017_init();   //Initializes  the AVR128DB48 I2C0 to communicate with MCP23017
    
	//Initializes the MCP23017. Port A of the GPIO (GPA) must be configured as
    	//an input with pull ups enabled. Port B must be configured as an output.
    	MCP23017_I2C_init();
	
	//Low level for interrupt to occur
	PORTF_PIN3CTRL |= PORT_ISC_LEVEL_gc;
	sei();  //Enable the interrupt
    	while (1) 
    	{
		asm volatile("nop");   //Do nothing
    	}
}

//Reads the address and its data which in this case from DIP switches 
//which are connected to GPA pins
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


//Initializes the AVR128DB48's I2C0 to communicate with the MCP23017.
//The bit transfer rate between the AVR128DB48 and the MCP23017 must be
//as fast as possible, but less than or equal to 400 kb/s.
void I2C0_MCP23017_init(){
	//Baud rate for the I2C
	TWI0.MBAUD = 0;
	
	TWI0.MCTRLA = 1 << TWI_ENABLE_bp; // Enable TWI Master: enabled
	
	TWI0_MSTATUS |= TWI_BUSSTATE_IDLE_gc;      //To force the I2C bus to its idle state
	
}

//This function initializes the MCP23017. Port A of the GPIO (GPA) must be
//configured as all inputs with pull ups enabled. GPB must be
//configured as all outputs.
void MCP23017_I2C_init(){
	MCP23017_I2C_write(WRITE_opcode, IOCONaddr_b0, 0xA0);	
	MCP23017_I2C_write(WRITE_opcode, IODIRAaddr_b1, 0xFF);  //Set GPIOA as inputs
	MCP23017_I2C_write(WRITE_opcode, GPPUAaddr_b1, 0xFF);	//Enable pull up resistors on GPIOA inputs 
	MCP23017_I2C_write(WRITE_opcode, IODIRBaddr_b1, 0x00);	//Configure GPIOB as outputs 
	MCP23017_I2C_write(WRITE_opcode, GPINTENA, 0xFF);	 //If a bit is set, the corresponding pin is enabled for interrupt-on-change
	
}



//This function is what write to the actual GPIO expander MCP23017
//to access the registers and modifying those bit fields
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
