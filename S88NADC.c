#define LED PB6 

#define FOSC 8000000 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h> 

void uart_init(unsigned int ubrr);
void USARTWriteChar(char data);

ISR(ADC_vect) {
   USARTWriteChar(ADCL);
};

int main (void) 
{ 
   DDRB |= (1 << LED); // Set LED1 as output 

   // Setup UART
   uart_init(MYUBRR);
   
   // TODO:  Configure ADC Hardware
   // Set the prescalar to 128 resulting in an ADC clock of 62.5 kHz
   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
   
   // Set the reference voltage to the input
   // ADMUX |= (1 << REFS0);
   
   // Set the channel to 0, no setting required
   
   // Set the ADC in free running mode
   ADCSRA |= (1 << ADATE); 
   
   // Set the ADC in left adjust mode, so we have just eight bits
   ADMUX |= (1 << ADLAR); 
   
   // TODO: Enable ADC 
   ADCSRA |= (1 << ADEN); 
   
   // TODO: Start A2D Conversions 
   ADCSRA |= (1 << ADSC);
   
for(;;)  // Loop Forever 
   { 
      // TODO: Test ADC Value and set LEDs 
   } 
}

void uart_init(unsigned int ubrr) {
  // set baud rate
	UBRR0H = (ubrr>>8);
	UBRR0L = ubrr;
  	// enable just uart TX
    UCSR0B = (1<<TXEN0);
  	// set 8N1 frame format
  	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
};

void USARTWriteChar(char data)
{
   //Wait until the transmitter is ready

   while(!(UCSR0A & (1<<UDRE0)))
   {
      //Do nothing
   }

   //Now write the data to USART buffer

   UDR0=data;
}