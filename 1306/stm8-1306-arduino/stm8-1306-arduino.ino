/*
   2020-10-12 under construction. It semi-works. Contents are nonsense.
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


#define HEIGHT  64
#define WIDTH  128
//u8 buffer1306[WIDTH * ((HEIGHT + 7) / 8)];

#define SID 0x3C // I2C 1306 slave ID ... for 64 height display. 32 hieight is 0x3D


void send2(u8 b1, u8 b2) {
  u8 buf = b2;
  //buf[0] = b1;
  //buf[1] = b2;
  I2C_write_sn(SID, b1, &buf, 1);
}
void ssd1306_command1(uint8_t c) {
  /*
  u8 buf[2];
  buf[0] = 0;
  buf[1] = c;
  write_i2c(SID, buf, 2);
  */
  send2(0, c);
}

void send_u8_i2c(u8 c) {
  u8 buff = c;
  //send_i2c(&buff, 1);
  I2C_write(SID, c);
}

void ssd1306_commandList(const uint8_t *c, uint8_t n) {
  /*
  begin_i2c(SID);
  send_u8_i2c(0x00); // Co = 0, D/C = 0
  while (n--) send_u8_i2c(*c++);
  end_i2c();
  */
  I2C_write_sn(SID, 0x00, c, n);
}


bool init1306(uint8_t vcs) {
  //clear1306();
  u8 vccstate = vcs;

  //i2caddr = addr ? addr : ((HEIGHT == 32) ? 0x3C : 0x3D);
  //init_i2c();

  uint8_t comPins = 0x02;
  u8 contrast = 0x8F;

  if ((WIDTH == 128) && (HEIGHT == 32)) {
    comPins = 0x02;
    contrast = 0x8F;
  } else if ((WIDTH == 128) && (HEIGHT == 64)) {
    comPins = 0x12;
    contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF;
  } else if ((WIDTH == 96) && (HEIGHT == 16)) {
    comPins = 0x2; // ada x12
    contrast = (vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0xAF;
  } else {
    // Other screen varieties -- TBD
  }



  u8 init1[] = {
    SSD1306_DISPLAYOFF,         // 0xAE
    SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
    0x80, // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX, // 0xA8
    HEIGHT - 1,
    SSD1306_SETDISPLAYOFFSET, // 0xD3
    0x0,                      // no offset
    SSD1306_SETSTARTLINE | 0x0, // line #0
    SSD1306_CHARGEPUMP,        // 0x8D
    (vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14,
    SSD1306_MEMORYMODE, // 0x20
    0x00, // 0x0 act like ks0108
    SSD1306_SEGREMAP | 0x1,
    SSD1306_COMSCANDEC,
    SSD1306_SETCOMPINS, comPins,
    SSD1306_SETCONTRAST, contrast,
    SSD1306_SETPRECHARGE, // 0xd9
    (vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1,
    SSD1306_SETVCOMDETECT, // 0xDB
    0x40,
    SSD1306_DISPLAYALLON_RESUME, // 0xA4
    SSD1306_NORMALDISPLAY,       // 0xA6
    SSD1306_DEACTIVATE_SCROLL,
    SSD1306_DISPLAYON
  };
  ssd1306_commandList(init1, sizeof(init1));

  return true;
}



void  low_level_test() {
  static const uint8_t  dlist1[] = {
    SSD1306_PAGEADDR,
    1,                      // Page start address
    1,                   // Page end (not really, but works here)
    SSD1306_COLUMNADDR, 0, 7
  };
  ssd1306_commandList(dlist1, sizeof(dlist1));
  /*
  begin_i2c(SID);
  send_u8_i2c(0x40);
  send_u8_i2c(0b10101010);
  end_i2c();
  */
  send2(0x40, 0b10101010);
  //ssd1306_command1(WIDTH - 1); // Column end address
}


void setup() {
  //Wire.begin();
  I2C_begin();
  init1306(SSD1306_SWITCHCAPVCC);
  //I2C_end(); // dunno if needed
  low_level_test();

}
void loop() {
}
