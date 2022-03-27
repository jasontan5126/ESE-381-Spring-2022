/*
 * ASCII_str_to_MAX5402.c
 *
 * Created: 3/18/2022 9:10:56 PM
 * Author : jason
 */ 

#define F_CPU 4000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 *(float)BAUD_RATE)))  //Calculation of baud rate from data sheet
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


/* UART Buffer Defines */
#define USART_RX_BUFFER_SIZE 16     /* 2,4,8,16,32,64,128 or 256 bytes */
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )

#if ( USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif


//The switch statement labeled FSM creates a FSM to parse the command
//string received in Task 3. You will have to analyze its operation
//to answer some of the questions for this laboratory.

//You will need to include the following declarations in your code
//as global variables. Accordingly, place the outside of all functions.
unsigned char sdr;	//serial data received
uint8_t MAX5402_data;	//data to be written to MAX5402
uint8_t pstate = 0; //present state
uint8_t d2, d1, d0;	//digits of the decimal value received
uint32_t decimal;		//binary value equal to decimal value received

/* Static Variables */
static unsigned char USART_RxBuf[USART_RX_BUFFER_SIZE];
static volatile unsigned char USART_RxHead;
static volatile unsigned char USART_RxTail;

/* Prototypes */
void USART3_Init( unsigned int baudrate );
unsigned char USART3_Receive(void);
void MAX5402_SPI0_write(uint8_t);
void FSMFunction(uint8_t);

static int i= 1;  //To loop through the RX array to get the character that will be parsed
int j = 0;  //Number of iterations to get the 6 ASCII values after user entered the desired format
static int temp = 1;
char c;
int receiveFlag = 0;


int main(void)
{
	unsigned int baudRate = 9600; //Baud rate value
	
	USART3_Init(baudRate);  //function to initialize for USART
	sei();   //Enable interrupts => enable USART3 interrupts
	while(1)
	{
		//_delay_us(10000);
		//_delay_us(10100);
		if(!(VPORTB_IN & PIN2_bm) || (VPORTC_IN & PIN2_bm)){
			//Delay loop to get all the characters entered by user from 'V' to 0x0A
			for(int k= 0 ; k < 10000; k++){
				asm volatile("nop");
			}
		//	asm volatile("nop");
		//	asm volatile("nop");
		//	asm volatile("nop");
		//	asm volatile("nop");
		//	asm volatile("nop");
			
			//Loop to get the characters that will be parsed from the RX_buffer
			while(j < 6){
				i = temp;
				if(i < 16){
					FSMFunction(USART_RxBuf[i]);
					i++;
					
				}
				else{
					i = 0;
					FSMFunction(USART_RxBuf[i]);
					i++;
					
				}
				j++;
				temp = i;
			}
			j = 0;
			
		}
		
		PORTC_OUT &= ~PIN2_bm;
	}
}


//Initialize USART and also the SPI configuration 
void USART3_Init(unsigned int baudrate){
	unsigned char x;
	
	PORTF_DIR |= PIN2_bm;   //Chose PF2 as the output to drive the /CS input
	PORTA_DIR |= PIN6_bm | ~(PIN5_bm) | PIN4_bm ;          //Configure as output where the pin level is controlled by the SPI
	SPI0_CTRLA = SPI_MASTER_bm | SPI_CLK2X_bm | SPI_ENABLE_bm;  //Enable the SPI and also Select the SPI master/slave operation by writing the Master/Slave Select
	//(MASTER) bit in the Control A (SPIn.CTRLA) register
	SPI0_CTRLB |= SPI_SSD_bm;   //Enable  the SPI by writing a 1 to the enable bit
	
	PORTC_DIR |= PIN2_bm;  //Dummy pin to know when there's data to be received to be sent to the SPI
	
	PORTB_DIR &= PIN1_bm;  //Set PB1 as the input (RX pin)
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(baudrate);   //Taken from data sheet to calculate baud rate
	USART3.CTRLB |= USART_RXEN_bm;  //Enable USART receiver
	USART3.CTRLA |= USART_RXCIE_bm;   //Enable the Receive complete interrupt
	
	//Guess set the default to being asynchronous, disable parity, 1 stop bit, 8 bits
	
	//Flush receive buffer
	x = 0;
	
	USART_RxTail = x;
	USART_RxHead = x;
}

//Interrupt service routine for receiving data from the Termite buffer
ISR(USART3_RXC_vect){
	unsigned char data;
	unsigned char tmphead;
	
	//Read the received data
	data = USART3_RXDATAL;
	
	/*Calculate the buffer index */
	tmphead = (USART_RxHead + 1) & USART_RX_BUFFER_MASK;
	USART_RxHead = tmphead;   //Store new index	
	
	if(tmphead == USART_RxTail){
	
	}
	PORTC_OUT |= PIN2_bm;
	USART_RxBuf[tmphead] = data;  //Store received data in buffer
}


//The function for doing the FSM and parsing the characters in the correct format entered by the user
void FSMFunction(uint8_t sdr1){
	switch (pstate)
	{
		case 0:
			//Read the received data
			//sdr = USART3_RXDATAL;
			if (sdr1 == 'V'){
				pstate = 1;
			}
			else
			pstate = 0;
			break;
			
			
		
		case 1:
			
			if ((sdr1 >= '0') && (sdr1 <= '9'))
			{
				d2 = sdr1 & 0x0F;
				pstate = 2;
			}
			else
				pstate = 0;
				break;
			
			
		
		case 2:
			if ((sdr1 >= '0') && (sdr1 <= '9'))
			{
				d1 = sdr1 & 0x0F;
				pstate = 3;
			}
			else
				pstate = '0';
			break;
		
		case 3:
			if ((sdr1 >= '0') && (sdr1 <= '9'))
			{
				d0 = sdr1 & 0x0F;
				pstate = 4;
			}
			else
				pstate = 0;
			break;
		
		case 4:
			if (sdr1 == 0x0d)
				pstate = 5;
			else
				pstate = 0;
			break;
		
		case 5:
			if (sdr1 == 0x0a)
			{
				pstate = 0;
				decimal = (((d2 * 10) + d1) * 10) + d0;
				MAX5402_data = (uint8_t)(((decimal) * 255)/333);	
				MAX5402_SPI0_write(MAX5402_data);  //Send the compute the binary value to be sent to the
				//MAX5402 to output this voltage value	
			}
			else
				pstate = 0;
			break;
		
		default:
			pstate = 0;
	}
}




//Read and write function
unsigned char USART3_Receive(void){
	unsigned char tmptail;
	
	while(USART_RxHead == USART_RxTail);  //Wait for incoming data
	
	tmptail = (USART_RxTail + 1) & USART_RX_BUFFER_MASK;  //Calculate buffer index
	
	USART_RxTail = tmptail;  //Store new index
	
	return USART_RxBuf[tmptail];    //return data
	
}



/*
the value to be send to the MAX5402 to set the position of the its wiper.
It is important that before this function returns, it must deselect the
MAX5402. If this is not done, there could be a subsequent SPI bus
conflict between the MAX5402 and other (future) SPI devices added to
the system.

*/
void MAX5402_SPI0_write(uint8_t data){
	
	PORTF_OUT &= ~PIN2_bm;   //Set to be 0 when transmission is happening
	
	//Poll until the  transmit buffer register are empty
	//when they contain data that has not been moved to
	//transmit shift register
	
	
	SPI0_DATA = data;
	
	//Poll to check if the sending of Serial data to the slave is done for the SPI
	while (!(SPI0_INTFLAGS & SPI_IF_bm))
	{
		;
	}
	
	PORTF_OUT |= PIN2_bm;
	
}


//Data received in the buffer
unsigned char DataInReceiveBuffer(void){
	/* Return 0 (FALSE) if the receive buffer is empty */
	return (USART_RxHead != USART_RxTail);
}

