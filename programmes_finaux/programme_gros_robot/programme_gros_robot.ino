/* Controle des 2 moteurs en même temps*/
/* Carte MD25 pilotée en mode serial UART*/
/* Voir documentation MD25...*/
/* Code et librairie perfectibles*/
/* By Ph.Gozlan_280222*/
/*For asso R2T2*/

#include "MD25_UART.h"

MD25_UART controller;

byte vitesse = 30;  // 0 < vitesse < 127
const float pi = 3.141593;
const float kP = 0.5;
const int ecartDistanceAccepte = 10;
const float wheelPerimeter = 9.8*pi; // en centimètres
const float distanceEntreRoues = 22;  // en centimètres

// Si les roues ne tournent pas comme on veut -> changer les constantes ci-dessous
const bool roue1Inversee = true;
const bool roue2Inversee = false; // Moteur gauche sur gros robot

#define equipe 8
#define portJack 2

void avancer(long distance, float wheelPerimeter);
void avancerRecu(long distanceTick, long distanceTotaleTick, long distanceTotaleParcourueTick);
void reculer(long distanceCentimeter, float wheelPerimeter);
void reculerRecu(long distanceTick, long distanceTotaleTick, long distanceTotaleParcourueTick);
void tourner(int degree, float wheelPerimeter, float distanceEntreRoues); // Si degree > 0, tourne à gauche. Sinon tourne à droite
void setSpeedToMotors(bool avancerMoteur1, bool avancerMoteur2);
void resetEncoders();
double convertCentimeterToTick(long distanceCm, float wheelPerimeter);

bool firstStart = true;
bool equipeVerte;
bool start = true;

void setup() 
{
  Serial.begin(9600);
  controller.setAccelerationRate(1);
  controller.enableSpeedRegulation(); // Active l'asservissement en vitesse de la MD25
  controller.setMode(0);  // mode 0 (0 = vitesse max en arrière, 128 = arrêt, 255 = vitesse max en avant) la vitesse est gérée sur les 2 moteurs indépendamment
  controller.disableTimeout();
  resetEncoders();

  /*
  *Lorsque c'est à 0 , l'équipe verte
  *Lorsque c'est à 1 , l'équipe  bleu
  *Pin 8 , on utilise un input_pullup pour savoir quand il y a changement de courant .
  *Pin GND, pour faire traverser le courant .
  *pin 3,3V pour faire envoyer le courant .
  */
  pinMode(equipe,INPUT_PULLUP);

  /*
  *Lorsque c'est à 0 , le jack est déconnecté
  *Lorsque c'est à 1 , le jack est connecté
  *Pin 2 , on utilise un input_pullup pour savoir quand il y a changement de courant .
  *Pin GND, pour faire traverser le courant .
  */
  pinMode(portJack,INPUT_PULLUP);

  delay(2000);
}

void loop()
{
  if(firstStart) {
    if(digitalRead(equipe)) {
      equipeVerte = false;
    } else {
      equipeVerte = true;
    }

    firstStart = false;
  }

  // DEBUT

  if(!digitalRead(portJack)) {

    int factor;
    if(equipeVerte) {
      factor = 1;
    } else {
      factor = -1;
    }

    if(start) {
      start = false;
      avancer(28, wheelPerimeter);  // Approche premier gâteau
      delay(500);  // Pose cerise
      //servoCerise();
      delay(500);
      reculer(10, wheelPerimeter); // Recule du gâteau
      delay(500);
      tourner(factor*(-40), wheelPerimeter, distanceEntreRoues);  // Tourne à droite
      delay(500);
      avancer(35, wheelPerimeter);  // Avance un peu
      delay(500);
      tourner(factor*(110), wheelPerimeter, distanceEntreRoues); // Tourne à gauche
      delay(500);
      avancer(15, wheelPerimeter);  // Avance jusqu'au niveau du second gâteau
      delay(500);
      //servoCerise()
      reculer(15,wheelPerimeter);
      delay(500);
      tourner(factor*(110), wheelPerimeter, distanceEntreRoues); // Tourne à gauche
      delay(500);
      avancer(43, wheelPerimeter);  // Avance un peu
      delay(500);
      tourner(factor*(65), wheelPerimeter, distanceEntreRoues);  // Demi tour
      delay(500);
      avancer(8, wheelPerimeter);  // Avance un peu
      delay(500);
      tourner(factor*(45), wheelPerimeter, distanceEntreRoues); // Tourne à gauche
      delay(500);  // Pose cerise
      //servoCerise()
      avancer(55, wheelPerimeter);  // 
      delay(500);
      tourner(factor*(90), wheelPerimeter, distanceEntreRoues); // Tourne à gauche
      delay(500);
      avancer(65, wheelPerimeter);  // Pousse les 3 gâteaux jusqu'à l'assiette
      delay(500);

      //Partie 2.
      reculer(35,wheelPerimeter); //recule 50 cm
      delay(500);
      tourner(factor*(120),wheelPerimeter,distanceEntreRoues);//Tourner pour suivre la ligne noir 
      delay(500);
      avancer(39,wheelPerimeter);// Avancer de 60 cm pour faire la grande ligne droite 
      delay(500);
      tourner(factor*(121),wheelPerimeter,distanceEntreRoues);//Tourne pour suivre la grosse ligne 
      delay(500);
      avancer(70,wheelPerimeter);//Avancer de 75 cm .
      delay(500);//Pose de la cerise
      //servoCerise()
      tourner(factor*(90),wheelPerimeter,distanceEntreRoues);
      delay(500);
      avancer(40,wheelPerimeter);//Amène le premier gateau dans l'autre casse 
      delay(500);
      reculer(5,wheelPerimeter);
      delay(500);
      tourner(factor*(-90),wheelPerimeter,distanceEntreRoues);
      delay(500);
      avancer(13,wheelPerimeter);//Arrive vers le 5 gateau 
      delay(500);
      //servoCerise()
      reculer(6,wheelPerimeter);
      delay(500);
      tourner(factor*(-52),wheelPerimeter,distanceEntreRoues);
      delay(500);
      avancer(19,wheelPerimeter);
      delay(500);
      tourner(factor*(70),wheelPerimeter,distanceEntreRoues);
      delay(500);
      avancer(20,wheelPerimeter);//arrive ver le 6 gateaux
      delay(500);
      tourner(factor*(56),wheelPerimeter,distanceEntreRoues);
      delay(500);
      avancer(40,wheelPerimeter);
      delay(500);
      reculer(12,wheelPerimeter);
      delay(500);
      tourner(factor*(86),wheelPerimeter,distanceEntreRoues);
      delay(500);
      avancer(38,wheelPerimeter);
      delay(500);
      tourner(factor*(90),wheelPerimeter,distanceEntreRoues);
      delay(500);
      reculer(57,wheelPerimeter);//Arrive dans la zone d'arrivé
    }
  }
}

void avancer(long distanceCentimeter, float wheelPerimeter)
{
  long distanceTotaleTick = convertCentimeterToTick(distanceCentimeter, wheelPerimeter);

  avancerRecu(kP*distanceTotaleTick, distanceTotaleTick, 0);
}


void avancerRecu(long distanceTick, long distanceTotaleTick, long distanceTotaleParcourueTick) {
  resetEncoders();
  if(distanceTotaleParcourueTick < (distanceTotaleTick-ecartDistanceAccepte)) {
    // Faire avancer moteur1 et avancer moteur2
    setSpeedToMotors(true, true);
    
    if(roue1Inversee) {
      while(-(controller.getEncoder1()) < distanceTick);
    } else {
      while(controller.getEncoder1() < distanceTick);
    }
    

    controller.stopMotors();

    distanceTotaleParcourueTick += distanceTick;
    avancerRecu(kP*(distanceTotaleTick - distanceTotaleParcourueTick), distanceTotaleTick, distanceTotaleParcourueTick);
  }
}





void reculer(long distanceCentimeter, float wheelPerimeter)
{
  long distanceTotaleTick = convertCentimeterToTick(distanceCentimeter, wheelPerimeter);
  reculerRecu(kP*distanceTotaleTick, distanceTotaleTick, 0);
}


void reculerRecu(long distanceTick, long distanceTotaleTick, long distanceTotaleParcourueTick) {
  resetEncoders();
  if(distanceTotaleParcourueTick < (distanceTotaleTick-ecartDistanceAccepte)) {
    
    // Faire reculer moteur1 et reculer moteur2
    setSpeedToMotors(false, false);

    if(!roue1Inversee) {
      while(-(controller.getEncoder1()) < distanceTick) {Serial.println(-(controller.getEncoder1()));};
    } else {
      while(controller.getEncoder1() < distanceTick) {Serial.println(controller.getEncoder1());};
    }
    controller.stopMotors();

    distanceTotaleParcourueTick += distanceTick;
    reculerRecu(kP*(distanceTotaleTick - distanceTotaleParcourueTick), distanceTotaleTick, distanceTotaleParcourueTick);
  }
}





// A corriger avec asservissement en position
void tourner(int degree, float wheelPerimeter, float distanceEntreRoues)
{
  resetEncoders();
  float centimetresParDegrees = (distanceEntreRoues*pi)/360;
  long distance = convertCentimeterToTick((degree*centimetresParDegrees)/2, wheelPerimeter);
  Serial.println(distance);
  
  if(distance > 0)
  {
    setSpeedToMotors(true, false);
    //controller.setMotorsSpeed(128+vitesse, 128-vitesse);
    if(roue1Inversee) {
      while(-(controller.getEncoder1()) < distance);
    } else {
      while(controller.getEncoder1() < distance);
    }
    controller.stopMotors();
  }
  else if(distance < 0)
  {
    setSpeedToMotors(false, true);
    //controller.setMotorsSpeed(128-vitesse, 128+vitesse);
    if(roue1Inversee) {
      while(-(controller.getEncoder1()) > distance);
    } else {
      while(controller.getEncoder1() > distance);
    }
    controller.stopMotors();
  }
}

void setSpeedToMotors(bool avancerMoteur1, bool avancerMoteur2) {
  byte vitesseMoteur1 = vitesse;
  if(!avancerMoteur1) {
    vitesseMoteur1 = -vitesseMoteur1;
  }
  if(roue1Inversee) {
    vitesseMoteur1 = -vitesseMoteur1;
  }

  byte vitesseMoteur2 = vitesse;
  if(!avancerMoteur2) {
    vitesseMoteur2 = -vitesseMoteur2;
  }
  if(roue2Inversee) {
    vitesseMoteur2 = -vitesseMoteur2;
  }

  controller.setMotorsSpeed(128+vitesseMoteur1, 128+vitesseMoteur2);
}

void resetEncoders() {
  controller.stopMotors();
  delay(100);
  controller.resetEncoders();
}

double convertCentimeterToTick(long distanceCentimeter, float wheelPerimeter)
{
  return (360/wheelPerimeter)*distanceCentimeter;
}
