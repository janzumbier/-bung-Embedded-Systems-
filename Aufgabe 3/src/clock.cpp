
/*
 * your comment
 */

#include <Arduino.h>
#include <avr/io.h>
#include <math.h>

#define LED_NUM_MAX 30

volatile uint8_t leave_loop = 0;
volatile int counter = 0;



extern "C"
{
  // function prototypes
  void STRIP_SPI_init(void);
  void STRIP_com(uint8_t one_byte);
  void STRIP_show(uint16_t index, char r, char g, char b, char bright);
  void END_frame(uint16_t count);
  void changeTime();
  void confirmTime();
  void setTimeToLed();
  //void timeAsBooleanArray(uint16_t timeInSeconds);
}

void setup()
{
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(0, changeTime, FALLING);

    pinMode(3, INPUT_PULLUP);
    attachInterrupt(1, confirmTime, FALLING);
    sei();

    STRIP_SPI_init();
    //STRIP_show(counter,10,0,0,10);
    setTimeToLed();
    delay(100);



    while (leave_loop != 1)
    {
        
    }
    

};

void loop()
{
  sei();  

  //LED - ON
  STRIP_show(counter, 0, 0, 10, 5);
  delay(1000);
  counter ++;
}

void changeTime(){
    counter ++;
    setTimeToLed();
}

void confirmTime(){
    leave_loop = 1;
}

void setTimeToLed(){
    int hours = counter / 3600;
    int min = (counter % 3600) / 60;
    int sec = counter % 60;

    // stunden h1h2 (14:00) ==> h1=1 h2=4
    int h2 = hours % 10;
    int h1 = (hours - h2) / 10;
    // minuten m1m2 (14:32) ==> m1=3 m2=2
    int m2 = min % 10;
    int m1 = (min - m2) / 10;
    // sekunden s1s2 (14:32:43) ==> s1=4 s2=3
    int s2 = sec % 10;
    int s1 = (sec - m2) / 10;

    int i = 29;
    int ex = 1;
    
    //hours first (lower leds getturnded of)
    while(ex >= 0){
        if(h1 >= pow(2,ex)){
            STRIP_show(i, 0,0,10,10);
            h1 = h1 - pow(2,ex);
        }
        ex --;
        i --;
    }
    // red light between digits
    STRIP_show(i, 10,0,0,10);
    i --;

    ex = 3;
    while(ex >= 0){
        if(h2 >= pow(2,ex)){
            STRIP_show(i, 0,0,10,10);
            h2 = h2 - pow(2,ex);
        }
        ex --;
        i --;
    }

    STRIP_show(i,10,0,0,10);
    i--;
    STRIP_show(i,10,0,0,10);
    i--;
    

    //minutes next (lower leds getturnded of)
    ex = 2;
    while(ex >= 0){
        if(m1 >= pow(2,ex)){
            STRIP_show(i, 0,0,10,10);
            m1 = m1 - pow(2,ex);
        }
        ex --;
        i --;
    }
    // red light between digits
    STRIP_show(i, 10,0,0,10);
    i --;

    ex = 3;
    while(ex >= 0){
        if(m2 >= pow(2,ex)){
            STRIP_show(i, 0,0,10,10);
            m2 = m2 - pow(2,ex);
        }
        ex --;
        i --;
    }

    STRIP_show(i,10,0,0,10);
    i--;
    STRIP_show(i,10,0,0,10);
    i--;

    //seconds next (lower leds getturnded of)
    ex = 2;
    while(ex >= 0){
        if(s1 >= pow(2,ex)){
            STRIP_show(i, 0,0,10,10);
            s1 = s1 - pow(2,ex);
        }
        ex --;
        i --;
    }
    // red light between digits
    STRIP_show(i, 10,0,0,10);
    i --;

    ex = 3;
    while(ex >= 0){
        if(s2 >= pow(2,ex)){
            STRIP_show(i, 0,0,10,10);
            s2 = s2 - pow(2,ex);
        }
        ex --;
        i --;
    }

}
  
/*
void timeAsBooleanArray(timeInSeconds: int16_t){

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
            STRIP_show(i,1,0,0,5)
            out[i] = true
            h1 = h1 - 2^i
        }

        ex --
    }
    //h2
    ex = 3
    while(i<=5){
        if(h2 >= 2^ex){
            STRIP_show(i,1,0,0,5)
            h2 = h2 - 2^ex
        }
        ex --
    }
    //m1
    ex = 2
    while(i<=8){
        int ex = i-6
        if(m1 >= 2^ex){
            STRIP_show(i,1,0,0,5)
            m1 = m1 - 2^ex
        }
        ex --
    }
    //m2
    ex = 3
    while(i<=12){
        int ex = i-9
        if(m2 >= 2^ex){
            STRIP_show(i,1,0,0,5)
            m2 = m2 - 2^ex
        }
        ex --
    }
    //s1
    ex = 2
    while(i<=15){
        int ex = i-13
        if(s1 >= 2^ex){
            STRIP_show(i,1,0,0,5)
            s1 = s1 - 2^ex
        }
        ex --
    }
    //s2
    ex = 3
    while(i<=19){
        int ex = i-16
        if(s2 >= 2^ex){
            STRIP_show(i,1,0,0,5)
            s2 = s2 - 2^ex
        }
        ex --
    }
}
*/
