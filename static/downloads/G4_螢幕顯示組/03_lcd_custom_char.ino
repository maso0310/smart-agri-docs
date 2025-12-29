/*
 * 第四組：螢幕顯示組
 * 範例 3：LCD 自訂字元
 *
 * 學習重點：
 * - 自訂符號設計
 * - 5x8 點陣繪製
 * - 特殊符號應用
 *
 * LCD 1602 可以儲存 8 個自訂字元（編號 0-7）
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS 0x27

LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

// 自訂字元定義（每個字元 8 bytes，代表 8 列）
// 每個 byte 的低 5 位代表一列的 5 個像素

// 溫度計符號
byte thermometer[] = {
  0b00100,  //   *
  0b01010,  //  * *
  0b01010,  //  * *
  0b01110,  //  ***
  0b01110,  //  ***
  0b11111,  // *****
  0b11111,  // *****
  0b01110   //  ***
};

// 水滴符號（濕度）
byte droplet[] = {
  0b00100,  //   *
  0b00100,  //   *
  0b01010,  //  * *
  0b01010,  //  * *
  0b10001,  // *   *
  0b10001,  // *   *
  0b10001,  // *   *
  0b01110   //  ***
};

// 太陽符號
byte sun[] = {
  0b00100,  //   *
  0b10101,  // * * *
  0b01110,  //  ***
  0b11111,  // *****
  0b01110,  //  ***
  0b10101,  // * * *
  0b00100,  //   *
  0b00000   //
};

// 植物/葉子符號
byte plant[] = {
  0b00100,  //   *
  0b00110,  //   **
  0b00100,  //   *
  0b01110,  //  ***
  0b10101,  // * * *
  0b00100,  //   *
  0b00100,  //   *
  0b01110   //  ***
};

// 風扇符號
byte fan[] = {
  0b00000,  //
  0b10001,  // *   *
  0b01010,  //  * *
  0b00100,  //   *
  0b01010,  //  * *
  0b10001,  // *   *
  0b00000,  //
  0b00000   //
};

// 進度條組件
byte bar1[] = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};  // |
byte bar2[] = {0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};  // ||
byte bar3[] = {0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C};  // |||
byte bar4[] = {0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E};  // ||||
byte bar5[] = {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};  // |||||

void setup() {
  Serial.begin(9600);
  Serial.println("=== LCD 自訂字元測試 ===");

  lcd.init();
  lcd.backlight();

  // 建立自訂字元（編號 0-7）
  lcd.createChar(0, thermometer);
  lcd.createChar(1, droplet);
  lcd.createChar(2, sun);
  lcd.createChar(3, plant);
  lcd.createChar(4, fan);

  // 顯示自訂字元
  lcd.home();
  lcd.write(byte(0));  // 溫度計
  lcd.print(" Temp: 25.5C");

  lcd.setCursor(0, 1);
  lcd.write(byte(1));  // 水滴
  lcd.print(" Humid: 65%");

  Serial.println("顯示基本自訂字元");
}

void loop() {
  delay(3000);

  // 顯示所有自訂圖示
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Icons: ");
  for (int i = 0; i < 5; i++) {
    lcd.write(byte(i));
    lcd.print(" ");
  }

  lcd.setCursor(0, 1);
  lcd.print("Smart Greenhouse");

  delay(3000);

  // 進度條展示
  showProgressBar();

  delay(2000);

  // 回到初始畫面
  lcd.clear();
  lcd.home();
  lcd.write(byte(0));
  lcd.print(" Temp: 25.5C");
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.print(" Humid: 65%");
}

void showProgressBar() {
  // 重新定義自訂字元為進度條
  lcd.createChar(0, bar1);
  lcd.createChar(1, bar2);
  lcd.createChar(2, bar3);
  lcd.createChar(3, bar4);
  lcd.createChar(4, bar5);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Loading...");

  // 動態進度條
  for (int progress = 0; progress <= 100; progress += 5) {
    lcd.setCursor(0, 1);

    // 計算需要多少完整格與部分格
    int fullBars = progress / 10;       // 每格代表 10%
    int partialBar = (progress % 10) / 2;  // 0-4

    // 顯示完整格
    for (int i = 0; i < fullBars && i < 10; i++) {
      lcd.write(byte(4));  // 全滿
    }

    // 顯示部分格
    if (fullBars < 10 && partialBar > 0) {
      lcd.write(byte(partialBar - 1));
      fullBars++;
    }

    // 顯示空格
    for (int i = fullBars; i < 10; i++) {
      lcd.print(" ");
    }

    // 顯示百分比
    lcd.print(" ");
    if (progress < 10) lcd.print(" ");
    if (progress < 100) lcd.print(" ");
    lcd.print(progress);
    lcd.print("%");

    delay(100);
  }

  // 恢復原本的自訂字元
  lcd.createChar(0, thermometer);
  lcd.createChar(1, droplet);
  lcd.createChar(2, sun);
  lcd.createChar(3, plant);
  lcd.createChar(4, fan);
}

/*
 * 知識補充：
 *
 * 自訂字元設計方法：
 *
 * 1. 畫出 5x8 的點陣圖
 * 2. 每列轉換為二進位
 * 3. 填入 byte 陣列
 *
 * 例如：笑臉
 *      01234
 *   0: 00000 = 0b00000
 *   1: 01010 = 0b01010
 *   2: 00000 = 0b00000
 *   3: 00000 = 0b00000
 *   4: 10001 = 0b10001
 *   5: 01110 = 0b01110
 *   6: 00000 = 0b00000
 *   7: 00000 = 0b00000
 *
 * 線上工具：
 * 搜尋 "LCD custom character generator"
 * 可以視覺化設計並產生程式碼
 *
 * 練習題：
 * 1. 設計一個「警告」三角形符號
 * 2. 設計「勾勾」和「叉叉」符號
 * 3. 製作動畫效果（多個自訂字元輪替）
 */
