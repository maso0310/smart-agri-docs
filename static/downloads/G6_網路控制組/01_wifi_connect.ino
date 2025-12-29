/*
 * 第六組：網路控制組
 * 範例 1：WiFi 連線測試
 *
 * 學習重點：
 * - ESP-01 開發環境確認
 * - WiFi 連線設定
 * - 連線狀態監控
 *
 * 硬體：ESP-01 (ESP8266)
 * 開發板設定：Generic ESP8266 Module
 */

#include <ESP8266WiFi.h>

//===== 請修改為你的 WiFi 資訊 =====
const char* ssid = "你的WiFi名稱";
const char* password = "你的WiFi密碼";
//=================================

// LED 腳位（ESP-01 內建 LED 在 GPIO2，低電平亮）
#define LED_PIN 2

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("=== ESP-01 WiFi 連線測試 ===");
  Serial.println();

  // 設定 LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // 先關閉

  // 開始連線 WiFi
  Serial.print("連線到 WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // 等待連線
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));  // LED 閃爍

    attempts++;
    if (attempts > 60) {  // 30 秒超時
      Serial.println();
      Serial.println("連線失敗！請檢查：");
      Serial.println("1. WiFi 名稱和密碼是否正確");
      Serial.println("2. WiFi 是否為 2.4GHz（ESP-01 不支援 5GHz）");
      Serial.println("3. 是否在 WiFi 訊號範圍內");
      Serial.println();
      Serial.println("重新啟動...");
      delay(5000);
      ESP.restart();
    }
  }

  // 連線成功
  Serial.println();
  Serial.println("WiFi 連線成功！");
  Serial.println();

  // 顯示連線資訊
  Serial.println("連線資訊：");
  Serial.print("- SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("- IP 位址: ");
  Serial.println(WiFi.localIP());
  Serial.print("- MAC 位址: ");
  Serial.println(WiFi.macAddress());
  Serial.print("- 訊號強度: ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  Serial.println();

  // LED 常亮表示已連線
  digitalWrite(LED_PIN, LOW);

  Serial.println("測試完成！ESP-01 已成功連上 WiFi");
}

void loop() {
  // 每 5 秒顯示連線狀態
  static unsigned long lastCheck = 0;

  if (millis() - lastCheck > 5000) {
    lastCheck = millis();

    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("[已連線] IP: ");
      Serial.print(WiFi.localIP());
      Serial.print(" | 訊號: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");

      // LED 短閃表示正常
      digitalWrite(LED_PIN, HIGH);
      delay(50);
      digitalWrite(LED_PIN, LOW);
    } else {
      Serial.println("[斷線] 嘗試重新連線...");
      digitalWrite(LED_PIN, HIGH);  // LED 滅

      WiFi.reconnect();
    }
  }
}

/*
 * 知識補充：
 *
 * ESP8266 WiFi 模式：
 * - WIFI_STA：Station 模式，連接到現有 WiFi
 * - WIFI_AP：Access Point 模式，自己當熱點
 * - WIFI_AP_STA：同時支援兩種模式
 *
 * WiFi 訊號強度（RSSI）：
 * - -30 dBm：非常好
 * - -50 dBm：好
 * - -60 dBm：普通
 * - -70 dBm：弱
 * - -80 dBm：很弱
 * - -90 dBm：幾乎無法使用
 *
 * ESP-01 GPIO 腳位限制：
 * - GPIO0：開機模式選擇（建議不要用）
 * - GPIO2：可用，但開機時需為高電平
 * - GPIO1：TX（序列埠）
 * - GPIO3：RX（序列埠）
 *
 * 常見問題：
 * 1. 連不上 5GHz WiFi → ESP8266 只支援 2.4GHz
 * 2. 密碼錯誤 → 注意大小寫
 * 3. 訊號太弱 → 靠近路由器
 *
 * 練習題：
 * 1. 修改程式在連線失敗時不重啟，改為持續嘗試
 * 2. 顯示連線所花的時間
 * 3. 加入按鈕控制重新連線
 */
