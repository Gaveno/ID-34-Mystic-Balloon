#ifndef Arglib_h
#define Arglib_h

#include <SPI.h>
#include <Print.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <limits.h>
#include <Arduino.h>
#include <EEPROM.h>
#include <avr/pgmspace.h>

// main hardware compile flags

#if !defined(ARDUBOY_10) && !defined(AB_DEVKIT)
/// defaults to Arduboy Release 1.0 if not using a boards.txt file
/**
   we default to Arduboy Release 1.0 if a compile flag has not been
   passed to us from a boards.txt file

   if you wish to compile for the devkit without using a boards.txt
   file simply comment out the ARDUBOY_10 define and uncomment
   the AB_DEVKIT define like this:

       // #define ARDUBOY_10
       #define AB_DEVKIT
*/
#define ARDUBOY_10   //< compile for the production Arduboy v1.0
//#define AB_DEVKIT    //< compile for the official dev kit
#endif

// EEPROM settings

#define EEPROM_VERSION 0
#define EEPROM_BRIGHTNESS 1
#define EEPROM_AUDIO_ON_OFF 2
// we reserve the first 16 byte of EEPROM for system use
#define EEPROM_STORAGE_SPACE_START 16 // and onward

// eeprom settings above are needed for audio

#define PIXEL_SAFE_MODE
#define SAFE_MODE

#ifdef AB_DEVKIT
#define CS 6
#define DC 4
#define RST 12
#else
#define CS 12
#define DC 4
#define RST 6
#endif

// compare Vcc to 1.1 bandgap
#define ADC_VOLTAGE _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1)
// compare temperature to 2.5 internal reference
// also _BV(MUX5)
#define ADC_TEMP _BV(REFS0) | _BV(REFS1) | _BV(MUX2) | _BV(MUX1) | _BV(MUX0)

#ifdef AB_DEVKIT
#define LEFT_BUTTON _BV(5)
#define RIGHT_BUTTON _BV(2)
#define UP_BUTTON _BV(4)
#define DOWN_BUTTON _BV(6)
#define A_BUTTON _BV(1)
#define B_BUTTON _BV(0)

#define PIN_LEFT_BUTTON 9
#define PIN_RIGHT_BUTTON 5
#define PIN_UP_BUTTON 8
#define PIN_DOWN_BUTTON 10
#define PIN_A_BUTTON A0
#define PIN_B_BUTTON A1

#define PIN_SPEAKER_1 A2
#define PIN_SPEAKER_2 A2
// SPEAKER_2 is purposely not defined for DEVKIT as it could potentially
// be dangerous and fry your hardware (because of the devkit wiring).
//
// Reference: https://github.com/Arduboy/Arduboy/issues/108
#else
#define LEFT_BUTTON _BV(5)
#define RIGHT_BUTTON _BV(6)
#define UP_BUTTON _BV(7)
#define DOWN_BUTTON _BV(4)
#define A_BUTTON _BV(3)
#define B_BUTTON _BV(2)

#define PIN_LEFT_BUTTON A2
#define PIN_RIGHT_BUTTON A1
#define PIN_UP_BUTTON A0
#define PIN_DOWN_BUTTON A3
#define PIN_A_BUTTON 7
#define PIN_B_BUTTON 8

#define PIN_SPEAKER_1 5
#define PIN_SPEAKER_2 13
#endif

#define WIDTH 128
#define HEIGHT 64

#define WHITE 1
#define BLACK 0

#define COLUMN_ADDRESS_END (WIDTH - 1) & 0x7F
#define PAGE_ADDRESS_END ((HEIGHT/8)-1) & 0x07

#define SPRITE_MASKED 1
#define SPRITE_UNMASKED 2
#define SPRITE_OVERWRITE 2
#define SPRITE_PLUS_MASK 3
#define SPRITE_IS_MASK 250
#define SPRITE_IS_MASK_ERASE 251
#define SPRITE_AUTO_MODE 255

class ArduboyAudio
{
  public:
    void setup();
    void on();
    void off();
    void saveOnOff();
    bool enabled();
    void tone(unsigned int frequency, unsigned long duration);

  protected:
    bool audio_enabled = false;
};

struct Rect
{
  public:
    int x;
    int y;
    uint8_t width;
    uint8_t height;
};

struct Point
{
  public:
    int x;
    int y;
};

class Arduboy : public Print
{
  public:
    Arduboy();
    void LCDDataMode();
    void LCDCommandMode();

    uint8_t getInput();
    void poll();
    boolean pressed(uint8_t buttons);
    boolean notPressed(uint8_t buttons);
    boolean justPressed(uint8_t buttons);
    void start();
    void saveMuchPower();
    void idle();
    void blank();
    void clearDisplay();
    void display();
    void drawScreen(const unsigned char *image);
    void drawScreen(unsigned char image[]);
    void drawPixel(int x, int y, uint8_t color);
    uint8_t getPixel(uint8_t x, uint8_t y);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
    void drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint8_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint8_t color);
    void fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint8_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint8_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint8_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint8_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
    void fillScreen(uint8_t color);
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color);
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint8_t color);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint8_t color);
    void drawCompressed(int16_t sx, int16_t sy, const uint8_t *bitmap, uint8_t color);
    unsigned char* getBuffer();
    uint8_t width();
    uint8_t height();
    virtual size_t write(uint8_t);
    void initRandomSeed();
    void swap(int16_t& a, int16_t& b);

    //ArduboyTunes tunes;
    ArduboyAudio audio;

    void setFrameRate(uint8_t rate);
    bool nextFrame();
    bool everyXFrames(uint8_t frames);
    int cpuLoad();
    uint8_t frameRate = 60;
    uint16_t frameCount = 0;
    uint8_t eachFrameMillis = 1000 / 60;
    long lastFrameStart = 0;
    long nextFrameStart = 0;
    bool post_render = false;
    uint8_t lastFrameDurationMs = 0;

    bool static collide(Point point, Rect rect);
    bool static collide(Rect rect, Rect rect2);

  private:
    unsigned char sBuffer[(HEIGHT * WIDTH) / 8];

    void bootLCD() __attribute__((always_inline));
    void safeMode() __attribute__((always_inline));
    void slowCPU() __attribute__((always_inline));
    uint8_t readCapacitivePin(int pinToMeasure);
    uint8_t readCapXtal(int pinToMeasure);
    uint16_t rawADC(byte adc_bits);
    volatile uint8_t *mosiport, *clkport, *csport, *dcport;
    uint8_t mosipinmask, clkpinmask, cspinmask, dcpinmask;
    uint8_t currentButtonState = 0;
    uint8_t previousButtonState = 0;
};


/////////////////////////////////
//      sprites by Dreamer3    //
/////////////////////////////////
class Sprites
{
  public:
    Sprites(Arduboy &arduboy);
    
    // drawExternalMask() uses a separate mask to mask image (MASKED)
    //
    // image  mask   before  after
    //
    // .....  .OOO.  .....   .....
    // ..O..  OOOOO  .....   ..O..
    // OO.OO  OO.OO  .....   OO.OO
    // ..O..  OOOOO  .....   ..O..
    // .....  .OOO.  .....   .....
    //
    // image  mask   before  after
    //
    // .....  .OOO.  OOOOO   O...O
    // ..O..  OOOOO  OOOOO   ..O..
    // OO.OO  OOOOO  OOOOO   OO.OO
    // ..O..  OOOOO  OOOOO   ..O..
    // .....  .OOO.  OOOOO   O...O
    //
    void drawExternalMask(int16_t x, int16_t y, const uint8_t *bitmap, const uint8_t *mask, uint8_t frame, uint8_t mask_frame);

    // drawPlusMask has the same behavior as drawExternalMask except the
    // data is arranged in byte tuples interposing the mask right along
    // with the image data (SPRITE_PLUS_MASK)
    //
    // typical image data (8 bytes):
    // [I][I][I][I][I][I][I][I]
    //
    // interposed image/mask data (8 byes):
    // [I][M][I][M][I][M][I][M]
    //
    // The byte order does not change, just for every image byte you mix
    // in it's matching mask byte.  Softare tools make easy work of this.
    //
    // See: https://github.com/yyyc514/img2ard
    void drawPlusMask(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);

    // drawOverwrite() replaces the existing content completely (UNMASKED)
    //
    // image  before  after
    //
    // .....  .....   .....
    // ..O..  .....   ..O..
    // OO.OO  .....   OO.OO
    // ..O..  .....   ..O..
    // .....  .....   .....
    //
    // image  before  after
    //
    // .....  OOOOO   .....
    // ..O..  OOOOO   ..O..
    // OO.OO  OOOOO   OO.OO
    // ..O..  OOOOO   ..O..
    // .....  OOOOO   .....
    //
    void drawOverwrite(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);

    // drawErase() removes the lit pixels in the image from the display
    // (SPRITE_IS_MASK_ERASE)
    //
    // image  before  after
    //
    // .....  .....   .....
    // ..O..  .....   .....
    // OO.OO  .....   .....
    // ..O..  .....   .....
    // .....  .....   .....
    //
    // image  before  after
    //
    // .....  OOOOO   OOOOO
    // ..O..  OOOOO   OO.OO
    // OO.OO  OOOOO   ..O..
    // ..O..  OOOOO   OO.OO
    // .....  OOOOO   OOOOO
    //

    void drawErase(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);

    // drawSelfMasked() only draws lit pixels, black pixels in
    // your image are treated as "transparent" (SPRITE_IS_MASK)
    //
    // image  before  after
    //
    // .....  .....   .....
    // ..O..  .....   ..O..
    // OO.OO  .....   OO.OO
    // ..O..  .....   ..O..
    // .....  .....   .....
    //
    // image  before  after
    //
    // .....  OOOOO   OOOOO  (no change because all pixels were
    // ..O..  OOOOO   OOOOO  already white)
    // OO.OO  OOOOO   OOOOO
    // ..O..  OOOOO   OOOOO
    // .....  OOOOO   OOOOO
    //
    void drawSelfMasked(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame);
    // master function, needs to be abstracted into sep function for
    // every render type
    void draw(int16_t x, int16_t y, const uint8_t *bitmap, uint8_t frame, const uint8_t *mask, uint8_t sprite_frame, uint8_t drawMode);
    void drawBitmap(int16_t x, int16_t y, const uint8_t *bitmap, const uint8_t *mask, int8_t w, int8_t h, uint8_t draw_mode);

  private:

    Arduboy *arduboy;
    unsigned char *sBuffer;
};

#endif
