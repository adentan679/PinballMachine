# Pinball Machine Setup

## Install and Upload

1. Download the repository and open [`code/Pinball_Main/Pinball_Main.ino`](../code/Pinball_Main/Pinball_Main.ino) in Arduino IDE. Keep all eight `.ino` files together in that folder.
2. Install **Arduino AVR Boards**, then select **Arduino Mega or Mega 2560** and the correct USB port.
3. Install **Servo** and **DFRobotDFPlayerMini** through Library Manager.
4. Disconnect actuator power, click **Verify**, then **Upload** after compilation succeeds.
5. Open Serial Monitor at **9600 baud** and check the startup messages.

**Validation status:** The revised timing logic passed a host-side simulation according to the firmware notes. Mega compilation and physical testing of this revision remain pending.

## Firmware Pin Assignments

| Component | Mega pin(s) |
|---|---|
| Start button | D4 |
| Flipper buttons | D2, D3 |
| Solenoid MOSFET gate controls | D12, D13 |
| Gear motor driver controls | D9, D10, D11 |
| Servo signal | D44 |
| Score / loss IR sensors | D7 / D8 |
| Piezo sensors | A6, A8, A9 |
| Score shift register: SER / RCLK / SRCLK | D26 / D28 / D30 |
| Score digit controls | D32, D34 |
| Lives shift register: SER / RCLK / SRCLK | D50 / D48 / D53 |
| Lives digit control | D52 |
| DFPlayer: TX1 / RX1 | D18 / D19 |

D2, D3, and D4 use `INPUT_PULLUP`: connect each button between its input and GND. Button inputs are separate from MOSFET outputs. Check older drawings against these assignments.

## Power and Audio

Use external supplies and driver circuits for motors and solenoids, with a common signal ground. Check supply ratings, polarity, and flyback protection before powering the actuators. Connect the speaker to the DFPlayer, not an Arduino pin.

Prepare a FAT16/FAT32 microSD card up to 32 GB with the files from [`media/`](../media/). Insert it with module power off. Wire D18 to DFPlayer RX through the recommended 1 kΩ resistor, and DFPlayer TX to D19; follow the [DFRobot wiring guide](https://wiki.dfrobot.com/DFPlayer_Mini_SKU_DFR0299).

| Track index | Intended file | Function |
|---|---|---|
| 1 | `0001.mp3` | Life lost |
| 2 | `0002.mp3` | Scoring |
| 3 | `0003.mp3` | Background music |

Verify actual playback order: filenames alone do not guarantee the indexes used by the code.

## Initial Checks

Check inputs first, then test one actuator subsystem at a time. Confirm three starting lives, the gate sequence, scoring, life loss, and flipper release. Verify that the servo's **155° closed / 60° open** settings fit the mechanism. Use brief solenoid tests; the firmware has no maximum on-time cutoff.

See [Firmware Notes](firmware_notes.md) for detailed behavior and [Troubleshooting](troubleshooting.md) for common issues.
