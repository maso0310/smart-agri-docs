/*
 * 第六組：網路控制組
 * 範例 3：發送模擬感測器資料
 *
 * 學習重點：
 * - 定時發送資料
 * - JSON 格式組織
 * - 多主題發布
 *
 * 這個範例模擬真實感測器，
 * 發送假資料到 MQTT Broker
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//===== WiFi 設定 =====
const char* ssid = "你的WiFi名稱";
const char* password = "你的WiFi密碼";

//===== MQTT 設定 =====
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

// 請修改為唯一名稱！
const char* mqtt_client_id = "ESP01_Group6_Sensor";
const char* topic_base = "greenhouse/group6";  // 基礎主題

WiFiClient espClient;
PubSubClient mqtt(espClient);

// 模擬感測器數值
float temperature = 25.0;
float humidity = 65.0;
int soilMoisture = 50;
int lightLevel = 500;

// 發送間隔
unsigned long lastPublish = 0;
const unsigned long publishInterval = 3000;  // 3 秒

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("=== 模擬感測器資料發送 ===");
  Serial.println();

  connectWiFi();

  mqtt.setServer(mqtt_server, mqtt_port);

  Serial.println("準備發送模擬資料...");
  Serial.print("主題前綴: ");
  Serial.println(topic_base);
  Serial.println();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqtt.connected()) {
    connectMQTT();
  }

  mqtt.loop();

  // 定時發送資料
  if (millis() - lastPublish > publishInterval) {
    lastPublish = millis();

    // 更新模擬數值（加入隨機波動）
    updateSimulatedData();

    // 發送各項資料
    publishSensorData();
  }
}

// 更新模擬數值
void updateSimulatedData() {
  // 溫度：22-30°C 波動
  temperature += (random(-10, 11) / 10.0);
  temperature = constrain(temperature, 22.0, 30.0);

  // 濕度：55-80% 波動
  humidity += (random(-20, 21) / 10.0);
  humidity = constrain(humidity, 55.0, 80.0);

  // 土壤濕度：30-70% 波動
  soilMoisture += random(-3, 4);
  soilMoisture = constrain(soilMoisture, 30, 70);

  // 光線：300-800 波動
  lightLevel += random(-30, 31);
  lightLevel = constrain(lightLevel, 300, 800);
}

// 發送感測器資料
void publishSensorData() {
  char topic[50];
  char payload[20];

  Serial.println("─────────────────────────");
  Serial.println("發送感測器資料：");

  // 溫度
  sprintf(topic, "%s/temperature", topic_base);
  dtostrf(temperature, 4, 1, payload);
  mqtt.publish(topic, payload);
  Serial.print("  溫度: ");
  Serial.print(payload);
  Serial.println(" °C");

  // 濕度
  sprintf(topic, "%s/humidity", topic_base);
  dtostrf(humidity, 4, 1, payload);
  mqtt.publish(topic, payload);
  Serial.print("  濕度: ");
  Serial.print(payload);
  Serial.println(" %");

  // 土壤濕度
  sprintf(topic, "%s/soil", topic_base);
  sprintf(payload, "%d", soilMoisture);
  mqtt.publish(topic, payload);
  Serial.print("  土壤: ");
  Serial.print(payload);
  Serial.println(" %");

  // 光線強度
  sprintf(topic, "%s/light", topic_base);
  sprintf(payload, "%d", lightLevel);
  mqtt.publish(topic, payload);
  Serial.print("  光線: ");
  Serial.println(payload);

  // 發送 JSON 格式的完整資料
  publishJSON();

  Serial.println("─────────────────────────");
  Serial.println();
}

// 發送 JSON 格式
void publishJSON() {
  char topic[50];
  char json[150];

  sprintf(topic, "%s/all", topic_base);

  // 組合 JSON 字串
  sprintf(json,
    "{\"temp\":%.1f,\"humid\":%.1f,\"soil\":%d,\"light\":%d}",
    temperature, humidity, soilMoisture, lightLevel
  );

  mqtt.publish(topic, json);

  Serial.print("  JSON: ");
  Serial.println(json);
}

void connectWiFi() {
  Serial.print("連線 WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" 成功！");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("連線 MQTT...");

    if (mqtt.connect(mqtt_client_id)) {
      Serial.println(" 成功！");

      // 發送上線訊息
      char topic[50];
      sprintf(topic, "%s/status", topic_base);
      mqtt.publish(topic, "online");

    } else {
      Serial.print(" 失敗 (");
      Serial.print(mqtt.state());
      Serial.println(")");
      delay(5000);
    }
  }
}

/*
 * 手機 APP 設定說明：
 *
 * IoT MQTT Panel 設定：
 *
 * 1. 新增連線
 *    - Broker: broker.hivemq.com
 *    - Port: 1883
 *
 * 2. 新增 Widget（溫度儀表）
 *    - Type: Gauge
 *    - Topic: greenhouse/group6/temperature
 *    - Min: 15, Max: 40
 *
 * 3. 新增 Widget（濕度儀表）
 *    - Type: Gauge
 *    - Topic: greenhouse/group6/humidity
 *    - Min: 0, Max: 100
 *
 * 4. 新增 Widget（土壤）
 *    - Type: Gauge 或 Progress
 *    - Topic: greenhouse/group6/soil
 *
 * 5. 新增 Widget（光線）
 *    - Type: Text 或 Gauge
 *    - Topic: greenhouse/group6/light
 *
 * 練習題：
 * 1. 修改 topic_base 為你的專屬名稱
 * 2. 在手機上建立完整的監控面板
 * 3. 嘗試加入更多模擬感測器（如 CO2、pH 值）
 */
