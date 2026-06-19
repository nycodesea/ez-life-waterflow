const int sensorPin = 35;  // 湿度センサー
const int relayPin = 26;   // リレー

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  int moisture = analogRead(sensorPin);

  Serial.println(moisture);

  if (moisture > 3000) {   // 乾燥判定（値は要調整）
    digitalWrite(relayPin, HIGH);   // リレーON
    delay(5000);
  } else {
    digitalWrite(relayPin, LOW);  // リレーOFF
  }

  
  digitalWrite(relayPin, LOW);
  delay(60*1000); //1分
}