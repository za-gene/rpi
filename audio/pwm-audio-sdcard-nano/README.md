# Audio player using PWM, SD card, and Nano

In this project, we will explore the use of **low-level timer programming**
on the 
Nano to play a song stored on an SD card to a speaker using PWM output. 
An Arduino Uno will work just as well.
The 
Arduino standard SD library is used to access the SD card. Interfacing
with SD cards is a real hairball.

Take a quick look at the [code](https://github.com/blippy/rpi/blob/master/audio/pwm-audio-sdcard-nano/pwm-audio-sdcard-nano.ino) for the project. 
An in-depth discussion now follows.

For the purposes of this demo you will need a file called `song.raw`. You can
[download](https://drive.google.com/file/d/1fU7viQhGI5p9GGPTPLHQ3CrVoz7BrtE9/view?usp=sharing) a song directly, or use a tool like Audacity to convert any audio
file to RAW format, single-channel, 8-bit unsigned, 8kHz sample-rate.

Our first decision is to decide what pin we are going to attach to the speaker.
We will use the Arduino's hardware capabilities as much as possible. We are
going to use PWM to produce the output, rather than a DAC. So our 
pin must support PWM. A 
[diagram](https://z900collector.files.wordpress.com/2014/11/arduino-nano-pins.png)
of the Nano PWM pinouts is summarised below:

```
D3  OC2B
D5  OC0B
D6  OC0A
D9  OC1A
D10 OC1B
D11 OC2A
        Table 1: pin timer/channel
```

The Nano/Uno has 3 timers: Timer0, Timer1, Timer2. Each timer has 2 channels.
The table above shows which pins are connected to which timer and channel. 
E.g. D3 is connected to Timer2, channel B.

Let us avoid the use of Timer0. Functions like delay() use Timer0, and they
are likely to malfunction if we start interfering with their configuration,
It is likely to be used by the SD library, amongst others, making it
especially advisable to avoid its use.

Timer1 is typically used for servos, as it uses a 16-bit counter rather
than an 8-bit one.That makes it a bit more fiddly to use, so it is likely
to be more convenient to use an 8-bit one, especially as our song uses
8-bit encoding.

We also want to avoid using SPI-specific pins. We will need those to hook
up to our SD card. Pin D11 is for MOSI, D12 for MISO, D10 is for SS 
(although we will actually use D8), and D13 is for SCK.

Taking all of the above into account, we select pin D3 as a suitable 
candidate. So, we will be using timer **1**, channel **B**.

There are 3 basic ways we can use a timer:
1. normal mode
2. CTC mode - where we can vary frequency, but keep PWM duty cycle fixed
3. fast PWM mode - where can vary the duty cycle, but keep the frequency fixed.

We choose the last option. Our song has been recorded at a fixed 8kHz
frequency. We will vary the duty cycle to vary the volume.

How do we generate the correct frequency? The formula in section 17.7.3 of the 
datasheet (page 122) gives us the answer:
```
fOCnxPWM = fclk/N/256
```
where fclk is the clock frequency (16MHz), and N is a prescale factor
(one of 1, 8, 32, 64, 128, 256, 1024).

During setup, the Arduino IDE initialises the prescaler to 64, which is
unsuitable for our purposes.

We *could* set the prescaler to 8 (N=8), giving us a frequency of 
7812.5Hz (=16e6/8/256), which is approximately 8kHz. We are not going
to be able to be more accurate than that for the mode we are using,
at least with an Arduino. 

However, this frequency is not suitable, because using PWM acts as a kind
of "carrier wave". The human ear can hear an 8kHz frequency, so there will
be an annoying tone at that frequency if we were to use it. 

We need to set the "carrying frequency" higher. 16kHz is likely to be 
acceptable. Young people might be able to hear it. The Nano isn't configurable
at that frequency in any event.

Instead, we have only one option: use a prescaler of N=1, which will give 
us a PWM frequency of 62500Hz (16e6/1/256). This means that instead of adjusting
a PWM duty cycle after every timer overflow, we adjust it after every 8 
overflows (62500/8 = 7813.5Hz, back to our original sample rate, 
approximately).

How the timer counts, what it counts to, and how it resets is dependent
on how it is configured. Consult table 17-8 (page 130) of the datasheet.
We want fast PWM mode, where the counter counts from 0 to 0xFF, then resets
back to 0. According to the table, we need mode 3, in which we set
WGM22 to 0, WGM21 to 1, and WGM20 to 1. Note that the table on the
datasheet seems mislabelled. "WGM2" should read "WGM22", "WGM1" should
read "WGM21", and "WGM0" should read "WGM20".

The bits WGM22, WGM21, and  WGM20 are spread over two control registers:
TCCR2A (Timer/Counter Control Register A) and TCCR2B (Timer/Counter
Control Register B). I don't know why they've implemented the hardware
in this way, it seems a little complicated. But it is what it is.

We do not need to worry about WGM22 anyway, because we want to leave it set 
at 0. Table 17-8 is a little confusing, but as you can see, setting WGM22 to 1
changes mode from 3 to 7. The effect is to invert the output signal. 
We don't want that, though. We want large values in our sample to correspond
to large duty cycles.

The duty cycle is determined by setting the "relevant" 
OCR (Output Compare Register), in our case OCR2B. Recall that we
are working with timer 2, channel B.
OCR2B takes values values from 0 to 0xFF, corresponding exactly to the
audio sample values.

When the timer counter is 0, output to the pin goes high. When the timer
counter reaches OCR2B, the pin goes low. The timer counter keeps on
counting until it reaches TOP (0xFF). The timer overflows and the 
process repeats. 

Now, we can set an interrupt when the counter overflows in order to 
change the value of OCR2B, and therefore change the duty cycle 
on the next (not current) reset. 

So our setup looks like this:
```
void setup()
{
  // low-level set-up of timer
  pinMode(3, OUTPUT);
  TCCR2A = 0; // clear it out, because the prescaler is 64, for example
  TCCR2A |= (1<<WGM21) | (1<<WGM20); // Fast PWM. Table 17-8. (WGM22 unset)
  TCCR2A |= (1<<COM2B1); // Clears OC2B on compare match, set OC2B at bottom, non-inverting mode
   
  TCCR2B = 0;
  TCCR2B |= (1<<CS20);       // Prescale 1, giving ~ 62500Hz

  // ignore duty cycle for this sketch. The timer sets it

  TIMSK2 |= (1<<TOIE2); //enable overflow interrupt for timer2


  setup_stuff();

}
```

We use pin D3 for speaker output, configure the relevant control registers
appropriately, and enables an overflow timer interrupt via `TIMSK2`. 

`setup_stuff()` sets up the serial port and SD card:

```
const int chipSelect = 8;

void setup_stuff() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
 
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  dataFile = SD.open("song.raw");
  if(dataFile) {
    opened = true;
  } else {
    Serial.println("Couldn't open file");
  }
}
```

We use pin D8 as the chip select (aka slave select) for the SD card,
and use the default MISO, MOSI and SCK pins. 

We create the timer interrupt for when the counter overflows:
```
static volatile bool read_flag = false;
static volatile u8 vol =0;

ISR(TIMER2_OVF_vect)
{
  static volatile int divider = 0;
  OCR2B = vol;
  if(++divider == 8) {
    read_flag = true;
    divider = 0;    
  } 
}
```

Every 8 overflows (recall that our clock has been configured with a frequency 
of 62500Hz, but we want 8000Hz, or as near as we can get it) we set the 
`read_flag` to `true`, which tells the main loop that we much reset the
volume. 

Here's the loop:
```
void loop()
{
  if(read_flag && opened) {
    read_flag = false;
   
    if(dataFile.available()) {
      vol = dataFile.read();
    } else { // end of file, reset to beginning
      vol = 0;
      dataFile.seek(0);
    }
    
  }
}
```

The file resets to the beginning once all the samples are played.

There are a few other minor details to consider, like include files. 
Consult the main listing for details.


## Pin hookup

For the speaker: one terminal to pin D3, the other to GND.

For the SD card: connect D8 (not D10) to chip select, D11 to MOSI, 
D12 to MISO, and D13 to SCK.


## Links to other sites

* [datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf) (PDF)


