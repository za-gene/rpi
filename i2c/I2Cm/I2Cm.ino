#include <Wire.h>


#ifdef MCU_STM32F103C8
auto &ser = Serial1;
#else
auto &ser = Serial;
#endif

void setup()
{
  ser.begin(115200); // start serial for output
  ser.println("i2c master here");
  Wire.begin(); // join i2c bus (address optional for master)

}

void loop()
{
  Wire.requestFrom(4, 1); // request 1 byte from slave device address 4

  while (Wire.available()) // slave may send less than requested
  {
    int i = Wire.read(); // receive a byte as character
    ser.println(i); // print the character
  }

  delay(1000);
}
