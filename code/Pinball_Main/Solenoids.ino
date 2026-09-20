volatile bool solenoid1State = false;
volatile bool solenoid2State = false;

void setupSolenoids() {
  pinMode(solenoid1Pin, OUTPUT);
  pinMode(solenoid2Pin, OUTPUT);
  digitalWrite(solenoid1Pin, LOW);
  digitalWrite(solenoid2Pin, LOW);

  pinMode(solenoid1ButtonPin, INPUT_PULLUP);
  pinMode(solenoid2ButtonPin, INPUT_PULLUP);

  noInterrupts();
  solenoid1State = digitalRead(solenoid1ButtonPin) == LOW;
  solenoid2State = digitalRead(solenoid2ButtonPin) == LOW;
  attachInterrupt(digitalPinToInterrupt(solenoid1ButtonPin), solenoid1ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(solenoid2ButtonPin), solenoid2ISR, CHANGE);
  interrupts();
}

void solenoid1ISR() {
  solenoid1State = digitalRead(solenoid1ButtonPin) == LOW;
  // Release switches the gate off even while the main loop is busy.
  if (!solenoid1State) digitalWrite(solenoid1Pin, LOW);
}

void solenoid2ISR() {
  solenoid2State = digitalRead(solenoid2ButtonPin) == LOW;
  if (!solenoid2State) digitalWrite(solenoid2Pin, LOW);
}

void updateSolenoids() {
  // Called only from loop(). Keep this critical section very short:
  // a release interrupt must not be overwritten by a stale HIGH write.
  noInterrupts();
  digitalWrite(solenoid1Pin, solenoid1State ? HIGH : LOW);
  digitalWrite(solenoid2Pin, solenoid2State ? HIGH : LOW);
  interrupts();
}