<BR>
<img width="715" height="418" alt="image" src="https://github.com/user-attachments/assets/724d5121-b44d-4c10-b50e-d3957e572378" />

# TimeKube Project
 - Video history of project: https://youtu.be/rv1fuwQbApo

# Project TimeKube
### Arduino WWVB/GPS Clock with WWV/WWVH Audio

An Arduino-based precision timekeeping device that synchronizes to atomic time via WWVB radio and GPS, displays multiple time zones on a 20×4 LCD, plays WWV/WWVH audio time announcements, and tracks personal calendar events — all in a 3D-printed enclosure inspired by the classic Radio Shack TimeKube of the 1970/80s.

---

## Features

- **WWVB atomic time sync** via EverSet ES100 receiver (60 kHz BPSK digital)
- **GPS time sync** with position, velocity, and distance-to-Ft. Collins display
- **RTC backup** (DS3231) for timekeeping when radio sync is unavailable
- **WWV and WWVH audio playback** — on-demand time-of-day announcements from stored MP3 files
- **Multi-timezone display** — UTC and configurable local time
- **Calendar events** — upcoming birthdays, anniversaries, and holidays shown on the display
- **GPS clock comparison** — side-by-side RTC vs. GPS time display for calibration
- **Rotary encoder menu navigation** — scroll through all pages and settings
- **3D-printed enclosure** — cube form factor with integrated speaker grill and controls

---

## Menu Pages

| Page | Description |
|---|---|
| Local Time | Timezone-adjusted display |
| UTC Time | Primary time display with date and sync source |
| Last Sync | Date/time of last WWVB or GPS sync |
| Clock Compare | Side-by-side GPS vs. RTC comparison (UTC) |
| GPS Coords | Latitude, longitude, altitude (DD/DMS) & (DecDeg) |
| Dist/TOF | Distance and time-of-flight to Ft. Collins, CO |
| Next DST | Upcoming DST change date |
| Events | Upcoming birthdays and anniversaries |
| WWV Ft. Collins | Audio time announcement — press to play ident |
| WWVH Kauai | Audio time announcement — press to play ident |

<img width="1086" height="604" alt="image" src="https://github.com/user-attachments/assets/7fcbc1f5-1380-4b70-bdc4-f22a5d11a0ea" />
<img width="1033" height="539" alt="image" src="https://github.com/user-attachments/assets/5b1c0f73-1ad3-4964-b0f5-fda43e100e84" />
<img width="1029" height="542" alt="image" src="https://github.com/user-attachments/assets/28c6d892-ca60-48dc-938e-6c54d806c259" />
<img width="991" height="571" alt="image" src="https://github.com/user-attachments/assets/ae3f3c39-a5c2-4c14-8054-6e6a3a93ac25" />




---

## Hardware

### Core Components

| Component | Notes | Link |
|---|---|---|
| Arduino Due | Multiple hardware serial ports, fast processor (Mega also works) | [Arduino Store — $48.40](https://store-usa.arduino.cc/products/arduino-due) |
| RTC DS3231 w/CR2032 | More accurate than DS1307; avoid chips with "L" suffix; runs at 3.3V | [Adafruit — $17.55](https://www.adafruit.com/product/3013) · [Amazon 5-pack ~$13](https://www.amazon.com/HiLetgo-AT24C32-Arduino-Without-Battery/dp/B00LX3V7F0) |
| EverSet ES100 WWVB Receiver | BPSK digital 60 kHz receiver; runs at 3.3V | [Universal Solder — CAD $32.99](https://www.universal-solder.ca/product/everset-es100-wwvb-bpsk-atomic-clock-starter-kit/) |
| HiLetgo GY-NEO6MV2 NEO-6M GPS | Runs at 3.3V | [Amazon — $8.99](https://www.amazon.com/dp/B01D1D0F5M) |
| 2004 LCD with I2C adapter | 20×4 display; address `0x3F`; 5V | [Amazon](https://www.amazon.com/dp/B0D2L9VGQ9) · [DigiKey](https://www.digikey.com/en/products/detail/sunfounder/CN0296D/18668625) |
| DFRobot DFPlayer Pro | Flash RAM; supports individual file/folder addressing (8.3 names required) | [Wiki](https://wiki.dfrobot.com/DFPlayer_PRO_SKU_DFR0768) · [DigiKey — $8.90](https://www.digikey.com/en/products/detail/dfrobot/DFR0768/13978502) |
| 2" 4Ω 3W Speaker | | [Amazon — $2](https://www.amazon.com/dp/B0DP9DVCD9) |
| Power Supply | | [Amazon — $9.99](https://www.amazon.com/dp/B0BP9V6WXX) |
| Rotary Encoder w/push button | Any quadrature type; set encoding correctly | [Amazon — $1](https://www.amazon.com/dp/B0C6Q67V97) |

### Passive Components

| Component | Value | Purpose |
|---|---|---|
| Capacitor | 470µF | Across MP3 player Vin/GND — eliminates audio pops |
| Resistors | 1kΩ (×2) | TX/RX serial lines to DFPlayer |
| Potentiometer (audio taper) | 10kΩ | Volume control (analog read → digital vol output) |
| I2C pull-up resistors | 2.2kΩ | Required on SDA/SCL — LCD behaves badly without them |

> **Choosing the right RTC:** [Hackaday guide](https://hackaday.com/2020/09/30/choosing-the-right-rtc-for-your-project/)

---

## Wiring Color Convention

| Color | Signal |
|---|---|
| Red | 5V+ |
| Orange | 3.3V+ |
| Black | Ground |
| Green | I2C SCL |
| Blue | I2C SDA |
| Yellow | TX (Arduino) → RX (Device) |
| Purple | RX (Arduino) ← TX (Device) |
| White | Speaker + / Volume pot |
| Black | Speaker − |

---

## Design Files

| Resource | Link |
|---|---|
| 3D Print Files (STL) | [OnShape CAD](https://cad.onshape.com/documents/f47f5ee7ce2854b4617142ce/w/78f16faa0b1682fba33c5218/e/67465c017aa9228f80cce16c) |


---

## Repository Contents

```
/
├── timekube_YYYY_MM_DD.ino     Main Arduino sketch
├── events.h                    Calendar events table (birthdays, anniversaries, holidays)
├── /audio                      WWV/WWVH MP3 source files
│   └── ...                     Organized in 8.3 folder/file format for DFPlayer Pro
├── /stl                        3D print enclosure files
├── /schematic                  Circuit diagram exports
└── /utilities                  Component test scripts, MP3 player setup tools
```

---

## Software Notes

### Calendar Events (`events.h`)

Events are defined in a single struct array — add, remove, or reorder with one line per event:

```cpp
const Event events[] = {
  { december, 25, "Christmas!"       },
  { january,   1, "New Years!"       },
  { july,      4, "JJ's Birthday"    },
  // ...
};
const int eventnumber = sizeof(events) / sizeof(events[0]);
```

No strings longer than 30 characters. `eventnumber` is computed automatically.

### MP3 Player File Structure

The DFPlayer Pro requires **8.3 format for all folder and file names**, including subdirectory names. Files are addressed individually by path, enabling on-demand playback of specific hours.

WWV/WWVH audio source files: [wwv.mcodes.org](https://wwv.mcodes.org/)

### WWVB Reception

The ES100 cannot decode the special extended time frame broadcast for 6 minutes each half-hour (minutes 10–16 and 40–46 past each hour). Reception attempts are skipped during these windows automatically.

**Important:** The WWVB antenna is very sensitive to electrical noise. In this build the antenna had to be mounted externally to get reliable reception. Keep it away from switching power supplies, LCD backlights, and other noise sources.

More on ES100 + Arduino: [Keith Greiner's site](https://sites.google.com/site/wwvbreceiverwitharduino/)  
WWVB protocol reference: [Wikipedia](https://en.wikipedia.org/wiki/WWVB)

---

## Build Notes & Lessons Learned

**Volume control** — Volume is read via analog pot on the Due and sent as a digital value to the DFPlayer. An L-pad attenuator would have been a better design: it handles attenuation in true analog, keeps speaker impedance constant, and removes the Due from the audio signal path entirely.

**LCD brightness** — The LCD backlight is 5V; PWM output from the Due is 3.3V max, which reduces maximum brightness. A logic-level shifter and a two-transistor adapter were both attempted without success. Abandoned for now — current brightness is acceptable.

**LCD I2C** — The 5V I2C LCD *should* use a logic-level shifter on SDA/SCL. Attempted, but caused problems. Operates without one for now — the reason it works is not fully understood.

**I2C pull-ups** — 2.2kΩ pull-up resistors on SDA/SCL are required. Without them the LCD behaves erratically.

**Power** — The TimeKube draws ~300 mA. The power supply includes a 9V battery snap connector, but a standard alkaline 9V will not last long — use a **lithium 9V battery**.

**Debugging** — Many timing-dependent functions are difficult to observe in real time. An inexpensive Wyze cam pointed at the display was invaluable for unattended verification — battery life, sync events, and scheduled audio playback could all be confirmed by reviewing the recording afterward.

---

## Additional Audio Sources

- Steve Johnston
- Myke Dodge Weiskopf

---

## Acknowledgments

Inspired by the cube-form weather radios of the 1970s (Realistic Weatheradio and similar).  
ES100 integration guidance: [Keith Greiner](https://sites.google.com/site/wwvbreceiverwitharduino/)




<BR>
<img width="838" height="457" alt="image" src="https://github.com/user-attachments/assets/a47f6c65-3a25-4f25-a78a-7144d3985e76" />
<BR>
<img width="866" height="667" alt="image" src="https://github.com/user-attachments/assets/0ff9269a-36ec-46c7-a911-5ecafa88eee9" />
<BR>

<BR>
<img width="821" height="449" alt="image" src="https://github.com/user-attachments/assets/fc665fb1-74f8-4578-b44e-41cc72476d86" />
<BR>
