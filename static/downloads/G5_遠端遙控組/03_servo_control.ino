/*
 * 第五組：遠端遙控組
 * 範例 3：伺服馬達控制
 *
 * 學習重點：
 * - Servo 函式庫使用
 * - 角度控制
 * - 平滑移動
 *
 * 接線：
 * - 訊號線（橙/黃）→ D9
 * - 電源線（紅）→ 5V
 * - 接地線（棕/黑）→ GND
 */

#include <Servo.h>

#define SERVO_PIN 9

Servo myServo;

int currentAngle = 90;  // 目前角度
int targetAngle = 90;   // 目標角度

void setup() {
  Serial.begin(9600);
  Serial.println("=== 伺服馬達控制測試 ===");
  Serial.println();

  // 連接伺服馬達
  myServo.attach(SERVO_PIN);

  // 移動到中間位置
  myServo.write(currentAngle);

  Serial.println("伺服馬達已初始化！");
  Serial.println("目前位置: 90 度（中間）");
  Serial.println();
  Serial.println("序列埠指令：");
  Serial.println("- 輸入 0-180 的數字設定角度");
  Serial.println("- 輸入 'l' = 左（0度）");
  Serial.println("- 輸入 'c' = 中（90度）");
  Serial.println("- 輸入 'r' = 右（180度）");
  Serial.println("- 輸入 's' = 掃描測試");
  Serial.println();
}

void loop() {
  // 檢查序列埠輸入
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input == "l" || input == "L") {
      targetAngle = 0;
      Serial.println("目標: 0 度（左）");
    }
    else if (input == "c" || input == "C") {
      targetAngle = 90;
      Serial.println("目標: 90 度（中）");
    }
    else if (input == "r" || input == "R") {
      targetAngle = 180;
      Serial.println("目標: 180 度（右）");
    }
    else if (input == "s" || input == "S") {
      Serial.println("執行掃描測試...");
      sweepTest();
      return;
    }
    else {
      // 嘗試解析為數字
      int angle = input.toInt();
      if (angle >= 0 && angle <= 180) {
        targetAngle = angle;
        Serial.print("目標: ");
        Serial.print(targetAngle);
        Serial.println(" 度");
      } else {
        Serial.println("無效輸入！請輸入 0-180");
      }
    }
  }

  // 平滑移動到目標角度
  smoothMove();
}

// 平滑移動函數
void smoothMove() {
  if (currentAngle != targetAngle) {
    if (currentAngle < targetAngle) {
      currentAngle++;
    } else {
      currentAngle--;
    }

    myServo.write(currentAngle);

    Serial.print("角度: ");
    Serial.println(currentAngle);

    delay(15);  // 調整此值改變移動速度
  }
}

// 掃描測試
void sweepTest() {
  Serial.println("掃描: 0 → 180");
  for (int angle = 0; angle <= 180; angle += 5) {
    myServo.write(angle);
    Serial.print(".");
    delay(50);
  }
  Serial.println();

  Serial.println("掃描: 180 → 0");
  for (int angle = 180; angle >= 0; angle -= 5) {
    myServo.write(angle);
    Serial.print(".");
    delay(50);
  }
  Serial.println();

  Serial.println("掃描完成！回到中間位置");
  myServo.write(90);
  currentAngle = 90;
  targetAngle = 90;
}

/*
 * 知識補充：
 *
 * Servo 函式庫主要函數：
 *
 * 1. servo.attach(pin)
 *    - 連接伺服馬達到指定腳位
 *    - 腳位需支援 PWM（Arduino UNO: 3,5,6,9,10,11）
 *
 * 2. servo.write(angle)
 *    - 設定角度（0-180 度）
 *    - 會立即移動到該角度
 *
 * 3. servo.writeMicroseconds(us)
 *    - 直接設定脈衝寬度（微秒）
 *    - 通常 500-2500μs
 *    - 比 write() 更精確
 *
 * 4. servo.read()
 *    - 讀取目前角度設定值
 *
 * 5. servo.detach()
 *    - 斷開伺服馬達
 *    - 馬達會停止保持位置
 *
 * 電源注意事項：
 * - SG90 小馬達可直接用 Arduino 5V
 * - 多顆馬達或大扭力馬達需外接電源
 * - 外接電源時，GND 要與 Arduino 共地
 *
 * 練習題：
 * 1. 修改移動速度（改變 delay 值）
 * 2. 加入角度限制（例如只能 30-150 度）
 * 3. 用電位器控制伺服馬達角度
 */
