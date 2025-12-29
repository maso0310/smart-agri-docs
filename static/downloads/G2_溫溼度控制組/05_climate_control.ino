/*
 * 第二組：溫溼度控制組
 * 範例 5：完整溫溼度控制系統（整合實作）
 *
 * 功能說明：
 * - DHT11 感測溫度與濕度
 * - 溫度過高自動開風扇
 * - 濕度過低自動開霧化器
 * - 包含遲滯控制與最小運轉時間
 * - LED 狀態指示
 *
 * 這就是智慧溫室環境控制的基本原型！
 */

#include <DHT.h>

//=== 腳位定義 ===
#define DHT_PIN 2
#define DHT_TYPE DHT11
#define RELAY_FAN 7
#define RELAY_HUMIDIFIER 8
#define LED_FAN 5
#define LED_HUMID 6

//=== 溫度控制參數 ===
#define TEMP_HIGH 28.0    // 高於此溫度開風扇
#define TEMP_LOW 26.0     // 低於此溫度關風扇

//=== 濕度控制參數 ===
#define HUMIDITY_LOW 55.0   // 低於此濕度開霧化器
#define HUMIDITY_HIGH 70.0  // 高於此濕度關霧化器

//=== 最小運轉時間（毫秒）===
#define MIN_RUN_TIME 10000

//=== 繼電器觸發方式 ===
#define RELAY_ACTIVE_HIGH true

//=== 全域變數 ===
DHT dht(DHT_PIN, DHT_TYPE);

bool fanOn = false;
bool humidifierOn = false;
unsigned long fanStartTime = 0;
unsigned long humidifierStartTime = 0;

void setup() {
  // 設定腳位
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_HUMIDIFIER, OUTPUT);
  pinMode(LED_FAN, OUTPUT);
  pinMode(LED_HUMID, OUTPUT);

  // 初始化為關閉
  relayOff(RELAY_FAN);
  relayOff(RELAY_HUMIDIFIER);
  digitalWrite(LED_FAN, LOW);
  digitalWrite(LED_HUMID, LOW);

  // 初始化感測器
  dht.begin();

  // 序列埠
  Serial.begin(9600);
  Serial.println("╔═══════════════════════════════════════╗");
  Serial.println("║    智慧溫室 溫溼度控制系統 v1.0       ║");
  Serial.println("╠═══════════════════════════════════════╣");
  Serial.print("║ 風扇: ");
  Serial.print(TEMP_LOW);
  Serial.print("°C 關, ");
  Serial.print(TEMP_HIGH);
  Serial.println("°C 開          ║");
  Serial.print("║ 霧化: ");
  Serial.print(HUMIDITY_LOW);
  Serial.print("% 開, ");
  Serial.print(HUMIDITY_HIGH);
  Serial.println("% 關            ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.println();
}

void loop() {
  delay(2000);

  // 讀取感測器
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // 檢查讀取是否成功
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("[錯誤] 感測器讀取失敗！");
    return;
  }

  // 控制風扇
  controlFan(temperature);

  // 控制霧化器
  controlHumidifier(humidity);

  // 顯示狀態
  displayStatus(temperature, humidity);
}

//=== 風扇控制 ===
void controlFan(float temp) {
  unsigned long runTime = fanOn ? (millis() - fanStartTime) : 0;

  if (!fanOn && temp > TEMP_HIGH) {
    // 溫度過高，開風扇
    Serial.println("[動作] 溫度過高 → 開啟風扇");
    relayOn(RELAY_FAN);
    digitalWrite(LED_FAN, HIGH);
    fanOn = true;
    fanStartTime = millis();

  } else if (fanOn && temp < TEMP_LOW) {
    // 溫度恢復，檢查最小運轉時間
    if (runTime >= MIN_RUN_TIME) {
      Serial.println("[動作] 溫度正常 → 關閉風扇");
      relayOff(RELAY_FAN);
      digitalWrite(LED_FAN, LOW);
      fanOn = false;
    }
  }
}

//=== 霧化器控制 ===
void controlHumidifier(float humid) {
  unsigned long runTime = humidifierOn ? (millis() - humidifierStartTime) : 0;

  if (!humidifierOn && humid < HUMIDITY_LOW) {
    // 濕度過低，開霧化器
    Serial.println("[動作] 濕度過低 → 開啟霧化器");
    relayOn(RELAY_HUMIDIFIER);
    digitalWrite(LED_HUMID, HIGH);
    humidifierOn = true;
    humidifierStartTime = millis();

  } else if (humidifierOn && humid > HUMIDITY_HIGH) {
    // 濕度足夠，檢查最小運轉時間
    if (runTime >= MIN_RUN_TIME) {
      Serial.println("[動作] 濕度正常 → 關閉霧化器");
      relayOff(RELAY_HUMIDIFIER);
      digitalWrite(LED_HUMID, LOW);
      humidifierOn = false;
    }
  }
}

//=== 顯示狀態 ===
void displayStatus(float temp, float humid) {
  Serial.print("溫度: ");
  Serial.print(temp, 1);
  Serial.print("°C ");

  // 溫度狀態指示
  if (temp > TEMP_HIGH) {
    Serial.print("[高!] ");
  } else if (temp < TEMP_LOW) {
    Serial.print("[正常] ");
  } else {
    Serial.print("[--] ");
  }

  Serial.print("| 濕度: ");
  Serial.print(humid, 1);
  Serial.print("% ");

  // 濕度狀態指示
  if (humid < HUMIDITY_LOW) {
    Serial.print("[低!] ");
  } else if (humid > HUMIDITY_HIGH) {
    Serial.print("[正常] ");
  } else {
    Serial.print("[--] ");
  }

  Serial.print("| 風扇:");
  Serial.print(fanOn ? "ON " : "OFF");
  Serial.print(" 霧化:");
  Serial.println(humidifierOn ? "ON" : "OFF");
}

//=== 繼電器控制函數 ===
void relayOn(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? HIGH : LOW);
}

void relayOff(int pin) {
  digitalWrite(pin, RELAY_ACTIVE_HIGH ? LOW : HIGH);
}

/*
 * 實作挑戰：
 *
 * 1. 加入警報功能
 *    溫度超過 35°C 或濕度低於 30% 時發出蜂鳴警報
 *
 * 2. 節能模式
 *    風扇與霧化器不能同時運轉（避免霧氣被吹散）
 *
 * 3. 手動模式
 *    透過序列埠輸入指令手動控制
 *    例如：輸入 "F1" 開風扇、"F0" 關風扇
 *
 * 4. 資料記錄
 *    記錄溫溼度變化（搭配 SD 卡或傳送到電腦）
 *
 * 5. 整合 MQTT
 *    將溫溼度資料傳送給第六組的網路系統
 */

/*
 * 延伸知識：實際溫室環境控制
 *
 * 1. 專業設備：
 *    - 工業級溫溼度感測器（精度更高）
 *    - 大型抽風機/循環扇
 *    - 高壓霧化系統
 *    - 水牆降溫系統
 *
 * 2. 控制策略：
 *    - PID 控制（更平滑的控制）
 *    - 時段控制（白天/夜晚不同設定）
 *    - 預測控制（根據天氣預報調整）
 *
 * 3. 作物需求：
 *    - 葉菜類：18-25°C，60-70%
 *    - 番茄：20-28°C，60-80%
 *    - 草莓：15-25°C，60-75%
 *    - 蘭花：20-28°C，70-80%
 */
