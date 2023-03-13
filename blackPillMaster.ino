// Wire Master Reader
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Reads data from an I2C/TWI slave device
// Refer to the "Wire Slave Sender" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include "AS5600.h"
#include <math.h>

int zeroAngle = 1024 + 2668;

int translatedCounter = 0;
char recieve[30] = {};
//int recieve = 0;
int j = 0;

int x = 0;
int y = 0;
int z = 0;

float armLenght = 65;
int scale = 1000;

int counterX;
int old_arcusX;
int HALL1X = 0;
int HALL2X = 0;
int new_arcusX;
int offset1X =  738;
int offset2X = 743;
int corseX = 0;
int moveConditionX = 0;

int counterZ;
int old_arcusZ;
int HALL1Z = 0;
int HALL2Z = 0;
int new_arcusZ;
int offset1Z =  738;
int offset2Z = 743;
int corseZ = 0;
int moveConditionZ = 0;


float angle;

int counterXdiv = 0;
int counterZdiv = 0;
int anglediv = 0;

char t[30];
char buffer[20];

TwoWire Wire2(PB7,PB6);
AS5600 as5600;

void setup() {
  pinMode(PA3, INPUT);
  pinMode(PA4, INPUT);
  pinMode(PA7, INPUT);
  pinMode(PB0, INPUT);

  Wire.setSDA(PB3);
  Wire.setSCL(PB10);
  Wire.begin();

  Wire2.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  new_arcusX = atan2(HALL1X, HALL2X) * 180 / PI * 5000/360 + 2500;
  old_arcusX = new_arcusX;

  new_arcusZ = atan2(HALL1Z, HALL2Z) * 180 / PI * 5000/360 + 2500;
  old_arcusZ = new_arcusZ;
}

void loop() {
  HALL1X = analogRead(PA4) - offset1X;
  HALL2X = analogRead(PA3) - offset2X;

  HALL1Z = analogRead(PA7) - offset1Z;
  HALL2Z = analogRead(PB0) - offset2Z;

  new_arcusX = atan2(HALL1X, HALL2X) * 180 / PI + 180;// * 5000/360 + 2500;
  new_arcusZ = atan2(HALL1Z, HALL2Z) * 180 / PI + 180;// * 5000/360 + 2500;


  moveConditionX = new_arcusX - old_arcusX;
  if (moveConditionX > 300) {
    corseX += 360;
    //fine = 0;
  }
  if (moveConditionX < -300) {
    corseX -= 360;
    //fine = 0;
  }

  moveConditionZ = new_arcusZ - old_arcusZ;
  if (moveConditionZ > 300) {
    corseZ += 360;
    //fine = 0;
  }
  if (moveConditionZ < -300) {
    corseZ -= 360;
    //fine = 0;
  }
  counterX = corseX - new_arcusX;
  counterZ = corseZ - new_arcusZ;

  old_arcusX = new_arcusX;
  old_arcusZ = new_arcusZ;


  angle = as5600.rawAngle() - zeroAngle;
  if(angle < 0)
  {
    angle = angle + 4096;//(4096 - zeroAngle) + angle;
  }

  x = counterX/10;
  y = angle;
  z = counterZ/10;
  
  dtostrf(x, 7, 0, buffer);
  t[0] = buffer[0];
  t[1] = buffer[1];
  t[2] = buffer[2];
  t[3] = buffer[3];
  t[4] = buffer[4];
  t[5] = buffer[5];
  t[6] = ';';

  
  dtostrf(y, 7, 0, buffer);
  t[7] = buffer[0];
  t[8] = buffer[1];
  t[9] = buffer[2];
  t[10] = buffer[3];
  t[11] = buffer[4];
  t[12] = buffer[5];
  t[13] = ';';

  
  dtostrf(z, 7, 0, buffer);
  t[14] = buffer[0];
  t[15] = buffer[1];
  t[16] = buffer[2];
  t[17] = buffer[3];
  t[18] = buffer[4];
  t[19] = buffer[5];
  t[20] = ';';
  //////////////////////////////////////////////////////////////
  Serial.println("BEGIN");
  Serial.println(t);
  for (int i = 2; i < 7; i++) {
    Wire2.requestFrom(i, 21);
    j = 0;
    while (Wire2.available())  // slave may send less than requested
    {
      recieve[j] = Wire2.read();  // receive a byte as character

      j++;
    }
    Serial.println(recieve);
    //delay();
  }
   Serial.println("END");

  //delay(500);

  
}