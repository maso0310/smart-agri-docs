/*
 * 第三組：土壤水份控制組
 * 範例 4：安全澆水機制
 *
 * 學習重點：
 * - 最大澆水時間限制
 * - 澆水間隔保護（冷卻時間）
 * - millis() 時間管理
 */

#define SOIL_PIN A0
#define RELAY_PUMP 7
#define LED_PUMP 9
#define LED_WARNING 11    // 紅色 LED 警告

// 校正值與門檻
int DRY_VALUE = 300;
int WET_VALUE = 700;
int DRY_THRESHOLD = 30;
int WET_THRESHOLD = 60;

// 安全限制
#define MAX_PUMP_TIME 30000     // 最大澆水時間：30 秒
#define MIN_PUMP_INTERVAL 60000 // 最小間隔時間：60 秒

#define RELAY_ACTIVE_HIGH true

bool pumpOn = false;
unsigned long pumpStartTime = 0;  // 開始澆水時間
unsigned long pumpStopTime = 0;   // 停止澆水時間
bool safetyLockout = false;       // 安全鎖定狀態

void setup() {
  pinMode(RELAY_PUMP, OUTPUT);
  pinMode(LED_PUMP, OUTPUT);
  pinMode(LED_WARNING, OUTPUT);

  relayOff(RELAY_PUMP);
  digitalWrite(LED_PUMP, LOW);
  digitalWrite(LED_WARNING, LOW);

  Serial.begin(9600);
  Serial.println("=== 安全澆水控制系統 ===");
  Serial.println();
  Serial.print("最大澆水時間: ");
  Serial.print(MAX_PUMP_TIME / 1000);
  Serial.println(" 秒");
  Serial.print("最小間隔時間: ");
  Serial.print(MIN_PUMP_INTERVAL / 1000);
  Serial.println(" 秒");
  Serial.println();
}

void loop() {
  unsigned long currentTime = millis();

  // 讀取土壤濕度
  int rawValue = analogRead(SOIL_PIN);
  int moisture = map(rawValue, DRY_VALUE, WET_VALUE, 0, 100);
  moisture = constrain(moisture, 0, 100);

  // 檢查安全鎖定是否解除
  if (safetyLockout) {
    unsigned long timeSinceStop = currentTime - pumpStopTime;
    if (timeSinceStop >= MIN_PUMP_INTERVAL) {
      safetyLockout = false;
      digitalWrite(LED_WARNING, LOW);
      Serial.println(">>> 冷卻時間結束，可以再次澆水 <<<");
    }
  }

  // 如果正在澆水，檢查是否超時
  if (pumpOn) {
    unsigned long pumpDuration = currentTime - pumpStartTime;

    if (pumpDuration >= MAX_PUMP_TIME) {
      // 強制停止澆水
      Serial.println("!!! 警告：達到最大澆水時間，強制停止 !!!");
      stopPump();
      safetyLockout = true;
      digitalWrite(LED_WARNING, HIGH);
    } else if (moisture > WET_THRESHOLD) {
      // 正常停止
      Serial.println(">>> 土壤濕潤，停止澆水 <<<");
      stopPump();
    }
  } else {
    // 檢查是否需要澆水
    if (!safetyLockout && moisture < DRY_THRESHOLD) {
      Serial.println(">>> 土壤乾燥，開始澆水 <<<");
      startPump();
    }
  }

  // 顯示狀態
  displayStatus(moisture, currentTime);

  delay(1000);
}

void startPump() {
  pumpOn = true;
  pumpStartTime = millis();
  relayOn(RELAY_PUMP);
  digitalWrite(LED_PUMP, HIGH);
}

void stopPump() {
  pumpOn = false;
  pumpStopTime = millis();
  relayOff(RELAY_PUMP);
  digitalWrite(LED_PUMP, LOW);
}

void displayStatus(int moisture, unsigned long currentTime) {
  Serial.print("濕度: ");
  Serial.print(moisture);
  Serial.print("% | 馬達: ");

  if (pumpOn) {
    unsigned long duration = (currentTime - pumpStartTime) / 1000;
    Serial.print("運轉中 (");
    Serial.print(duration);
    Serial.print("/");
    Serial.print(MAX_PUMP_TIME / 1000);
    Serial.print("秒)");
  } else if (safetyLockout) {
    unsigned long remaining = (MIN_PUMP_INTERVAL - (currentTime - pumpStopTime)) / 1000;
    Serial.print("冷卻中 (");
    Serial.print(remaining);
    Serial.print("秒)");
  } else {
    Serial.print("待機");
  }

  Serial.println();
}

void relayOn(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? HIGH : LOW);
}

void relayOff(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? LOW : HIGH);
}

/*
 * 知識補充：
 *
 * 為什麼需要安全機制？
 *
 * 1. 感測器故障：
 *    - 如果感測器損壞一直回報「乾燥」
 *    - 沒有限制會一直澆水造成水災
 *
 * 2. 馬達保護：
 *    - 連續運轉會過熱
 *    - 冷卻時間讓馬達休息
 *
 * 3. 水資源保護：
 *    - 避免過度澆水浪費水
 *    - 防止植物爛根
 *
 * millis() 函數：
 * - 回傳 Arduino 開機後的毫秒數
 * - 約 49 天會溢位歸零
 * - 適合長時間運行的程式
 * - 不會被 delay() 影響
 *
 * 練習題：
 * 1. 加入「緊急停止」按鈕
 * 2. 記錄每次澆水的時間與水量
 * 3. 加入「維護模式」暫停自動控制
 */
