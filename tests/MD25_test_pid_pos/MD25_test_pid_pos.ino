/* Controle des 2 moteurs en même temps*/
/* Carte MD25 pilotée en mode serial UART*/
/* Voir documentation MD25...*/
/* Code et librairie perfectibles*/
/* By Ph.Gozlan_280222*/
/*For asso R2T2*/

#include "MD25_UART.h"

MD25_UART controller;

int ValEnc1;
int ValEnc2;

int nbTicksAAtteindre = 720; // 2 tours

const float Kp = 0.005;
const float Ki = 0;
const float Kd = 0;
int vitesseGauchePid = 0;
int vitesseDroitPid = 0;
int vitesseMax = 20; // Entre 0 et 127
byte vitesseGauche = 0;
byte vitesseDroit = 0;
int ecartGauche, ecartDroit;
int marge = 10;

void deplacer(double distance);

volatile byte cpt_ovf=0;

ISR(TIMER2_OVF_vect)  // Avec un prescaler de 1024 sur le timer2, on atteint 98.304 ms après 6 overflows
{
  if(++cpt_ovf >= 6)
  {
    cpt_ovf = 0;
    if(ValEnc1 < nbTicksAAtteindre+marge && ValEnc1 > nbTicksAAtteindre-marge)
    {
      ValEnc1 = nbTicksAAtteindre;
    }
    if(ValEnc2 < nbTicksAAtteindre+marge && ValEnc2 > nbTicksAAtteindre-marge)
    {
      ValEnc2 = nbTicksAAtteindre;
    }
    ecartGauche = nbTicksAAtteindre - ValEnc1;
    ecartDroit = nbTicksAAtteindre - ValEnc2;
    
    if(ecartGauche == 0)
    {
      vitesseGauchePid = 0;
    }
    else
    {
      if((int) (Kp*ecartGauche)<0)
      {
        vitesseGauchePid += (int) (Kp*ecartGauche)-1;
      }
      else
      {
        vitesseGauchePid += (int) (Kp*ecartGauche)+1;
      }
    }
    
    if(vitesseGauchePid > vitesseMax)
    {
      vitesseGauchePid = vitesseMax;
    }
    else if(vitesseGauchePid < (0-vitesseMax))
    {
      vitesseGauchePid = (0-vitesseMax);
    }
    vitesseGauche = (byte) 128+vitesseGauchePid; // Récupère la vitesse en mode 0 (0 = vitesse max en arrière, 128 = arrêt, 255 = vitesse max en avant)

    if(ecartDroit == 0)
    {
      vitesseDroitPid = 0;
    }
    else
    {
      if((int) (Kp*ecartDroit)<0)
      {
        vitesseDroitPid += (int) (Kp*ecartDroit)-1;
      }
      else
      {
        vitesseDroitPid += (int) (Kp*ecartDroit)+1;
      }
    }
    
    if(vitesseDroitPid > vitesseMax)
    {
      vitesseDroitPid = vitesseMax;
    }
    else if(vitesseDroitPid < (0-vitesseMax))
    {
      vitesseDroitPid = (0-vitesseMax);
    }
    vitesseDroit = (byte) 128+vitesseDroitPid;
  }
}

void setup() 
{
  cli();
  Serial.begin(9600);
  controller.setAccelerationRate(1);
  controller.enableSpeedRegulation(); // Active l'asservissement en vitesse de la MD25
  controller.setMode(0);  // mode 0 (0 = vitesse max en arrière, 128 = arrêt, 255 = vitesse max en avant) la vitesse est gérée sur les 2 moteurs indépendamment 
  controller.resetEncoders();
  controller.disableTimeout();
  TCCR2A=0;     // Timer2 en mode normal
  TCCR2B=0x07;  // Prescaler = 1024
  TIMSK2=0x01;  // Interruption Timer2 overflow active
  sei();
  delay(2000);
 
  controller.stopMotors();
}

void loop()
{
    Serial.print("vitesseGauche : ");
    Serial.println(ValEnc1);
    Serial.print("vitesseDroit : ");
    Serial.println(ValEnc2);
    //controller.setMotorsSpeed(147,147);
    controller.setMotorsSpeed(vitesseGauche,vitesseDroit);
    ValEnc1=controller.getEncoder1();
    ValEnc2=controller.getEncoder2();
}

void deplacer(double distance)
{
  float kP = 0.05; 
  long distanceAFaireGauche = 0;
  long distanceAFaireDroite = 0;
  long valEnc1=0;
  long valEnc2=0;
  long ecartGauche, ecartDroit;
  bool distanceRealisee = false;

  int vitesseGauche=0;
  int vitesseDroit=0;
  
  
  
  while(!distanceRealisee)
  {
    ecartGauche = distance - valEnc1;
    ecartDroit = distance - valEnc2;

    distanceAFaireGauche += kP*ecartGauche;
    distanceAFaireDroit += kP*ecartDroit;
    
    controller.resetEncoders();
    controller.setMotorsSpeed()
    
    while((controller.getEncoder1() < distanceAFaireGauche) && (controller.getEncoder2() < distanceAFaireDroit))
    {
       
    }
  }
}
