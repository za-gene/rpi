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

//#include <Wire.h>

//#include <gpio.h>

auto& ser = Serial1;

typedef uint8_t u8;
typedef uint32_t u32;

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


#define SID 0x3C


void write_i2c(u8 sid, const u8* buffer, u32 len);
void begin_i2c(u8 sid);
void end_i2c();
void  send_i2c(const u8* buffer, u32 len);
void init_i2c();


void pu32(char* str, u32 v) {
  ser.print(str);
  ser.print(v);
  ser.print(" 0b");
  for (int i = 0; i < 8; i++) {
    if (i) ser.print("'");
    for (int j = 0; j < 4; j++) {
      //u32 x = v & (0b10000000000000000000000000000000);
      u32 x = v & (1 << 31);
      if (x > 0) {
        ser.print(1);
      } else {
        ser.print(0);
      }

      v = (v << 1);
    }
  }
  ser.println("");

}

/*!
    @brief  Class that stores state and functions for interacting with
            SSD1306 OLED displays.
*/
class Adafruit_SSD1306  {
  public:
    // NEW CONSTRUCTORS -- recommended for new projects
    Adafruit_SSD1306(uint8_t w, uint8_t h,
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


    //TwoWire *wire;
    uint8_t *buffer;
    int8_t i2caddr, vccstate, page_end;
    //int8_t mosiPin, clkPin, dcPin, csPin, rstPin;

    uint32_t wireClk;    // Wire speed for SSD1306 transfers
    uint32_t restoreClk; // Wire speed following SSD1306 transfers
    uint8_t contrast; // normal contrast setting for this device
};




Adafruit_SSD1306::Adafruit_SSD1306(uint8_t w, uint8_t h,
                                   uint32_t clkDuring,
                                   uint32_t clkAfter)
  :   buffer(NULL)
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
  u8 buf[2];
  buf[0] = 0;
  buf[1] = c;
  write_i2c(i2caddr, buf, 2);
}

void send_u8_i2c(u8 c) {
  u8 buff = c;
  send_i2c(&buff, 1);
}

// Issue list of commands to SSD1306, same rules as above re: transactions.
// This is a private function, not exposed.
void Adafruit_SSD1306::ssd1306_commandList(const uint8_t *c, uint8_t n) {
  begin_i2c(i2caddr);
  send_u8_i2c(0x00); // Co = 0, D/C = 0
  while (n--) send_u8_i2c(*c++);
  end_i2c();
}

void Adafruit_SSD1306::ssd1306_command(uint8_t c) {
  ssd1306_command1(c);
}

bool Adafruit_SSD1306::begin(uint8_t vcs, uint8_t addr, bool reset,
                             bool periphBegin) {

  if ((!buffer) && !(buffer = (uint8_t *)malloc(WIDTH * ((HEIGHT + 7) / 8))))
    return false;

  clearDisplay();


  vccstate = vcs;

  i2caddr = addr ? addr : ((HEIGHT == 32) ? 0x3C : 0x3D);
  init_i2c();

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

  //TRANSACTION_START


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

  return true; 
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
  static const uint8_t  dlist1[] = {
    SSD1306_PAGEADDR,
    0,                      // Page start address
    0xFF,                   // Page end (not really, but works here)
    SSD1306_COLUMNADDR, 0
  }; // Column start address
  ssd1306_commandList(dlist1, sizeof(dlist1));
  ssd1306_command1(WIDTH - 1); // Column end address


  uint16_t count = WIDTH * ((HEIGHT + 7) / 8);
  uint8_t *ptr = buffer;
  begin_i2c(i2caddr);
  send_u8_i2c(0x40);
  while (count--) send_u8_i2c(*ptr++);
  end_i2c();
}




/*!
    @brief  Cease a previously-begun scrolling action.
    @return None (void).
*/
void Adafruit_SSD1306::stopscroll(void) {
  ssd1306_command1(SSD1306_DEACTIVATE_SCROLL);
}

void Adafruit_SSD1306::invertDisplay(bool i) {
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
void Adafruit_SSD1306::dim(bool dim) {
  // the range of contrast to too small to be really useful
  // it is useful to dim the display
  ssd1306_command1(SSD1306_SETCONTRAST);
  ssd1306_command1(dim ? 0 : contrast);
}



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);



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


u8* the_letter = letterH;

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

  draw_letter(the_letter);

}
void loop() {
}




void draw_letter(u8 letter[]) {
  display.clearDisplay();
  for (int i = 0; i < 8; i++) {
    u8 row = letter[i];
    for (int j = 0; j < 8; j++) {
      u8 on = (row & 1 << 7) ? 1 : 0;
      display.drawPixel(j, i, on);
      row <<= 1;
    }

    //display.drawPixel(i, 0, row);
  }
  display.display();
  delay(2000);
}

typedef struct {
  __IO u32 CR1; //0x00
  __IO u32 CR2; // 0x04
  __IO u32 OAR1; //0x08
  __IO u32 OAR2; //0x0C
  __IO u32 DR; //0x10
  __IO u32 SR1; //0x14
  __IO u32 SR2; //0x18
  __IO u32 CCR; // 0x1C
  __IO u32 TRISE; // 0x20
} I2C_t;

#define I2C1 ((I2C_t*) (0x40005400))
#define I2C2 ((I2C_t*) (0x40005800))


#define I2C_CR1_PE (1<<0)

#define I2C_CR1_STOP (1<<9)
#define I2C_CR1_ACK (1<<10)
#define I2C_CR1_START (1<<8)
#define I2C_CR1_SWRST (1<<15)

#define I2C_CR2_LAST (1<<12)
#define I2C_CR2_DMAEN (1<<11)

#define I2C_SR1_SB (1<<0)
#define I2C_SR1_RXNE (1<<6)
#define I2C_SR1_TXE  (1<<7)
#define I2C_SR1_ADDR (1<<1)

void begin_i2c(u8 sid) {
  //U32 TEMP;
  I2C1->CR1 |= I2C_CR1_START; // GENERATE A START CONDITION

  while (!(I2C1->SR1 & I2C_SR1_SB));
  I2C1->DR = sid << 1;
  while (!(I2C1->SR1 & I2C_SR1_ADDR));
  I2C1->SR2;
}

void end_i2c() {
  I2C1->CR1 |= I2C_CR1_STOP;
}

#define I2C_SR1_BTF (1<<2)

void  send_i2c(const u8* buffer, u32 len) {
  for (u32 i = 0; i < len; i++) {
    I2C1->DR = buffer[i];
    while (!(I2C1->SR1 & I2C_SR1_TXE));
    //I2C1->SR2;
    while (!(I2C1->SR1 & I2C_SR1_BTF)); // added mcarter 2020-10-10. Seems necessary

  }
}

void write_i2c(u8 sid, const u8* buffer, u32 len)
{
  begin_i2c(sid);
  send_i2c(buffer, len);

  // COULR DO THIS MULTIPLE TIMES TO SEND LOTS OF DATA
  //I2C2->DR = DATA;
  //WHILE(!(I2C2->SR1 & I2C_SR1_TXE));

  end_i2c();
}



#define GPIO_BASE 0x40010800
#define GPIOC ((GPIO_t*) (GPIO_BASE + 0x800))
#define GPIOB ((GPIO_t*) (GPIO_BASE + 0x400))
#define GPIOA ((GPIO_t*) (GPIO_BASE + 0x000))


#define PA4  0x04
#define PA5  0x05
#define PA6  0x06
#define PA7  0x07
#define PA9  0x09
#define PA10 0x0A
#define PA13 0x0D
#define PB0  0x10
#define PB1  0x11
#define PB6  0x16
#define PB7  0x17
#define PB10 0x1A
#define PB11 0x1B
#define PB13 0x1D
#define PC13 0x2D
#define PC15 0x2F

#define BUILTIN_LED PC13

// section 9.2.1 and 9.2.2 CNF/MODE combos
#define OSPEED 0b10 // 2MHz
#define INPUT      0b0100 // CNF=1, MODE=0, floating input
#define INPUT_PULLUP  0b1000 // CNF=2, MODE =0. input with pull-up/down
#define OUTPUT    (0b0000 | OSPEED) // CNF=0, MODE=1, output push-pull ("regular" out)
#define OUTPUT_ALT  (0b1000 | OSPEED) // alt out push-pull


u32 pin_to_gpio(u32 pin)
{
  u32 port = pin >> 4;
  return GPIO_BASE + port * 0x400;
}


void put32(u32 addr, u32 val)
{
  *(volatile u32*) addr = val;
}

u32 get32(u32 addr)
{
  return *(volatile u32*) addr;
}



#define RCC_BASE        0x40021000

// section 7.3.11 RCC register map page 121
#define RCC_CR   *(volatile uint32_t *)(RCC_BASE + 0x00)
#define RCC_CR_HSION (1<<0)
#define RCC_CR_HSIRDY (1<<1)
#define RCC_CFGR   *(volatile uint32_t *)(RCC_BASE + 0x04)
#define RCC_CFGR_SW (1<<0)
#define RCC_APB1ENR   *(volatile uint32_t *)(RCC_BASE   + 0x1C) // page 148
#define RCC_APB1ENR_TIM4EN (1<<2)
#define RCC_APB1ENR_USART2EN  (1<<17)

#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)
#define RCC_APB2ENR_USART1EN (1<<14)
#define RCC_APB2ENR_IOPAEN  (1<<2)
#define RCC_APB2ENR_AFIOEN (1<<0)
#define RCC_APB2ENR_SPI1EN (1<<12)

#define RCC_APB2ENR_AFIOEN (1<<0)

#define RCC_APB1ENR_I2C1EN (1<<21)
#define RCC_AHBENR   REG(RCC_BASE   + 0x14)
#define RCC_AHBENR_DMA1EN (1<<0)

void gpio_write(u32 pin, u32 val)
{
  u32 pos = pin & 0xF;
  u32 GPIO_BSRR = pin_to_gpio(pin) +  0x10;
  if (val == 0 ) pos += 16; // Reset rather than Set
  put32(GPIO_BSRR, 1 << pos);

}

// return either GPIO_CRL, GPIO_CRH, and a pin offset into it
static void gpio_crx_offset(u32 pin, u32 *crx, u32 *offset)
{
  *crx = pin_to_gpio(pin) + 0x00; // assume GPIOx_CRL
  u32 pin_num = pin & 0x0F;
  if (pin_num > 7) {
    *crx += 0x04; // bump to GPIOx_CRH;
    pin_num -= 8;
  }
  *offset = pin_num * 4;
}

void gpio_mode(u32 pin, u8 mode)
{
  u32 port = pin >> 4; // gives 0 for Port A, 1 for port B, 2 for port C

  // only enable port if currently unenabled
  if ((RCC_APB2ENR & (1 << (port + 2))) == 0)
    RCC_APB2ENR |= (1 << (port + 2)); // enable port

  u32 crx, offset;
  gpio_crx_offset(pin, &crx, &offset);
  u32 mask = 0b1111 << offset;
  *(volatile u32*) crx &= ~mask; // mask out the mode and CNF
  *(volatile u32*) crx |= (((u32)mode) << offset);
}

void gpio_mode_out(u32 pin)
{
  gpio_mode(pin, OUTPUT);
}


void gpio_toggle(u32 pin)
{
  gpio_write(pin, 1 - gpio_read(pin));
#if 0
  u32 GPIO_ODR = pin_to_gpio(pin) + 0x0C;
  u32 on = get32(GPIO_ODR) &  (1 << (pin & 0xF));
  if (on)
    gpio_write(pin, 0);
  else
    gpio_write(pin, 1);
#endif
}

u8 gpio_read(u32 pin)
{
  u32 crx, offset;
  gpio_crx_offset(pin, &crx, &offset);
  u32 config = get32(crx); // the CNF and MODE settings for the whole port
  u32 mode = config & ( 0b11 << offset); // mask out non-MODE bits

  // determine whether to read IDR or ODR
  u32 addr = pin_to_gpio(pin); // GPIO A/B/C
#if 1
  if (mode) {
    // it's an output
    addr += 0x0C; // we're now pointing to GPIOx_ODR
  } else {
    addr += 0x08; // we're now pointint to GPIOx_IDR
  }
#else
  addr += 0x0C;
#endif
  u32 port_data = get32(addr);
  if (port_data & (1 << (pin & 0xF)))
    return 1;
  else
    return 0;
#if 0
  u32 GPIO_ODR = pin_to_gpio(pin) + 0x0C;
  u32 on = get32(GPIO_ODR) & (1 << (pin & 0xF));
  if (on) return 1;
  return 0;
#endif
}




void init_i2c() // this seems to be correct
{
  //RCC->AB1ENR &= ~(RCC_APB1ENR_I2CEN);
  I2C1->CR1 = 0X00;
  I2C1->CR2 = 0X00;
  //RCC_APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
  RCC_APB2ENR |= RCC_APB2ENR_AFIOEN;
  RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;

  //I2C1_->CR1 |= I2C_CR1_SWRST; // reset I2C for good measure. bad-t


  I2C1->CR2 |= 36; // FREQ OF APB1 BUS = 72MHZ/2 = 36MHZ

  /* TAKE THE PERIOS OF THE FREQ SET INTO CR2 EX: 1/10MHZ = 100NS
     NOW PICK A FREQ YOU WANT THE I2C TO RUN AT (MAX 100KHZ IN STD
     MODE) (400KHZ MAX IN FAST MODE)
     THEN IF IN STD MODE GET THE PERIOD OF THAT FREQ
    EX:1/100KHZ = 10US THEN DIVIDE BY 2 TO GET THE
    HTIME BECAUSE THE DUTY CYCLE IS 50% SO HIGH TIME AND LOW TIME
    IS THE SAME SO WE JUST DIVIDE PERIOD BY 2 TO GET
    5US
    THEN CCR REGISTER = 5US/100NS = 50
    SOO CCR IS 50, THIS IS THE MAX TIME THE I2C WILL WAIT FOR A
    RISE TIME AND NOT NECESSARILY THE RISE TIME THAT IT WILL
    ACTUALLY
  */
  I2C1->CCR |= 180; // SPEED TO 100KHZ STD MODE MAX: I2C_PERIOD /2 * ABI = (1/100K/2)*36MHZ
  I2C1->TRISE |= 37; // 1000NS/(CR2 PERIOD=1/36MHZ) = TRISE +1
  I2C1->CR1 |= I2C_CR1_ACK; // ENABLE ACKS

  // stretch mode enabled by default

  // 7 bit addressing mode enabled by default



  //GPIOB->CRL = 0b11111111010010000100010001000100;
  gpio_mode(PB6, 0b1111);
  gpio_mode(PB7, 0b1111);


  I2C1->CR1 |= I2C_CR1_PE; // ENABLE PERIPHERAL
}
