const int relayPin = 4;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  Serial.println("HIGH");
  digitalWrite(relayPin, HIGH);
  delay(2000);

  Serial.println("LOW");
  digitalWrite(relayPin, LOW);
  delay(2000);
}
