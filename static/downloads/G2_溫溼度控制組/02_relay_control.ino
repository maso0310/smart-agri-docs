/*
 * 第二組：溫溼度控制組
 * 範例 2：繼電器控制
 *
 * 學習重點：
 * - 繼電器工作原理
 * - digitalWrite() 控制繼電器
 * - 高/低電平觸發差異
 */

#define RELAY_FAN 7       // 風扇繼電器
#define RELAY_HUMIDIFIER 8  // 霧化器繼電器
#define LED_FAN 5         // 風扇狀態 LED
#define LED_HUMID 6       // 霧化器狀態 LED

// 設定繼電器觸發方式（依照你的繼電器模組調整）
// true = 高電平觸發（HIGH 時吸合）
// false = 低電平觸發（LOW 時吸合）
#define RELAY_ACTIVE_HIGH true

void setup() {
  // 設定腳位模式
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_HUMIDIFIER, OUTPUT);
  pinMode(LED_FAN, OUTPUT);
  pinMode(LED_HUMID, OUTPUT);

  // 初始化為關閉狀態
  relayOff(RELAY_FAN);
  relayOff(RELAY_HUMIDIFIER);
  digitalWrite(LED_FAN, LOW);
  digitalWrite(LED_HUMID, LOW);

  Serial.begin(9600);
  Serial.println("=== 繼電器控制測試 ===");
  Serial.println();
  Serial.println("測試順序：");
  Serial.println("1. 風扇 ON (2秒)");
  Serial.println("2. 風扇 OFF");
  Serial.println("3. 霧化器 ON (2秒)");
  Serial.println("4. 霧化器 OFF");
  Serial.println("5. 兩者同時 ON (2秒)");
  Serial.println("6. 兩者同時 OFF");
  Serial.println();
}

void loop() {
  // 測試 1：風扇開啟
  Serial.println("[測試] 風扇 ON");
  relayOn(RELAY_FAN);
  digitalWrite(LED_FAN, HIGH);
  delay(2000);

  // 測試 2：風扇關閉
  Serial.println("[測試] 風扇 OFF");
  relayOff(RELAY_FAN);
  digitalWrite(LED_FAN, LOW);
  delay(1000);

  // 測試 3：霧化器開啟
  Serial.println("[測試] 霧化器 ON");
  relayOn(RELAY_HUMIDIFIER);
  digitalWrite(LED_HUMID, HIGH);
  delay(2000);

  // 測試 4：霧化器關閉
  Serial.println("[測試] 霧化器 OFF");
  relayOff(RELAY_HUMIDIFIER);
  digitalWrite(LED_HUMID, LOW);
  delay(1000);

  // 測試 5：兩者同時開啟
  Serial.println("[測試] 風扇 + 霧化器 ON");
  relayOn(RELAY_FAN);
  relayOn(RELAY_HUMIDIFIER);
  digitalWrite(LED_FAN, HIGH);
  digitalWrite(LED_HUMID, HIGH);
  delay(2000);

  // 測試 6：兩者同時關閉
  Serial.println("[測試] 全部 OFF");
  relayOff(RELAY_FAN);
  relayOff(RELAY_HUMIDIFIER);
  digitalWrite(LED_FAN, LOW);
  digitalWrite(LED_HUMID, LOW);
  delay(3000);

  Serial.println("--- 重新開始測試 ---");
  Serial.println();
}

// 開啟繼電器（考慮觸發電平）
void relayOn(int pin) {
  if (RELAY_ACTIVE_HIGH) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

// 關閉繼電器（考慮觸發電平）
void relayOff(int pin) {
  if (RELAY_ACTIVE_HIGH) {
    digitalWrite(pin, LOW);
  } else {
    digitalWrite(pin, HIGH);
  }
}

/*
 * 知識補充：
 *
 * 繼電器（Relay）原理：
 * - 用小電流控制大電流的電磁開關
 * - 5V DC 控制端可以切換 220V AC 的電器
 * - 聽到「喀嗒」聲表示繼電器動作
 *
 * 繼電器模組腳位：
 * - VCC：電源正極（5V）
 * - GND：電源負極
 * - IN：控制訊號輸入
 * - COM：共用端（接電器一端）
 * - NO：常開端（平時斷開，通電接通）
 * - NC：常閉端（平時接通，通電斷開）
 *
 * 高電平 vs 低電平觸發：
 * - 高電平觸發：IN = HIGH (5V) 時繼電器吸合
 * - 低電平觸發：IN = LOW (0V) 時繼電器吸合
 * - 常見模組多為低電平觸發！
 *
 * 安全注意事項：
 * - 操作 AC 電源時務必注意安全
 * - 教學時建議只使用 DC 設備或 LED 模擬
 * - 不確定時不要亂接 AC 電源！
 *
 * 練習題：
 * 1. 確認你的繼電器是高電平還是低電平觸發
 * 2. 修改程式，使用序列埠輸入控制繼電器
 * 3. 加入 LED 呼吸燈效果表示設備運轉中
 */
