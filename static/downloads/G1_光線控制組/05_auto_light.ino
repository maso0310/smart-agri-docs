/*
 * 第一組：光線控制組
 * 範例 5：自動補光系統（整合實作）
 *
 * 功能說明：
 * - 光敏電阻感測環境光線
 * - 電位器設定光線門檻值
 * - 當光線低於門檻時，LED 自動亮起
 * - LED 亮度與環境光線成反比（越暗越亮）
 *
 * 這就是智慧溫室補光系統的基本原型！
 */

//=== 腳位定義 ===
#define LDR_PIN A0      // 光敏電阻
#define POT_PIN A1      // 電位器（設定門檻）
#define LED_RED 9       // 紅色 LED（補光燈）
#define LED_GREEN 10    // 綠色 LED（狀態指示）

//=== 變數 ===
int lightValue = 0;     // 光線數值
int threshold = 512;    // 門檻值（由電位器控制）
int ledBrightness = 0;  // LED 亮度

void setup() {
  // 設定腳位
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // 初始化序列埠
  Serial.begin(9600);
  Serial.println("=== 智慧溫室自動補光系統 ===");
  Serial.println();
  Serial.println("說明：");
  Serial.println("- 轉動電位器可調整光線門檻值");
  Serial.println("- 綠色 LED 亮 = 光線充足");
  Serial.println("- 紅色 LED 亮 = 補光模式");
  Serial.println();
}

void loop() {
  // 讀取感測器數值
  lightValue = analogRead(LDR_PIN);
  threshold = analogRead(POT_PIN);

  // 判斷是否需要補光
  if (lightValue < threshold) {
    // 光線不足，啟動補光
    // 亮度與光線成反比：光線越暗，補光越亮
    ledBrightness = map(lightValue, 0, threshold, 255, 50);
    ledBrightness = constrain(ledBrightness, 50, 255);

    analogWrite(LED_RED, ledBrightness);   // 補光燈亮
    digitalWrite(LED_GREEN, LOW);           // 狀態燈滅

    Serial.print("[補光模式] ");
  } else {
    // 光線充足，關閉補光
    analogWrite(LED_RED, 0);               // 補光燈滅
    digitalWrite(LED_GREEN, HIGH);          // 狀態燈亮

    Serial.print("[光線充足] ");
  }

  // 顯示數值
  Serial.print("光線: ");
  Serial.print(lightValue);
  Serial.print(" | 門檻: ");
  Serial.print(threshold);
  if (lightValue < threshold) {
    Serial.print(" | 補光亮度: ");
    Serial.println(ledBrightness);
  } else {
    Serial.println();
  }

  delay(200);
}

/*
 * 實作挑戰：
 *
 * 1. 加入遲滯效果（Hysteresis）
 *    避免在門檻值附近頻繁切換
 *    提示：設定兩個門檻值，開啟門檻和關閉門檻
 *
 * 2. 加入漸變效果
 *    補光燈開啟/關閉時漸變，而非突然切換
 *    提示：使用變數追蹤目前亮度，每次迴圈微調
 *
 * 3. 加入時間延遲
 *    光線不足超過 3 秒才啟動補光
 *    提示：使用 millis() 記錄時間
 *
 * 4. 整合到溫室系統
 *    思考如何將此感測數值傳送給其他組別
 *    （提示：MQTT）
 */

/*
 * 延伸知識：實際溫室補光系統
 *
 * 1. 光源類型：
 *    - LED 植物燈（紅光 630-660nm、藍光 450-470nm）
 *    - 高壓鈉燈（HPS）
 *    - 金屬鹵化物燈（MH）
 *
 * 2. 補光策略：
 *    - 延長日照（早晚補光）
 *    - 陰天補光
 *    - 特定波長補光（促進開花/生長）
 *
 * 3. 專業感測器：
 *    - 照度計（Lux meter）
 *    - PAR 感測器（光合有效輻射）
 *    - 分光光度計
 */
