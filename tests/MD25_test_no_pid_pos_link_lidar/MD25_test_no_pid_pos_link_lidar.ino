/* Controle des 2 moteurs en même temps*/
/* Carte MD25 pilotée en mode serial UART*/
/* Voir documentation MD25...*/
/* Code et librairie perfectibles*/
/* By Ph.Gozlan_280222*/
/*For asso R2T2*/

#include "MD25_UART.h"
#include <Wire.h>
#include "rgb_lcd.h"

MD25_UART controller;

rgb_lcd lcd;

bool bnord;
bool bdroite;
bool bgauche;

// LCD Colors
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

byte vitesse = 30;
const float pi = 3.141593;
const float kP = 0.005;

void avancer(long distance, float wheelPerimeter);
void reculer(long distance, float wheelPerimeter);
void tourner(int degree, float wheelPerimeter, float distanceEntreRoues); // Si degree > 0, tourne à gauche. Sinon tourne à droite
double convertCentimeterToTick(double distanceCm, float wheelPerimeter);

void setup() 
{
  Serial.begin(9600);
  controller.setAccelerationRate(1);
  controller.enableSpeedRegulation(); // Active l'asservissement en vitesse de la MD25
  controller.setMode(0);  // mode 0 (0 = vitesse max en arrière, 128 = arrêt, 255 = vitesse max en avant) la vitesse est gérée sur les 2 moteurs indépendamment 
  controller.resetEncoders();
  controller.disableTimeout();
  controller.stopMotors();

  bnord = 0;
  bdroite = 0;
  bgauche = 0;

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);
  lcd.print("Ready");

  delay(2000);
 
  controller.stopMotors();

}

void loop()
{
  float wheelPerimeter = 9.8*pi; // en centimètres
  //long distance = 10;
  float distanceEntreRoues = 22;  // en centimètres

  if() {
    avancer(30, wheelPerimeter);
    delay(500);
  } else if() {
    reculer(30, wheelPerimeter);
    delay(500);
  } else if() {
    // Tourne à droite
    tourner(100, wheelPerimeter, distanceEntreRoues);
    delay(500);
  } else if() {
    // Tourne à gauche
    tourner(-100, wheelPerimeter, distanceEntreRoues);
    delay(500);
  }
}

void avancer(long distance, float wheelPerimeter)
{
  controller.stopMotors();
  delay(100);
  controller.resetEncoders();
  distance = convertCentimeterToTick(distance, wheelPerimeter);
  if(vitesse>127)
  {
    vitesse = 127;
  }
  
  double distanceTotale = 0;
  double newDistance=0;
  while(distanceTotale < distance)
  {
    double ecart = distance-distanceTotale;
    newDistance += kP*ecart;
    distanceTotale+=controller.getEncoder1();
    controller.resetEncoders();
    controller.setMotorsSpeed(128+vitesse, 128+vitesse);
    Serial.print("newDistance : ");
    Serial.println(newDistance);
    while(controller.getEncoder1() < newDistance)
    {
      Serial.print("encoder : ");
      Serial.println(controller.getEncoder1());
      Serial.print("test : ");
      Serial.println(controller.getEncoder1() < newDistance);
    }
    controller.stopMotors();
  }
  Serial.println("Motors stopped");
}

void reculer(long distance, float wheelPerimeter)
{
  controller.stopMotors();
  delay(100);
  controller.resetEncoders();
  distance = convertCentimeterToTick(distance, wheelPerimeter);
  if(vitesse>127)
  {
    vitesse = 127;
  }

  double distanceTotale = 0;
  double newDistance=0;
  while(distanceTotale < distance)
  {
    double ecart = distance-distanceTotale;
    newDistance += kP*ecart;
    distanceTotale-=controller.getEncoder1();
    controller.resetEncoders();
    controller.setMotorsSpeed(128-vitesse, 128-vitesse);
    Serial.println(newDistance);
    while(controller.getEncoder1() > -newDistance);
    controller.stopMotors();
  }
}

// A corriger avec asservissement en position
void tourner(int degree, float wheelPerimeter, float distanceEntreRoues)
{
  float centimetresParDegrees = (distanceEntreRoues*pi)/360;
  controller.stopMotors();
  delay(100);
  controller.resetEncoders();
  long distance = convertCentimeterToTick((degree*centimetresParDegrees)/2, wheelPerimeter);
  Serial.println(distance);
  if(vitesse>127)
  {
    vitesse = 127;
  }
  
  if(distance > 0)
  {
    /*
    controller.setMotor1Speed(128-vitesse);
    controller.setMotor2Speed(128+vitesse);
    */
    controller.setMotorsSpeed(128+vitesse, 128-vitesse);
    while(controller.getEncoder2()<distance);
    controller.stopMotors();
  }
  else if(distance < 0)
  {
    /*
    controller.setMotor1Speed(128+vitesse);
    controller.setMotor2Speed(128-vitesse);
    */
    controller.setMotorsSpeed(128-vitesse, 128+vitesse);
    while(controller.getEncoder1()>distance);
    controller.stopMotors();
  }
}

double convertCentimeterToTick(double distanceCm, float wheelPerimeter)
{
  return (360/wheelPerimeter)*distanceCm;
}
