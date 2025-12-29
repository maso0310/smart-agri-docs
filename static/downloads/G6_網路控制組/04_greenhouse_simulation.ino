/*
 * 第六組：網路控制組
 * 範例 4：隨機模擬溫室資料
 *
 * 學習重點：
 * - 模擬更真實的感測器波動
 * - 多種感測器資料模式
 * - 狀態與警報訊息
 *
 * 此範例模擬一天中溫室的環境變化
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//===== WiFi 設定 =====
const char* ssid = "你的WiFi名稱";
const char* password = "你的WiFi密碼";

//===== MQTT 設定 =====
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_client_id = "ESP01_Greenhouse_Sim";
const char* topic_base = "greenhouse/group6";

WiFiClient espClient;
PubSubClient mqtt(espClient);

// 模擬時間（每 2 秒 = 模擬 1 小時）
int simulatedHour = 6;  // 從早上 6 點開始
unsigned long lastTimeUpdate = 0;

// 感測器資料
float temperature = 18.0;
float humidity = 75.0;
int soilMoisture = 60;
int lightLevel = 100;

// 設備狀態
bool fanOn = false;
bool pumpOn = false;
bool lightOn = false;

// 門檻值
const float TEMP_HIGH = 28.0;
const float TEMP_LOW = 18.0;
const float HUMID_LOW = 50.0;
const int SOIL_DRY = 40;

unsigned long lastPublish = 0;
const unsigned long publishInterval = 2000;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("=== 智慧溫室模擬系統 ===");
  Serial.println("模擬一天的溫室環境變化");
  Serial.println();

  randomSeed(analogRead(0));

  connectWiFi();
  mqtt.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqtt.connected()) {
    connectMQTT();
  }

  mqtt.loop();

  if (millis() - lastPublish > publishInterval) {
    lastPublish = millis();

    // 更新模擬時間
    updateSimulatedTime();

    // 根據時間更新環境資料
    updateEnvironment();

    // 更新設備狀態
    updateDeviceStatus();

    // 發送所有資料
    publishAllData();
  }
}

// 更新模擬時間
void updateSimulatedTime() {
  simulatedHour = (simulatedHour + 1) % 24;
}

// 根據時間模擬環境變化
void updateEnvironment() {
  // 基礎溫度曲線（隨時間變化）
  float baseTemp;
  if (simulatedHour >= 6 && simulatedHour < 12) {
    // 早上升溫
    baseTemp = 18.0 + (simulatedHour - 6) * 2.0;
  } else if (simulatedHour >= 12 && simulatedHour < 16) {
    // 下午最熱
    baseTemp = 28.0 + random(-10, 10) / 10.0;
  } else if (simulatedHour >= 16 && simulatedHour < 20) {
    // 傍晚降溫
    baseTemp = 28.0 - (simulatedHour - 16) * 2.0;
  } else {
    // 夜間低溫
    baseTemp = 18.0 + random(-20, 20) / 10.0;
  }

  // 加入隨機波動
  temperature = baseTemp + random(-10, 11) / 10.0;
  temperature = constrain(temperature, 15.0, 35.0);

  // 濕度（與溫度略呈反比）
  float baseHumid = 80.0 - (temperature - 18.0) * 2;
  humidity = baseHumid + random(-50, 51) / 10.0;
  humidity = constrain(humidity, 40.0, 95.0);

  // 光線（根據時間）
  if (simulatedHour >= 6 && simulatedHour < 18) {
    // 白天
    int hourFromNoon = abs(simulatedHour - 12);
    lightLevel = 1000 - hourFromNoon * 100 + random(-50, 51);
  } else {
    // 夜間
    lightLevel = 50 + random(0, 50);
  }
  lightLevel = constrain(lightLevel, 0, 1100);

  // 土壤濕度（緩慢下降，澆水後上升）
  soilMoisture -= random(0, 3);
  if (pumpOn) {
    soilMoisture += 10;  // 澆水增加
  }
  soilMoisture = constrain(soilMoisture, 20, 90);
}

// 更新設備狀態
void updateDeviceStatus() {
  // 風扇控制
  fanOn = (temperature > TEMP_HIGH);

  // 抽水馬達控制
  pumpOn = (soilMoisture < SOIL_DRY);

  // 補光燈控制
  lightOn = (lightLevel < 300 && simulatedHour >= 6 && simulatedHour < 18);
}

// 發送所有資料
void publishAllData() {
  char topic[60];
  char payload[200];

  Serial.println();
  Serial.print("═══ 模擬時間: ");
  Serial.print(simulatedHour);
  Serial.println(":00 ═══");

  // 發送個別感測器資料
  sprintf(topic, "%s/temperature", topic_base);
  sprintf(payload, "%.1f", temperature);
  mqtt.publish(topic, payload);
  Serial.print("溫度: ");
  Serial.print(payload);
  Serial.print("°C");
  if (temperature > TEMP_HIGH) Serial.print(" [高溫!]");
  Serial.println();

  sprintf(topic, "%s/humidity", topic_base);
  sprintf(payload, "%.1f", humidity);
  mqtt.publish(topic, payload);
  Serial.print("濕度: ");
  Serial.print(payload);
  Serial.print("%");
  if (humidity < HUMID_LOW) Serial.print(" [低濕!]");
  Serial.println();

  sprintf(topic, "%s/soil", topic_base);
  sprintf(payload, "%d", soilMoisture);
  mqtt.publish(topic, payload);
  Serial.print("土壤: ");
  Serial.print(payload);
  Serial.print("%");
  if (soilMoisture < SOIL_DRY) Serial.print(" [乾燥!]");
  Serial.println();

  sprintf(topic, "%s/light", topic_base);
  sprintf(payload, "%d", lightLevel);
  mqtt.publish(topic, payload);
  Serial.print("光線: ");
  Serial.println(payload);

  // 發送設備狀態
  sprintf(topic, "%s/devices", topic_base);
  sprintf(payload, "{\"fan\":%s,\"pump\":%s,\"light\":%s}",
    fanOn ? "true" : "false",
    pumpOn ? "true" : "false",
    lightOn ? "true" : "false"
  );
  mqtt.publish(topic, payload);
  Serial.print("設備: ");
  Serial.print("風扇="); Serial.print(fanOn ? "ON" : "OFF");
  Serial.print(" 抽水="); Serial.print(pumpOn ? "ON" : "OFF");
  Serial.print(" 補光="); Serial.println(lightOn ? "ON" : "OFF");

  // 發送完整 JSON
  sprintf(topic, "%s/all", topic_base);
  sprintf(payload,
    "{\"hour\":%d,\"temp\":%.1f,\"humid\":%.1f,\"soil\":%d,\"light\":%d,\"fan\":%s,\"pump\":%s,\"led\":%s}",
    simulatedHour, temperature, humidity, soilMoisture, lightLevel,
    fanOn ? "true" : "false",
    pumpOn ? "true" : "false",
    lightOn ? "true" : "false"
  );
  mqtt.publish(topic, payload);
}

void connectWiFi() {
  Serial.print("連線 WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" OK");
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("連線 MQTT...");
    if (mqtt.connect(mqtt_client_id)) {
      Serial.println(" OK");
      char topic[50];
      sprintf(topic, "%s/status", topic_base);
      mqtt.publish(topic, "Greenhouse Simulation Online");
    } else {
      Serial.println(" 失敗");
      delay(5000);
    }
  }
}

/*
 * 此範例特色：
 *
 * 1. 模擬真實的日夜變化
 *    - 早上溫度上升
 *    - 下午最熱
 *    - 晚上降溫
 *
 * 2. 自動設備控制邏輯
 *    - 溫度高 → 開風扇
 *    - 土壤乾 → 開抽水
 *    - 光線弱 → 開補光燈
 *
 * 3. 多主題發布
 *    - 個別感測器主題
 *    - 設備狀態主題
 *    - 完整 JSON 主題
 *
 * 在手機 APP 上可以：
 * - 觀察一天的環境變化
 * - 看到自動控制的運作
 * - 用折線圖看趨勢
 */
