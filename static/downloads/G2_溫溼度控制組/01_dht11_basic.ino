/*
 * 第二組：溫溼度控制組
 * 範例 1：DHT11 溫溼度讀取
 *
 * 學習重點：
 * - DHT11 感測器初始化
 * - 讀取溫度與濕度數值
 * - 處理讀取失敗的情況
 *
 * 需安裝函式庫：DHT sensor library (by Adafruit)
 */

#include <DHT.h>

#define DHT_PIN 2       // DHT11 資料腳位
#define DHT_TYPE DHT11  // 感測器型號

// 建立 DHT 物件
DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("=== DHT11 溫溼度感測器測試 ===");
  Serial.println();

  // 初始化 DHT 感測器
  dht.begin();

  Serial.println("感測器初始化完成！");
  Serial.println("每 2 秒讀取一次數據...");
  Serial.println();
}

void loop() {
  // DHT11 讀取速度較慢，需等待約 2 秒
  delay(2000);

  // 讀取濕度
  float humidity = dht.readHumidity();
  // 讀取溫度（攝氏）
  float temperature = dht.readTemperature();

  // 檢查讀取是否成功
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("錯誤：無法讀取 DHT11 感測器！");
    Serial.println("請檢查接線是否正確。");
    return;
  }

  // 計算體感溫度（Heat Index）
  float heatIndex = dht.computeHeatIndex(temperature, humidity, false);

  // 顯示結果
  Serial.println("┌─────────────────────────┐");
  Serial.print("│ 溫度: ");
  Serial.print(temperature, 1);
  Serial.println(" °C           │");

  Serial.print("│ 濕度: ");
  Serial.print(humidity, 1);
  Serial.println(" %            │");

  Serial.print("│ 體感: ");
  Serial.print(heatIndex, 1);
  Serial.println(" °C           │");
  Serial.println("└─────────────────────────┘");
  Serial.println();
}

/*
 * 知識補充：
 *
 * DHT11 感測器特性：
 * - 工作電壓：3.3V ~ 5V
 * - 溫度範圍：0 ~ 50°C，精度 ±2°C
 * - 濕度範圍：20 ~ 90%，精度 ±5%
 * - 取樣速率：1Hz（每秒一次）
 *
 * 模組版 vs 裸元件版：
 * - 模組版（三腳）：S（訊號）、+（VCC）、-（GND）
 *   已內建 10K 提升電阻，可直接使用
 * - 裸元件版（四腳）：需自行接 10K 提升電阻
 *   資料腳與 VCC 之間需接 10K 電阻
 *
 * 練習題：
 * 1. 嘗試用手捂住感測器，觀察溫度變化
 * 2. 對感測器呵氣，觀察濕度變化
 * 3. 修改程式，當溫度超過 30°C 時顯示警告
 */
