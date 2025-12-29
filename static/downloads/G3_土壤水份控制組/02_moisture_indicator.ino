/*
 * 第三組：土壤水份控制組
 * 範例 2：乾濕度狀態指示燈
 *
 * 學習重點：
 * - 多 LED 狀態顯示
 * - 門檻判斷邏輯
 * - 電位器調整門檻
 */

#define SOIL_PIN A0      // 土壤感測器
#define POT_PIN A1       // 電位器（調整門檻）
#define LED_RED 11       // 紅色 LED（乾燥警告）
#define LED_GREEN 10     // 綠色 LED（濕潤正常）
#define LED_BLUE 9       // 藍色 LED（過濕警告）

// 校正值
int DRY_VALUE = 300;
int WET_VALUE = 700;

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  Serial.begin(9600);
  Serial.println("=== 土壤濕度狀態指示 ===");
  Serial.println();
  Serial.println("LED 指示說明：");
  Serial.println("- 紅色：土壤乾燥，需要澆水");
  Serial.println("- 綠色：土壤濕度適中");
  Serial.println("- 藍色：土壤過濕，注意排水");
  Serial.println();
  Serial.println("轉動電位器可調整乾燥門檻");
  Serial.println();
}

void loop() {
  // 讀取感測器
  int rawValue = analogRead(SOIL_PIN);
  int potValue = analogRead(POT_PIN);

  // 將電位器轉換為乾燥門檻（20-50%）
  int dryThreshold = map(potValue, 0, 1023, 20, 50);
  int wetThreshold = dryThreshold + 30;  // 濕潤門檻比乾燥高 30%

  // 計算濕度百分比
  int moisture = map(rawValue, DRY_VALUE, WET_VALUE, 0, 100);
  moisture = constrain(moisture, 0, 100);

  // 根據濕度設定 LED
  if (moisture < dryThreshold) {
    // 乾燥 - 紅燈亮
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    Serial.print("[乾燥!] ");
  } else if (moisture > wetThreshold) {
    // 過濕 - 藍燈亮
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, HIGH);
    Serial.print("[過濕!] ");
  } else {
    // 適中 - 綠燈亮
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
    Serial.print("[正常] ");
  }

  // 顯示數值
  Serial.print("濕度: ");
  Serial.print(moisture);
  Serial.print("% | 門檻: ");
  Serial.print(dryThreshold);
  Serial.print("-");
  Serial.print(wetThreshold);
  Serial.println("%");

  delay(500);
}

/*
 * 知識補充：
 *
 * 三色 LED 狀態設計：
 *
 *   濕度 ↑
 *   100% ─────────────────────
 *         │ 藍燈區（過濕）
 *   70%  ─┼─────────────────── wetThreshold
 *         │ 綠燈區（正常）
 *   40%  ─┼─────────────────── dryThreshold
 *         │ 紅燈區（乾燥）
 *    0%  ─┴───────────────────
 *
 * 使用電位器調整的好處：
 * - 可以即時調整，不需要重新燒錄程式
 * - 適應不同植物的需求
 * - 方便現場調校
 *
 * 練習題：
 * 1. 加入 LED 閃爍效果表示警告
 * 2. 用 PWM 讓 LED 亮度反映濕度程度
 * 3. 加入蜂鳴器在極端乾燥時發出警報
 */
