void runGearMotors() {
  if (motor1Boosted) {
    analogWrite(gearMotor1Pin, boostedMotorSpeed);  // pin 9 faster
  } else {
    analogWrite(gearMotor1Pin, normalMotorSpeed);   // pin 9 normal
  }

  analogWrite(gearMotor2Pin, normalMotorSpeed);
  analogWrite(gearMotor3Pin, normalMotorSpeed);
}
void stopGearMotors() {
  analogWrite(gearMotor1Pin, 0);
  analogWrite(gearMotor2Pin, 0);
  analogWrite(gearMotor3Pin, 0);
}

// Call every loop while in LAUNCH_BALL. Returns true when finished.
// Each step is timed from its actual command time, so a slow loop never
// skips a movement or shortens its settling time.
bool launchBall() {
  static bool active = false;
  static byte step = 0;
  static unsigned long stepStarted = 0;
  const byte angles[] = {
    servoClosedAngle, 150, 145, 140, 135, 130, 125, 120,
    115, 110, 105, 100, 95, 92, servoOpenAngle, servoClosedAngle
  };
  const byte stepCount = sizeof(angles) / sizeof(angles[0]);
  const unsigned long now = millis();

  if (!active) {
    active = true;
    step = 0;
    Serial.println("Launching ball");
    launcherServo.attach(servoPin);
    launcherServo.write(angles[step]);
    stepStarted = millis();
    return false;
  }

  // Hold the open position for 1 second; all other steps last 500 ms.
  const unsigned long holdTime = (step == stepCount - 2) ? 1000UL : 500UL;
  if (now - stepStarted < holdTime) return false;

  ++step;
  if (step >= stepCount) {
    launcherServo.detach();
    active = false;
    return true;
  }

  launcherServo.write(angles[step]);
  stepStarted = millis();
  return false;
}