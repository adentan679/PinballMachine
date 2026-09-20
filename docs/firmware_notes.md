## Current Firmware Behavior

The following settings describe the Arduino code included in this repository.

### Servo Gate and Launch Sequence

- Servo signal: **D44**; start button: **D4**.

- Rest / closed angle: **155°** (`servoClosedAngle`).

- Open angle: **60°** (`servoOpenAngle`).

- On launch, the servo steps through **155°, 150°, 145°, 140°, 135°, 130°, 125°, 120°, 115°, 110°, 105°, 100°, 95°, and 92°**, pausing **500 ms** at each position.

- It then moves to **60°** for **1 second**, returns to **155°**, waits **500 ms**, and detaches to reduce buzzing.

- The sequence contains approximately **8.5 seconds of blocking delays**. The game enters `IN_PLAY` and starts the gear motors after the sequence finishes. During these delays, the main loop does not refresh the score display, poll sensors, or update solenoid outputs; button interrupts only update their stored flags.

### Gear Motors

| Motor | Pin | PWM during `IN_PLAY` |

|---|---|---|

| Gear motor 1 | D9 | **120** normal; **255** boosted |

| Gear motor 2 | D10 | **120** |

| Gear motor 3 | D11 | **120** |

Each accepted A6 piezo hit toggles motor 1 between normal and boosted speed. Motors 2 and 3 remain at normal speed. Motors stop between rounds and at game over. The boost setting carries over between rounds and resets to normal when the game resets.

### Scoring and Hit Sensors

| Input | Pin | Detection in the current code | Action during `IN_PLAY` |

|---|---|---|---|

| Score IR sensor | D7 | LOW-to-HIGH transition; 300 ms cooldown | Add 1 point and play the score sound |

| Loss IR sensor | D8 | HIGH-to-LOW transition; 1,000 ms cooldown | Enter `BALL_LOST` |

| Main piezo sensor | A6 | Analog reading above 150; 500 ms cooldown | Add 1 point, play the score sound, and toggle motor 1 speed |

| Additional piezo sensor | A8 | Analog reading above 190; 500 ms cooldown | Add 1 point and play the score sound |

| Additional piezo sensor | A9 | Analog reading above 190; 500 ms cooldown | Add 1 point and play the score sound |

Each piezo input has its own cooldown. Detection uses a threshold and cooldown rather than a rising-edge check, so a sustained reading above the threshold can trigger again after the cooldown expires. All scoring is capped at **99**.

### DFPlayer Mini Audio

The DFPlayer Mini communicates over **Serial1 at 9600 baud**, using Mega **D18 (TX1)** and **D19 (RX1)**. The firmware sets the volume to **28** and continues without audio if initialization fails.

| Track number used by the code | Included audio file | Purpose |

|---|---|---|

| 1 | `media/0001.mp3` | Losing a life |

| 2 | `media/0002.mp3` | IR scoring and all piezo scoring events |

| 3 | `media/0003.mp3` | Background music |

Background music starts after reset and is stopped when a launch is requested. The firmware uses `play(track)` for effects and `loop(3)` for background music. The comment mentioning `0004.mp3` in `Sound.ino` is outdated; the actual background track constant is **3**.

**Current losing-sound timing:** `BALL_LOST` starts track 1, waits **800 ms**, and then requests background music before entering `NEXT_ROUND` or `GAME_OVER`. Separately, `Sound.ino` schedules another background-music request **3 seconds** after the losing sound starts. Consequently, the losing sound can be interrupted after about 800 ms; the code does not guarantee three seconds of uninterrupted playback. The pending three-second request is not canceled when a new round starts, so it can also restart background music during a subsequent launch or gameplay.