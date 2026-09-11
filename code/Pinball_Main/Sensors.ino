bool startButtonPressed() {
  bool current = digitalRead(startButtonPin);

  if (current == LOW && lastStartButton == HIGH) {
    lastStartButton = current;
    delay(30);
    return true;
  }

  lastStartButton = current;
  return false;
}

bool scoreSensorTriggered() {
  bool current = digitalRead(scoreIRPin);
  unsigned long now = millis();

  // Trigger when IR sensor goes LOW -> HIGH
  if (current == HIGH && lastScoreIR == LOW && now - lastScoreTime > scoreCooldown) {
    lastScoreTime = now;
    lastScoreIR = current;
    return true;
  }

  lastScoreIR = current;
  return false;
}

bool lossSensorTriggered() {
  bool current = digitalRead(lossIRPin);
  unsigned long now = millis();

  if (current == LOW && lastLossIR == HIGH && now - lastLossTime > lossCooldown) {
    lastLossTime = now;
    lastLossIR = current;
    return true;
  }

  lastLossIR = current;
  return false;
}

void checkPiezo() {
  unsigned long now = millis();

  // ================= A6 PIEZO =================
  // Adds score, plays sound, and changes motor 9 speed
  int piezoValue = analogRead(piezoPin);

  if (piezoValue > 150 && now - lastPiezoTime > piezoCooldown) {
    lastPiezoTime = now;

    if (state == IN_PLAY) {
      score++;

      if (score > 99) {
        score = 99;
      }

      motor1Boosted = !motor1Boosted;

      playPiezoSound();

      Serial.print("A6 piezo hit: ");
      Serial.print(piezoValue);
      Serial.print(" | Score: ");
      Serial.println(score);

      if (motor1Boosted) {
        Serial.println("Motor 9 boosted speed");
      } else {
        Serial.println("Motor 9 normal speed");
      }
    }
  }

  // ================= A8 PIEZO =================
  // Adds score only
  int piezoValue2 = analogRead(piezoPin2);

  if (piezoValue2 > 190 && now - lastPiezo2Time > piezoCooldown) {
    lastPiezo2Time = now;

    if (state == IN_PLAY) {
      score++;

      if (score > 99) {
        score = 99;
      }

      playPiezoSound();

      Serial.print("A8 piezo hit: ");
      Serial.print(piezoValue2);
      Serial.print(" | Score: ");
      Serial.println(score);
    }
  }

  // ================= A9 PIEZO =================
  // Adds score only
  int piezoValue3 = analogRead(piezoPin3);

  if (piezoValue3 > 190 && now - lastPiezo3Time > piezoCooldown) {
    lastPiezo3Time = now;

    if (state == IN_PLAY) {
      score++;

      if (score > 99) {
        score = 99;
      }

      playPiezoSound();

      Serial.print("A9 piezo hit: ");
      Serial.print(piezoValue3);
      Serial.print(" | Score: ");
      Serial.println(score);
    }
  }
}