#include <avr/io.h>

# Port definitions:
S88LOAD = PD3
S88CLK=  PD2
S88DATAIN = PB7
S88DATAOUT = PD0
LED = PB6 

# buffer is stored in r25:r24
bufh = r24
bufl = r25

# register to save SREG
inttmp  =  19

	.section .text
	
	.global main
main:
	rcall ioinit
1:
	rjmp 1b
	
	.global INT0_vect
INT0_vect:  ; Clock
	sbis _SFR_IO_ADDR(PIND), S88CLK  ; Skip next line if clock is high
	rjmp clock_low
	sbrs r24, 0  ; lsb set
	rjmp data_low
	cbi _SFR_IO_ADDR(PORTD), S88DATAOUT
	rjmp rotate_buf
data_low:
	sbi _SFR_IO_ADDR(PORTD), S88DATAOUT
	
rotate_buf:
	in  r19, _SFR_IO_ADDR(SREG) ; Up to now we didn't touch SREG and toggling the pin is more important!'
	lsr r25
	ror r24
	rjmp int0_done
clock_low:
	sbis _SFR_IO_ADDR(PIND), S88DATAIN
	sbr r24, 0x80
int0_done:	
	out     _SFR_IO_ADDR(SREG), r19
	reti
	
	.global INT1_vect
INT1_vect:  ; Load
	in  r19, _SFR_IO_ADDR(SREG) ; Up to now we didn't touch SREG and toggling the pin is more important!'
	sbi _SFR_IO_ADDR(PIND), LED ; toggle the led
#	ldi	r24, 0xF0
#	ldi	r25, 0xCE
	#  The four msb of D become the lsb of the buffer
	in      r23, _SFR_IO_ADDR(PIND)
	swap    r23 ; Swap nibbles
	andi    r23, 0x0F
	or      r25, r23
        # All bits of B become the 5-10 bits of the buffer
	in      r23, _SFR_IO_ADDR(PINB)
	swap    r23
	or	r24, r23
	andi	r24, 0x03
	andi    r23, 0xF0
	or	r25, r23
	# The lsb of C become the 11-16 bits of the buffer
	in	r23, _SFR_IO_ADDR(PINC)
	lsl	r23
	lsl	r23
	or	r25, r23
	out     _SFR_IO_ADDR(SREG), r19
	reti

ioinit:
	# Set port direction:
	sbi _SFR_IO_ADDR(DDRD), S88DATAOUT
	sbi _SFR_IO_ADDR(DDRD), LED
	
	# Set pullup on intputs
	ldi	r19, 0xFF;
	out	_SFR_IO_ADDR(PORTC), r19;
	out	_SFR_IO_ADDR(PORTB), r19;
	ldi	r19, 0xF0;
	out	_SFR_IO_ADDR(PORTD), r19;
	# Set interrupts on load en clock lines:
	lds	r24, EICRA
    ori	r24, 0x01
    sts	EICRA, r24
    sbi	0x1d, 0

    lds	r24, EICRA
    ori	r24, 0x04;
    sts	EICRA, r24
        
    lds	r24, EICRA
    ori	r24, 0x08	
    sts	EICRA, r24

    sbi	0x1d, 1	
	
	# Enable interrupts
	sei
	
	# done!
	ret

	
	.global __vector_default
__vector_default:
	reti

	.end
	
	
