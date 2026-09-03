# Arduino Pinball Machine

A custom electromechanical pinball machine built using an Arduino Mega, MOSFET driver circuits, solenoids, IR sensors, piezo sensors, DC gear motors, servo-controlled ball launch gate, dual 7-segment displays, and DFPlayer Mini audio feedback.

I served as the **Hardware Lead** for this project, focusing on circuit integration, actuator control, sensor bring-up, power distribution, and debugging of the full electromechanical system.

---

## Project Overview

This project was developed as a final project for ECE 115. The goal was to design and build a functional pinball machine with interactive scoring, lives tracking, ball launch control, sound effects, and motorized gameplay elements.

The final system uses a finite state machine to control the game flow:

1. Ready / idle state
2. Ball launch
3. In-play mode
4. Score event
5. Ball lost
6. Next round
7. Game over

The player starts with 3 lives. When the ball enters the losing zone, one life is removed and the player must press the start button again to launch the next round.

---

## Key Features

- Arduino Mega-based game controller
- MOSFET driver circuits for solenoids and DC gear motors
- Solenoid flippers controlled using interrupt pins
- Servo-controlled launch gate
- Three DC gear motors active during gameplay
- IR break-beam sensor for scoring
- IR sensor for ball-loss detection
- Piezo sensor for hit detection and motor speed changes
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
| Piezo sensors | Hit detection |
| SN74HC595 shift registers | 7-segment display control |
| 7-segment displays | Score and lives display |
| DFPlayer Mini | Audio playback |
| 8 ohm speaker | Sound output |
| External power supplies | Motor, solenoid, and servo power |

---

## System Architecture

The Arduino Mega reads sensor inputs and controls outputs based on the current game state.

```text
Start Button ───────┐
Score IR Sensor ────┤
Loss IR Sensor ─────┤
Piezo Sensor ───────┤
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