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
  delay(2000);
 
  controller.stopMotors();

  float wheelPerimeter = 9.8*pi; // en centimètres
  //long distance = 10;
  float distanceEntreRoues = 22;  // en centimètres
  
  avancer(10, wheelPerimeter);  // Approche premier gâteau
  delay(2000);  // Pose cerise
  reculer(5, wheelPerimeter); // Recule du gâteau
  delay(500);
  tourner(-100, wheelPerimeter, distanceEntreRoues);  // Tourne à droite
  delay(500);
  avancer(20, wheelPerimeter);  // Avance un peu
  delay(500);
  tourner(90, wheelPerimeter, distanceEntreRoues); // Tourne à gauche
  delay(500);
  avancer(35, wheelPerimeter);  // Avance jusqu'au niveau du second gâteau
  delay(500);
  tourner(100, wheelPerimeter, distanceEntreRoues); // Tourne à gauche
  delay(500);
  avancer(5, wheelPerimeter);  // Avance un peu
  delay(2000);  // Pose cerise
  reculer(50, wheelPerimeter);  // Recule
  delay(500);
  tourner(-80, wheelPerimeter, distanceEntreRoues);  // Demi tour
  delay(500);
  avancer(35, wheelPerimeter);  // Avance un peu
  delay(500);
  tourner(100, wheelPerimeter, distanceEntreRoues); // Tourne à gauche
  delay(500);
  avancer(10, wheelPerimeter);  // Avance un peu
  delay(2000);  // Pose cerise
  avancer(50, wheelPerimeter);  // Pousse le gâteau jusqu'au bord
  delay(500);
  tourner(100, wheelPerimeter, distanceEntreRoues); // Se tourne vers l'assiette (vers la gauche)
  delay(500);
  avancer(50, wheelPerimeter);  // Pousse les 3 gâteaux jusqu'à l'assiette
  delay(500);
  
  
  /*
  for(int i=0; i<4; i++)
  {
    avancer(distance, wheelPerimeter);
    //reculer(distance, wheelPerimeter);
    tourner(90,wheelPerimeter , distanceEntreRoues);
  }
  */
}

void loop()
{
  
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
