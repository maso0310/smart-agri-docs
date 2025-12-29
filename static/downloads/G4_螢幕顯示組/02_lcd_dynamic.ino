/*
 * 第四組：螢幕顯示組
 * 範例 2：LCD 動態更新顯示
 *
 * 學習重點：
 * - 變數顯示
 * - 格式化輸出
 * - 畫面更新技巧（避免閃爍）
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// 模擬感測器資料
float temperature = 25.0;
float humidity = 65.0;
int counter = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("=== LCD 動態更新測試 ===");

  lcd.init();
  lcd.backlight();

  // 顯示固定文字部分
  lcd.setCursor(0, 0);
  lcd.print("Temp:      C");
  lcd.setCursor(0, 1);
  lcd.print("Humid:     %");

  // 顯示溫度符號
  lcd.setCursor(11, 0);
  lcd.print((char)223);  // 度數符號 °
}

void loop() {
  // 模擬感測器變化
  temperature = 25.0 + random(-30, 30) / 10.0;  // 22.0-28.0
  humidity = 65.0 + random(-50, 50) / 10.0;     // 60.0-70.0
  counter++;

  // 只更新數字部分，避免閃爍
  updateTemperature(temperature);
  updateHumidity(humidity);

  // 序列埠顯示
  Serial.print("Update #");
  Serial.print(counter);
  Serial.print(" | Temp: ");
  Serial.print(temperature);
  Serial.print("C | Humid: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(1000);
}

// 更新溫度顯示
void updateTemperature(float temp) {
  lcd.setCursor(6, 0);

  // 格式化：固定寬度，避免殘留字元
  if (temp < 10.0) {
    lcd.print(" ");  // 補空格對齊
  }
  lcd.print(temp, 1);  // 顯示一位小數
}

// 更新濕度顯示
void updateHumidity(float humid) {
  lcd.setCursor(7, 1);

  if (humid < 10.0) {
    lcd.print(" ");
  }
  lcd.print(humid, 1);
}

/*
 * 知識補充：
 *
 * 避免畫面閃爍的技巧：
 *
 * 不好的做法（會閃爍）：
 * void loop() {
 *   lcd.clear();           // 清除整個畫面
 *   lcd.setCursor(0, 0);
 *   lcd.print("Temp: ");
 *   lcd.print(temp);       // 每次都重畫
 *   delay(500);
 * }
 *
 * 好的做法（不閃爍）：
 * void setup() {
 *   lcd.print("Temp:     ");  // 固定文字只寫一次
 * }
 * void loop() {
 *   lcd.setCursor(6, 0);     // 只移動到數字位置
 *   lcd.print(temp);         // 只更新變化的部分
 *   delay(500);
 * }
 *
 * 格式化數字的技巧：
 *
 * 1. 固定小數位數：
 *    lcd.print(25.567, 2);  // 顯示 "25.57"
 *
 * 2. 補空格對齊：
 *    if (num < 10) lcd.print(" ");
 *    if (num < 100) lcd.print(" ");
 *    lcd.print(num);
 *
 * 3. 清除殘留字元：
 *    lcd.print("    ");  // 先用空格清除
 *    lcd.setCursor(x, y);
 *    lcd.print(num);
 *
 * 練習題：
 * 1. 加入第三行顯示計數器
 * 2. 溫度超過 27 度時閃爍警告
 * 3. 加入實際的感測器（如 DHT11）
 */
