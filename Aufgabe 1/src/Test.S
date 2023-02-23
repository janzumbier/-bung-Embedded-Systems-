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
.section .text

;******************************* MAIN ************************************
.global main
main:
    ;Data Direction Register DDR for Port C as Output
    ser TEMP
    out	DDRC, TEMP
    ;all LEDs off
    ser TEMP
    out PORTC, TEMP
    clr LED_COUNTER 
    ;endless loop
    rcall setLED

    rcall divide

    _loop1:
        rjmp _loop1

;****************************OUT LED*****************************************
setLED:
    ;One’s Complement
    com	LED_COUNTER			
    ;set PORTC	
	out PORTC,LED_COUNTER
    ;One’s Complement
	com LED_COUNTER
ret

divide:
    ldi r25, hi8 (9)
    ldi r24, lo8 (9)

    _loop:
        inc LED_COUNTER
        sbiw r24, 16
        cpc r24, 0
        breq _result
        brmi _result
    rjmp _loop
    _result:
        rcall setLED
ret


