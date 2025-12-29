/*
 * 第五組：遠端遙控組
 * 範例 1：紅外線接收測試
 *
 * 學習重點：
 * - 紅外線接收器初始化
 * - 解碼遙控器訊號
 * - 記錄各按鍵編碼
 *
 * 需安裝函式庫：IRremote (by shirriff)
 *
 * 接線：
 * - OUT → D2
 * - VCC → 5V
 * - GND → GND
 */

#include <IRremote.hpp>

#define IR_RECEIVE_PIN 2  // 紅外線接收腳位

void setup() {
  Serial.begin(9600);
  Serial.println("=== 紅外線接收測試 ===");
  Serial.println();

  // 初始化紅外線接收
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("紅外線接收器已啟動！");
  Serial.println("請按下遙控器按鍵...");
  Serial.println();
  Serial.println("提示：記錄每個按鍵的編碼，");
  Serial.println("稍後用於控制程式。");
  Serial.println();
  Serial.println("─────────────────────────");
}

void loop() {
  // 檢查是否收到紅外線訊號
  if (IrReceiver.decode()) {

    // 過濾重複碼
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      Serial.println("(重複按壓)");
    } else {
      // 顯示接收到的資訊
      Serial.print("協定: ");
      Serial.println(getProtocolString(IrReceiver.decodedIRData.protocol));

      Serial.print("位址: 0x");
      Serial.println(IrReceiver.decodedIRData.address, HEX);

      Serial.print("指令: 0x");
      Serial.println(IrReceiver.decodedIRData.command, HEX);

      Serial.print("原始資料: 0x");
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);

      Serial.println("─────────────────────────");
    }

    // 準備接收下一個訊號
    IrReceiver.resume();
  }
}

/*
 * 知識補充：
 *
 * 紅外線遙控器訊號：
 *
 * 不同廠牌使用不同的協定：
 * - NEC：最常見（大多數便宜遙控器）
 * - Sony：SONY 產品
 * - RC5/RC6：飛利浦
 * - Samsung：三星
 *
 * 重複碼（Repeat Code）：
 * - 當持續按住按鍵時，遙控器會發送重複碼
 * - 重複碼比完整碼短，節省時間
 * - 程式中通常需要特別處理
 *
 * 常見遙控器按鍵編碼範例（NEC 協定）：
 * - 電源：0x45 或 0xA2
 * - 1：0x16 或 0x30
 * - 2：0x19 或 0x18
 * - ▲：0x18 或 0x52
 * - ▼：0x52 或 0x4A
 *
 * 注意：每個遙控器編碼不同，
 * 請用此程式記錄你的遙控器編碼！
 *
 * 練習題：
 * 1. 記錄遙控器上至少 5 個按鍵的編碼
 * 2. 觀察長按與短按的差異
 * 3. 嘗試用不同遙控器測試
 */
