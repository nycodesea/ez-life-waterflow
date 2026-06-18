// ピン設定
const int sensorPin = 34;   // 土壌センサー（ADC入力）
const int relayPin  = 26;   // リレー制御

// しきい値（ここは実測で調整）
int dryThreshold = 3000; 

void setup() {
  Serial.begin(115200);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // 最初はOFF
}

void loop() {
  int moistureValue = analogRead(sensorPin);

  Serial.print("Moisture: ");
  Serial.println(moistureValue);

  // 乾いてる判定
  if (moistureValue > dryThreshold) {
    Serial.println("Dry soil -> Water ON");
    digitalWrite(relayPin, HIGH); // ポンプON
    delay(3000); // 3秒水やり
    digitalWrite(relayPin, LOW);  // OFF
  } 
  else {
    Serial.println("Wet enough -> OFF");
    digitalWrite(relayPin, LOW);
  }

  delay(1000 * 60 * 30); // 30分ごとにチェック
}
