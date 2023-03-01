/* The following is needed to subtract 0x20 from I/O addresses */ 
#define __SFR_OFFSET 0 

#include <avr/io.h>

;define a name for Register 16 and 17
#define TEMP R16
#define LED_COUNTER R17
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
    inc LED_COUNTER
    rcall _loopLeft
;*******************************running light********************************
    _loopRightbefore:
        rcall wait_400ms
        rcall setLED
        ror LED_COUNTER
        _loopRight:
            rcall wait_400ms
            rcall setLED
            lsr LED_COUNTER
            brcs _loopLeftBefore
        rjmp _loopRight

    ret
    _loopLeftBefore:
        rcall wait_400ms
        rcall setLED
        rol LED_COUNTER
        _loopLeft:
            rcall wait_400ms
            rcall setLED
            lsl LED_COUNTER
            brcs _loopRightbefore
        rjmp _loopLeft
    ret

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
    ; Initialize loop counter
    ldi r25, hi8 (40000) 
    ldi r24, lo8 (40000)
    wloop: 
     sbiw r24, 1
    brne wloop
; Delay complete
ret

wait_400ms:
    ldi r18, 40
    aloop:
        rcall wait_10ms
        dec r18
    brne aloop
ret