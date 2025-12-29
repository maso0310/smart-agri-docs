/*
 * 第三組：土壤水份控制組
 * 範例 3：繼電器控制抽水馬達
 *
 * 學習重點：
 * - 繼電器控制
 * - 模擬抽水馬達開關
 * - 基本安全保護
 */

#define SOIL_PIN A0
#define RELAY_PUMP 7      // 繼電器控制抽水馬達
#define LED_PUMP 9        // 藍色 LED 模擬馬達運轉
#define LED_STATUS 10     // 綠色 LED 狀態指示

// 校正值與門檻
int DRY_VALUE = 300;
int WET_VALUE = 700;
int DRY_THRESHOLD = 30;   // 低於此濕度開始澆水
int WET_THRESHOLD = 60;   // 高於此濕度停止澆水

// 繼電器觸發方式
#define RELAY_ACTIVE_HIGH true

bool pumpOn = false;

void setup() {
  pinMode(RELAY_PUMP, OUTPUT);
  pinMode(LED_PUMP, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);

  relayOff(RELAY_PUMP);
  digitalWrite(LED_PUMP, LOW);
  digitalWrite(LED_STATUS, HIGH);

  Serial.begin(9600);
  Serial.println("=== 自動澆水控制測試 ===");
  Serial.println();
  Serial.print("乾燥門檻: ");
  Serial.print(DRY_THRESHOLD);
  Serial.println("% (開始澆水)");
  Serial.print("濕潤門檻: ");
  Serial.print(WET_THRESHOLD);
  Serial.println("% (停止澆水)");
  Serial.println();
}

void loop() {
  // 讀取土壤濕度
  int rawValue = analogRead(SOIL_PIN);
  int moisture = map(rawValue, DRY_VALUE, WET_VALUE, 0, 100);
  moisture = constrain(moisture, 0, 100);

  // 控制邏輯（含遲滯）
  if (!pumpOn && moisture < DRY_THRESHOLD) {
    // 太乾了，開始澆水
    Serial.println(">>> 土壤乾燥！開始澆水 <<<");
    pumpOn = true;
    relayOn(RELAY_PUMP);
    digitalWrite(LED_PUMP, HIGH);
    digitalWrite(LED_STATUS, LOW);

  } else if (pumpOn && moisture > WET_THRESHOLD) {
    // 夠濕了，停止澆水
    Serial.println(">>> 土壤濕潤！停止澆水 <<<");
    pumpOn = false;
    relayOff(RELAY_PUMP);
    digitalWrite(LED_PUMP, LOW);
    digitalWrite(LED_STATUS, HIGH);
  }

  // 顯示狀態
  Serial.print("濕度: ");
  Serial.print(moisture);
  Serial.print("% | 馬達: ");
  Serial.println(pumpOn ? "運轉中" : "停止");

  delay(1000);
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
 * 抽水馬達類型：
 *
 * 1. 潛水馬達（Submersible）：
 *    - 放在水中使用
 *    - 適合水槽、魚缸
 *
 * 2. 蠕動泵（Peristaltic）：
 *    - 精確控制流量
 *    - 不接觸液體，衛生
 *
 * 3. 隔膜泵（Diaphragm）：
 *    - 自吸能力強
 *    - 可以抽取有氣泡的液體
 *
 * 注意事項：
 * - 馬達需要的電流通常超過 Arduino 輸出能力
 * - 必須使用繼電器或電晶體驅動
 * - 注意馬達電壓規格
 * - 考慮防水問題
 *
 * 練習題：
 * 1. 加入手動控制按鈕
 * 2. LED 閃爍表示正在澆水
 * 3. 加入「預覽模式」只顯示不實際動作
 */
