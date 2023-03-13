#include "AS5600.h"
#include "Wire.h"

#include <math.h>


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
int offset1X =  738;//było 738
int offset2X = 743;
int corseX = 0;
int moveConditionX = 0;

int counterZ;
int old_arcusZ;
int HALL1Z = 0;
int HALL2Z = 0;
int new_arcusZ;
int offset1Z =  738;//było 738
int offset2Z = 743;
int corseZ = 0;
int moveConditionZ = 0;


int maxHall1X = 0;
int minHall1X = 1000;
int maxHall2X = 0;
int minHall2X = 1000;

int maxHall1Z = 0;
int minHall1Z = 1000;
int maxHall2Z = 0;
int minHall2Z = 1000;


float angle;

int counterXdiv = 0;
int counterZdiv = 0;
int anglediv = 0;


//char* transmitBuffer = "hellaa";
byte transmit[2];
char t[30];
char buffer[20];

AS5600 as5600;

TwoWire Wire2(PB7,PB6);

void setup() {
  pinMode(PA3, INPUT);
  pinMode(PA4, INPUT);
  pinMode(PB0, INPUT);
  pinMode(PB1, INPUT);
  
  Serial.begin(9600);
  
  Wire.setSDA(PB3);
  Wire.setSCL(PB10);
  Wire.begin();

  
  Wire2.begin(5);
  Wire2.onRequest(requestEvent);

  as5600.begin();
  
  new_arcusX = atan2(HALL1X, HALL2X) * 180 / PI * 5000/360 + 2500;
  old_arcusX = new_arcusX;

  new_arcusZ = atan2(HALL1Z, HALL2Z) * 180 / PI * 5000/360 + 2500;
  old_arcusZ = new_arcusZ;
 
}

void loop() {
  HALL1X = analogRead(PA4) - offset1X;
  HALL2X = analogRead(PA3) - offset2X;

  HALL1Z = analogRead(PB0) - offset1Z;
  HALL2Z = analogRead(PB1) - offset2Z;

  if(HALL1X < minHall1X)
  {
    minHall1X = HALL1X; 
  }

   if(HALL1X > maxHall1X)
  {
    maxHall1X = HALL1X; 
  }
 
  if(HALL2X < minHall2X)
  {
    minHall2X = HALL2X ;
  }
 
  if(HALL2X > maxHall2X)
  {
    maxHall2X = HALL2X; 
  }
////////////
  if(HALL1Z < minHall1Z)
  {
    minHall1Z = HALL1Z; 
  }

   if(HALL1Z > maxHall1Z)
  {
    maxHall1Z = HALL1Z; 
  }
 
  if(HALL2Z < minHall2Z)
  {
    minHall2Z = HALL2Z; 
  }
 
  if(HALL2Z > maxHall2Z)
  {
    maxHall2Z = HALL2Z; 
  }
 
 
  new_arcusX = atan2(HALL1X, HALL2X) * 180 / PI * 5000/360 + 2500;
  new_arcusZ = atan2(HALL1Z, HALL2Z) * 180 / PI * 5000/360 + 2500;

  moveConditionX = new_arcusX - old_arcusX;
  if (moveConditionX > 4000) {
    corseX += 5000;
    //fine = 0;
  }
  if (moveConditionX < -4000) {
    corseX -= 5000;
    //fine = 0;
  }

  moveConditionZ = new_arcusZ - old_arcusZ;
  if (moveConditionZ > 4000) {
    corseZ += 5000;
    //fine = 0;
  }
  if (moveConditionZ < -4000) {
    corseZ -= 5000;
    //fine = 0;
  }
  /*if (new_arcusX < old_arcusX) {
    fine++; //POSSIBLE INACURACZ!
  }
  if (new_arcus > old_arcus) {
    fine--; //POSSIBLE INACURACZ!
  }*/

  counterX = corseX - new_arcusX;
  counterZ = corseZ - new_arcusZ;
  /*Serial.print("Czujki halla: ");
  Serial.println(HALL1);
  Serial.println(HALL2);
  Serial.print("Kąt: ");
  Serial.println(as5600.readAngle());*/
  /*Serial.print("minHall1: ");
  Serial.println(minHall1);
  Serial.print("maxHall1: ");
  Serial.println(maxHall1);
  Serial.print("minHall2: ");
  Serial.println(minHall2);
  Serial.print("maxHall2: ");
  Serial.println(maxHall2);
  */
  //Serial.println(new_arcus);
  //transmitBuffer = as5600.readAngle();
  //strcpy((char *)transmitBuffer,(char *)as5600.readAngle());
  //strcpy(transmitBuffer,"helloo");
  old_arcusX = new_arcusX;
  old_arcusZ = new_arcusZ;
  angle = as5600.rawAngle() * 0.088;
 // angle = angle * (360/4096);
  //transmit[0] = offset2 & 0xff;
  //transmit[1] = (offset2 >> 8);
  //transmit[3] = 
  ///memcpy(transmit, &angle, 4);
 
  //HALL2 = 123;

  x = counterX/scale;
  y = armLenght * cos(angle * (PI / 180));
  z = counterZ/scale + (armLenght * sin(angle * (PI / 180)));
  
  dtostrf(maxHall1X, 7, 0, buffer);
  t[0] = buffer[0];
  t[1] = buffer[1];
  t[2] = buffer[2];
  t[3] = buffer[3];
  t[4] = buffer[4];
  t[5] = buffer[5];
  t[6] = ';';

  
  dtostrf(maxHall1Z, 7, 0, buffer);
  t[7] = buffer[0];
  t[8] = buffer[1];
  t[9] = buffer[2];
  t[10] = buffer[3];
  t[11] = buffer[4];
  t[12] = buffer[5];
  t[13] = ';';

  
  dtostrf(maxHall2X, 7, 0, buffer);
  t[14] = buffer[0];
  t[15] = buffer[1];
  t[16] = buffer[2];
  t[17] = buffer[3];
  t[18] = buffer[4];
  t[19] = buffer[5];
  t[20] = ';';

  dtostrf(maxHall2Z, 7, 0, buffer);
  t[21] = buffer[0];
  t[22] = buffer[1];
  t[23] = buffer[2];
  t[24] = buffer[3];
  t[25] = buffer[4];
  t[26] = buffer[5];
  t[27] = ';';




// TO MOŻE BYĆ PROBLEMEM NA #5
  
  

  //dtostrf(counter2, 6, 0, buffer)
  //strcpy(t,buffer);

}

void requestEvent()
{
  
  //strcpy(transmitBuffer,"helloo");
  
  //buffer[4] = 'b';
  //buffer[5] = 'c';
  //buffer[6] = 'a';
  //buffer[7] = 'a';
  
  //strcpy(t,buffer);
  //dtostrf(HALL2, 6, 0, t);
  
  Wire2.write(t,28);//as5600.readAngle());

}