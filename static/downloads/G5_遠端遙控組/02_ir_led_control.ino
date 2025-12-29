/*
 * 第五組：遠端遙控組
 * 範例 2：紅外線控制 LED
 *
 * 學習重點：
 * - 根據按鍵執行動作
 * - switch-case 結構
 * - 狀態切換邏輯
 *
 * 請先執行範例 1 記錄你的遙控器編碼，
 * 然後修改下方的 IR_xxx 定義！
 */

#include <IRremote.hpp>

#define IR_RECEIVE_PIN 2

// LED 腳位
#define LED_RED 3
#define LED_YELLOW 4
#define LED_GREEN 5

// ===== 請修改為你的遙控器編碼 =====
// 以下是範例編碼，需要替換成你的遙控器
#define IR_POWER  0x45    // 電源鍵 - 全部開關
#define IR_1      0x16    // 按鍵 1 - 紅燈
#define IR_2      0x19    // 按鍵 2 - 黃燈
#define IR_3      0x0D    // 按鍵 3 - 綠燈
#define IR_UP     0x18    // 向上鍵 - 全亮
#define IR_DOWN   0x52    // 向下鍵 - 全滅
// =================================

bool redState = false;
bool yellowState = false;
bool greenState = false;

void setup() {
  Serial.begin(9600);
  Serial.println("=== 紅外線控制 LED ===");

  // 初始化紅外線
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  // 設定 LED 腳位
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  // 初始化 LED 關閉
  allOff();

  Serial.println();
  Serial.println("按鍵功能：");
  Serial.println("1 = 紅燈切換");
  Serial.println("2 = 黃燈切換");
  Serial.println("3 = 綠燈切換");
  Serial.println("▲ = 全部亮");
  Serial.println("▼ = 全部滅");
  Serial.println("電源 = 全部切換");
  Serial.println();
}

void loop() {
  if (IrReceiver.decode()) {
    // 過濾重複碼
    if (!(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {

      uint32_t command = IrReceiver.decodedIRData.command;

      Serial.print("收到指令: 0x");
      Serial.print(command, HEX);
      Serial.print(" → ");

      // 根據按鍵執行動作
      switch (command) {
        case IR_1:
          redState = !redState;
          digitalWrite(LED_RED, redState);
          Serial.println("紅燈切換");
          break;

        case IR_2:
          yellowState = !yellowState;
          digitalWrite(LED_YELLOW, yellowState);
          Serial.println("黃燈切換");
          break;

        case IR_3:
          greenState = !greenState;
          digitalWrite(LED_GREEN, greenState);
          Serial.println("綠燈切換");
          break;

        case IR_UP:
          allOn();
          Serial.println("全部亮");
          break;

        case IR_DOWN:
          allOff();
          Serial.println("全部滅");
          break;

        case IR_POWER:
          toggleAll();
          Serial.println("全部切換");
          break;

        default:
          Serial.println("未定義按鍵");
          break;
      }
    }

    IrReceiver.resume();
  }
}

void allOn() {
  redState = yellowState = greenState = true;
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_GREEN, HIGH);
}

void allOff() {
  redState = yellowState = greenState = false;
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_GREEN, LOW);
}

void toggleAll() {
  // 如果任何燈亮著，就全滅；否則全亮
  if (redState || yellowState || greenState) {
    allOff();
  } else {
    allOn();
  }
}

/*
 * 知識補充：
 *
 * switch-case 結構：
 *
 * switch (變數) {
 *   case 值1:
 *     // 執行動作
 *     break;
 *   case 值2:
 *     // 執行動作
 *     break;
 *   default:
 *     // 其他情況
 *     break;
 * }
 *
 * 注意：
 * - 每個 case 後面要有 break
 * - 沒有 break 會繼續執行下一個 case
 * - default 處理未列出的情況
 *
 * 狀態切換技巧：
 * - state = !state; // 反轉布林值
 * - true 變 false，false 變 true
 *
 * 練習題：
 * 1. 加入更多按鍵控制
 * 2. 實作長按功能（閃爍）
 * 3. 加入蜂鳴器反饋聲音
 */
