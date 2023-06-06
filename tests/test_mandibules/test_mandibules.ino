#include <Servo.h>

Servo servoPanier1;
Servo servoPanier2;

void setup() {
  servoPanier1.attach(2);
  servoPanier2.attach(3);
}

void loop() {
  servoPanier1.write(0);
  servoPanier2.write(0);
  delay(2000);
  servoPanier1.write(10);
  servoPanier2.write(10);
  delay(2000);
}
