# Arduino Pinball Machine

A custom electromechanical pinball machine built using an Arduino Mega, MOSFET driver circuits, solenoids, IR sensors, piezo sensors, DC gear motors, servo-controlled ball launch gate, dual 7-segment displays, and DFPlayer Mini audio feedback.

I served as the **Hardware Lead** for this project, focusing on circuit integration, actuator control, sensor bring-up, power distribution, and debugging of the full electromechanical system.

---

## Project Overview

This project was developed as a final project for ECE 115. The goal was to design and build a functional pinball machine with interactive scoring, lives tracking, ball launch control, sound effects, and motorized gameplay elements.

The final system uses a finite state machine to control the game flow:

1. `RESET` — reset score, lives, and motor boost
2. `READY_IDLE` — wait for the start button
3. `LAUNCH_BALL` — run the servo gate sequence
4. `IN_PLAY` — run motors and process scoring and loss sensors
5. `BALL_LOST` — stop motors, remove one life, and play the losing sound
6. `NEXT_ROUND` — wait for the start button if lives remain
7. `GAME_OVER` — wait for the start button to reset the game

The player starts with 3 lives. When the ball enters the losing zone, one life is removed. If lives remain, the player must press the start button again to launch the next round. At zero lives, pressing start resets the game; another press launches the first ball. Scoring is handled within `IN_PLAY`, and the score is capped at 99.

---

## Key Features

- Arduino Mega-based game controller
- MOSFET driver circuits for solenoids and DC gear motors
- Solenoid flippers controlled using interrupt pins
- Servo-controlled launch gate
- Three DC gear motors active during gameplay
- IR break-beam sensor for scoring
- IR sensor for ball-loss detection
- Three piezo sensor inputs: A6 for scoring and motor speed changes; A8 and A9 for scoring
- Dual 7-segment displays for score and remaining lives
- DFPlayer Mini audio system for score, life-loss, and background music effects
- Finite state machine for reliable game-state control

---

## My Role: Hardware Lead

As Hardware Lead, I worked on the integration and debugging of the main electromechanical subsystems, including:

- Designed and tested MOSFET driver circuits for solenoids and DC gear motors
- Added flyback diode protection for inductive loads
- Integrated IR sensors for scoring and life-loss detection
- Integrated piezo sensors for collision/hit detection
- Wired and tested dual SN74HC595-based 7-segment displays
- Integrated DFPlayer Mini audio playback to replace direct speaker driving from Arduino pins
- Helped debug power, grounding, overheating, and signal-noise issues across the full system
- Assisted with organizing the Arduino code into modular tabs for motors, sensors, displays, solenoids, sound, and game logic

---

## Hardware Used

| Component | Purpose |
|---|---|
| Arduino Mega | Main game controller |
| N-channel MOSFETs | Switching solenoids and DC motors |
| Flyback diodes | Protection from inductive voltage spikes |
| Pull-type solenoids | Flipper / actuator control |
| DC gear motors | Motorized gameplay elements |
| Servo motor | Ball launch gate |
| IR break-beam sensors | Scoring and ball-loss detection |
| Three piezo sensors | A6: scoring and motor speed toggle; A8/A9: scoring |
| SN74HC595 shift registers | 7-segment display control |
| 7-segment displays | Score and lives display |
| DFPlayer Mini | Audio playback |
| 8 ohm speaker | Sound output |
| External power supplies | Motor, solenoid, and servo power |

---

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

---

## System Architecture

The Arduino Mega reads sensor inputs and controls outputs based on the current game state.

```text
Start Button ───────┐
Score IR Sensor ────┤
Loss IR Sensor ─────┤
Piezo Sensors ──────┤
Solenoid Buttons ───┤
                    ↓
              Arduino Mega
                    ↓
 ┌─────────────┬─────────────┬──────────────┬──────────────┐
 │ Solenoids   │ Gear Motors │ Servo Gate   │ DFPlayer Mini│
 │ Flippers    │ Gameplay    │ Ball Launch  │ Audio        │
 └─────────────┴─────────────┴──────────────┴──────────────┘
                    ↓
        Score Display + Lives Display
```
