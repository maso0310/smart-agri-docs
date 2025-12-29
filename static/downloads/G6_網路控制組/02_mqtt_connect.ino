/*
 * 第六組：網路控制組
 * 範例 2：MQTT 連線測試
 *
 * 學習重點：
 * - MQTT 基本連線
 * - 發布簡單訊息
 * - 連線保持（Keep Alive）
 *
 * 需安裝函式庫：PubSubClient
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//===== WiFi 設定 =====
const char* ssid = "你的WiFi名稱";
const char* password = "你的WiFi密碼";

//===== MQTT 設定 =====
const char* mqtt_server = "broker.hivemq.com";  // 公開 Broker
const int mqtt_port = 1883;

// 請修改為唯一的名稱（如：學號或姓名）避免衝突
const char* mqtt_client_id = "ESP01_Group6_Test";
const char* mqtt_topic = "greenhouse/group6/test";

WiFiClient espClient;
PubSubClient mqtt(espClient);

int messageCount = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("=== ESP-01 MQTT 連線測試 ===");
  Serial.println();

  // 連接 WiFi
  connectWiFi();

  // 設定 MQTT
  mqtt.setServer(mqtt_server, mqtt_port);
  mqtt.setCallback(mqttCallback);  // 設定接收訊息的回調函數

  Serial.println("開始連線 MQTT Broker...");
}

void loop() {
  // 確保 WiFi 連線
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  // 確保 MQTT 連線
  if (!mqtt.connected()) {
    connectMQTT();
  }

  // 處理 MQTT 訊息
  mqtt.loop();

  // 每 5 秒發送一次訊息
  static unsigned long lastPublish = 0;
  if (millis() - lastPublish > 5000) {
    lastPublish = millis();

    messageCount++;

    // 建立訊息
    String message = "Hello from ESP-01! Count: ";
    message += messageCount;

    // 發布訊息
    Serial.print("發布訊息: ");
    Serial.println(message);

    if (mqtt.publish(mqtt_topic, message.c_str())) {
      Serial.println("發布成功！");
    } else {
      Serial.println("發布失敗！");
    }

    Serial.println();
  }
}

// 連接 WiFi
void connectWiFi() {
  Serial.print("連線 WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi 已連線！");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// 連接 MQTT
void connectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("連線 MQTT Broker...");

    if (mqtt.connect(mqtt_client_id)) {
      Serial.println("成功！");

      // 訂閱主題（用於接收訊息）
      String subTopic = String(mqtt_topic) + "/cmd";
      mqtt.subscribe(subTopic.c_str());
      Serial.print("已訂閱: ");
      Serial.println(subTopic);

      // 發送上線訊息
      mqtt.publish(mqtt_topic, "ESP-01 已上線！");

    } else {
      Serial.print("失敗，錯誤碼: ");
      Serial.println(mqtt.state());
      Serial.println("5 秒後重試...");
      delay(5000);
    }
  }
}

// MQTT 訊息回調函數
void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("收到訊息 [");
  Serial.print(topic);
  Serial.print("]: ");

  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
}

/*
 * 知識補充：
 *
 * MQTT 連線狀態碼（mqtt.state()）：
 * - -4：MQTT_CONNECTION_TIMEOUT
 * - -3：MQTT_CONNECTION_LOST
 * - -2：MQTT_CONNECT_FAILED
 * - -1：MQTT_DISCONNECTED
 * -  0：MQTT_CONNECTED
 * -  1：MQTT_CONNECT_BAD_PROTOCOL
 * -  2：MQTT_CONNECT_BAD_CLIENT_ID
 * -  3：MQTT_CONNECT_UNAVAILABLE
 * -  4：MQTT_CONNECT_BAD_CREDENTIALS
 * -  5：MQTT_CONNECT_UNAUTHORIZED
 *
 * PubSubClient 主要函數：
 * - mqtt.connect(clientId)：連線
 * - mqtt.publish(topic, message)：發布
 * - mqtt.subscribe(topic)：訂閱
 * - mqtt.loop()：處理訊息（必須在 loop 中呼叫）
 * - mqtt.connected()：檢查連線狀態
 *
 * 主題（Topic）命名規則：
 * - 使用 / 分隔層級
 * - 區分大小寫
 * - 不要以 $ 開頭（保留給系統）
 * - 建議使用唯一前綴避免衝突
 *
 * 練習題：
 * 1. 修改主題名稱為你的專屬名稱
 * 2. 用手機 APP 訂閱該主題
 * 3. 從手機 APP 發送訊息，觀察 ESP-01 接收
 */
