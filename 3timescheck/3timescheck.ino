const int sensorPin = 35;  // 湿度センサー
const int relayPin = 26;   // リレー
int count =0;

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void loop() {
  int moisture = analogRead(sensorPin);

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
      delay(5000);
      count=0;
      Serial.println("Count 0");
      
  }
  
  digitalWrite(relayPin, LOW);
  Serial.println("Set low");
  delay(60 * 1000);
}