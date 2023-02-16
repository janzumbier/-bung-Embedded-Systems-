/* The following is needed to subtract 0x20 from I/O addresses */ 
#define __SFR_OFFSET 0 

#include <avr/io.h>

;define a name for Register 16 and 17
#define TEMP R16
#define LED_COUNTER R17


.global TIMER1_COMPA_vect
TIMER1_COMPA_vect:
        rcall setLED
        rcall setLED
        inc LED_COUNTER
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
    inc LED_COUNTER
    

    cli
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

setLED:
        ;One’s Complement
        com	LED_COUNTER			
        ;set PORTC	
        out PORTC, LED_COUNTER
        ;One’s Complement
        com LED_COUNTER
        ret