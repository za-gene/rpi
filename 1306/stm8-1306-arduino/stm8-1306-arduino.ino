/*
   2021-02-02 under construction, but working.
*/

//#include "Wire8.h"
#include <I2C.h>
//#include "I2C.h"

typedef uint8_t u8;
typedef uint32_t u32;

#define SSD1306_BLACK 0   ///< Draw 'off' pixels
#define SSD1306_WHITE 1   ///< Draw 'on' pixels
#define SSD1306_INVERSE 2 ///< Invert pixels

#define SSD1306_MEMORYMODE 0x20          ///< See datasheet
#define SSD1306_COLUMNADDR 0x21          ///< See datasheet
#define SSD1306_PAGEADDR 0x22            ///< See datasheet
#define SSD1306_SETCONTRAST 0x81         ///< See datasheet
#define SSD1306_CHARGEPUMP 0x8D          ///< See datasheet
#define SSD1306_SEGREMAP 0xA0            ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON 0xA5        ///< Not currently used
#define SSD1306_NORMALDISPLAY 0xA6       ///< See datasheet
#define SSD1306_INVERTDISPLAY 0xA7       ///< See datasheet
#define SSD1306_SETMULTIPLEX 0xA8        ///< See datasheet
#define SSD1306_DISPLAYOFF 0xAE          ///< See datasheet
#define SSD1306_DISPLAYON 0xAF           ///< See datasheet
#define SSD1306_COMSCANINC 0xC0          ///< Not currently used
#define SSD1306_COMSCANDEC 0xC8          ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET 0xD3    ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5  ///< See datasheet
#define SSD1306_SETPRECHARGE 0xD9        ///< See datasheet
#define SSD1306_SETCOMPINS 0xDA          ///< See datasheet
#define SSD1306_SETVCOMDETECT 0xDB       ///< See datasheet

#define SSD1306_SETLOWCOLUMN 0x00  ///< Not currently used
#define SSD1306_SETHIGHCOLUMN 0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE 0x40  ///< See datasheet

#define SSD1306_EXTERNALVCC 0x01  ///< External display voltage source
#define SSD1306_SWITCHCAPVCC 0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26              ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27               ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A  ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL 0x2E                    ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F                      ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3             ///< Set scroll range


#define WIDTH  128

#if 0
  #define HEIGHT  32
#else  
  #define HEIGHT  64
#endif


#define SID 0x3C

void draw_glyph(u8 n);

void send2(u8 b1, u8 b2) {
  u8 buf = b2;
  I2C_write_sn(SID, b1, &buf, 1);
}
void ssd1306_command1(uint8_t c) {
  send2(0x80, c);
}


void ssd1306_commandList(const uint8_t *c, uint8_t n) {
  for (int i = 0; i < n; i++) {
    send2(0x80, c[i]);
  }
}


void home()
{
  static const uint8_t  dlist1[] = {
    SSD1306_PAGEADDR,
    0,                      // Page start address
    HEIGHT/8-1,                   // Page end (not really, but works here)
    SSD1306_COLUMNADDR, 0, 127
  };
  ssd1306_commandList(dlist1, sizeof(dlist1));
}

void clr_scr() {
  home();
  for(int i = 0; i< HEIGHT/8*128/5; i++)
  draw_glyph(10);
  
}

bool init1306() {
  uint8_t comPins = 0x12;
  if (HEIGHT == 32) comPins = 0x02;

  u8 init1[] = {
    SSD1306_DISPLAYOFF,         // 0xAE
    SSD1306_SETDISPLAYCLOCKDIV, 0x80, // 0xD5
    SSD1306_SETMULTIPLEX, HEIGHT - 1, // 0xA8
    
    SSD1306_SETDISPLAYOFFSET, 0x00, // 0xD3
    SSD1306_SETSTARTLINE | 0x0, // line #0
    SSD1306_CHARGEPUMP, 0x14,       // 0x8D
    SSD1306_MEMORYMODE, 0, // 0x20
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDEC,
    SSD1306_SETCOMPINS, comPins,
    SSD1306_SETCONTRAST, 0x7F,
    SSD1306_SETPRECHARGE, 0xF1, // 0xd9
    SSD1306_SETVCOMDETECT, 0x40, // 0xDB
    SSD1306_DISPLAYALLON_RESUME, // 0xA4
    SSD1306_NORMALDISPLAY,       // 0xA6
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYON
  };
  ssd1306_commandList(init1, sizeof(init1));
  clr_scr();
  home();

  return true;
}

uint8_t digital_font5x7_123[] =
{
  0x36, 0x41, 0x41, 0x36, 0x00, // char '0' (0x30/48)
  0x00, 0x00, 0x00, 0x36, 0x00, // char '1' (0x31/49)
  0x30, 0x49, 0x49, 0x06, 0x00, // char '2' (0x32/50)
  0x00, 0x49, 0x49, 0x36, 0x00, // char '3' (0x33/51)
  0x06, 0x08, 0x08, 0x36, 0x00, // char '4' (0x34/52)
  0x06, 0x49, 0x49, 0x30, 0x00, // char '5' (0x35/53)
  0x36, 0x49, 0x49, 0x30, 0x00, // char '6' (0x36/54)
  0x00, 0x01, 0x01, 0x36, 0x00, // char '7' (0x37/55)
  0x36, 0x49, 0x49, 0x36, 0x00, // char '8' (0x38/56)
  0x06, 0x49, 0x49, 0x36, 0x00, // char '9' (0x39/57)
  0x00, 0x00, 0x00, 0x00, 0x00, // 10 space
};


void send_data(u8* buf, int len)
{
  I2C_write_sn(SID, 0x40, buf, len);
}

void draw_glyph(u8 n) {
  send_data(digital_font5x7_123 + n * 5, 5);
}

void  low_level_test() {
  home();
 
  draw_glyph(9);
  draw_glyph(1);
  draw_glyph(2);
}


void setup() {
  //Wire.begin();
  I2C_begin();
  init1306();
  //I2C_end(); // dunno if needed
  low_level_test();

}
void loop() {
}
