#include <Wire.h>
#include "rgb_lcd.h"
#include <Servo.h>

Servo servoPanier;

rgb_lcd lcd;

long timeA;

const byte pinLedBlanche = 4;
const byte pinServo = 7;
int count = 0;
int servoAngle = 0;

void setup() {
  servoPanier.attach(pinServo);

  lcd.begin(16, 2);
  lcd.setRGB(100, 0, 0);

  Serial.begin(9600);
  pinMode(pinLedBlanche, OUTPUT);
  digitalWrite(pinLedBlanche, HIGH);
  lcd.setCursor(0, 0);
  lcd.print("cerises :");
  lcd.setRGB(50, 50, 50);
}

void loop() {
  if(millis()-timeA >= 2000) {
    timeA = millis();
    if(servoAngle == 0) {
      servoAngle = 90;
    } else {
      servoAngle = 0;
    }
  }
  servoPanier.write(servoAngle);

  int result = analogRead(A0);
  if(result > 150) {
    lcd.setCursor(0, 1);
    lcd.print(++count);
    delay(100);
  }
}
