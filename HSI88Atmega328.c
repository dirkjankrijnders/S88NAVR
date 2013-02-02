#include <avr/io.h>
#include <avr/interrupt.h> 

#define PSC 256
#define T0CNT 15
#define START_TIMER0 TCCR0B |= (1<<CS00)|(1<<CS01) 
#define STOP_TIMER0  TCCR0B &= 0B11111000 


// PIN OUT S88N
#define CLK PD2
#define PS PD3
#define DATA PD4
#define RST PD6

// Defines for USART
#define USART_BAUDRATE 9600
#define UBRR_VALUE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

int modules = 2*2;
int CLKC = 16*1;
enum states {
	IDLE = 0, 
	RESET, 
	PRELOAD, 
	POSTLOAD, 
	CLOCK
};
int state = IDLE;

volatile int data[4];
int module = 0;
int bit = 0;
int t = 0;

void USART0SendByte(uint8_t u8Data);
 
ISR(TIMER0_COMPA_vect) {
	if (CLKC ==0 && state == CLOCK)  {
		CLKC = 16*modules;
		state = IDLE;
		// Stop the clock
		STOP_TIMER0;
		int i = 0;
		for (i = 0; i < modules; i++) {
			USART0SendByte(data[i]);
			data[i] = 0;
		}
		USART0SendByte(bit);
		USART0SendByte(module);
		USART0SendByte(10);

		// Start the clock again
		START_TIMER0;
		
	} else if (state == IDLE) { // && (PIND & (1 << RST))) { // Pulse reset
		PORTD ^= (1<<RST); // Toggle the pin
		state = RESET;
	} else if (state == RESET) {
		PORTD ^= (1<<RST); // Toggle the pin
		state = PRELOAD;
		module = 0;
	} else if (state == PRELOAD && !(PIND & (1 << PS))) {
		PORTD ^= (1<<PS);
	} else if (state == PRELOAD && (PIND & (1 << PS))) {
		CLKC--;
		PORTD ^= (1<<CLK); // Toggle the pin (up)
		state = POSTLOAD;
	} else if (state == POSTLOAD && (PIND & (1 << CLK))) {
		CLKC--;
		PORTD ^= (1<<CLK); // Toggle the pin (down)
		bit = 0;
		t = (PIND & (1<<DATA)) ? 1 : 0 ;
		data[module] |= t;
	} else if (state == POSTLOAD && !(PIND & (1 << CLK))) {
		PORTD ^= (1<<PS);
		state = CLOCK;
	} else if (state == CLOCK) {
		CLKC--;
		PORTD ^= (1<<CLK); // Toggle the pin
		if  (!(PIND & (1 << CLK))) // Down flank
		{ 
			bit++;
			if (bit > 7) {
				module++;
				bit = 0;
			}
			t = (PIND & (1<<DATA)) ? 1 : 0 ;
			data[module] |= (t << bit);
		}
	}
}

void setupt0() {
	TIMSK0 = _BV(OCIE0A); // Interrupt on T0CNT == COMPA
	TCCR0A = _BV(WGM01);  // Timer clear on Compare match
	OCR0A = 8; //T0CNT; // Set the compare value
	START_TIMER0;
}

void setupUSART(){
	// Set baud rate
	UBRR0 = UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C=(1<<7)|(1<<2)|(1<<1);
	//enable transmission and reception
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
};

void USART0SendByte(uint8_t u8Data)
{
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};
	// Transmit data
	UDR0 = u8Data;
}

int main() {
	CLKC = 16*modules;
	/* Main timer is TIMER0. PRESCALER will set to 256 */
	setupt0();

	setupUSART();
 	// Arduino pin 4/PD4 as output
	DDRD = _BV(CLK) | (1 << RST) | (1 << PS);

	sei();

	while(1);
	return 0;
}

