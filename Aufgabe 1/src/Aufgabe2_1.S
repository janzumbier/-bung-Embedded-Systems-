;**************************************************************************
;Universitaet der Bundeswehr Muenchen
;02.12.2020 UN Version for VS-Code
;2011       Klaus Buchenrieder Initial version
;
;Jan Biermann 1202245
;Philipp Schwerdfeger 1201714
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
    
    _mloop:
        rcall wait_400ms
        rcall setLED
        ;increment LED_COUNTER
	    inc LED_COUNTER
    rjmp _mloop

;****************************OUT LED*****************************************
setLED:
    ;One’s Complement
    com	LED_COUNTER			
    ;set PORTC	
	out PORTC,LED_COUNTER
    ;One’s Complement
	com LED_COUNTER
ret

wait_10ms:
    ; Initialize loop counter 4*40000 = 160000
    ldi r25, hi8 (40000) 
    ldi r24, lo8 (40000)
    wloop:       ; 1 cyc
     sbiw r24, 1 ; 1 cyc
    brne wloop   ; 2 cyc
; Delay complete
ret

wait_400ms:
    ldi r18, 40
    aloop:
        rcall wait_10ms
        dec r18
    brne aloop
ret







