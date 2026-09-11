byte numbers[10] = {
  0b00111111,
  0b00000110,
  0b01011011,
  0b01001111,
  0b01100110,
  0b01101101,
  0b01111101,
  0b00000111,
  0b01111111,
  0b01101111
};

void setupDisplay() {
  pinMode(SER_PIN, OUTPUT);
  pinMode(RCLK_PIN, OUTPUT);
  pinMode(SRCLK_PIN, OUTPUT);

  pinMode(DIGIT1_PIN, OUTPUT);
  pinMode(DIGIT2_PIN, OUTPUT);

  digitalWrite(DIGIT1_PIN, LOW);
  digitalWrite(DIGIT2_PIN, LOW);
}

void refreshDisplay(int value) {
  int tens = value / 10;
  int ones = value % 10;

  digitalWrite(DIGIT1_PIN, LOW);
  digitalWrite(DIGIT2_PIN, LOW);

  writeToShiftRegister(numbers[tens]);
  digitalWrite(DIGIT1_PIN, HIGH);
  delay(3);

  digitalWrite(DIGIT1_PIN, LOW);
  digitalWrite(DIGIT2_PIN, LOW);

  writeToShiftRegister(numbers[ones]);
  digitalWrite(DIGIT2_PIN, HIGH);
  delay(3);

  digitalWrite(DIGIT1_PIN, LOW);
  digitalWrite(DIGIT2_PIN, LOW);
}

void writeToShiftRegister(byte pattern) {
  digitalWrite(RCLK_PIN, LOW);
  shiftOut(SER_PIN, SRCLK_PIN, MSBFIRST, pattern);
  digitalWrite(RCLK_PIN, HIGH);
}