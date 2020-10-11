//#include <blue.h>
#include <dma.h>
#include <gpio.h>
#include <i2c.h>

//auto& ser = Serial1;


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


//const int HEIGHT = 64;
//const int WIDTH = 128;
#define HEIGHT 64
#define WIDTH 128

u8 buffer1306[WIDTH * ((HEIGHT + 7) / 8)];

#define SID 0x3C // I2C 1306 slave ID ... for 64 height display. 32 hieight is 0x3D



// Issue single command to SSD1306, using I2C or hard/soft SPI as needed.
// Because command calls are often grouped, SPI transaction and selection
// must be started/ended in calling function for efficiency.
// This is a private function, not exposed (see ssd1306_command() instead).
void ssd1306_command1(uint8_t c) {
  u8 buf[2];
  buf[0] = 0;
  buf[1] = c;
  write_i2c(SID, buf, 2, true);
}

void send_u8_i2c(u8 c) {
  u8 buff = c;
  send_i2c(&buff, 1, true);
}

void ssd1306_commandList(const uint8_t *c, uint8_t n) {
  begin_i2c(SID, false);
  send_u8_i2c(0x00); // Co = 0, D/C = 0
  while (n--) send_u8_i2c(*c++);
  end_i2c();
}

// silly delay function
void delayish(u32 n)
{
	while(n--) {
		for(int i = 0; i <200; i++) nop();
	}
}


void clear1306() {
  memset(buffer1306, 0, WIDTH * ((HEIGHT + 7) / 8));
}	

bool init1306(uint8_t vcs) {
  clear1306();
  u8 vccstate = vcs;

  //i2caddr = addr ? addr : ((HEIGHT == 32) ? 0x3C : 0x3D);
  init_i2c();

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

void draw_pixel1306(int16_t x, int16_t y, uint16_t color) {
  if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT)) {
    switch (color) {
      case SSD1306_WHITE:
        buffer1306[x + (y / 8) * WIDTH] |= (1 << (y & 7));
        break;
      case SSD1306_BLACK:
        buffer1306[x + (y / 8) * WIDTH] &= ~(1 << (y & 7));
        break;
      case SSD1306_INVERSE:
        buffer1306[x + (y / 8) * WIDTH] ^= (1 << (y & 7));
        break;
    }
  }
}







// REFRESH DISPLAY ---------------------------------------------------------

/*!
    @brief  Push data currently in RAM to SSD1306 display.
    @return None (void).
    @note   Drawing operations are not visible until this function is
            called. Call after each graphics command, or after a whole set
            of graphics commands, as best needed by one's own application.
*/
void display1306(void) {
  static const uint8_t  dlist1[] = {
    SSD1306_PAGEADDR,
    0,                      // Page start address
    0xFF,                   // Page end (not really, but works here)
    SSD1306_COLUMNADDR, 0
  }; // Column start address
  ssd1306_commandList(dlist1, sizeof(dlist1));
  ssd1306_command1(WIDTH - 1); // Column end address


  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr = buffer1306;
  begin_i2c(SID, false);
  send_u8_i2c(0x40);
  while (count--) send_u8_i2c(*ptr++);
  end_i2c();
}




/*!
    @brief  Cease a previously-begun scrolling action.
    @return None (void).
*/
void stopscroll1306(void) {
  ssd1306_command1(SSD1306_DEACTIVATE_SCROLL);
}

void invert1306(bool i) {
  ssd1306_command1(i ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}

/*!
    @brief  Dim the display.
    @param  dim
            true to enable lower brightness mode, false for full brightness.
    @return None (void).
    @note   This has an immediate effect on the display, no need to call the
            display() function -- buffer contents are not changed.
*/
void dim1306(bool dim) {
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  ssd1306_command1(SSD1306_SETCONTRAST);
  ssd1306_command1(dim);
}



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels


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


u8* the_letter = letterP;

void draw_letter(u8 letter[]) {
  clear1306();
  for (int i = 0; i < 8; i++) {
    u8 row = letter[i];
    for (int j = 0; j < 8; j++) {
      u8 on = (row & 1 << 7) ? 1 : 0;
      draw_pixel1306(j, i, on);
      row <<= 1;
    }
  }
  display1306();
  delayish(2000);
}

int main() {
	//ser.begin(115200);
	//ser.println("testing oled");
	
	// SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
	if (!init1306(SSD1306_SWITCHCAPVCC)) {
		//ser.println(F("SSD1306 allocation failed"));
		for (;;);
	}
	  //ser.println("display begun");
	
	 draw_pixel1306(10, 10, SSD1306_WHITE);
	
	  // Show the display buffer on the screen. You MUST call display() after
	  // drawing commands to make them visible on screen!
	  display1306();
	  delayish(2000);
	  // display.display() is NOT necessary after every single drawing command,
	  // unless that's what you want...rather, you can batch up a bunch of
	  // drawing operations and then update the screen all at once by calling
	  // display.display(). These examples demonstrate both approaches...
	
	  draw_letter(the_letter);
	
	while(1);
}

