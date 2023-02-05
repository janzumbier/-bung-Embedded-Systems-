/* The following is needed to subtract 0x20 from I/O addresses */ 
#define __SFR_OFFSET 0 

#include <avr/io.h>

;define a name for Register 16 and 17
#define TEMP R16
#define LED_COUNTER R24
.section .text

.global main
main:
    ;Data Direction Register DDR for Port C as Output
    ser TEMP
    out	DDRC, TEMP
    ;all LEDs off
    ser TEMP
    out PORTC, TEMP
    clr LED_COUNTER 
    ;LED => 00000001
    inc LED_COUNTER
    ;LED => 10000000
    ror LED_COUNTER
    rcall setLED
;*******************************running light********************************
_loopRight:
    rcall wait_400ms
    lsr LED_COUNTER
    rcall setLED
    brcs _loopLeft
rjmp _loopRight

_loopLeft:
    rcall wait_400ms
    lsl LED_COUNTER
    rcall setLED
    brcs _loopRight
rjmp _loopLeft
;****************************OUT LED*****************************************
setLED:
    ;One’s Complement
    com	LED_COUNTER			
    ;set PORTC	
	out PORTC,LED_COUNTER
    ;One’s Complement
	com LED_COUNTER
ret

;******************************wait functions******************************
wait_10ms:
    ldi r25, hi8 (160000)
    ldi r24, lo8 (160000)
    wloop: 
     sbiw r24, 1
    brne wloop
ret

wait_400ms:
    ldi r18, 10
    aloop:
        rcall wait_10ms
        dec r18
    brne aloop
ret