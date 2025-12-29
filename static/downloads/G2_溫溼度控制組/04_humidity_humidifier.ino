/*
 * 第二組：溫溼度控制組
 * 範例 4：濕度控制霧化器（含遲滯控制）
 *
 * 學習重點：
 * - 根據濕度判斷啟動霧化器
 * - 遲滯控制避免頻繁開關
 * - 最小運轉時間保護
 */

#include <DHT.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11
#define RELAY_HUMIDIFIER 8
#define LED_HUMID 6

// 濕度門檻設定（遲滯控制）
#define HUMIDITY_LOW 55.0   // 低於此值開啟霧化器
#define HUMIDITY_HIGH 70.0  // 高於此值關閉霧化器

// 最小運轉時間（毫秒）
#define MIN_RUN_TIME 10000  // 開啟後至少運轉 10 秒

// 繼電器觸發方式
#define RELAY_ACTIVE_HIGH true

DHT dht(DHT_PIN, DHT_TYPE);
bool humidifierStatus = false;
unsigned long startTime = 0;  // 開啟時間記錄

void setup() {
  pinMode(RELAY_HUMIDIFIER, OUTPUT);
  pinMode(LED_HUMID, OUTPUT);

  relayOff(RELAY_HUMIDIFIER);
  digitalWrite(LED_HUMID, LOW);

  dht.begin();

  Serial.begin(9600);
  Serial.println("=== 濕度控制霧化器系統 ===");
  Serial.println();
  Serial.print("低濕度門檻: ");
  Serial.print(HUMIDITY_LOW);
  Serial.println(" % (開啟霧化器)");
  Serial.print("高濕度門檻: ");
  Serial.print(HUMIDITY_HIGH);
  Serial.println(" % (關閉霧化器)");
  Serial.print("最小運轉時間: ");
  Serial.print(MIN_RUN_TIME / 1000);
  Serial.println(" 秒");
  Serial.println();
}

void loop() {
  delay(2000);

  // 讀取濕度
  float humidity = dht.readHumidity();

  if (isnan(humidity)) {
    Serial.println("錯誤：無法讀取濕度！");
    return;
  }

  // 計算運轉時間
  unsigned long runTime = 0;
  if (humidifierStatus) {
    runTime = millis() - startTime;
  }

  // 控制邏輯（含遲滯）
  if (!humidifierStatus && humidity < HUMIDITY_LOW) {
    // 濕度過低，開啟霧化器
    Serial.println(">>> 濕度過低！啟動霧化器 <<<");
    relayOn(RELAY_HUMIDIFIER);
    digitalWrite(LED_HUMID, HIGH);
    humidifierStatus = true;
    startTime = millis();

  } else if (humidifierStatus && humidity > HUMIDITY_HIGH) {
    // 濕度足夠，檢查是否達到最小運轉時間
    if (runTime >= MIN_RUN_TIME) {
      Serial.println(">>> 濕度恢復正常，關閉霧化器 <<<");
      relayOff(RELAY_HUMIDIFIER);
      digitalWrite(LED_HUMID, LOW);
      humidifierStatus = false;
    } else {
      Serial.print("(等待最小運轉時間，剩餘 ");
      Serial.print((MIN_RUN_TIME - runTime) / 1000);
      Serial.println(" 秒)");
    }
  }

  // 顯示狀態
  Serial.print("濕度: ");
  Serial.print(humidity, 1);
  Serial.print(" % | 範圍: ");
  Serial.print(HUMIDITY_LOW);
  Serial.print("-");
  Serial.print(HUMIDITY_HIGH);
  Serial.print(" % | 霧化器: ");
  Serial.print(humidifierStatus ? "ON" : "OFF");

  if (humidifierStatus) {
    Serial.print(" (已運轉 ");
    Serial.print(runTime / 1000);
    Serial.print(" 秒)");
  }
  Serial.println();
}

void relayOn(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? HIGH : LOW);
}

void relayOff(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? LOW : HIGH);
}

/*
 * 知識補充：
 *
 * 遲滯控制（Hysteresis）的好處：
 * 1. 避免設備頻繁開關（顫振）
 * 2. 延長設備壽命
 * 3. 節省能源
 * 4. 控制更穩定
 *
 * 遲滯區間示意：
 *
 *   濕度 ↑
 *   100% ─────────────────────
 *
 *    70% ═════════════════════  ← 關閉門檻
 *         ↑ 遲滯區間 (55-70%)
 *    55% ═════════════════════  ← 開啟門檻
 *
 *     0% ─────────────────────
 *
 * 當濕度在 55-70% 之間時，保持目前狀態不變
 *
 * 最小運轉時間的目的：
 * - 保護壓縮機、馬達等設備
 * - 避免啟動電流造成的損耗
 * - 讓控制效果更明顯
 *
 * 練習題：
 * 1. 調整遲滯區間，觀察控制效果
 * 2. 加入「冷卻時間」（關閉後等待一段時間才能再開）
 * 3. 用 LED 亮度表示目前濕度（PWM）
 */
