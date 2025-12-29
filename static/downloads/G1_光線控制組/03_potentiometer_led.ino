/*
 * 第一組：光線控制組
 * 範例 3：電位器控制 LED 亮度
 *
 * 學習重點：
 * - analogRead() 類比輸入
 * - 類比輸入範圍：0-1023 (10-bit ADC)
 * - map() 函數數值映射
 */

#define POT_PIN A1   // 電位器接在 A1 腳位
#define LED_PIN 9    // LED 接在 D9 腳位

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("電位器控制 LED 亮度");
  Serial.println("類比輸入範圍: 0-1023");
  Serial.println("PWM 輸出範圍: 0-255");
}

void loop() {
  // 讀取電位器數值 (0-1023)
  int potValue = analogRead(POT_PIN);

  // 將 0-1023 映射到 0-255
  int brightness = map(potValue, 0, 1023, 0, 255);

  // 設定 LED 亮度
  analogWrite(LED_PIN, brightness);

  // 顯示數值（每 200ms 更新一次，避免刷太快）
  Serial.print("電位器: ");
  Serial.print(potValue);
  Serial.print(" -> LED 亮度: ");
  Serial.println(brightness);

  delay(200);
}

/*
 * 知識補充：
 * map(value, fromLow, fromHigh, toLow, toHigh)
 *
 * 將數值從一個範圍映射到另一個範圍
 * 例如：map(512, 0, 1023, 0, 255) = 127
 *
 * 電位器（可變電阻）原理：
 * - 三隻腳：VCC, 訊號輸出, GND
 * - 旋轉時改變電阻值，從而改變輸出電壓
 * - 輸出電壓範圍：0V ~ 5V
 *
 * 練習題：
 * 1. 反轉控制方向（電位器轉到底時 LED 最暗）
 * 2. 設定最低亮度（永遠不會全暗）
 * 3. 加入第二顆 LED，一顆漸亮另一顆漸暗
 */
