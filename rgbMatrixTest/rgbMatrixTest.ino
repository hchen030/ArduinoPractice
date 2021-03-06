#include <I2Cdev.h>
#include <MPU6050.h>

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include <Wire.h>
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

int converted_ax = 0;
int converted_ay = 0;

#define OUTPUT_READABLE_ACCELGYRO

int dataPin = 2;
int latchPin = 3;
int clockPin = 4;
int clearPin = 5;
int redByte = 0x01; int greenByte = 0x01; int blueByte = 0x01; int colByte = 0x01;
int x = 1;
int y = 1;
int timer = 0;
int timerVar = 128;
int timerVarState = 1;
int corner = 1;
int ledPointRow = 0x08;
int ledPointCol = 0x10;


void setup()
{
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    Serial.begin(38400);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    accelgyro.setXGyroOffset(220);
    accelgyro.setYGyroOffset(76);
    accelgyro.setZGyroOffset(-85);
  
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
}

void shiftVal(int redByte, int greenByte, int blueByte, int colByte)
{
  digitalWrite(latchPin, LOW);
  digitalWrite(clearPin, LOW);
  digitalWrite(clearPin, HIGH);
  //shiftOut(dataPin, clockPin, bitOrder, value);
  shiftOut(dataPin, clockPin, MSBFIRST, colByte);
  shiftOut(dataPin, clockPin, MSBFIRST, (~blueByte)&0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, (~greenByte)&0xFF);
  shiftOut(dataPin, clockPin, MSBFIRST, (~redByte)&0xFF);
  digitalWrite(latchPin, HIGH);
}

void controller1()
{
  while(timer < timerVar)
  {
    shiftVal(redByte, 0, 0, 0xFF);
    shiftVal(0xFF, 0, 0, colByte);
    timer++;
    if(timer % (timerVar / 8) == 0)
    {
      if(corner % 2 == 1)
      {
        redByte = redByte << 1;
        colByte = colByte << 1;
      }
      else
      {
        redByte = redByte >> 1;
        colByte = colByte >> 1;
      }
    }
  }
  timer = 0;
  corner++;
  if(timerVarState == 1)
  {
    if(timerVar > 400)
    {
      timerVarState = 2;
    }
    else
    {
      timerVar += 8;
    }
  }
  else
  {
    if(timerVar < 128)
    {
      timerVarState = 1;
    }
    else
    {
      timerVar -= 8;
    }
  } 
  if(corner % 2 == 1)
  {
    greenByte = 0x01;
    colByte = 0x01;
  }
  else
  {
    greenByte = 0x80;
    colByte = 0x80;
  }
  while(timer < timerVar)
  {
    shiftVal(0, greenByte, 0, 0xFF);
    shiftVal(0, 0xFF, 0, colByte);
    timer++;
    if(timer % (timerVar / 8) == 0)
    {
      if(corner % 2 == 1)
       {
         greenByte = greenByte << 1;
         colByte = colByte << 1;
       }
       else
       {
         greenByte = greenByte >> 1;
         colByte = colByte >> 1;
       }
    }
  }
  timer = 0;
  corner++;
  if(timerVarState == 1)
  {
    if(timerVar > 400)
    {
      timerVarState = 2;
    }
    else
    {
      timerVar += 8;
    }
  }
  else
  {
    if(timerVar < 128)
    {
      timerVarState = 1;
    }
    else
    {
      timerVar -= 8;
    }
  } 
  if(corner % 2 == 1)
  {
    blueByte = 0x01;
    colByte = 0x01;
  }
  else
  {
    blueByte = 0x80;
    colByte = 0x80;
  }
  while(timer < timerVar)
  {
    shiftVal(0, 0, blueByte, 0xFF);
    shiftVal(0, 0, 0xFF, colByte);
    timer++;
    if(timer % (timerVar / 8) == 0)
    {
      if(corner % 2 == 1)
       {
         blueByte = blueByte << 1;
         colByte = colByte << 1;
       }
       else
       {
         blueByte = blueByte >> 1;
         colByte = colByte >> 1;
       }
    }
  }
  timer = 0;
  corner++;
  if(timerVarState == 1)
  {
    if(timerVar > 400)
    {
      timerVarState = 2;
    }
    else
    {
      timerVar += 8;
    }
  }
  else
  {
    if(timerVar < 128)
    {
      timerVarState = 1;
    }
    else
    {
      timerVar -= 8;
    }
  } 
  if(corner % 2 == 1)
  {
    redByte = 0x01;
    colByte = 0x01;
  }
  else
  {
    redByte = 0x80;
    colByte = 0x80;
  }
}

void controller2()
{
  accelgyro.getRotation(&gx, &gy, &gz);
  shiftVal(0,0,ledPointRow,ledPointCol);
}

void loop()
{
  controller2();
}
