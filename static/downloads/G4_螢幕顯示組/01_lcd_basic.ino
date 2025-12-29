/*
 * 第四組：螢幕顯示組
 * 範例 1：LCD 基本顯示
 *
 * 學習重點：
 * - LCD 初始化
 * - 顯示文字與數字
 * - 游標位置控制
 *
 * 需安裝函式庫：LiquidCrystal I2C (by Frank de Brabander)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// 設定 LCD 參數
// 位址通常是 0x27 或 0x3F，如果不確定請執行 I2C Scanner
#define LCD_ADDRESS 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

// 建立 LCD 物件
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

void setup() {
  Serial.begin(9600);
  Serial.println("=== LCD 基本測試 ===");

  // 初始化 LCD
  lcd.init();

  // 開啟背光
  lcd.backlight();

  // 顯示歡迎訊息
  lcd.setCursor(0, 0);           // 第一行開頭
  lcd.print("Hello World!");

  lcd.setCursor(0, 1);           // 第二行開頭
  lcd.print("Smart Greenhouse");

  Serial.println("LCD 初始化完成！");
  Serial.println("如果沒有顯示，請：");
  Serial.println("1. 檢查接線");
  Serial.println("2. 調整背板電位器（對比度）");
  Serial.println("3. 確認 I2C 位址");
}

void loop() {
  // 等待 3 秒
  delay(3000);

  // 清除螢幕
  lcd.clear();

  // 顯示第二個畫面
  lcd.setCursor(2, 0);
  lcd.print("Arduino LCD");
  lcd.setCursor(3, 1);
  lcd.print("Demo v1.0");

  delay(3000);

  // 清除螢幕
  lcd.clear();

  // 顯示第三個畫面
  lcd.setCursor(0, 0);
  lcd.print("Col: 0123456789");
  lcd.setCursor(0, 1);
  lcd.print("0123456789ABCDEF");

  delay(3000);

  // 回到歡迎畫面
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
  lcd.setCursor(0, 1);
  lcd.print("Smart Greenhouse");
}

/*
 * 知識補充：
 *
 * LCD 1602 基本操作：
 *
 * 1. lcd.init()      - 初始化 LCD
 * 2. lcd.backlight() - 開啟背光
 * 3. lcd.noBacklight() - 關閉背光
 * 4. lcd.clear()     - 清除螢幕
 * 5. lcd.setCursor(col, row) - 設定游標位置
 * 6. lcd.print("text") - 顯示文字
 * 7. lcd.home()      - 游標回到原點 (0,0)
 *
 * 座標說明：
 * - 第一個參數 col（欄）：0-15
 * - 第二個參數 row（列）：0-1
 * - setCursor(0, 0) = 左上角
 * - setCursor(15, 1) = 右下角
 *
 * I2C 位址說明：
 * - 常見位址：0x27（大多數）、0x3F
 * - 可用 I2C Scanner 程式掃描
 * - 位址由 A0, A1, A2 跳線決定
 *
 * 練習題：
 * 1. 修改顯示的文字內容
 * 2. 嘗試在不同位置顯示文字
 * 3. 製作循環顯示多個畫面
 */

/*
 * I2C Scanner 程式（用於找尋位址）：
 *
 * #include <Wire.h>
 *
 * void setup() {
 *   Wire.begin();
 *   Serial.begin(9600);
 *   Serial.println("I2C Scanner");
 * }
 *
 * void loop() {
 *   byte error, address;
 *   int nDevices = 0;
 *
 *   Serial.println("Scanning...");
 *
 *   for(address = 1; address < 127; address++) {
 *     Wire.beginTransmission(address);
 *     error = Wire.endTransmission();
 *
 *     if (error == 0) {
 *       Serial.print("Found device at 0x");
 *       Serial.println(address, HEX);
 *       nDevices++;
 *     }
 *   }
 *
 *   if (nDevices == 0) {
 *     Serial.println("No devices found");
 *   }
 *
 *   delay(5000);
 * }
 */
