/*
 * 第六組：網路控制組
 * 範例 5：完整物聯網溫室系統（整合實作）
 *
 * 功能說明：
 * - WiFi 自動連線與重連
 * - MQTT 發布感測器資料
 * - 接收遠端控制指令
 * - LED 狀態指示
 * - 斷線重連機制
 *
 * 這就是智慧溫室物聯網的基本原型！
 *
 * 接收控制指令：訂閱 greenhouse/group6/cmd 主題
 * 發送指令範例：{"fan":true} 或 {"led":"on"}
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//===== WiFi 設定（請修改）=====
const char* ssid = "你的WiFi名稱";
const char* password = "你的WiFi密碼";

//===== MQTT 設定（請修改 client_id 和 topic_base）=====
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_client_id = "ESP01_SmartGreenhouse_v1";
const char* topic_base = "greenhouse/group6";

//===== 硬體腳位 =====
#define LED_PIN 2  // ESP-01 內建 LED（GPIO2，低電平亮）

WiFiClient espClient;
PubSubClient mqtt(espClient);

//===== 模擬感測器資料 =====
float temperature = 25.0;
float humidity = 65.0;
int soilMoisture = 50;
int lightLevel = 500;

//===== 模擬設備狀態 =====
bool fanEnabled = false;
bool pumpEnabled = false;
bool ledEnabled = false;

//===== 時間控制 =====
unsigned long lastPublish = 0;
unsigned long lastReconnect = 0;
const unsigned long publishInterval = 3000;  // 3 秒
const unsigned long reconnectInterval = 5000;

//===== 統計資料 =====
unsigned long messagesSent = 0;
unsigned long messagesReceived = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED 關閉

  Serial.println();
  Serial.println("╔═══════════════════════════════════════╗");
  Serial.println("║    智慧溫室 物聯網系統 v1.0           ║");
  Serial.println("║    ESP-01 + MQTT + IoT MQTT Panel     ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.println();

  randomSeed(analogRead(0));

  connectWiFi();

  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(mqttCallback);
  mqtt.setBufferSize(512);

  connectMQTT();

  Serial.println("系統啟動完成！");
  Serial.println();
  printTopics();
}

void loop() {
  // 維護連線
  maintainConnection();

  // MQTT 迴圈
  mqtt.loop();

  // 定時發送資料
  if (millis() - lastPublish > publishInterval) {
    lastPublish = millis();

    updateSimulatedData();
    publishSensorData();

    // LED 閃爍表示活動
    blinkLED();
  }
}

//===== 維護連線 =====
void maintainConnection() {
  // WiFi 斷線重連
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[!] WiFi 斷線，重新連線...");
    connectWiFi();
  }

  // MQTT 斷線重連
  if (!mqtt.connected()) {
    if (millis() - lastReconnect > reconnectInterval) {
      lastReconnect = millis();
      Serial.println("[!] MQTT 斷線，重新連線...");
      connectMQTT();
    }
  }
}

//===== 連接 WiFi =====
void connectWiFi() {
  Serial.print("[WiFi] 連線中");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 40) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" 成功！");
    Serial.print("[WiFi] IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("[WiFi] 訊號: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println(" 失敗！");
  }
}

//===== 連接 MQTT =====
void connectMQTT() {
  Serial.print("[MQTT] 連線中...");

  if (mqtt.connect(mqtt_client_id)) {
    Serial.println(" 成功！");

    // 訂閱控制主題
    char cmdTopic[60];
    sprintf(cmdTopic, "%s/cmd", topic_base);
    mqtt.subscribe(cmdTopic);
    Serial.print("[MQTT] 訂閱: ");
    Serial.println(cmdTopic);

    // 發送上線訊息
    char statusTopic[60];
    sprintf(statusTopic, "%s/status", topic_base);
    mqtt.publish(statusTopic, "{\"status\":\"online\",\"version\":\"1.0\"}");

    digitalWrite(LED_PIN, LOW);  // LED 常亮表示連線

  } else {
    Serial.print(" 失敗 (");
    Serial.print(mqtt.state());
    Serial.println(")");
    digitalWrite(LED_PIN, HIGH);  // LED 關閉表示離線
  }
}

//===== MQTT 訊息回調 =====
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  messagesReceived++;

  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println();
  Serial.print("[收到] ");
  Serial.print(topic);
  Serial.print(" → ");
  Serial.println(message);

  // 解析控制指令
  parseCommand(message);

  // 發送確認
  char ackTopic[60];
  sprintf(ackTopic, "%s/ack", topic_base);
  mqtt.publish(ackTopic, message.c_str());
}

//===== 解析控制指令 =====
void parseCommand(String cmd) {
  // 簡易 JSON 解析
  if (cmd.indexOf("\"fan\":true") >= 0 || cmd.indexOf("\"fan\":1") >= 0) {
    fanEnabled = true;
    Serial.println("[控制] 風扇 ON");
  }
  if (cmd.indexOf("\"fan\":false") >= 0 || cmd.indexOf("\"fan\":0") >= 0) {
    fanEnabled = false;
    Serial.println("[控制] 風扇 OFF");
  }

  if (cmd.indexOf("\"pump\":true") >= 0 || cmd.indexOf("\"pump\":1") >= 0) {
    pumpEnabled = true;
    Serial.println("[控制] 抽水 ON");
  }
  if (cmd.indexOf("\"pump\":false") >= 0 || cmd.indexOf("\"pump\":0") >= 0) {
    pumpEnabled = false;
    Serial.println("[控制] 抽水 OFF");
  }

  if (cmd.indexOf("\"led\":\"on\"") >= 0 || cmd.indexOf("\"led\":true") >= 0) {
    ledEnabled = true;
    Serial.println("[控制] LED ON");
  }
  if (cmd.indexOf("\"led\":\"off\"") >= 0 || cmd.indexOf("\"led\":false") >= 0) {
    ledEnabled = false;
    Serial.println("[控制] LED OFF");
  }
}

//===== 更新模擬資料 =====
void updateSimulatedData() {
  temperature += (random(-10, 11) / 10.0);
  temperature = constrain(temperature, 18.0, 35.0);

  humidity += (random(-15, 16) / 10.0);
  humidity = constrain(humidity, 40.0, 90.0);

  soilMoisture += random(-2, 3);
  if (pumpEnabled) soilMoisture += 5;
  soilMoisture = constrain(soilMoisture, 20, 80);

  lightLevel += random(-30, 31);
  lightLevel = constrain(lightLevel, 100, 1000);
}

//===== 發送感測器資料 =====
void publishSensorData() {
  char topic[60];
  char payload[256];

  messagesSent++;

  // 個別主題
  sprintf(topic, "%s/temperature", topic_base);
  sprintf(payload, "%.1f", temperature);
  mqtt.publish(topic, payload);

  sprintf(topic, "%s/humidity", topic_base);
  sprintf(payload, "%.1f", humidity);
  mqtt.publish(topic, payload);

  sprintf(topic, "%s/soil", topic_base);
  sprintf(payload, "%d", soilMoisture);
  mqtt.publish(topic, payload);

  sprintf(topic, "%s/light", topic_base);
  sprintf(payload, "%d", lightLevel);
  mqtt.publish(topic, payload);

  // 完整 JSON
  sprintf(topic, "%s/all", topic_base);
  sprintf(payload,
    "{\"temp\":%.1f,\"humid\":%.1f,\"soil\":%d,\"light\":%d,"
    "\"fan\":%s,\"pump\":%s,\"led\":%s,"
    "\"rssi\":%d,\"sent\":%lu,\"recv\":%lu}",
    temperature, humidity, soilMoisture, lightLevel,
    fanEnabled ? "true" : "false",
    pumpEnabled ? "true" : "false",
    ledEnabled ? "true" : "false",
    WiFi.RSSI(), messagesSent, messagesReceived
  );
  mqtt.publish(topic, payload);

  // 序列埠輸出
  Serial.print("[發送 #");
  Serial.print(messagesSent);
  Serial.print("] T:");
  Serial.print(temperature, 1);
  Serial.print(" H:");
  Serial.print(humidity, 1);
  Serial.print(" S:");
  Serial.print(soilMoisture);
  Serial.print(" L:");
  Serial.println(lightLevel);
}

//===== LED 閃爍 =====
void blinkLED() {
  if (mqtt.connected()) {
    digitalWrite(LED_PIN, HIGH);
    delay(30);
    digitalWrite(LED_PIN, LOW);
  }
}

//===== 顯示主題資訊 =====
void printTopics() {
  Serial.println("─────────────────────────────────────────");
  Serial.println("MQTT 主題說明：");
  Serial.print("  發布: ");
  Serial.print(topic_base);
  Serial.println("/temperature");
  Serial.print("  發布: ");
  Serial.print(topic_base);
  Serial.println("/humidity");
  Serial.print("  發布: ");
  Serial.print(topic_base);
  Serial.println("/soil");
  Serial.print("  發布: ");
  Serial.print(topic_base);
  Serial.println("/light");
  Serial.print("  發布: ");
  Serial.print(topic_base);
  Serial.println("/all (JSON)");
  Serial.print("  訂閱: ");
  Serial.print(topic_base);
  Serial.println("/cmd (控制)");
  Serial.println();
  Serial.println("控制指令範例：");
  Serial.println("  {\"fan\":true}");
  Serial.println("  {\"pump\":false}");
  Serial.println("  {\"led\":\"on\"}");
  Serial.println("─────────────────────────────────────────");
  Serial.println();
}

/*
 * ═══════════════════════════════════════════
 *        手機 APP 設定指南
 * ═══════════════════════════════════════════
 *
 * 1. 下載 IoT MQTT Panel（iOS / Android）
 *
 * 2. 新增連線：
 *    - Name: Greenhouse
 *    - Broker: broker.hivemq.com
 *    - Port: 1883
 *
 * 3. 新增 Widget：
 *
 *    [溫度儀表]
 *    - Type: Gauge
 *    - Topic: greenhouse/group6/temperature
 *    - Min: 15, Max: 40
 *    - Unit: °C
 *
 *    [濕度儀表]
 *    - Type: Gauge
 *    - Topic: greenhouse/group6/humidity
 *    - Min: 0, Max: 100
 *    - Unit: %
 *
 *    [土壤進度條]
 *    - Type: Progress
 *    - Topic: greenhouse/group6/soil
 *    - Min: 0, Max: 100
 *
 *    [光線數值]
 *    - Type: Text
 *    - Topic: greenhouse/group6/light
 *
 *    [風扇開關]
 *    - Type: Switch
 *    - Topic Pub: greenhouse/group6/cmd
 *    - Payload On: {"fan":true}
 *    - Payload Off: {"fan":false}
 *
 *    [抽水開關]
 *    - Type: Switch
 *    - Topic Pub: greenhouse/group6/cmd
 *    - Payload On: {"pump":true}
 *    - Payload Off: {"pump":false}
 *
 * ═══════════════════════════════════════════
 *
 * 實作挑戰：
 *
 * 1. 連接真實感測器
 *    將模擬資料換成真實的 DHT11、土壤感測器
 *
 * 2. 加入更多控制
 *    窗戶開關、遮陽棚、警報器等
 *
 * 3. 資料記錄
 *    將資料存到 ThingSpeak 或自架伺服器
 *
 * 4. 多裝置協作
 *    多個 ESP-01 組成感測網路
 *
 * 5. 安全性
 *    加入 MQTT 帳號密碼認證
 *
 * 恭喜完成物聯網溫室的基礎！
 * ═══════════════════════════════════════════
 */
