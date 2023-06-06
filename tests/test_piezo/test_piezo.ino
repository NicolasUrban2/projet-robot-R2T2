int sensorOutput = 3;

int predValue = 0;
int currentValue;

void setup() 
{
    Serial.begin(9600);
    predValue = analogRead(sensorOutput);
}

void loop() {
  currentValue = analogRead(sensorOutput);
  //Serial.println(currentValue);
  if(currentValue - predValue > 100)
  {
    Serial.println("CERISE !!");
    delay(100);
  }
  predValue = currentValue;
}
