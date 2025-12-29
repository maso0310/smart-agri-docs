/*
 * 第一組：光線控制組
 * 範例 4：光敏電阻感測光線
 *
 * 學習重點：
 * - 光敏電阻（LDR）原理
 * - 分壓電路原理
 * - 感測器校正
 */

#define LDR_PIN A0  // 光敏電阻接在 A0 腳位

// 校正用變數
int lightMin = 1023;  // 最暗時的數值
int lightMax = 0;     // 最亮時的數值

void setup() {
  Serial.begin(9600);
  Serial.println("=== 光敏電阻測試程式 ===");
  Serial.println();
  Serial.println("數值說明：");
  Serial.println("- 光線越強，電阻越小，讀值越高");
  Serial.println("- 光線越弱，電阻越大，讀值越低");
  Serial.println();
  Serial.println("請用手遮擋或用手電筒照射來測試...");
  Serial.println();
}

void loop() {
  // 讀取光敏電阻數值
  int lightValue = analogRead(LDR_PIN);

  // 更新最大最小值（用於校正）
  if (lightValue < lightMin) lightMin = lightValue;
  if (lightValue > lightMax) lightMax = lightValue;

  // 計算光線百分比（基於目前校正值）
  int lightPercent = 0;
  if (lightMax > lightMin) {
    lightPercent = map(lightValue, lightMin, lightMax, 0, 100);
    lightPercent = constrain(lightPercent, 0, 100);
  }

  // 顯示資訊
  Serial.print("原始值: ");
  Serial.print(lightValue);
  Serial.print(" | 光線強度: ");
  Serial.print(lightPercent);
  Serial.print("% | 範圍: ");
  Serial.print(lightMin);
  Serial.print(" - ");
  Serial.println(lightMax);

  // 根據光線強度給予文字描述
  Serial.print("狀態: ");
  if (lightPercent < 20) {
    Serial.println("很暗");
  } else if (lightPercent < 40) {
    Serial.println("偏暗");
  } else if (lightPercent < 60) {
    Serial.println("適中");
  } else if (lightPercent < 80) {
    Serial.println("明亮");
  } else {
    Serial.println("非常亮");
  }

  Serial.println();
  delay(500);
}

/*
 * 知識補充：
 *
 * 光敏電阻（LDR - Light Dependent Resistor）：
 * - 又稱光敏電阻、光導體
 * - 材料：硫化鎘（CdS）或硒化鎘（CdSe）
 * - 特性：光線越強，電阻越小
 * - 典型範圍：暗處 1MΩ ~ 亮處 1KΩ
 *
 * 分壓電路：
 *         VCC (5V)
 *            │
 *          [LDR]  <- 光敏電阻
 *            │
 *            ├──→ A0 (讀取電壓)
 *            │
 *          [10K]  <- 固定電阻
 *            │
 *          GND
 *
 * Vout = 5V × 10K / (LDR + 10K)
 *
 * 練習題：
 * 1. 嘗試使用不同阻值的固定電阻，觀察感測範圍變化
 * 2. 記錄不同環境下的讀值（室內、窗邊、戶外等）
 * 3. 思考如何將讀值轉換為實際的照度單位（Lux）
 */
