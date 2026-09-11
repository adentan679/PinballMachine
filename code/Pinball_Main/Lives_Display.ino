byte livesNumbers[10] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

void setupLivesDisplay() {
  pinMode(LIVES_SER_PIN, OUTPUT);
  pinMode(LIVES_RCLK_PIN, OUTPUT);
  pinMode(LIVES_SRCLK_PIN, OUTPUT);
  pinMode(LIVES_DIGIT_PIN, OUTPUT);

  digitalWrite(LIVES_DIGIT_PIN, LOW);
}

void refreshLivesDisplay(int livesValue) {
  if (livesValue < 0) livesValue = 0;
  if (livesValue > 9) livesValue = 9;

  writeToLivesShiftRegister(livesNumbers[livesValue]);

  digitalWrite(LIVES_DIGIT_PIN, HIGH);
}

void writeToLivesShiftRegister(byte pattern) {
  digitalWrite(LIVES_RCLK_PIN, LOW);
  shiftOut(LIVES_SER_PIN, LIVES_SRCLK_PIN, MSBFIRST, pattern);
  digitalWrite(LIVES_RCLK_PIN, HIGH);
}