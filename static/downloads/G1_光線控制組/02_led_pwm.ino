/*
 * 第一組：光線控制組
 * 範例 2：PWM 控制 LED 亮度（呼吸燈效果）
 *
 * 學習重點：
 * - analogWrite() PWM 輸出
 * - PWM 範圍：0-255
 * - for 迴圈控制
 */

#define LED_PIN 9  // LED 接在 D9 腳位（PWM 腳位）

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("PWM 呼吸燈程式啟動");
  Serial.println("PWM 範圍: 0 (全暗) ~ 255 (全亮)");
}

void loop() {
  // 漸亮：0 -> 255
  Serial.println("漸亮中...");
  for (int brightness = 0; brightness <= 255; brightness += 5) {
    analogWrite(LED_PIN, brightness);
    Serial.print("亮度: ");
    Serial.println(brightness);
    delay(30);
  }

  // 漸暗：255 -> 0
  Serial.println("漸暗中...");
  for (int brightness = 255; brightness >= 0; brightness -= 5) {
    analogWrite(LED_PIN, brightness);
    Serial.print("亮度: ");
    Serial.println(brightness);
    delay(30);
  }
}

/*
 * 知識補充：
 * PWM (Pulse Width Modulation) 脈衝寬度調變
 *
 * - Arduino 的 PWM 頻率約為 490Hz（腳位 5, 6 約 980Hz）
 * - 透過調整 duty cycle（工作週期）來模擬類比輸出
 * - 0 = 0% duty cycle = 0V
 * - 255 = 100% duty cycle = 5V
 * - 127 = 50% duty cycle ≈ 2.5V
 *
 * 練習題：
 * 1. 修改 delay 時間，改變呼吸速度
 * 2. 修改步進值（+= 5），改變呼吸細膩度
 * 3. 嘗試只使用一半亮度（0-127）
 */
