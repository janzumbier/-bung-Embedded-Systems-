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
volatile int counter = 0;

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
            // hier uhrzeit erhöhen
            sw1_pressed = 0;
            counter ++
            // mod sekunden pro tag damit nur 24 stunden
            counter = counter % 86400
        }
    }



    // Eintragen des Counters als LED Wert
    return 0;
}


bool[] timeAsBooleanArray(timeInSeconds: int){
    bool out[20] = {false};

    int hours = timeInSeconds / 3600;
    int min = (timeInSeconds % 3600) / 60;
    int sec = timeInSeconds % 60;

    // stunden h1h2 (14:00) ==> h1=1 h2=4
    int h2 = hours % 10
    int h1 = (hours - h2) / 10
    // minuten m1m2 (14:32) ==> m1=3 m2=2
    int m2 = min % 10
    int m1 = (min - m2) / 10
    // sekunden s1s2 (14:32:43) ==> s1=4 s2=3
    int s2 = sec % 10
    int s1 = (sec - m2) / 10

    int i = 0
    int ex = 1
    //h1
    while(i<=1){
        if(h1 >= 2^ex){
            out[i] = true
            h1 = h1 - 2^i
        }
        else out[i] = false

        ex --
    }
    //h2
    ex = 3
    while(i<=5){
        if(h2 >= 2^ex){
            out[i] = true
            h2 = h2 - 2^ex
        }
        else out[i] = false
        ex --
    }
    //m1
    ex = 2
    while(i<=8){
        int ex = i-6
        if(m1 >= 2^ex){
            out[i] = true
            m1 = m1 - 2^ex
        }
        else out[i] = false
        ex --
    }
    //m2
    ex = 3
    while(i<=12){
        int ex = i-9
        if(m2 >= 2^ex){
            out[i] = true
            m2 = m2 - 2^ex
        }
        else out[i] = false
        ex --
    }
    //s1
    ex = 2
    while(i<=15){
        int ex = i-13
        if(s1 >= 2^ex){
            out[i] = true
            s1 = s1 - 2^ex
        }
        else out[i] = false
        ex --
    }
    //s2
    ex = 3
    while(i<=19){
        int ex = i-16
        if(s2 >= 2^ex){
            out[i] = true
            s2 = s2 - 2^ex
        }
        else out[i] = false
        ex --
    }


    return out
}
