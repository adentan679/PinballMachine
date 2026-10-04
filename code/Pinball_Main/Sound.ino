bool dfPlayerReady = false;

const int losingSoundTrack = 1;      // 0001.mp3
const int scoreSoundTrack = 2;       // 0002.mp3
const int backgroundMusicTrack = 3;  // Background music

bool backgroundMusicOn = false;

bool loseLifeSoundPlaying = false;
bool loseLifeSoundComplete = false;


// --------------------------------------------------
// SETUP
// --------------------------------------------------

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


// --------------------------------------------------
// BACKGROUND MUSIC
// --------------------------------------------------

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


// --------------------------------------------------
// SCORE SOUND
// --------------------------------------------------

void playScoreSound() {
  if (!dfPlayerReady) return;
  myDFPlayer.play(scoreSoundTrack);
  backgroundMusicOn = false;
  Serial.println("Score sound played");
}


// --------------------------------------------------
// LIFE LOST SOUND
// --------------------------------------------------

void playLoseLifeSound() {

  // If the DFPlayer is unavailable, allow the game
  // to continue instead of getting stuck in BALL_LOST.
  if (!dfPlayerReady) {
    loseLifeSoundPlaying = false;
    loseLifeSoundComplete = true;
    return;
  }

  // Stop the background music before playing
  // the life-lost sound.
  stopBackgroundMusic();
  loseLifeSoundComplete = false;
  loseLifeSoundPlaying = true;
  myDFPlayer.play(losingSoundTrack);
  Serial.println("Lose life sound played");
}


// Used by Pinball_Main.ino to determine when
// BALL_LOST can transition to the next state.
bool isLoseLifeSoundComplete() {
  return !dfPlayerReady || loseLifeSoundComplete;
}


// --------------------------------------------------
// PIEZO SCORE SOUND
// --------------------------------------------------

void playPiezoSound() {
  if (!dfPlayerReady) return;
  myDFPlayer.play(scoreSoundTrack);
  backgroundMusicOn = false;
  Serial.println("Piezo score sound played");
}


// --------------------------------------------------
// SOUND UPDATE
// --------------------------------------------------

void updateSound() {
  if (!dfPlayerReady) return;
  // Check whether the DFPlayer has sent an event.
  if (myDFPlayer.available()) {
    uint8_t eventType = myDFPlayer.readType();
    int eventValue = myDFPlayer.read();
    // DFPlayerPlayFinished means a track reached
    // the actual end of the audio file.
    if (eventType == DFPlayerPlayFinished &&
        loseLifeSoundPlaying &&
        eventValue == losingSoundTrack) {

      loseLifeSoundPlaying = false;
      loseLifeSoundComplete = true;
      Serial.println("Lose life sound finished");
      // Immediately resume the background music.
      startBackgroundMusic();
      Serial.println("Background music resumed");
      
    }
  }
}