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
#define isInterupt R18

.global INT0_vect
INT0_vect:
    ;check if inter allready set
    cpi isInterupt, 1
    breq getOut
    ;set the interrupt boolean
    ldi isInterupt, 1
    
    _loopInt0:
        ;checks if isInterrupt = 0 =>getOut or 1 => stay in loop
        cpi isInterupt, 0
        breq getOut 
    rjmp _loopInt0

    getOut:
        ldi isInterupt, 0
reti

.global INT1_vect
INT1_vect:
    clr isInterupt
    reti

;******************************* MAIN ************************************
.global main
.section .text
main:
    clr isInterupt
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
    ldi TEMP, (1<<INT0) | (1<<INT1)
    out EIMSK, TEMP
    sei

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

