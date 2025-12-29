/*
 * 第五組：遠端遙控組
 * 範例 5：完整遙控系統（整合實作）
 *
 * 功能說明：
 * - 紅外線遙控器控制
 * - 伺服馬達控制通風窗角度
 * - LED 狀態指示
 * - 預設位置記憶
 *
 * 這就是智慧溫室遠端控制的基本原型！
 *
 * 請修改下方 IR_xxx 為你的遙控器編碼！
 */

#include <IRremote.hpp>
#include <Servo.h>

//=== 腳位定義 ===
#define IR_RECEIVE_PIN 2
#define SERVO_PIN 9
#define LED_OPEN 3      // 綠色 LED - 窗戶開啟
#define LED_CLOSE 4     // 紅色 LED - 窗戶關閉
#define LED_MOVING 5    // 黃色 LED - 正在移動

//=== 紅外線編碼（請修改為你的遙控器）===
#define IR_POWER  0x45  // 電源 - 切換開關
#define IR_UP     0x18  // 向上 - 開窗（增加角度）
#define IR_DOWN   0x52  // 向下 - 關窗（減少角度）
#define IR_1      0x16  // 按鍵1 - 預設位置：關閉 (0°)
#define IR_2      0x19  // 按鍵2 - 預設位置：半開 (45°)
#define IR_3      0x0D  // 按鍵3 - 預設位置：全開 (90°)
#define IR_OK     0x1C  // OK鍵 - 顯示目前狀態

//=== 窗戶參數 ===
#define ANGLE_CLOSE 0     // 關閉角度
#define ANGLE_HALF 45     // 半開角度
#define ANGLE_OPEN 90     // 全開角度
#define ANGLE_MIN 0       // 最小角度
#define ANGLE_MAX 90      // 最大角度
#define ANGLE_STEP 5      // 每次調整角度

//=== 全域變數 ===
Servo windowServo;
int currentAngle = 0;
int targetAngle = 0;
bool isMoving = false;
unsigned long lastMoveTime = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("╔═══════════════════════════════════════╗");
  Serial.println("║     智慧溫室 遙控通風系統 v1.0        ║");
  Serial.println("╚═══════════════════════════════════════╝");
  Serial.println();

  // 初始化紅外線
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  // 初始化伺服馬達
  windowServo.attach(SERVO_PIN);
  windowServo.write(currentAngle);

  // 初始化 LED
  pinMode(LED_OPEN, OUTPUT);
  pinMode(LED_CLOSE, OUTPUT);
  pinMode(LED_MOVING, OUTPUT);

  updateLEDs();

  Serial.println("系統初始化完成！");
  Serial.println();
  printControls();
}

void loop() {
  // 處理紅外線訊號
  handleIR();

  // 平滑移動伺服馬達
  smoothMove();

  // 更新 LED 狀態
  updateLEDs();
}

//=== 處理紅外線訊號 ===
void handleIR() {
  if (IrReceiver.decode()) {
    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

      uint32_t command = IrReceiver.decodedIRData.command;

      switch (command) {
        case IR_UP:
          // 開窗（增加角度）
          targetAngle = constrain(targetAngle + ANGLE_STEP, ANGLE_MIN, ANGLE_MAX);
          Serial.print("開窗 → 目標: ");
          Serial.print(targetAngle);
          Serial.println("°");
          break;

        case IR_DOWN:
          // 關窗（減少角度）
          targetAngle = constrain(targetAngle - ANGLE_STEP, ANGLE_MIN, ANGLE_MAX);
          Serial.print("關窗 → 目標: ");
          Serial.print(targetAngle);
          Serial.println("°");
          break;

        case IR_1:
          // 預設：關閉
          targetAngle = ANGLE_CLOSE;
          Serial.println("預設位置: 關閉 (0°)");
          break;

        case IR_2:
          // 預設：半開
          targetAngle = ANGLE_HALF;
          Serial.println("預設位置: 半開 (45°)");
          break;

        case IR_3:
          // 預設：全開
          targetAngle = ANGLE_OPEN;
          Serial.println("預設位置: 全開 (90°)");
          break;

        case IR_POWER:
          // 切換開關
          if (currentAngle > ANGLE_MIN) {
            targetAngle = ANGLE_CLOSE;
            Serial.println("切換: 關閉窗戶");
          } else {
            targetAngle = ANGLE_OPEN;
            Serial.println("切換: 開啟窗戶");
          }
          break;

        case IR_OK:
          // 顯示狀態
          printStatus();
          break;

        default:
          Serial.print("未定義按鍵: 0x");
          Serial.println(command, HEX);
          break;
      }
    }

    IrReceiver.resume();
  }
}

//=== 平滑移動 ===
void smoothMove() {
  if (currentAngle != targetAngle) {
    unsigned long now = millis();
    if (now - lastMoveTime >= 20) {  // 每 20ms 移動一度
      lastMoveTime = now;
      isMoving = true;

      if (currentAngle < targetAngle) {
        currentAngle++;
      } else {
        currentAngle--;
      }

      windowServo.write(currentAngle);
    }
  } else {
    isMoving = false;
  }
}

//=== 更新 LED 狀態 ===
void updateLEDs() {
  if (isMoving) {
    // 移動中：黃燈閃爍
    digitalWrite(LED_MOVING, (millis() / 200) % 2);
    digitalWrite(LED_OPEN, LOW);
    digitalWrite(LED_CLOSE, LOW);
  } else {
    digitalWrite(LED_MOVING, LOW);

    if (currentAngle >= ANGLE_MAX) {
      // 全開：綠燈亮
      digitalWrite(LED_OPEN, HIGH);
      digitalWrite(LED_CLOSE, LOW);
    } else if (currentAngle <= ANGLE_MIN) {
      // 關閉：紅燈亮
      digitalWrite(LED_OPEN, LOW);
      digitalWrite(LED_CLOSE, HIGH);
    } else {
      // 半開：綠紅燈都微亮
      digitalWrite(LED_OPEN, HIGH);
      digitalWrite(LED_CLOSE, HIGH);
    }
  }
}

//=== 顯示狀態 ===
void printStatus() {
  Serial.println();
  Serial.println("┌─────────────────────────┐");
  Serial.print("│ 目前角度: ");
  Serial.print(currentAngle);
  Serial.println("°            │");

  Serial.print("│ 開啟程度: ");
  int percent = map(currentAngle, ANGLE_MIN, ANGLE_MAX, 0, 100);
  Serial.print(percent);
  Serial.println("%            │");

  Serial.print("│ 狀態: ");
  if (currentAngle >= ANGLE_MAX) {
    Serial.println("全開             │");
  } else if (currentAngle <= ANGLE_MIN) {
    Serial.println("關閉             │");
  } else {
    Serial.println("半開             │");
  }
  Serial.println("└─────────────────────────┘");
  Serial.println();
}

//=== 顯示控制說明 ===
void printControls() {
  Serial.println("遙控器按鍵功能：");
  Serial.println("─────────────────────────");
  Serial.println("▲ (向上) = 開窗 (+5°)");
  Serial.println("▼ (向下) = 關窗 (-5°)");
  Serial.println("1 = 關閉 (0°)");
  Serial.println("2 = 半開 (45°)");
  Serial.println("3 = 全開 (90°)");
  Serial.println("電源 = 切換開關");
  Serial.println("OK = 顯示狀態");
  Serial.println("─────────────────────────");
  Serial.println();
}

/*
 * 實作挑戰：
 *
 * 1. 加入步進馬達控制窗簾
 *    使用不同按鍵控制第二個馬達
 *
 * 2. 自動模式
 *    按特定鍵切換自動/手動模式
 *    自動模式根據時間或感測器控制
 *
 * 3. 位置記憶
 *    記住上次位置，開機自動恢復
 *    （提示：EEPROM）
 *
 * 4. 安全限位
 *    加入限位開關，防止過度轉動
 *
 * 5. 整合 MQTT
 *    接收網路指令控制馬達
 */

/*
 * 延伸知識：智慧溫室通風控制
 *
 * 1. 通風策略：
 *    - 溫度控制：高溫時開窗散熱
 *    - 濕度控制：高濕時通風排濕
 *    - CO2 控制：補充新鮮空氣
 *
 * 2. 商業系統：
 *    - 電動推桿：大型窗戶
 *    - 齒輪減速馬達：重型百葉窗
 *    - 氣動開窗器：溫室天窗
 *
 * 3. 控制方式：
 *    - 手動控制：遙控器/按鈕
 *    - 自動控制：感測器觸發
 *    - 遠端控制：手機 APP/網頁
 *    - 時間排程：定時開關
 */
