int moisturePin = 34;

void setup() {
  Serial.begin(115200);
}

void loop() {
  int moisture = analogRead(moisturePin);

  Serial.println(moisture);

  delay(1000);
}