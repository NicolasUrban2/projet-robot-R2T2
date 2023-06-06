const byte ledBlanche = 4;
int count = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledBlanche, OUTPUT);
  digitalWrite(ledBlanche, HIGH);
  delay(2000);
  Serial.println("debut");
}

void loop() {
  //delay(1000);
 // int resultA = resultB;
  int result = analogRead(A0);
  //Serial.println(result);
  if(result < 750) {
    Serial.print("Nombre de balles detectees : ");
    Serial.println(++count);
    delay(100);
  }
  //delay(250);
  //digitalWrite(ledBlanche, HIGH);

}
