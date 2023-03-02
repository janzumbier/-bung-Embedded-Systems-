;your comment

;The following is needed to subtract 
;0x20 from I/O addresses
#define __SFR_OFFSET 0
#include "avr/io.h"
; Arduino MEGA2560 PIN51 -- DATA  -- PB2
; Arduino MEGA2560 PIN52 -- CLOCK -- PB1
; Arduino MEGA2560 PIN53 -- SS    -- PB0
; STRIP_com: initializes the SPI
; Parameter: none
; SPCR REG SPI SPR SPR2
; SPSR für SPI2x
.global STRIP_SPI_init
STRIP_SPI_init:
  ; Initialize the data direction registers
  ldi r16, 0b00000111 ; Set PB0-PB2 as outputs
  out DDRB, r16

  ; Initialize the SPI control register
  ldi r16, 0b01011100 ; Enable SPI as Master,
  out SPCR, r16

  ; Initialize the SPI status register for double clock rate to 8MHZ
  ldi r16, 0b00000001 ; Set the SPI2X bit to double the clock rate
  out SPSR, r16

ret
; STRIP_com: implements the protocol to communicate with the LED-Strip
; Parameter: R24
; Values:
;   0 - 255: data
.global STRIP_com
STRIP_com:
  out	SPDR, r24
  _wait:		; Warte bis Senden fertig
    in r17, SPSR
    sbrs r17, SPIF
  rjmp _wait
ret

; STRIP_show: sets a single pixel to the specified color (and switches off all others before it)
; Parameter: r25-r16
; Values:
;   R25, R24 (MSB, LSB)   0 - 2^16: index of pixel to set
;   R22, R20, R18 (R,G,B) 0 - 255:  RGB colors
;   R16                   0-31:     Brigthness
.global STRIP_show
STRIP_show:
  ;save registries
  mov r26, r24           
  mov r27, r25

  mov r30, r24
  mov r31, r25

  ; 4*8 Zero Bytes
  ldi r24, 0
  rcall STRIP_com
  rcall STRIP_com
  rcall STRIP_com
  rcall STRIP_com

  cp r26, 0
  breq _frame

  _loop:
    ldi r24, 0xE0
    rcall STRIP_com
    ldi r24, 0
    rcall STRIP_com
    rcall STRIP_com
    rcall STRIP_com
    sbiw r26,1
  brne _loop

  ;Sending data for frame
_frame: 
  ;1. Sending 111 + brightness
  ldi r24, 0xE0
  add r24, r16
  rcall STRIP_com
  ; sending color
  ;sending blue
  mov r24, r18
  rcall STRIP_com
  ;sending green
  mov r24, r20
  rcall STRIP_com
  ;sending red
  mov r24, r22
  rcall STRIP_com
  ;call END_frame
  mov r30, r24
  mov r31, r25

  rcall END_frame
  ret

 ;END_frame: implements the variable bytes for the End-Frame
 ;Parameter R25, R24 (MSB, LSB)
.global END_frame
END_frame:
  mov r30, r24 
  mov r31, r25
  ldi r24, 0
  
  _divide:
    inc r24
    sbiw r30, 16
    cpc r30, 0
    breq _result
    brmi _result
  rjmp _divide
    
  _result:
    mov r30, r24
    ldi r24, 0
    _sendBytes:
      rcall STRIP_com
      dec r30
    brne _sendBytes
ret
 

;########################################################################################################################################
;