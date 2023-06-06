#include "r2t2_md25.h"

// prends en argument le port Serial qu'on utilise pour la communication
// ainsi que le diamètre des roues
R2T2MD25 md25(&Serial1, 10);

// indice des pins utilisé pour les capteurs TOF
int tofPins[2] = {11, 10};

void setup() {
    // nombre de capteurs + l'index de chaque capteur utilisé
    CapteurTOF* tof = new CapteurTOF(2, tofPins);
    md25.set_distance_sensor(tof);
}

void loop() {
    // on avance en avant
    // - à une vitesse de 20
    // - sur 100 centimètres
    // - avec des capteurs qui détectent des objets à 230mm
    md25.move(FORWARD, 20, 100, 230);

    // une fois qu'on a parcouru la distance souhaitée
    // on tourne de 90° à droite en avant à la même vitesse
    md25.turn(RIGHT, FORWARD, 90, 20);

    // on ne fait plus rien
    while (true);
}
