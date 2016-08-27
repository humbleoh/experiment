volatile int r1 = 0;
volatile int f1 = 0;
volatile int pulseWidth = 0;
volatile int period = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(9, OUTPUT);
  analogWrite(9, 254);

  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), test, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:

}

void test() {
  attachInterrupt(digitalPinToInterrupt(2), test2, FALLING);
  int r2 = micros();
  if (r1 > 0) {
    period = r2 - r1; // 没有处理 timer overflow 问题
    pulseWidth = f1 - r1; // 没有处理 timer overflow 问题
    //! 没有处理 0 and 255 的场景
    unsigned int dutyCycle = (int32_t) pulseWidth * 255 / period;
    Serial.println(dutyCycle);
    r1 = r2;
  } else {
    r1 = r2;
  }
}

void test2() {
  attachInterrupt(digitalPinToInterrupt(2), test, RISING);
  f1 = micros();
}

