/* Controle des 2 moteurs en même temps*/
/* Carte MD25 pilotée en mode serial UART*/
/* Voir documentation MD25...*/
/* Code et librairie perfectibles*/
/* By Ph.Gozlan_280222*/
/*For asso R2T2*/

#include "MD25_UART.h"

MD25_UART controller;

byte vitesse = 30;
const float pi = 3.141593;
const float kP = 0.005;

void avancer(long distance, long distanceParcouru, long distanceTotale);
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
  delay(3000);
 
  controller.stopMotors();
  
  float wheelPerimeter = 9.8*pi; // en centimètres
  //long distance = 10;
  float distanceEntreRoues = 21;  // en centimètres
  
  avancerInit(10, wheelPerimeter);
}

void loop()
{
  
}

// distanceTotale en cm
void avancerInit(long distanceTotale, float wheelPerimeter)
{
  controller.stopMotors();
  delay(100);
  controller.resetEncoders();
  distanceTotale = convertCentimeterToTick(distanceTotale, wheelPerimeter);
  if(vitesse>127)
  {
    vitesse = 127;
  }

  double distance = kP*(distanceTotale);
  avancer(distance, 0, distanceTotale);
}

// distances en ticks
void avancer(long distance, long distanceParcouru, long distanceTotale)
{
  controller.stopMotors();
  delay(500);
  controller.resetEncoders();
  if(vitesse>127)
  {
    vitesse = 127;
  }
  controller.setMotorsSpeed(128+vitesse, 128+vitesse);
  while(controller.getEncoder1() < distance);
  controller.stopMotors();
  distanceParcouru += controller.getEncoder1();
  if(distanceParcouru < distanceTotale+10)
  {
    avancer(kP*(distanceTotale-distanceParcouru), distanceParcouru, distanceTotale);
  }
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
  if(vitesse>127)
  {
    vitesse = 127;
  }
  
  if(distance > 0)
  {
    controller.setMotor1Speed(128-vitesse);
    controller.setMotor2Speed(128+vitesse);
    while(controller.getEncoder2()<distance);
    controller.stopMotors();
  }
  else if(distance < 0)
  {
    controller.setMotor1Speed(128+vitesse);
    controller.setMotor2Speed(128-vitesse);
    while(controller.getEncoder1()<distance);
    controller.stopMotors();
  }
  /*
  if(vitesse>127)
  {
    vitesse = 127;
  }
  if(distance > 0)
  {
    
    //controller.setMotor1Speed(128-vitesse);
    //controller.setMotor2Speed(128+vitesse);
    //while(controller.getEncoder2()<distance); 
    
    double distanceTotale = 0;
    double newDistance=0;
    while(distanceTotale < distance)
    {
      double ecart = distance-distanceTotale;
      newDistance += kP*ecart;
      distanceTotale+=controller.getEncoder2();
      controller.resetEncoders();
      controller.setMotorsSpeed(128-vitesse, 128+vitesse);
      Serial.println(newDistance);
      while(controller.getEncoder2() < newDistance);
      controller.stopMotors();
    }
  }
  else if(distance < 0)
  {
    double distanceTotale = 0;
    double newDistance=0;
    while(distanceTotale < distance)
    {
      double ecart = distance-distanceTotale;
      newDistance += kP*ecart;
      distanceTotale+=controller.getEncoder1();
      controller.resetEncoders();
      controller.setMotorsSpeed(128+vitesse, 128-vitesse);
      Serial.println(newDistance);
      while(controller.getEncoder1() < newDistance);
      controller.stopMotors();
    }
  }
  */
}

double convertCentimeterToTick(double distanceCm, float wheelPerimeter)
{
  return (360/wheelPerimeter)*distanceCm;
}
