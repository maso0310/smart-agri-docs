/*
 * 第三組：土壤水份控制組
 * 範例 5：完整自動灌溉系統（整合實作）
 *
 * 功能說明：
 * - 土壤濕度感測與顯示
 * - 電位器調整乾燥門檻
 * - 自動控制抽水馬達
 * - 完整安全保護機制
 * - 三色 LED 狀態指示
 *
 * 這就是智慧溫室自動灌溉的基本原型！
 */

//=== 腳位定義 ===
#define SOIL_PIN A0       // 土壤濕度感測器
#define POT_PIN A1        // 電位器（調整門檻）
#define RELAY_PUMP 7      // 繼電器控制馬達
#define LED_PUMP 9        // 藍色 LED（馬達運轉）
#define LED_OK 10         // 綠色 LED（狀態正常）
#define LED_WARN 11       // 紅色 LED（需要注意）

//=== 校正值（請根據實際感測器調整）===
int DRY_VALUE = 300;      // 空氣中的讀值
int WET_VALUE = 700;      // 濕土中的讀值

//=== 安全參數 ===
#define MAX_PUMP_TIME 30000       // 最大澆水：30 秒
#define MIN_PUMP_INTERVAL 60000   // 最小間隔：60 秒
#define HYSTERESIS 10             // 遲滯範圍：10%

//=== 繼電器設定 ===
#define RELAY_ACTIVE_HIGH true

//=== 狀態變數 ===
bool pumpOn = false;
unsigned long pumpStartTime = 0;
unsigned long pumpStopTime = 0;
bool safetyLockout = false;

// 讀值平滑化
const int SAMPLES = 5;
int readings[SAMPLES];
int readIndex = 0;

void setup() {
  // 初始化讀值陣列
  for (int i = 0; i < SAMPLES; i++) {
    readings[i] = 0;
  }

  // 設定腳位
  pinMode(RELAY_PUMP, OUTPUT);
  pinMode(LED_PUMP, OUTPUT);
  pinMode(LED_OK, OUTPUT);
  pinMode(LED_WARN, OUTPUT);

  // 初始化狀態
  relayOff(RELAY_PUMP);
  allLEDsOff();
  digitalWrite(LED_OK, HIGH);

  Serial.begin(9600);
  Serial.println("╔═══════════════════════════════════════╗");
  Serial.println("║     智慧溫室 自動灌溉系統 v1.0        ║");
  Serial.println("╠═══════════════════════════════════════╣");
  Serial.println("║ 轉動電位器調整乾燥門檻                ║");
  Serial.println("║ 藍燈=澆水中 綠燈=正常 紅燈=警告       ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.println();
}

void loop() {
  unsigned long currentTime = millis();

  // 讀取並平滑化土壤濕度
  int moisture = readSoilMoisture();

  // 讀取電位器設定門檻（20-50%）
  int potValue = analogRead(POT_PIN);
  int dryThreshold = map(potValue, 0, 1023, 20, 50);
  int wetThreshold = dryThreshold + HYSTERESIS + 10;

  // 更新安全鎖定狀態
  updateSafetyLockout(currentTime);

  // 控制邏輯
  if (pumpOn) {
    handlePumpOn(moisture, wetThreshold, currentTime);
  } else {
    handlePumpOff(moisture, dryThreshold);
  }

  // 更新 LED 狀態
  updateLEDs(moisture, dryThreshold, wetThreshold);

  // 顯示狀態
  displayStatus(moisture, dryThreshold, wetThreshold, currentTime);

  delay(500);
}

//=== 讀取土壤濕度（平滑化）===
int readSoilMoisture() {
  int rawValue = analogRead(SOIL_PIN);

  // 移動平均
  readings[readIndex] = rawValue;
  readIndex = (readIndex + 1) % SAMPLES;

  int total = 0;
  for (int i = 0; i < SAMPLES; i++) {
    total += readings[i];
  }
  int average = total / SAMPLES;

  // 轉換為百分比
  int moisture = map(average, DRY_VALUE, WET_VALUE, 0, 100);
  return constrain(moisture, 0, 100);
}

//=== 更新安全鎖定狀態 ===
void updateSafetyLockout(unsigned long currentTime) {
  if (safetyLockout) {
    if (currentTime - pumpStopTime >= MIN_PUMP_INTERVAL) {
      safetyLockout = false;
      Serial.println(">>> 冷卻完成，系統就緒 <<<");
    }
  }
}

//=== 處理澆水中狀態 ===
void handlePumpOn(int moisture, int wetThreshold, unsigned long currentTime) {
  unsigned long duration = currentTime - pumpStartTime;

  if (duration >= MAX_PUMP_TIME) {
    // 超時強制停止
    Serial.println("!!! 安全停止：達到最大澆水時間 !!!");
    stopPump();
    safetyLockout = true;
  } else if (moisture >= wetThreshold) {
    // 正常停止
    Serial.println(">>> 土壤濕潤，停止澆水 <<<");
    stopPump();
  }
}

//=== 處理待機狀態 ===
void handlePumpOff(int moisture, int dryThreshold) {
  if (!safetyLockout && moisture < dryThreshold) {
    Serial.println(">>> 土壤乾燥，開始澆水 <<<");
    startPump();
  }
}

//=== 開始澆水 ===
void startPump() {
  pumpOn = true;
  pumpStartTime = millis();
  relayOn(RELAY_PUMP);
  digitalWrite(LED_PUMP, HIGH);
}

//=== 停止澆水 ===
void stopPump() {
  pumpOn = false;
  pumpStopTime = millis();
  relayOff(RELAY_PUMP);
  digitalWrite(LED_PUMP, LOW);
}

//=== 更新 LED 狀態 ===
void updateLEDs(int moisture, int dryThreshold, int wetThreshold) {
  if (pumpOn) {
    // 澆水中：藍燈閃爍
    static bool blinkState = false;
    blinkState = !blinkState;
    digitalWrite(LED_PUMP, blinkState ? HIGH : LOW);
    digitalWrite(LED_OK, LOW);
    digitalWrite(LED_WARN, LOW);
  } else if (safetyLockout) {
    // 安全鎖定：紅燈閃爍
    static bool warnBlink = false;
    warnBlink = !warnBlink;
    digitalWrite(LED_PUMP, LOW);
    digitalWrite(LED_OK, LOW);
    digitalWrite(LED_WARN, warnBlink ? HIGH : LOW);
  } else if (moisture < dryThreshold) {
    // 乾燥警告
    digitalWrite(LED_PUMP, LOW);
    digitalWrite(LED_OK, LOW);
    digitalWrite(LED_WARN, HIGH);
  } else if (moisture > wetThreshold) {
    // 濕潤正常
    digitalWrite(LED_PUMP, LOW);
    digitalWrite(LED_OK, HIGH);
    digitalWrite(LED_WARN, LOW);
  } else {
    // 適中
    digitalWrite(LED_PUMP, LOW);
    digitalWrite(LED_OK, HIGH);
    digitalWrite(LED_WARN, LOW);
  }
}

//=== 顯示狀態 ===
void displayStatus(int moisture, int dryThreshold, int wetThreshold, unsigned long currentTime) {
  // 濕度條
  Serial.print("[");
  int bars = moisture / 10;
  for (int i = 0; i < 10; i++) {
    Serial.print(i < bars ? "█" : "░");
  }
  Serial.print("] ");

  Serial.print(moisture);
  Serial.print("% | 門檻:");
  Serial.print(dryThreshold);
  Serial.print("-");
  Serial.print(wetThreshold);
  Serial.print("% | ");

  if (pumpOn) {
    unsigned long sec = (currentTime - pumpStartTime) / 1000;
    Serial.print("澆水中(");
    Serial.print(sec);
    Serial.print("s)");
  } else if (safetyLockout) {
    unsigned long remaining = (MIN_PUMP_INTERVAL - (currentTime - pumpStopTime)) / 1000;
    Serial.print("冷卻(");
    Serial.print(remaining);
    Serial.print("s)");
  } else {
    Serial.print("待機");
  }

  Serial.println();
}

void allLEDsOff() {
  digitalWrite(LED_PUMP, LOW);
  digitalWrite(LED_OK, LOW);
  digitalWrite(LED_WARN, LOW);
}

void relayOn(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? HIGH : LOW);
}

void relayOff(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? LOW : HIGH);
}

/*
 * 實作挑戰：
 *
 * 1. 多區域灌溉
 *    加入第二個土壤感測器和繼電器
 *    獨立控制兩個灌溉區
 *
 * 2. 水量估算
 *    根據澆水時間估算用水量
 *    累計並顯示總用水量
 *
 * 3. 排程澆水
 *    設定每天固定時間檢查土壤
 *    避免半夜澆水
 *
 * 4. 資料傳送
 *    將土壤濕度傳送給第六組
 *    接收遠端控制指令
 *
 * 5. 智慧節水
 *    考慮天氣因素
 *    雨天減少澆水
 */

/*
 * 延伸知識：專業灌溉系統
 *
 * 1. 精準農業：
 *    - 多點感測土壤狀況
 *    - 根據作物需求調整
 *    - 結合氣象資料預測
 *
 * 2. 商業系統功能：
 *    - 流量計測量用水量
 *    - 壓力感測確保正常運作
 *    - 遠端監控與控制
 *    - 歷史資料分析
 *
 * 3. 環保考量：
 *    - 雨水回收利用
 *    - 滴灌減少蒸發
 *    - 夜間灌溉效率高
 */
