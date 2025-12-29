/*
 * 第四組：螢幕顯示組
 * 範例 4：8x8 LED 點矩陣顯示
 *
 * 學習重點：
 * - MAX7219 控制
 * - 圖案繪製
 * - 簡單動畫
 *
 * 需安裝函式庫：LedControl (by Eberhard Fahle)
 *
 * 接線：
 * - DIN → D11
 * - CS  → D10
 * - CLK → D13
 */

#include <LedControl.h>

// LedControl(DIN, CLK, CS, 裝置數量)
LedControl lc = LedControl(11, 13, 10, 1);

// 定義圖案（每個 byte 代表一列）
// 最高位元 = 最左邊的 LED

// 笑臉
byte smile[] = {
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
};

// 愛心
byte heart[] = {
  0b00000000,
  0b01100110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000
};

// 太陽
byte sun[] = {
  0b00011000,
  0b10100101,
  0b01011010,
  0b00111100,
  0b00111100,
  0b01011010,
  0b10100101,
  0b00011000
};

// 植物
byte plant[] = {
  0b00001000,
  0b00011100,
  0b00101010,
  0b00001000,
  0b01001001,
  0b00101010,
  0b00011100,
  0b00001000
};

// 水滴
byte water[] = {
  0b00011000,
  0b00011000,
  0b00111100,
  0b00111100,
  0b01111110,
  0b01111110,
  0b01111110,
  0b00111100
};

// 警告三角形
byte warning[] = {
  0b00000000,
  0b00011000,
  0b00011000,
  0b00111100,
  0b00100100,
  0b01100110,
  0b01111110,
  0b00000000
};

// 勾勾（OK）
byte checkmark[] = {
  0b00000000,
  0b00000001,
  0b00000010,
  0b00000100,
  0b10001000,
  0b01010000,
  0b00100000,
  0b00000000
};

void setup() {
  Serial.begin(9600);
  Serial.println("=== LED 點矩陣測試 ===");

  // 喚醒 MAX7219
  lc.shutdown(0, false);

  // 設定亮度（0-15）
  lc.setIntensity(0, 8);

  // 清除顯示
  lc.clearDisplay(0);

  Serial.println("初始化完成！");
}

void loop() {
  // 顯示笑臉
  Serial.println("顯示：笑臉");
  displayPattern(smile);
  delay(1500);

  // 顯示愛心
  Serial.println("顯示：愛心");
  displayPattern(heart);
  delay(1500);

  // 顯示太陽
  Serial.println("顯示：太陽");
  displayPattern(sun);
  delay(1500);

  // 顯示植物
  Serial.println("顯示：植物");
  displayPattern(plant);
  delay(1500);

  // 顯示水滴
  Serial.println("顯示：水滴");
  displayPattern(water);
  delay(1500);

  // 顯示警告
  Serial.println("顯示：警告");
  displayPattern(warning);
  delay(1500);

  // 顯示勾勾
  Serial.println("顯示：OK");
  displayPattern(checkmark);
  delay(1500);

  // 動畫效果
  Serial.println("動畫：閃爍");
  blinkAnimation();

  Serial.println("動畫：掃描");
  scanAnimation();

  Serial.println("---");
}

// 顯示圖案
void displayPattern(byte pattern[]) {
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, pattern[row]);
  }
}

// 閃爍動畫
void blinkAnimation() {
  for (int i = 0; i < 5; i++) {
    displayPattern(heart);
    delay(300);
    lc.clearDisplay(0);
    delay(200);
  }
  displayPattern(heart);
}

// 掃描動畫
void scanAnimation() {
  // 由上到下
  for (int row = 0; row < 8; row++) {
    lc.clearDisplay(0);
    lc.setRow(0, row, 0xFF);  // 整列亮
    delay(100);
  }

  // 由下到上
  for (int row = 7; row >= 0; row--) {
    lc.clearDisplay(0);
    lc.setRow(0, row, 0xFF);
    delay(100);
  }

  // 由左到右
  for (int col = 0; col < 8; col++) {
    lc.clearDisplay(0);
    for (int row = 0; row < 8; row++) {
      lc.setLed(0, row, col, true);
    }
    delay(100);
  }

  lc.clearDisplay(0);
}

/*
 * 知識補充：
 *
 * MAX7219 函數說明：
 *
 * 1. lc.shutdown(addr, status)
 *    - addr: 裝置編號（從 0 開始）
 *    - status: true=省電模式, false=正常運作
 *
 * 2. lc.setIntensity(addr, intensity)
 *    - intensity: 0-15（亮度）
 *
 * 3. lc.clearDisplay(addr)
 *    - 清除顯示
 *
 * 4. lc.setRow(addr, row, value)
 *    - row: 0-7（列編號）
 *    - value: 8-bit 數值，表示這一列的 LED 狀態
 *
 * 5. lc.setColumn(addr, col, value)
 *    - col: 0-7（欄編號）
 *    - value: 8-bit 數值
 *
 * 6. lc.setLed(addr, row, col, state)
 *    - 設定單一 LED
 *    - state: true/false
 *
 * 圖案設計技巧：
 * - 使用 Excel 或繪圖軟體設計 8x8 圖案
 * - 每列轉換為二進位/十六進位
 * - 線上工具：搜尋 "LED matrix generator"
 *
 * 練習題：
 * 1. 設計自己的圖案（英文字母、數字）
 * 2. 製作跑馬燈文字效果
 * 3. 製作漸層亮度動畫
 */
