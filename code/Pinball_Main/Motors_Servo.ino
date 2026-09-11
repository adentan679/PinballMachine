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

void launchBall() {
  Serial.println("Launching ball");

  launcherServo.attach(servoPin);

  launcherServo.write(155);
  delay(500);

  
  launcherServo.write(150);
  delay(500);

  launcherServo.write(145);
  delay(500);

  launcherServo.write(140);
  delay(500);

  launcherServo.write(135);
  delay(500);

  launcherServo.write(130);
  delay(500);

  launcherServo.write(125);
  delay(500);

  launcherServo.write(120);
  delay(500);

  launcherServo.write(115);
  delay(500);

  launcherServo.write(110);
  delay(500);

  launcherServo.write(105);
  delay(500);

  launcherServo.write(100);
  delay(500);

  launcherServo.write(95);
  delay(500);
  launcherServo.write(92);
  delay(500);
  /*
  launcherServo.write(90);
  delay(500);

  launcherServo.write(87);
  delay(500);

  launcherServo.write(85);
  delay(500);

  launcherServo.write(82);
  delay(500);

  launcherServo.write(80);
  delay(500);

  launcherServo.write(78);
  delay(500);

  launcherServo.write(75);
  delay(500);

  launcherServo.write(72);
  delay(500);

  launcherServo.write(70);
  delay(500);

  launcherServo.write(67);
  delay(500);

  launcherServo.write(65);
  delay(500);

  launcherServo.write(62);
  delay(500);
  */
  
  launcherServo.write(servoOpenAngle);    // goes to 60
  delay(1000);

  launcherServo.write(servoClosedAngle);  // returns to 160
  delay(500);

  launcherServo.detach();                 // optional: stops servo buzzing
}