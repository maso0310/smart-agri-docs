/*
 * 第四組：螢幕顯示組
 * 範例 5：完整監控面板（整合實作）
 *
 * 功能說明：
 * - LCD 顯示溫度、濕度、土壤資訊
 * - LED 點矩陣顯示狀態圖示
 * - 模擬接收感測器資料
 * - 多頁面切換顯示
 *
 * 這就是智慧溫室監控面板的基本原型！
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LedControl.h>

//=== 腳位與設定 ===
#define LCD_ADDRESS 0x27
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);
LedControl lc = LedControl(11, 13, 10, 1);

//=== 自訂字元 ===
byte charTemp[] = {0x04, 0x0A, 0x0A, 0x0E, 0x0E, 0x1F, 0x1F, 0x0E};
byte charHumid[] = {0x04, 0x04, 0x0A, 0x0A, 0x11, 0x11, 0x11, 0x0E};
byte charSoil[] = {0x04, 0x04, 0x1F, 0x11, 0x11, 0x11, 0x1F, 0x00};
byte charSun[] = {0x04, 0x15, 0x0E, 0x1F, 0x0E, 0x15, 0x04, 0x00};

//=== LED 點矩陣圖案 ===
byte iconOK[] = {0x00, 0x01, 0x02, 0x04, 0x88, 0x50, 0x20, 0x00};
byte iconWarn[] = {0x00, 0x18, 0x18, 0x3C, 0x24, 0x66, 0x7E, 0x00};
byte iconHot[] = {0x08, 0x54, 0x22, 0x41, 0x41, 0x22, 0x14, 0x08};
byte iconDry[] = {0x18, 0x24, 0x42, 0x42, 0x24, 0x18, 0x00, 0x7E};
byte iconWet[] = {0x18, 0x3C, 0x7E, 0x7E, 0x3C, 0x18, 0x00, 0x7E};

//=== 模擬感測器資料 ===
float temperature = 26.5;
float humidity = 68.0;
int soilMoisture = 45;
int lightLevel = 720;

//=== 狀態變數 ===
int currentPage = 0;
const int totalPages = 3;
unsigned long lastPageChange = 0;
const unsigned long pageInterval = 4000;  // 每頁顯示 4 秒

//=== 警報門檻 ===
#define TEMP_HIGH 30.0
#define TEMP_LOW 18.0
#define HUMID_LOW 40.0
#define SOIL_DRY 30

void setup() {
  Serial.begin(9600);
  Serial.println("╔═══════════════════════════════════════╗");
  Serial.println("║    智慧溫室 監控面板 v1.0             ║");
  Serial.println("╚═══════════════════════════════════════╝");

  // 初始化 LCD
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, charTemp);
  lcd.createChar(1, charHumid);
  lcd.createChar(2, charSoil);
  lcd.createChar(3, charSun);

  // 初始化 LED 點矩陣
  lc.shutdown(0, false);
  lc.setIntensity(0, 6);
  lc.clearDisplay(0);

  // 顯示開機畫面
  showBootScreen();
}

void loop() {
  // 更新模擬資料
  updateSimulatedData();

  // 自動切換頁面
  if (millis() - lastPageChange > pageInterval) {
    currentPage = (currentPage + 1) % totalPages;
    lastPageChange = millis();
  }

  // 顯示目前頁面
  switch (currentPage) {
    case 0:
      showPage1_TempHumid();
      break;
    case 1:
      showPage2_SoilLight();
      break;
    case 2:
      showPage3_Status();
      break;
  }

  // 更新狀態圖示
  updateStatusIcon();

  // 序列埠輸出
  printSerialStatus();

  delay(500);
}

//=== 開機畫面 ===
void showBootScreen() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Smart Garden");
  lcd.setCursor(3, 1);
  lcd.print("Loading...");

  // 進度條動畫
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, 0xFF);
    delay(150);
  }
  delay(500);
  lc.clearDisplay(0);
}

//=== 第一頁：溫度與濕度 ===
void showPage1_TempHumid() {
  lcd.clear();

  // 溫度
  lcd.setCursor(0, 0);
  lcd.write(byte(0));  // 溫度圖示
  lcd.print(" Temp: ");
  lcd.print(temperature, 1);
  lcd.print((char)223);
  lcd.print("C");

  // 濕度
  lcd.setCursor(0, 1);
  lcd.write(byte(1));  // 濕度圖示
  lcd.print(" Humid:");
  lcd.print(humidity, 0);
  lcd.print("%");

  // 頁面指示
  lcd.setCursor(15, 0);
  lcd.print("1");
}

//=== 第二頁：土壤與光線 ===
void showPage2_SoilLight() {
  lcd.clear();

  // 土壤
  lcd.setCursor(0, 0);
  lcd.write(byte(2));  // 土壤圖示
  lcd.print(" Soil: ");
  lcd.print(soilMoisture);
  lcd.print("%");

  // 光線
  lcd.setCursor(0, 1);
  lcd.write(byte(3));  // 太陽圖示
  lcd.print(" Light:");
  lcd.print(lightLevel);

  // 頁面指示
  lcd.setCursor(15, 0);
  lcd.print("2");
}

//=== 第三頁：系統狀態 ===
void showPage3_Status() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("System Status:");

  lcd.setCursor(0, 1);

  // 根據狀態顯示
  if (temperature > TEMP_HIGH) {
    lcd.print("! HIGH TEMP !");
  } else if (temperature < TEMP_LOW) {
    lcd.print("! LOW TEMP !");
  } else if (humidity < HUMID_LOW) {
    lcd.print("! LOW HUMID !");
  } else if (soilMoisture < SOIL_DRY) {
    lcd.print("! SOIL DRY !");
  } else {
    lcd.print("All Normal ");
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.write(byte(2));
  }

  // 頁面指示
  lcd.setCursor(15, 0);
  lcd.print("3");
}

//=== 更新狀態圖示 ===
void updateStatusIcon() {
  byte* icon;

  // 根據最嚴重的問題選擇圖示
  if (temperature > TEMP_HIGH) {
    icon = iconHot;
  } else if (soilMoisture < SOIL_DRY) {
    icon = iconDry;
  } else if (humidity > 80) {
    icon = iconWet;
  } else if (temperature < TEMP_LOW || humidity < HUMID_LOW) {
    icon = iconWarn;
  } else {
    icon = iconOK;
  }

  // 顯示圖示
  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, icon[row]);
  }
}

//=== 更新模擬資料 ===
void updateSimulatedData() {
  // 模擬資料波動
  temperature += (random(-10, 11) / 10.0);
  temperature = constrain(temperature, 15.0, 38.0);

  humidity += (random(-20, 21) / 10.0);
  humidity = constrain(humidity, 30.0, 95.0);

  soilMoisture += random(-3, 4);
  soilMoisture = constrain(soilMoisture, 10, 90);

  lightLevel += random(-50, 51);
  lightLevel = constrain(lightLevel, 100, 1000);
}

//=== 序列埠輸出 ===
void printSerialStatus() {
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint < 2000) return;
  lastPrint = millis();

  Serial.print("Page ");
  Serial.print(currentPage + 1);
  Serial.print(" | T:");
  Serial.print(temperature, 1);
  Serial.print("C H:");
  Serial.print(humidity, 0);
  Serial.print("% S:");
  Serial.print(soilMoisture);
  Serial.print("% L:");
  Serial.println(lightLevel);
}

/*
 * 實作挑戰：
 *
 * 1. 按鈕切換頁面
 *    加入按鈕讓使用者手動切換
 *
 * 2. 接收真實資料
 *    透過序列埠或 I2C 接收其他 Arduino 的感測資料
 *
 * 3. 警報閃爍
 *    異常時讓 LCD 背光或 LED 點矩陣閃爍
 *
 * 4. 歷史圖表
 *    在點矩陣上顯示簡易的趨勢圖
 *
 * 5. MQTT 整合
 *    接收第六組發送的 MQTT 資料並顯示
 */

/*
 * 延伸知識：專業監控系統
 *
 * 1. 顯示技術：
 *    - OLED 螢幕（高對比、省電）
 *    - TFT 彩色螢幕（全彩顯示）
 *    - E-Paper 電子紙（超省電）
 *
 * 2. 使用者介面設計：
 *    - 資訊分層（重要資訊優先）
 *    - 顏色編碼（紅=危險, 黃=警告, 綠=正常）
 *    - 圖形化呈現（圖表、儀表）
 *
 * 3. 商業系統功能：
 *    - 觸控螢幕操作
 *    - 歷史資料查詢
 *    - 警報通知設定
 *    - 遠端網頁監控
 */
