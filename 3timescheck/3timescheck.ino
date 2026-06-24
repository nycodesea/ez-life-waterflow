const int relayPin = 26;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
}

void loop() {
  Serial.println("HIGH");
  digitalWrite(relayPin, HIGH);
  delay(2000);

  Serial.println(moisture);

  if (moisture > 3000) {   // 乾燥判定（値は要調整）
    count+=1;
    Serial.println("Count up!");
    Serial.println(count);
  } else {
    digitalWrite(relayPin, LOW);  // リレーOFF
    count = 0;
    Serial.println("Not dry.");
  }

  if (count >= 3) {   // 乾燥判定（値は要調整）
      digitalWrite(relayPin, HIGH);   // リレーON
      Serial.println("Turn on!!");
      delay(3000);
      count=0;
      Serial.println("Count 0");
      
  }
  
  Serial.println("LOW");
  digitalWrite(relayPin, LOW);
  delay(2000);
}
