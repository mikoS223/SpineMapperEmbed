#include "AS5600.h"
#include "Wire.h"

#include <math.h>



float x = 0;
int y = 0;
int z = 0;

int zeroAngle =    (1024 + 3938) - 4096;

float armLenght = 65;
//float scale = 3.2/5000;

int counterX;
int old_arcusX;
int HALL1X = 0;
int HALL2X = 0;
int new_arcusX;
int offset1X =  738-58;//było 738
int offset2X = 743-67;
int corseX = 0;
int moveConditionX = 0;

int counterZ;
int old_arcusZ;
int HALL1Z = 0;
int HALL2Z = 0;
int new_arcusZ;
int offset1Z =  738-82;//było 738
int offset2Z = 743-59;
int corseZ = 0;
int moveConditionZ = 0;


/*int maxHall1 = 0;
int minHall1 = 1000;
int maxHall2 = 0;
int minHall2 = 1000;
*/

int angle;
int angleMapped;

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

  
  Wire2.begin(6);
  Wire2.onRequest(requestEvent);

  as5600.begin();
  
  new_arcusX = atan2(HALL1X, HALL2X) * 180 / PI * 5000/360 + 2500;
  old_arcusX = new_arcusX;

  new_arcusZ = atan2(HALL1Z, HALL2Z) * 180 / PI * 5000/360 + 2500;
  old_arcusZ = new_arcusZ;
 
}

void loop() {
  HALL1X = analogRead(PA3) - offset1X;
  HALL2X = analogRead(PA4) - offset2X;

  HALL1Z = analogRead(PB0) - offset1Z;
  HALL2Z = analogRead(PB1) - offset2Z;

 /*if(HALL1X < minHall1)
  {
    minHall1 = HALL1; 
  }

   if(HALL1 > maxHall1)
  {
    maxHall1 = HALL1; 
  }
 
  if(HALL2 < minHall2)
  {
    minHall2 = HALL2; 
  }
 
  if(HALL2 > maxHall2)
  {
    maxHall2 = HALL2; 
  }
 */
 
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

  
  angle = as5600.rawAngle() - zeroAngle;
  if(angle < 0)
  {
    angle = angle + 4096;//(4096 - zeroAngle) + angle;
  }
 
 // angle = angle * (360/4096);
  //transmit[0] = offset2 & 0xff;
  //transmit[1] = (offset2 >> 8);
  //transmit[3] = 
  ///memcpy(transmit, &angle, 4);
 
  //HALL2 = 123;

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
  
  Wire2.write(t,21);//as5600.readAngle());

}