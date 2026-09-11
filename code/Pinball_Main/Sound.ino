bool dfPlayerReady = false;

const unsigned long loseSoundDuration = 3000;

const int losingSoundTrack = 1;      // 0001.mp3
const int scoreSoundTrack = 2;       // 0002.mp3
const int backgroundMusicTrack = 3;  // 0004.mp3

bool backgroundMusicOn = false;
bool waitingToResumeBackground = false;
unsigned long resumeBackgroundTime = 0;

void setupSound() {
  Serial1.begin(9600);

  Serial.println("Starting DFPlayer...");

  if (!myDFPlayer.begin(Serial1)) {
    Serial.println("DFPlayer not detected. Continuing without sound.");
    dfPlayerReady = false;
    return;
  }

  Serial.println("DFPlayer ready.");
  myDFPlayer.volume(28);
  dfPlayerReady = true;
}

void startBackgroundMusic() {
  if (!dfPlayerReady) return;
  if (backgroundMusicOn) return;

  myDFPlayer.loop(backgroundMusicTrack);
  backgroundMusicOn = true;

  Serial.println("Background music started");
}

void stopBackgroundMusic() {
  if (!dfPlayerReady) return;

  myDFPlayer.stop();
  backgroundMusicOn = false;

  Serial.println("Background music stopped");
}

void playScoreSound() {
  if (!dfPlayerReady) return;

  myDFPlayer.play(scoreSoundTrack);
  backgroundMusicOn = false;

  Serial.println("Score sound played");
}

void playLoseLifeSound() {
  if (!dfPlayerReady) return;

  stopBackgroundMusic();

  myDFPlayer.play(losingSoundTrack);

  waitingToResumeBackground = true;
  resumeBackgroundTime = millis() + loseSoundDuration;

  Serial.println("Lose life sound played");
}

void playPiezoSound() {
  if (!dfPlayerReady) return;

  myDFPlayer.play(scoreSoundTrack);
  backgroundMusicOn = false;

  Serial.println("Piezo score sound played");
}

void updateSound() {
  if (!dfPlayerReady) return;

  if (waitingToResumeBackground && millis() >= resumeBackgroundTime) {
    waitingToResumeBackground = false;
    startBackgroundMusic();
  }
}