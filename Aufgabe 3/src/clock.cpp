
/*
 * your comment
 */

#include <Arduino.h>
#include <avr/io.h>
#include <math.h>
#include <U8g2lib.h>
#include <time.h>


#define LED_NUM_MAX 30

volatile uint8_t leave_loop = 0;
volatile int64_t counter = 86390;
volatile int16_t day = 28;
volatile int16_t month = 2;
volatile int16_t year = 2023;
int days_in_month[]={31,28,31,30,31,30,31,31,30,31,30,31};
int days_in_month_schalt[] = {31,29,31,30,31,30,31,31,30,31,30,31};
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
    if(counter == 86400){
        day++;
    }
    counter = counter % 86400;



    u8g2.begin();
    setTimeToLed();
    delay(100);
    while (leave_loop != 1)
    {
        sendToDisplay();
        delay(100);
    }
};

void loop(void)
{  
    counter ++;
    if(counter == 86400){
        day++;
    }
    counter = counter % 86400;
    setTimeToLed();
    sendToDisplay();
    delay(1000);
}

void sendToDisplay()
{
    
    int hours = counter / 3600;
    int min = (counter % 3600) / 60;
    int sec = counter % 60;
    
    if(day == 32 && month == 12){
        year = year + 1;
        month = 1;
        day = 1;
    }
    if(((year% 4 == 0 && year%100 != 0) || year%400 == 0))
    {
        if(day == days_in_month_schalt[month-1]+1)
        {
            day = 1;
            month = month + 1;
        }
        
    } 
    else 
    {
        if(day == days_in_month[month-1]+1)
        {
            day = 1;
            month = month + 1;
        }

    }


    

    char hoursStr[3];
    char minutesStr[3];
    char secondsStr[3];
    sprintf(hoursStr, "%02u", hours);
    sprintf(minutesStr, "%02u", min);
    sprintf(secondsStr, "%02u", sec);

    char yearStr[5];
    char monthStr[3];
    char dayStr[3];
    sprintf(yearStr, "%02u", year);
    sprintf(monthStr, "%02u", month);
    sprintf(dayStr, "%02u", day);

    // Clear the display
    u8g2.clearBuffer();
  
  // Draw the time on the display
    u8g2.setFont(u8g2_font_tinytim_tr);
    u8g2.setCursor(0, 10);
    u8g2.print(hoursStr);
    u8g2.print(":");
    u8g2.print(minutesStr);
    u8g2.print(":");
    u8g2.print(secondsStr);

    u8g2.setCursor(0, 30);
    u8g2.print(dayStr);
    u8g2.print(".");
    u8g2.print(monthStr);
    u8g2.print(".");
    u8g2.print(yearStr);



  
  // Send the buffer to the display
  u8g2.sendBuffer();
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
