# Pinball Machine Troubleshooting

## Build Experience

- **Power brownouts:** Separating the regulated 5 V electronics supply from the 12 V motor/solenoid supply resolved the reported resets during actuator operation.
- **Speaker-related overheating:** An early direct-speaker connection was associated with MCU overheating. The board was replaced, and audio was moved to a DFPlayer Mini.
- **Blocking timing:** The later firmware revision uses timed launch/life-loss sequences and direct solenoid-release handling. The revised firmware was successfully compiled, uploaded to the Arduino Mega, and physically tested on the completed machine. The launch sequence, life-loss transitions, and solenoid-release behavior operated correctly during testing.

## Common Checks

These are suggested checks, not additional recorded test results.

| Problem | What to check |
|---|---|
| Compilation or upload fails | Select Mega 2560 and the correct port, install Servo and DFRobotDFPlayerMini, and keep all eight sketch tabs together. |
| Arduino resets under load | Check supply voltage during switching, current capacity, grounding, and flyback protection. |
| Flipper does not respond | Buttons use D2/D3 to GND; MOSFET gate outputs use D12/D13. Check inputs and control signals before powering the coils. |
| Solenoid stays on or overheats | Disconnect actuator power. Check button wiring and the driver circuit. The code has no maximum on-time cutoff. |
| Servo stalls or moves incorrectly | Check D44, supply capacity, grounding, clearance, and the 155° closed / 60° open settings. |
| Motors remain off | They run only in `IN_PLAY`, after launch finishes. Check D9–D11 and the driver supply. |
| Start button does not respond | D4 must connect to GND when pressed. Release between presses. |
| IR detection fails | D7 scores on LOW → HIGH; D8 detects loss on HIGH → LOW. Check alignment and signal polarity. |
| Piezo scoring is unreliable | Check A6/A8/A9 wiring and measured ADC levels. Do not leave active inputs floating. Only A6 toggles motor speed. |
| Display is incorrect | Check shift-register wiring, digit controls, and segment mapping against the setup guide. |
| DFPlayer is silent or undetected | Check power, common ground, D18/D19 TX/RX wiring, speaker, and microSD card. Verify track order if the wrong sound plays. |

## Known Audio Issue

The game requests background music **800 ms** after life loss, while `Sound.ino` separately schedules it after **3 seconds**. This can interrupt the loss sound or restart music during the next round. The conflict remains unresolved and requires a firmware change.

See [Setup](setup.md) for connections and [Firmware Notes](firmware_notes.md) for thresholds, timing, and validation details.
