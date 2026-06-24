//Wifi情報あり！！　注意！！
#include <WiFi.h>
#include <time.h>
#include <WebServer.h>
#include <HTTPClient.h>

WebServer server(80);

int currentMoisture = 0;

const char* ssid = "Input SSID";
const char* password = "Input password";

const int sensorPin = 35;
const int relayPin = 26;

int dryDays = 0;
int lastCheckedDay = -1;

void handleRoot() {
  String html = "";

  html += "<h1>EZ Water Flow</h1>";
  html += "<p>Moisture: " + String(currentMoisture) + "</p>";
  html += "<p>Dry Days: " + String(dryDays) + "</p>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  configTime(9 * 3600, 0, "ntp.nict.jp", "pool.ntp.org");

  server.on("/", handleRoot);
  server.begin();

  Serial.println(WiFi.localIP());

  pinMode(relayPin, OUTPUT);

  Serial.println("Watering!"); // 起動時水やり
  digitalWrite(relayPin, HIGH);
  delay(2000);
  digitalWrite(relayPin, LOW);

  Serial.println(WiFi.localIP());
}

void loop() {
  struct tm timeinfo;
  currentMoisture = analogRead(sensorPin);
  bool watered = false;

  //APIにデータを送信
  HTTPClient http;

  http.begin("http://192.168.1.100:5000/api/data");
  http.addHeader("Content-Type", "application/json");

  String json =
    "{\"moisture\":" + String(currentMoisture) +
    ",\"dryDays\":" + String(dryDays) + 
    ",\"watered\":" + String(watered ? "true" : "false") + "}";

  int responseCode = http.POST(json);

  Serial.print("Response: ");
  Serial.println(responseCode);

  http.end();

  if (getLocalTime(&timeinfo)) {

    // 毎朝8:00に1回だけ実行
    if (timeinfo.tm_hour == 8 &&
        timeinfo.tm_min == 0 &&
        timeinfo.tm_mday != lastCheckedDay) {

      lastCheckedDay = timeinfo.tm_mday;

      int total = 0;

      for(int i = 0; i < 10; i++) {
        total += analogRead(sensorPin);
        delay(100);
      }

      int moisture = total / 10;
      currentMoisture = moisture;

      Serial.println("湿度チェック！");
      Serial.print("Moisture: ");
      Serial.println(moisture);

      if (moisture > 3000) {
        dryDays++;
        Serial.print("Dry days: ");
        Serial.println(dryDays);
      } else {
        dryDays = 0;
        Serial.println("Not dry. Reset dryDays.");
      }

      if (dryDays > 3) {
        Serial.println("Watering!");

        digitalWrite(relayPin, HIGH);
        delay(2000);
        digitalWrite(relayPin, LOW);

        dryDays = 0;
        watered = true;
        Serial.println("dryDays reset.");
      }
    }
  }
  server.handleClient();
  delay(10*1000);
}
