/*
 * 第三組：土壤水份控制組
 * 範例 1：土壤濕度感測器讀取
 *
 * 學習重點：
 * - 讀取土壤濕度感測器
 * - 數值校正與百分比轉換
 * - 了解感測器特性
 */

#define SOIL_PIN A0  // 土壤濕度感測器接在 A0

// 校正值（請根據你的感測器實際測量調整）
// 方法：在空氣中測量得到 DRY_VALUE，插入濕土測量得到 WET_VALUE
int DRY_VALUE = 300;   // 完全乾燥時的讀值（空氣中）
int WET_VALUE = 700;   // 完全濕潤時的讀值（插入水中或濕土）

void setup() {
  Serial.begin(9600);
  Serial.println("=== 土壤濕度感測器測試 ===");
  Serial.println();
  Serial.println("校正說明：");
  Serial.println("1. 將探針放在空氣中，記錄 DRY_VALUE");
  Serial.println("2. 將探針插入濕土或水中，記錄 WET_VALUE");
  Serial.println("3. 修改程式中的校正值");
  Serial.println();
  Serial.print("目前校正值：DRY=");
  Serial.print(DRY_VALUE);
  Serial.print(", WET=");
  Serial.println(WET_VALUE);
  Serial.println();
}

void loop() {
  // 讀取感測器原始值
  int rawValue = analogRead(SOIL_PIN);

  // 轉換為濕度百分比
  int moisturePercent = map(rawValue, DRY_VALUE, WET_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // 顯示結果
  Serial.print("原始值: ");
  Serial.print(rawValue);
  Serial.print(" | 濕度: ");
  Serial.print(moisturePercent);
  Serial.print("% | ");

  // 狀態描述
  if (moisturePercent < 20) {
    Serial.println("非常乾燥 - 需要澆水！");
  } else if (moisturePercent < 40) {
    Serial.println("偏乾 - 可能需要澆水");
  } else if (moisturePercent < 60) {
    Serial.println("適中 - 狀態良好");
  } else if (moisturePercent < 80) {
    Serial.println("濕潤 - 不需要澆水");
  } else {
    Serial.println("非常濕 - 注意排水");
  }

  delay(1000);
}

/*
 * 知識補充：
 *
 * 土壤濕度感測器類型：
 *
 * 1. 電阻式（常見便宜款）：
 *    - 原理：測量土壤電阻
 *    - 優點：便宜、簡單
 *    - 缺點：探針易腐蝕、受鹽分影響
 *
 * 2. 電容式（推薦）：
 *    - 原理：測量介電常數
 *    - 優點：不易腐蝕、更穩定
 *    - 缺點：價格稍高
 *
 * 注意事項：
 * - 不同土壤類型讀值差異很大
 * - 沙質土排水快，黏土保水強
 * - 建議在實際使用環境校正
 * - 長時間通電會加速腐蝕
 *
 * 練習題：
 * 1. 記錄你的感測器在不同狀態的讀值
 * 2. 修改校正值讓百分比更準確
 * 3. 多次讀取取平均值，減少跳動
 */
