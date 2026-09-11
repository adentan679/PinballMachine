void resetGame() {
  score = 0;
  lives = 3;
  motor1Boosted = false;

  stopGearMotors();

  launcherServo.attach(servoPin);
  launcherServo.write(servoClosedAngle);
  delay(500);
  launcherServo.detach();

  Serial.println("Game reset");
  Serial.println("Lives = 3");
}