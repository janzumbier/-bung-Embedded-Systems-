/* The following is needed to subtract 0x20 from I/O addresses */ 
#define __SFR_OFFSET 0 

#include <avr/io.h>

;define a name for Register 16 and 17
#define TEMP R16
#define LED_COUNTER R17
;0=left 1=r
#define dir R18
#define carrySet R19


.global TIMER1_COMPA_vect
TIMER1_COMPA_vect:
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

.section .text
.global main

main:
    clr TEMP
    ser TEMP
    out	DDRC, TEMP
    ;all LEDs off
    ser TEMP
    out PORTC, TEMP
    clr LED_COUNTER
    ldi LED_COUNTER,1
    cli
    ldi r18,0
    ldi r19,0
    ;16MHz/1024=15625Hz mit OCR1A=31249 Interrupt alle 2s
    ldi TEMP, 0x7A
    STS OCR1AH, TEMP
    ldi TEMP, 0x11
    STS OCR1AL, TEMP
    ; CTC Modus einschalten
    ; Vorteiler auf 1024
    ldi TEMP, (1 << WGM12)|(1 << CS10)|(1 << CS12)
    STS TCCR1B, TEMP

    ldi TEMP, (1 << OCIE1A) ; OCIE1A: Interrupt bei Timer Compare
    STS TIMSK1, TEMP
    sei
    

    LOOP:
        rjmp LOOP
; ##############################################################################################
setLED:
        ;One’s Complement
        com	LED_COUNTER			
        ;set PORTC	
        out PORTC, LED_COUNTER
        ;One’s Complement
        com LED_COUNTER
ret

 