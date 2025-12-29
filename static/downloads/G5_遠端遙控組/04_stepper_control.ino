/*
 * 第五組：遠端遙控組
 * 範例 4：步進馬達控制
 *
 * 學習重點：
 * - 步進馬達原理
 * - 步數與角度換算
 * - 正反轉控制
 *
 * 使用 28BYJ-48 步進馬達 + ULN2003 驅動板
 *
 * 接線：
 * - IN1 → D8
 * - IN2 → D9
 * - IN3 → D10
 * - IN4 → D11
 * - 電源 → 5V, GND
 */

#include <Stepper.h>

// 28BYJ-48 步進馬達參數
// 內部馬達每圈 32 步，減速比 64:1
// 所以輸出軸每圈需要 32 * 64 = 2048 步
#define STEPS_PER_REVOLUTION 2048

// ULN2003 驅動板接線
// 注意：IN1-IN3-IN2-IN4 的順序是正確的！
#define IN1 8
#define IN2 10
#define IN3 9
#define IN4 11

// 建立步進馬達物件
// 注意順序：IN1, IN3, IN2, IN4（交叉接法）
Stepper stepper(STEPS_PER_REVOLUTION, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(9600);
  Serial.println("=== 步進馬達控制測試 ===");
  Serial.println();

  // 設定馬達速度（RPM）
  // 28BYJ-48 建議 10-15 RPM
  stepper.setSpeed(12);

  Serial.println("步進馬達已初始化！");
  Serial.println();
  Serial.println("序列埠指令：");
  Serial.println("- 輸入正數: 順時針旋轉（步數）");
  Serial.println("- 輸入負數: 逆時針旋轉（步數）");
  Serial.println("- 輸入 'f': 正轉一圈");
  Serial.println("- 輸入 'b': 反轉一圈");
  Serial.println("- 輸入 'h': 轉半圈");
  Serial.println("- 輸入 'q': 轉 90 度");
  Serial.println();
  Serial.println("提示: 2048 步 = 1 圈");
}

void loop() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "f" || input == "F") {
      Serial.println("正轉一圈...");
      stepper.step(STEPS_PER_REVOLUTION);
      Serial.println("完成！");
    }
    else if (input == "b" || input == "B") {
      Serial.println("反轉一圈...");
      stepper.step(-STEPS_PER_REVOLUTION);
      Serial.println("完成！");
    }
    else if (input == "h" || input == "H") {
      Serial.println("轉半圈...");
      stepper.step(STEPS_PER_REVOLUTION / 2);
      Serial.println("完成！");
    }
    else if (input == "q" || input == "Q") {
      Serial.println("轉 90 度...");
      stepper.step(STEPS_PER_REVOLUTION / 4);
      Serial.println("完成！");
    }
    else {
      // 嘗試解析為步數
      int steps = input.toInt();
      if (steps != 0) {
        Serial.print("移動 ");
        Serial.print(steps);
        Serial.println(" 步...");
        stepper.step(steps);
        Serial.println("完成！");
      } else {
        Serial.println("無效輸入！");
      }
    }

    // 釋放馬達（減少發熱）
    releaseMotor();
  }
}

// 釋放馬達線圈（減少發熱與耗電）
void releaseMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

/*
 * 知識補充：
 *
 * 步進馬達 vs 伺服馬達：
 *
 * 步進馬達：
 * - 可以精確控制旋轉角度
 * - 可以連續旋轉（不限 180 度）
 * - 扭力較大
 * - 速度較慢
 * - 需要驅動電路
 * - 靜止時也耗電（保持扭力）
 *
 * 伺服馬達：
 * - 通常限制 0-180 度
 * - 內建控制電路
 * - 有位置反饋
 * - 速度較快
 * - 接線簡單
 *
 * 28BYJ-48 規格：
 * - 工作電壓：5V DC
 * - 步進角：5.625°/64（內部）
 * - 減速比：1:64
 * - 每圈步數：2048（半步模式）
 * - 建議轉速：10-15 RPM
 *
 * 步數換算：
 * - 1 圈 = 2048 步
 * - 1 步 = 360° / 2048 ≈ 0.176°
 * - 90° = 512 步
 * - 180° = 1024 步
 *
 * 練習題：
 * 1. 修改速度，觀察最大可用速度
 * 2. 實作「來回擺動」功能
 * 3. 計算移動特定角度所需的步數
 */
