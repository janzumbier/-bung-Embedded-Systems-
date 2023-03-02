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
#define TEMP2 R21
#define LED_COUNTER R17
#define isInterupt R20

;0=left 1=r
#define dir R18
#define carrySet R19


.global TIMER1_COMPA_vect
TIMER1_COMPA_vect:
        cpi isInterupt, 1
        breq _intSet
        cpi r18, 0
        BREQ _left
        BRNE _right

        _left:
                cpi carrySet, 0
                BREQ _lshift
                rcall setLED
                rol LED_COUNTER
                ldi carrySet, 0
                reti
        _lshift:
                rcall setLED
                lsl LED_COUNTER
                brcs _lcarry
                reti
        _lcarry:
                ldi carrySet, 1
                ldi dir, 1
                reti


        _right:
                cpi carrySet, 0
                BREQ _rshift
                rcall setLED
                ror LED_COUNTER
                ldi carrySet, 0
                reti
        _rshift:
                rcall setLED
                lsr LED_COUNTER
                brcs _rcarry
                reti
        _rcarry:
                ldi carrySet, 1
                ldi dir, 0
                reti
        _intSet:
            reti
.section .text
.global main

.global INT0_vect
INT0_vect:
    ;check if inter allready set
    cpi isInterupt, 1
    breq endInt
    ;set the interrupt boolean
    ldi isInterupt, 1
        reti

    endInt:
        ldi isInterupt, 0
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
    rcall setLED


    cli
    ldi TEMP, (1<<ISC01) | (1<<ISC00)
    sts EICRA, TEMP
    ldi TEMP, (1<<INT0) | (1<<INT1)
    out EIMSK, TEMP
    ldi r18,0
    ldi r19,0
    ;16MHz/1024=15625Hz mit OCR1A=31249 Interrupt alle 2s
    ldi TEMP2, hi8(24999)
    STS OCR1AH, TEMP2
    ldi TEMP2, lo8(24999)
    STS OCR1AL, TEMP2
    ; CTC Modus einschalten
    ; Vorteiler auf 1024
    ldi TEMP2, (1 << WGM12)|(1 << CS12)
    STS TCCR1B, TEMP2
    ldi TEMP2, (1 << OCIE1A) ; OCIE1A: Interrupt bei Timer Compare
    STS TIMSK1, TEMP2
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

_loopInt0:
        ;checks if isInterrupt = 0 =>getOut or 1 => stay in loop
        cpi isInterupt, 0
        breq getOut 
        rjmp _loopInt0
getOut: ret