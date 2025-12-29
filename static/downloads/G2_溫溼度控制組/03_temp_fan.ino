/*
 * 第二組：溫溼度控制組
 * 範例 3：溫度控制風扇
 *
 * 學習重點：
 * - 根據溫度判斷啟動風扇
 * - 設定溫度門檻值
 * - 簡單的自動控制邏輯
 */

#include <DHT.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11
#define RELAY_FAN 7
#define LED_FAN 5

// 溫度門檻設定
#define TEMP_THRESHOLD 28.0  // 超過此溫度開啟風扇

// 繼電器觸發方式
#define RELAY_ACTIVE_HIGH true

DHT dht(DHT_PIN, DHT_TYPE);
bool fanStatus = false;

void setup() {
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(LED_FAN, OUTPUT);

  relayOff(RELAY_FAN);
  digitalWrite(LED_FAN, LOW);

  dht.begin();

  Serial.begin(9600);
  Serial.println("=== 溫度控制風扇系統 ===");
  Serial.println();
  Serial.print("溫度門檻: ");
  Serial.print(TEMP_THRESHOLD);
  Serial.println(" °C");
  Serial.println("超過門檻溫度時，風扇自動開啟");
  Serial.println();
}

void loop() {
  delay(2000);

  // 讀取溫度
  float temperature = dht.readTemperature();

  if (isnan(temperature)) {
    Serial.println("錯誤：無法讀取溫度！");
    return;
  }

  // 判斷是否需要開啟風扇
  if (temperature > TEMP_THRESHOLD) {
    // 溫度過高，開啟風扇
    if (!fanStatus) {
      Serial.println(">>> 溫度過高！啟動風扇 <<<");
      relayOn(RELAY_FAN);
      digitalWrite(LED_FAN, HIGH);
      fanStatus = true;
    }
  } else {
    // 溫度正常，關閉風扇
    if (fanStatus) {
      Serial.println(">>> 溫度恢復正常，關閉風扇 <<<");
      relayOff(RELAY_FAN);
      digitalWrite(LED_FAN, LOW);
      fanStatus = false;
    }
  }

  // 顯示狀態
  Serial.print("溫度: ");
  Serial.print(temperature, 1);
  Serial.print(" °C | 門檻: ");
  Serial.print(TEMP_THRESHOLD);
  Serial.print(" °C | 風扇: ");
  Serial.println(fanStatus ? "ON" : "OFF");
}

void relayOn(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? HIGH : LOW);
}

void relayOff(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? LOW : HIGH);
}

/*
 * 問題思考：
 *
 * 目前的控制方式有個問題：
 * 假設門檻是 28°C，當溫度在 27.9°C 和 28.1°C 之間波動時，
 * 風扇會一直開開關關，這叫做「顫振」(Chattering)。
 *
 * 解決方法：遲滯控制（Hysteresis）
 * - 設定兩個門檻：開啟門檻和關閉門檻
 * - 例如：28°C 開風扇，26°C 關風扇
 * - 中間的 2°C 差距就是「遲滯區間」
 *
 * 練習題：
 * 1. 加入遲滯控制，避免顫振
 * 2. 用電位器動態調整溫度門檻
 * 3. 加入風扇「預熱」功能（開啟後至少運轉 30 秒）
 */
