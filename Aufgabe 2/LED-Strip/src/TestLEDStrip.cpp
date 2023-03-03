//Jan Biermann 1202245
//Philipp Schwerdfeger 1201714

#include <Arduino.h>
#include <avr/io.h>
#define LED_NUM_MAX 30

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
  initStrip();
}

/*void loop()
{
  // LED-ON
  // Start-Frame
  STRIP_com(0);
  STRIP_com(0);
  STRIP_com(0);
  STRIP_com(0);
  // LED-Frame
  STRIP_com(0xE5); // Brigtness 5
  STRIP_com(0x0A); // Blue
  STRIP_com(0);    // Green
  STRIP_com(0);    // Red
  // END-Frame
  STRIP_com(0);
  delay(500);
  // LED-OFF
  // Start-Frame
  STRIP_com(0);
  STRIP_com(0);
  STRIP_com(0);
  STRIP_com(0);
  // LED-Frame
  STRIP_com(0xE0); // Brigtness 0
  STRIP_com(0);    // Blue
  STRIP_com(0);    // Green
  STRIP_com(0);    // Red
  // END-Frame
  STRIP_com(0);
  delay(500);
  
}
*/


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