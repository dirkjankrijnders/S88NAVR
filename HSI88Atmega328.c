#include <avr/io.h>
#include <avr/interrupt.h> 
#include <string.h>

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

int modules[3] = {2*2, 0,0};
int CLKC = 16*1;
enum states {
	IDLE = 0, 
	RESET, 
	PRELOAD, 
	POSTLOAD, 
	CLOCK
};

enum mainstates {
	START = 0,
	RECIEVECMD,
	READS88
};

enum readstates {
	FULL = 0,
	DIFF
};

enum modes {
	TERM = 0,
	BIN
};


#define cTERM  't'
#define cINIT  's'
#define cFULL  'm'
#define cVERS  'v'

const char versionInfo[40] = "AVR88-HSI 0.1\r\0";

char commandBuffer[8]; // Max 5 in binary mode, 8 in terminal mode
int commandBufferIndex = 0;
char sendBuffer[18]; // Version string is longest 
int state = IDLE;
int mode = BIN;
int mainState = START;
int readState = FULL;

int data[2][62]; // 31 Modules with 16 bits, so 62 8 bytes... Two buffers to calculate the differences
int currentBuffer = 0;
int diffBuffer = 1;
int module = 0;
int bit = 0;
int t = 0;

void USART0SendByte(uint8_t u8Data);
void USART0SendCString(char* str);
void cmdDispatcher();
 
ISR(TIMER0_COMPA_vect) {
	if (CLKC ==0 && state == CLOCK)  {
		CLKC = 16*modules[0];
		state = IDLE;
		// Stop the clock
		STOP_TIMER0;
		// Disable RX interupt and fire command dispatcher;
		// UCSR0B &= ~((1<<RXEN0)|(1<<RXCIE0));
		// Enable TX
		UCSR0B |= (1<<TXEN0);
		int i = 0;
		USART0SendByte('i');
		diffBuffer = currentBuffer ^ 1;
		if (readstate == FULL) {
			for (i = 0; i < modules[0]/2; i++) {
				USART0SendByte(i+1);
				USART0SendByte(data[currentBuffer][i*2]);
				USART0SendByte(data[currentBuffer][(i*2)+1]);
				data[diffBuffer][i*2] = 0;
				data[diffBuffer][(i*2)+1] = 0;
				currentBuffer = diffBuffer;
			}
		} else {
			int changedModules = 0;
			for (i = 0; i < modules[0]/2; i++) {
				// We reuse the diffBuffer for the data to send, the data itself is not interesting once the difference check has been made.
				if ((data[currentBuffer][i*2] != data[diffBuffer][i*2]) | (data[currentBuffer][(i*2)+1] != data[diffBuffer][(i*2)+1])) {
					changeModules++;
					data[diffBuffer][changedModules*2] = data[currentBuffer][i*2];
					data[diffBuffer][(changedModules*2)+1] = data[currentBuffer][(i*2)+1];
				};
			}
			// Send diffBuffer
			USART0SendByte(changedModules);
			for (i = 0; i < changedModules; i++) {
				USART0SendByte(i);
				USART0SendByte(data[diffBuffer][i*2]);
				USART0SendByte(data[diffBuffer][(i*2)+1]);
			}		
			// Clear the oldest buffer
			for (i = 0; i < modules[0]/2; i++) {
				data[diffBuffer][i*2] = 0;
				data[diffBuffer][(i*2)+1] = 0;
			};

			// Swap buffers
			currentBuffer = diffBuffer;
		};
		USART0SendByte('\r');
		// Disable TX
		UCSR0B &= ~(1<<TXEN0);

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
			data[currentBuffer][module] |= (t << bit);
		}
	}
}

ISR(USART_RX_vect) {
	commandBuffer[commandBufferIndex] = UDR0;
	if (commandBuffer[commandBufferIndex] == '\r') { // End of command
		// Disable RX interupt and fire command dispatcher;
		UCSR0B &= ~((1<<RXEN0)|(1<<RXCIE0));
		// Enable TX
		//UCSR0B |= (1<<TXEN0);
		cmdDispatcher();
		// Disable TX
		// UCSR0B &= ~(1<<TXEN0);
		// Enable RX interupt
		// UCSR0B |= ((1<<RXEN0)|(1<<RXCIE0));
		commandBufferIndex = 0;
	} else {
		mainState = RECIEVECMD;
		// Echo
		//	USART0SendByte(commandBuffer[commandBufferIndex] );
		commandBufferIndex++;
	}
}

void cmdDispatcher() {
	switch (commandBuffer[0]) {
	case cTERM: // Mode change, ignored only binary supported, reply "t0"
		strcpy(sendBuffer, "t0\r\0");
		//sendBuffer = {'t', '0', '\r', '\0'};
		mainState = START;
/*		USART0SendByte('t');
		USART0SendByte('0');
		USART0SendByte('\r');*/
		break;
	case cINIT:
		modules[0]=(int)commandBuffer[1]*2;
		modules[1]=(int)commandBuffer[2]*2;
		modules[2]=(int)commandBuffer[3]*2;
		CLKC = 16*modules[0];
		sendBuffer[0] = 's';
		sendBuffer[1] = (char)((modules[0]+modules[1]+modules[2])/2);
		sendBuffer[2] = '\r';
		sendBuffer[3] = '\0';
		state = IDLE;
		START_TIMER0;
		mainState = READS88;
		readState = FULL;
		break;
	case cFULL:
		state = IDLE;
		START_TIMER0;
		mainState = READS88;
		readState = FULL;
		break;
	case cVERS:
		strcpy(sendBuffer, versionInfo);
		mainState = START;
		break;
	}
	USART0SendCString(sendBuffer);
};

void setupt0() {
	TIMSK0 = _BV(OCIE0A); // Interrupt on T0CNT == COMPA
	TCCR0A = _BV(WGM01);  // Timer clear on Compare match
	OCR0A = 8; //T0CNT; // Set the compare value
}

void setupUSART(){
	// Set baud rate
	UBRR0 = UBRR_VALUE;
	// Set frame format to 8 data bits, no parity, 1 stop bit
	UCSR0C=(1<<7)|(1<<2)|(1<<1);
	//enable receptiondd
};

void USART0SendByte(uint8_t u8Data)
{
	//wait while previous byte is completed
	while(!(UCSR0A&(1<<UDRE0))){};
	// Transmit data
	UDR0 = u8Data;
}

void USART0SendCString(char* str) {
	UCSR0B |= (1<<TXEN0);
	int i = 0;
	while (str[i] != '\0')
		USART0SendByte(str[i++]);
}

int main() {
	CLKC = 16*modules[0];
	/* Main timer is TIMER0. PRESCALER will set to 256 */
	setupt0();

	setupUSART();
 	// Arduino pin 4/PD4 as output
	DDRD = _BV(CLK) | (1 << RST) | (1 << PS);

	sei();

	while(1) {
		switch (mainState) {
		case START:
			// Enable recieve of commands, disable TX
			UCSR0B |= ((1<<RXEN0)|(1<<RXCIE0));
		break;
		case READS88:
			// Disable RX, enable TX
			UCSR0B &= ~((1<<RXEN0)|(1<<RXCIE0));
			UCSR0B |= (1<<TXEN0);

			// Start timer0
			START_TIMER0;
		break;
		}
	};
	return 0;
}

