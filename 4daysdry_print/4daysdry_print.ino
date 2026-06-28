//Wifi情報あり！！　注意！！
#include <WiFi.h>
#include <time.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

bool useWeather = true; //天気の利用（屋内屋外）
bool useApi = true;

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

  pinMode(relayPin, OUTPUT);

  Serial.println("Watering!"); // 起動時水やり
  digitalWrite(relayPin, HIGH);
  delay(2000);
  digitalWrite(relayPin, LOW);

  delay(50000);
  Serial.println(WiFi.localIP());
}

bool watered = false;
float rain_sum = 0.0;

void loop() {
  struct tm timeinfo;
  currentMoisture = analogRead(sensorPin);
  
  //APIにデータを送信
  HTTPClient http;
  if(useApi){
    http.begin("http://192.168.3.24:8050/api/sensor");
    http.addHeader("Content-Type", "application/json");

    String json =
      "{\"moisture\":" + String(currentMoisture) +
      ",\"dryDays\":" + String(dryDays) + 
      ",\"watered\":" + String(watered ? "true" : "false") + "}";

    int responseCode = http.POST(json);

    if (responseCode > 0) {
      Serial.println("Sensor API OK");
    } else {
      Serial.println("Sensor API failed");
    }

    http.end();
  }

  watered = false;
  int responseCode = -1;
  //weatherAPI取得
  if (useApi && useWeather){
    http.begin("http://192.168.3.24:8050/api/weather");

    responseCode = http.GET();

    if (responseCode == 200) {
        String payload = http.getString();
        Serial.println(payload);

        JsonDocument doc;
        deserializeJson(doc, payload);

        rain_sum = doc["next_3days_rain_sum"];
    } else {
      rain_sum = 0.0;
      Serial.println("Weather API failed");
    }

    http.end();
  }

  if (getLocalTime(&timeinfo)) {

    // AM8:00
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

      bool shouldWater = dryDays > 3 && (!useWeather || rain_sum < 10.0);
      if (shouldWater) {
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
