const int relayPin = 26;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);

  // 安全のため最初はOFF
  digitalWrite(relayPin, LOW);
}

void loop() {
  Serial.println("ON");
  digitalWrite(relayPin, HIGH);  // リレーON
  delay(3000);

  Serial.println("OFF");
  digitalWrite(relayPin, LOW);   // リレーOFF
  delay(3000);
}