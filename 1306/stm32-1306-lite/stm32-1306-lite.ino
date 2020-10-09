/*
This is the core graphics library for all our displays, providing a common
set of graphics primitives (points, lines, circles, etc.).  It needs to be
paired with a hardware-specific library for each display device we carry
(to handle the lower-level functions).

Adafruit invests time and resources providing this open source code, please
support Adafruit & open-source hardware by purchasing products from Adafruit!

Copyright (c) 2013 Adafruit Industries.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
 */
 
#include <Wire.h>

typedef uint8_t u8;

/// The following "raw" color names are kept for backwards client compatability
/// They can be disabled by predefining this macro before including the Adafruit
/// header client code will then need to be modified to use the scoped enum
/// values directly
#ifndef NO_ADAFRUIT_SSD1306_COLOR_COMPATIBILITY
#define BLACK SSD1306_BLACK     ///< Draw 'off' pixels
#define WHITE SSD1306_WHITE     ///< Draw 'on' pixels
#define INVERSE SSD1306_INVERSE ///< Invert pixels
#endif
/// fit into the SSD1306_ naming scheme
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


/*!
    @brief  Class that stores state and functions for interacting with
            SSD1306 OLED displays.
*/
class Adafruit_SSD1306  {
  public:
    // NEW CONSTRUCTORS -- recommended for new projects
    Adafruit_SSD1306(uint8_t w, uint8_t h, TwoWire *twi = &Wire,
                     uint32_t clkDuring = 400000UL,
                     uint32_t clkAfter = 100000UL);

    ~Adafruit_SSD1306(void);

    bool begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0,
               bool reset = true, bool periphBegin = true);
    void display(void);
    void clearDisplay(void);
    void invertDisplay(bool i);
    void dim(bool dim);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void stopscroll(void);
    void ssd1306_command(uint8_t c);
    int HEIGHT = 64;
    int WIDTH = 128;

  private:
    void ssd1306_command1(uint8_t c);
    void ssd1306_commandList(const uint8_t *c, uint8_t n);


    TwoWire *wire;
    uint8_t *buffer;
    int8_t i2caddr, vccstate, page_end;
    int8_t mosiPin, clkPin, dcPin, csPin, rstPin;

    uint32_t wireClk;    // Wire speed for SSD1306 transfers
    uint32_t restoreClk; // Wire speed following SSD1306 transfers
    uint8_t contrast; // normal contrast setting for this device
};


// SOME DEFINES AND STATIC VARIABLES USED INTERNALLY -----------------------

#if defined(BUFFER_LENGTH)
#define WIRE_MAX BUFFER_LENGTH ///< AVR or similar Wire lib
#elif defined(SERIAL_BUFFER_SIZE)
#define WIRE_MAX (SERIAL_BUFFER_SIZE - 1) ///< Newer Wire uses RingBuffer
#else
#define WIRE_MAX 32 ///< Use common Arduino core default
#endif

#define WIRE_WRITE wire->write ///< Wire write function in recent Arduino lib

#define TRANSACTION_START  wire->setClock(wireClk);
#define TRANSACTION_END wire->setClock(restoreClk);


Adafruit_SSD1306::Adafruit_SSD1306(uint8_t w, uint8_t h, TwoWire *twi,
                                   uint32_t clkDuring,
                                   uint32_t clkAfter)
  :  wire(twi ? twi : & Wire), buffer(NULL),
     mosiPin(-1), clkPin(-1), dcPin(-1), csPin(-1)
#if ARDUINO >= 157
  ,
     wireClk(clkDuring), restoreClk(clkAfter)
#endif
{
}


/*!
    @brief  Destructor for Adafruit_SSD1306 object.
*/
Adafruit_SSD1306::~Adafruit_SSD1306(void) {
  if (buffer) {
    free(buffer);
    buffer = NULL;
  }
}


// Issue single command to SSD1306, using I2C or hard/soft SPI as needed.
// Because command calls are often grouped, SPI transaction and selection
// must be started/ended in calling function for efficiency.
// This is a private function, not exposed (see ssd1306_command() instead).
void Adafruit_SSD1306::ssd1306_command1(uint8_t c) {
  wire->beginTransmission(i2caddr);
  WIRE_WRITE((uint8_t)0x00); // Co = 0, D/C = 0
  WIRE_WRITE(c);
  wire->endTransmission();
}

// Issue list of commands to SSD1306, same rules as above re: transactions.
// This is a private function, not exposed.
void Adafruit_SSD1306::ssd1306_commandList(const uint8_t *c, uint8_t n) {
  wire->beginTransmission(i2caddr);
  WIRE_WRITE((uint8_t)0x00); // Co = 0, D/C = 0
  uint8_t bytesOut = 1;
  while (n--) {
    if (bytesOut >= WIRE_MAX) {
      wire->endTransmission();
      wire->beginTransmission(i2caddr);
      WIRE_WRITE((uint8_t)0x00); // Co = 0, D/C = 0
      bytesOut = 1;
    }
    WIRE_WRITE(*c++);
    bytesOut++;
  }
  wire->endTransmission();
}

void Adafruit_SSD1306::ssd1306_command(uint8_t c) {
  TRANSACTION_START
  ssd1306_command1(c);
  TRANSACTION_END
}

bool Adafruit_SSD1306::begin(uint8_t vcs, uint8_t addr, bool reset,
                             bool periphBegin) {

  if ((!buffer) && !(buffer = (uint8_t *)malloc(WIDTH * ((HEIGHT + 7) / 8))))
    return false;

  clearDisplay();


  vccstate = vcs;

  // Setup pin directions
  if (wire) { // Using I2C
    // If I2C address is unspecified, use default
    // (0x3C for 32-pixel-tall displays, 0x3D for all others).
    i2caddr = addr ? addr : ((HEIGHT == 32) ? 0x3C : 0x3D);
    // TwoWire begin() function might be already performed by the calling
    // function if it has unusual circumstances (e.g. TWI variants that
    // can accept different SDA/SCL pins, or if two SSD1306 instances
    // with different addresses -- only a single begin() is needed).
    if (periphBegin)
      wire->begin();
  } else {
  }




  uint8_t comPins = 0x02;
  contrast = 0x8F;

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

  TRANSACTION_START


  u8 init1[] = {
    // init1
    SSD1306_DISPLAYOFF,         // 0xAE
    SSD1306_SETDISPLAYCLOCKDIV, // 0xD5
    0x80, // the suggested ratio 0x80
    SSD1306_SETMULTIPLEX, // 0xA8

    HEIGHT - 1,
    // init2

    SSD1306_SETDISPLAYOFFSET, // 0xD3
    0x0,                      // no offset
    SSD1306_SETSTARTLINE | 0x0, // line #0
    SSD1306_CHARGEPUMP,        // 0x8D

    (vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14,

    // init3
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



  TRANSACTION_END

  return true; // Success
}

void Adafruit_SSD1306::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT)) {
    switch (color) {
      case SSD1306_WHITE:
        buffer[x + (y / 8) * WIDTH] |= (1 << (y & 7));
        break;
      case SSD1306_BLACK:
        buffer[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
        break;
      case SSD1306_INVERSE:
        buffer[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
        break;
    }
  }
}


void Adafruit_SSD1306::clearDisplay(void) {
  memset(buffer, 0, WIDTH * ((HEIGHT + 7) / 8));
}





// REFRESH DISPLAY ---------------------------------------------------------

/*!
    @brief  Push data currently in RAM to SSD1306 display.
    @return None (void).
    @note   Drawing operations are not visible until this function is
            called. Call after each graphics command, or after a whole set
            of graphics commands, as best needed by one's own application.
*/
void Adafruit_SSD1306::display(void) {
  TRANSACTION_START
  static const uint8_t PROGMEM dlist1[] = {
    SSD1306_PAGEADDR,
    0,                      // Page start address
    0xFF,                   // Page end (not really, but works here)
    SSD1306_COLUMNADDR, 0
  }; // Column start address
  ssd1306_commandList(dlist1, sizeof(dlist1));
  ssd1306_command1(WIDTH - 1); // Column end address


  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr = buffer;
  wire->beginTransmission(i2caddr);
  WIRE_WRITE((uint8_t)0x40);
  uint8_t bytesOut = 1;
  while (count--) {
    if (bytesOut >= WIRE_MAX) {
      wire->endTransmission();
      wire->beginTransmission(i2caddr);
      WIRE_WRITE((uint8_t)0x40);
      bytesOut = 1;
    }
    WIRE_WRITE(*ptr++);
    bytesOut++;
  }
  wire->endTransmission();

  TRANSACTION_END

}




/*!
    @brief  Cease a previously-begun scrolling action.
    @return None (void).
*/
void Adafruit_SSD1306::stopscroll(void) {
  TRANSACTION_START
  ssd1306_command1(SSD1306_DEACTIVATE_SCROLL);
  TRANSACTION_END
}

void Adafruit_SSD1306::invertDisplay(bool i) {
  TRANSACTION_START
  ssd1306_command1(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
  TRANSACTION_END
}

/*!
    @brief  Dim the display.
    @param  dim
            true to enable lower brightness mode, false for full brightness.
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            display() function -- buffer contents are not changed.
*/
void Adafruit_SSD1306::dim(bool dim) {
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  TRANSACTION_START
  ssd1306_command1(SSD1306_SETCONTRAST);
  ssd1306_command1(dim ? 0 : contrast);
  TRANSACTION_END
}

auto& ser = Serial1;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);



u8 letterH[] = {
  0b10000010,
  0b10000010,
  0b10000010,
  0b11111110,
  0b10000010,
  0b10000010,
  0b10000010,
  0b00000000
};

u8 letterP[] = { 
  0b11111100,
  0b10000010,
  0b10000010,
  0b11111100,
  0b10000000,
  0b10000000,
  0b10000000,
  0b00000000
};


void setup() {
  ser.begin(115200);
  ser.println("testing oled");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    ser.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  ser.println("display begun");

  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  draw_letter(letterP);

}
void loop() {
}




void draw_letter(u8 letter[]) {
  display.clearDisplay();
  for(int i=0; i<8; i++) {
    u8 row = letter[i];
    for(int j=0; j<8; j++) {
      u8 on = (row & 1<<7) ? 1 : 0;
      display.drawPixel(j, i, on);
      row <<= 1;
    }
    
    //display.drawPixel(i, 0, row);
  }
  display.display();
  delay(2000);
}
