/*
 * 第一組：光線控制組
 * 範例 1：LED 閃爍（認識數位輸出）
 *
 * 學習重點：
 * - pinMode() 設定腳位模式
 * - digitalWrite() 數位輸出
 * - delay() 延遲函數
 */

#define LED_PIN 9  // LED 接在 D9 腳位

void setup() {
  // 設定 LED 腳位為輸出模式
  pinMode(LED_PIN, OUTPUT);

  // 初始化序列埠（用於除錯）
  Serial.begin(9600);
  Serial.println("LED 閃爍程式啟動");
}

void loop() {
  // 點亮 LED
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED ON");
  delay(1000);  // 等待 1 秒

  // 熄滅 LED
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED OFF");
  delay(1000);  // 等待 1 秒
}

/*
 * 練習題：
 * 1. 修改 delay 時間，讓 LED 閃爍更快或更慢
 * 2. 加入第二顆 LED，讓兩顆 LED 交替閃爍
 * 3. 製作 SOS 摩斯密碼閃爍（... --- ...）
 */
