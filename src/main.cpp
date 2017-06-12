#include <LiquidCrystal.h>
#include <Arduino.h>
#include <FastLED.h>


char LCDChar[2]="";

unsigned long pgmCurrentMillis;
unsigned long pgmPreviousMillis;
unsigned long uiCurrentMillis;
unsigned long uiPreviousMillis;

// define some values used by the panel and buttons
uint8_t lcd_key     = 0;  //will contain voltage depending on pressed button
uint8_t pgm         = 0;  //will hold the currently selected program
uint8_t pgm_of_the_past = 0;  //will hold the last pushed program
uint8_t targetColor       = 0;
uint8_t currentColor       = 0;

uint16_t adc_key_in  = 0; //will contain Values of up to 1024

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define NUM_LEDS  15
#define DATA_PIN  3

#define PGMDELAY     500

#define UIMENUITEMS 8
#define UIDELAY   500

#define SATURATION 255
#define VALUE 255

// select how many leds does your stehlampe has
CRGB leds[NUM_LEDS];

// select the pins used on the LCD panel:
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup()
{
  lcd.begin(16, 2);              // 16x2 LCD panel
  lcd.setCursor(0,0);
  lcd.print("Stehlampe");
  lcd.setCursor(0,1);
  lcd.print("0.1 morelikely");
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  delay(1000);
  lcd.clear();
}

int read_LCD_buttons(){               // read the buttons
    adc_key_in = analogRead(0);       // read the value from the sensor

    lcd.setCursor(5,1);
    lcd.print(adc_key_in);

    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    // We make this the 1st option for speed reasons since it will be the most likely result

    if (adc_key_in > 1000) return btnNONE;

    // For V1.1 use this threshold
    /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if (adc_key_in < 850)  return btnSELECT;
    */

    // For V1.0 comment the other threshold and use the one below:
    /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
    */

    // my board behaves slightly wiered so I made up my own
     if (adc_key_in < 50)   return btnRIGHT;
     if (adc_key_in < 195)  return btnUP;
     if (adc_key_in < 380)  return btnDOWN;
     if (adc_key_in < 555)  return btnLEFT;
     if (adc_key_in < 790)  return btnSELECT;

    return btnNONE;                // when all others fail, return this.
}

void ui_check()
{
  uiCurrentMillis = millis();
  lcd_key = read_LCD_buttons();   // read the buttons
  switch (lcd_key) {
    case btnUP:{
      if(uiCurrentMillis - uiPreviousMillis > UIDELAY) {
        uiPreviousMillis = uiCurrentMillis;
        if (pgm < UIMENUITEMS) {
          pgm=pgm+1;
          lcd.setCursor(0,0);
          lcd.print(pgm);
        }
        break;
      }
      break;
    }
    case btnDOWN:{
      if(uiCurrentMillis - uiPreviousMillis > UIDELAY) {
        uiPreviousMillis = uiCurrentMillis;
        if (pgm <= 0 ) {
          break;
        }
        pgm=pgm-1;
        lcd.setCursor(0,0);
        lcd.print(pgm);
      }
      break;
    }
    case btnRIGHT:{
      if (pgm == pgm_of_the_past) {
        break;
      }
      //Do something;
      pgm_of_the_past = pgm;
      lcd.setCursor(0,1);
      lcd.print(pgm_of_the_past);
      break;

    }
  }
}


void loop()
{

  ui_check();


  pgmCurrentMillis = millis();

  switch (pgm_of_the_past) {
    case 0:{ //off
      if(pgmCurrentMillis - pgmPreviousMillis > PGMDELAY) {
        leds[0] = CRGB::Black;
        FastLED.show();
        pgmPreviousMillis = pgmCurrentMillis;
      }
      break;
    }
    case 1:{ //test green
      if(pgmCurrentMillis - pgmPreviousMillis > PGMDELAY) {
        leds[0] = CRGB::Green;
        FastLED.show();
        pgmPreviousMillis = pgmCurrentMillis;
      }
      break;
    }
    case 2:{ //test blue
      if(pgmCurrentMillis - pgmPreviousMillis > PGMDELAY) {
        leds[0] = CRGB::Blue;
        FastLED.show();
        pgmPreviousMillis = pgmCurrentMillis;
      }
      break;
    }
    case 3:{ //test red
      if(pgmCurrentMillis - pgmPreviousMillis > PGMDELAY) {
        leds[0] = CRGB::Red;
        FastLED.show();
        pgmPreviousMillis = pgmCurrentMillis;
      }
      break;
    }
    case 4:{//test white
      if(pgmCurrentMillis - pgmPreviousMillis > PGMDELAY) {
        leds[0] = CRGB::White;
        FastLED.show();
        pgmPreviousMillis = pgmCurrentMillis;
      }
      break;
    }
    case 5:{//test fade
      if(pgmCurrentMillis - pgmPreviousMillis > PGMDELAY) {
        if (currentColor == targetColor) {
          targetColor = random8();
        }
        if (currentColor > targetColor) {
          leds[0].setHSV(--currentColor, SATURATION, VALUE);
        }
        if (currentColor < targetColor) {
          leds[0].setHSV(++currentColor, SATURATION, VALUE);
        }
        FastLED.show();
        pgmPreviousMillis = pgmCurrentMillis;
      }
      break;
    }
  }


}
