/*
 * your comment
 */

#include <Arduino.h>
#include <avr/io.h>

#define LED_NUM_MAX 30

#define SW2_PIN 5
#define SW1_PIN 4

volatile uint8_t sw2_pressed = 0;
volatile uint8_t sw1_pressed = 0;
volatile uint8_t leave_loop = 0;

ISR(PCINT2_vect) {  
    if (bit_is_set(PINB, SW2_PIN)) {
        sw2_pressed = 1;
    }
}

ISR(PCINT1_vect) {  
    if (bit_is_set(PINB, SW1_PIN)) {
        sw1_pressed = 1;
    }
}


extern "C"
{
  // function prototypes
  void STRIP_SPI_init(void);
  void STRIP_com(uint8_t one_byte);
  void STRIP_show(uint16_t index, char r, char g, char b, char bright);
  void END_frame(uint16_t count);
}

void initStrip(void);

void setup()
{
    
    DDRB &= ~((1 << SW2_PIN) | (1 << SW1_PIN));  
    PORTB |= ((1 << SW2_PIN) | (1 << SW1_PIN));  

    PCICR |= (1 << PCIE0);  
    PCMSK0 |= ((1 << SW2_PIN) | (1 << SW1_PIN));  
    sei();  

    while (!leave_loop) {
        if (sw2_pressed) {
            // hier aus schleife raus
            sw0_pressed = 0;
            leave_loop = 1;  
        }
        if (sw1_pressed) {
            // hier uhrzeit ändern
            sw1_pressed = 0;
        }
    }

    return 0;
}

void loop()
{
  //LED - ON
  STRIP_show(LED_NUM_MAX - 1, 0, 0, 10, 5);
  STRIP_show(0, 0, 0, 10, 5);
  delay(500);
  //LED - OFF
  STRIP_show(LED_NUM_MAX, 0, 0, 0, 0);
  delay(500);
}

void initStrip()
{
  STRIP_SPI_init();
}