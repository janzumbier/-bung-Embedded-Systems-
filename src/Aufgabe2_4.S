; INT0_vect
; INT1_vect

;**************************************************************************
;Universitaet der Bundeswehr Muenchen
;02.12.2020 UN Version for VS-Code
;2011       Klaus Buchenrieder Initial version
;
;your comment
;
;**************************************************************************
/* The following is needed to subtract 0x20 from I/O addresses */ 
#define __SFR_OFFSET 0 

#include <avr/io.h>

;define a name for Register 16 and 17
#define TEMP R16
#define LED_COUNTER R17

.global INT0_vect
INT0_vect:
    inc LED_COUNTER
    rcall setLED
    reti

;******************************* MAIN ************************************
.global main
.section .text
main:
    ;Data Direction Register DDR for Port C as Output
    ser TEMP
    out	DDRC, TEMP
    ;all LEDs off
    ser TEMP
    out PORTC, TEMP
    clr LED_COUNTER
    inc LED_COUNTER 
    ;endless loop
    rcall setLED
    cli
    ldi TEMP, (1<<ISC01) | (1<<ISC00)
    sts EICRA, TEMP
    ldi TEMP, (1<<INT0)
    out EIMSK, TEMP
    sei

    LOOP:
        rjmp LOOP
;****************************OUT LED*****************************************
setLED:
    ;One’s Complement
    com	LED_COUNTER			
    ;set PORTC	
	out PORTC,LED_COUNTER
    ;One’s Complement
	com LED_COUNTER
ret
