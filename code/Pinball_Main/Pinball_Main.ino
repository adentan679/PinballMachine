#include "DFRobotDFPlayerMini.h"
#include <Servo.h>

// ================= PIN DEFINITIONS =================

// Buttons / sensors
const int startButtonPin = 4;
const int piezoPin = A6;
const int scoreIRPin = 7;
const int lossIRPin = 8;
const int piezoPin2 = A8;
const int piezoPin3 = A9;

// Speaker
const int speakerPin = 5;

// Gear motors
const int gearMotor1Pin = 9;
const int gearMotor2Pin = 10;
const int gearMotor3Pin = 11;

// Solenoids
const int solenoid1Pin = 12;
const int solenoid2Pin = 13;

const int solenoid1ButtonPin = 2;
const int solenoid2ButtonPin = 3;

// Servo
const int servoPin = 44;
const int servoClosedAngle = 155;
const int servoOpenAngle = 60;

// Score 7-segment shift register pins
const int SER_PIN = 26;
const int RCLK_PIN = 28;
const int SRCLK_PIN = 30;
const int DIGIT1_PIN = 32;
const int DIGIT2_PIN = 34;

// Lives 7-segment shift register pins
const int LIVES_SER_PIN = 50;
const int LIVES_RCLK_PIN = 48;
const int LIVES_SRCLK_PIN = 53;
const int LIVES_DIGIT_PIN = 52;

// ================= FSM STATE =================

enum GameState {
  RESET,
  READY_IDLE,
  LAUNCH_BALL,
  IN_PLAY,
  BALL_LOST,
  NEXT_ROUND,
  GAME_OVER
};

GameState state = RESET;

// ================= GLOBAL VARIABLES =================

int score = 0;
int lives = 3;
bool motor1Boosted = false;

// Nonblocking life-loss pause; entry actions run only once.
bool ballLostStarted = false;
unsigned long ballLostStartedAt = 0;

const int normalMotorSpeed = 120;
const int boostedMotorSpeed = 255;

bool lastStartButton = HIGH;
bool lastScoreIR = HIGH;
bool lastLossIR = HIGH;

unsigned long lastScoreTime = 0;
unsigned long lastLossTime = 0;
unsigned long lastPiezoTime = 0;
unsigned long lastPiezo2Time = 0;
unsigned long lastPiezo3Time = 0;

const unsigned long scoreCooldown = 300;
const unsigned long lossCooldown = 1000;
const unsigned long piezoCooldown = 500;

DFRobotDFPlayerMini myDFPlayer;
Servo launcherServo;

// ================= PIN SETUP FUNCTION =================

void setupPins() {
  pinMode(startButtonPin, INPUT_PULLUP);

  pinMode(scoreIRPin, INPUT);
  pinMode(lossIRPin, INPUT_PULLUP);

  pinMode(gearMotor1Pin, OUTPUT);
  pinMode(gearMotor2Pin, OUTPUT);
  pinMode(gearMotor3Pin, OUTPUT);

  pinMode(speakerPin, OUTPUT);

  analogWrite(gearMotor1Pin, 0);
  analogWrite(gearMotor2Pin, 0);
  analogWrite(gearMotor3Pin, 0);
}

// ================= SETUP =================

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Setup started");

  setupSound();
  Serial.println("Sound setup done");

  setupPins();
  Serial.println("Pins setup done");

  setupDisplay();
  Serial.println("Score display setup done");

  setupLivesDisplay();
  Serial.println("Lives display setup done");

  setupSolenoids();
  Serial.println("Solenoids setup done");


  launcherServo.attach(servoPin);
  launcherServo.write(servoClosedAngle);

  resetGame();

  Serial.println("Pinball FSM started");
}

// ================= MAIN LOOP =================

void loop() {
  refreshDisplay(score);
  refreshLivesDisplay(lives);
  updateSound();

  updateSolenoids();
  checkPiezo();

  switch (state) {

    case RESET:
      resetGame();

      // Start pre-game music once after reset
      startBackgroundMusic();

      state = READY_IDLE;
      break;

    case READY_IDLE:
      stopGearMotors();

      if (startButtonPressed()) {
        stopBackgroundMusic();
        state = LAUNCH_BALL;
      }
      break;

    case LAUNCH_BALL:
      if (launchBall()) {
        state = IN_PLAY;
      }
      break;

    case IN_PLAY:
      runGearMotors();

      if (scoreSensorTriggered()) {
        score++;

        if (score > 99) {
          score = 99;
        }

        playScoreSound();

        Serial.print("Score: ");
        Serial.println(score);
      }

      if (lossSensorTriggered()) {
        state = BALL_LOST;
      }
      break;

    case BALL_LOST:
      if (!ballLostStarted) {
        ballLostStarted = true;
        stopGearMotors();
        lives--;
        if (lives < 0) lives = 0;

        playLoseLifeSound();
        Serial.print("Life lost. Lives left: ");
        Serial.println(lives);
        ballLostStartedAt = millis();
      }

      // Preserve the existing 800 ms pause while servicing the main loop.
      if (millis() - ballLostStartedAt >= 800UL) {
        startBackgroundMusic();
        ballLostStarted = false;
        state = (lives <= 0) ? GAME_OVER : NEXT_ROUND;
      }
      break;

    case NEXT_ROUND:
      stopGearMotors();

      if (startButtonPressed()) {
        stopBackgroundMusic();
        state = LAUNCH_BALL;
      }
      break;

    case GAME_OVER:
      stopGearMotors();

      if (startButtonPressed()) {
        stopBackgroundMusic();
        state = RESET;
      }
      break;
  }
}