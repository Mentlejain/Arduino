int obstacle;
void setup() {
  pinMode(8,INPUT);
  Serial.begin(9600);
}
void loop() {
  
  obstacle=HIGH;
  if (obstacle==LOW)
  {
    Serial.println("All Clear");
  }
  else
  {
    Serial.println("Stop");
  }
  delay(500);
}
