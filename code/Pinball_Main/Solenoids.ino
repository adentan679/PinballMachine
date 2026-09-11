volatile bool solenoid1State = false;
volatile bool solenoid2State = false;

void setupSolenoids() {
  pinMode(solenoid1Pin, OUTPUT);
  pinMode(solenoid2Pin, OUTPUT);

  pinMode(solenoid1ButtonPin, INPUT_PULLUP);
  pinMode(solenoid2ButtonPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(solenoid1ButtonPin), solenoid1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(solenoid2ButtonPin), solenoid2ISR, CHANGE);

  digitalWrite(solenoid1Pin, LOW);
  digitalWrite(solenoid2Pin, LOW);
}

void solenoid1ISR() {
  solenoid1State = digitalRead(solenoid1ButtonPin) == LOW;
}

void solenoid2ISR() {
  solenoid2State = digitalRead(solenoid2ButtonPin) == LOW;
}

void updateSolenoids() {
  digitalWrite(solenoid1Pin, solenoid1State ? HIGH : LOW);
  digitalWrite(solenoid2Pin, solenoid2State ? HIGH : LOW);
}