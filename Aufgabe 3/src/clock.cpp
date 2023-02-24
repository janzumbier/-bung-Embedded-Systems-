
/*
 * your comment
 */

#include <Arduino.h>
#include <avr/io.h>
#include <math.h>
#include <U8g2lib.h>


#define LED_NUM_MAX 30

volatile uint8_t leave_loop = 0;
volatile int64_t counter = 0;

U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);



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
  void sendToDisplay();
  //void timeAsBooleanArray(uint16_t timeInSeconds);
}

void setup(void)
{
    pinMode(11, OUTPUT);
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(0, changeTime, FALLING);
    pinMode(3, INPUT_PULLUP);
    attachInterrupt(1, confirmTime, FALLING);
    sei();
    STRIP_SPI_init();
    STRIP_show(counter,10,0,0,10);
    counter = counter % 86400;
    u8g2.begin();
    setTimeToLed();
    delay(100);
    while (leave_loop != 1)
    {
        
    }
};

void loop(void)
{  
     counter ++;
    counter = counter % 86400;
    setTimeToLed();
    sendToDisplay();
}

void sendToDisplay()
{
    int hours = counter / 3600;
    int min = (counter % 3600) / 60;
    int sec = counter % 60;

    char hoursStr[3];
    char minutesStr[3];
    char secondsStr[3];
    sprintf(hoursStr, "%02u", hours);
    sprintf(minutesStr, "%02u", min);
    sprintf(secondsStr, "%02u", sec);

    // Clear the display
  u8g2.clearBuffer();
  
  // Draw the time on the display
  u8g2.setFont(u8g2_font_logisoso16_tn);
  u8g2.setCursor(0, 30);
  u8g2.print(hoursStr);
  u8g2.print(":");
  u8g2.print(minutesStr);
  u8g2.print(":");
  u8g2.print(secondsStr);
  
  // Send the buffer to the display
  u8g2.sendBuffer();
  delay(1000);
}

// ein durchgehender ton ist ein starkes signal
void changeTime(){
    counter ++;
    counter = counter % 86400;
    setTimeToLed();
}

void confirmTime(){
    leave_loop = 1;
    tone(11, 1, 1870);
}

void setTimeToLed(){
    STRIP_show(30,0,0,0,0);

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
