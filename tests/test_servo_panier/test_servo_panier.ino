#include <Servo.h>

Servo servoPanier;
bool start = true;

void setup() {
  servoPanier.attach(2);
}

// 25 haut
// 0 bas
void loop() {
  if(start) {
    start = false;
    release();
  }

}

void release() {
  for(int i=0; i<5; i++) {
    servoPanier.write(0);
    delay(500);
    servoPanier.write(25);
    delay(500);
  }
}

void lock() {
  servoPanier.write(25);
}
