#TimeKube Project
 - Video history of project: https://youtu.be/rv1fuwQbApo

# Bill of Materials

## Design Files
| Resource | Link |
|---|---|
| 3D Print Files | [OnShape CAD](https://cad.onshape.com/documents/f47f5ee7ce2854b4617142ce/w/78f16faa0b1682fba33c5218/e/67465c017aa9228f80cce16c) |
| Circuit Diagram | [Cirkit Designer](https://app.cirkitdesigner.com/project/91a7f5a8-7523-460a-bbd8-373d6684543a) |
| BOM Spreadsheet | [Google Sheet](https://docs.google.com/spreadsheets/d/1GN6ld3xGk032MxlrJK5zELR0rM6Zr99TY1Dd1nu3Fkk/edit?usp=sharing) |

---

## Components

### Microcontroller
**Arduino Due** — $48.40
- Chosen for multiple hardware serial ports and fast processor
- [Store link](https://store-usa.arduino.cc/products/arduino-due)

---

### RTC Module
**DS3231** — ~$3–$18
- More accurate than the DS1307; make sure the chip does **not** have an "L" suffix
- Runs at 3.3V
- [Choosing the right RTC (Hackaday)](https://hackaday.com/2020/09/30/choosing-the-right-rtc-for-your-project/)
- [Adafruit — $17.55](https://www.adafruit.com/product/3013)
- [Amazon (5-pack ~$13)](https://www.amazon.com/HiLetgo-AT24C32-Arduino-Without-Battery/dp/B00LX3V7F0) — note: lower-quality chips have been reported from this seller

---

### WWVB Receiver
**EverSet ES100 WWVB BPSK Atomic Clock Starter Kit** — CAD $32.99
- Runs at 3.3V
- [Universal Solder](https://www.universal-solder.ca/product/everset-es100-wwvb-bpsk-atomic-clock-starter-kit/)

---

### GPS Receiver
**HiLetgo GY-NEO6MV2 NEO-6M** — $8.99
- [Amazon](https://www.amazon.com/dp/B01D1D0F5M)
- Alternative (untested): [Adafruit Ultimate GPS Breakout PA1616S, 66-channel, 10 Hz — $29.95](https://www.adafruit.com/product/746)

---

### LCD Display
**2004 LCD Display Module with I2C Interface** — ~$10
- Want red backlight, but brightness has been difficult to achieve; blue currently in use
- I2C address: `0x3F`, configured as `LiquidCrystal_I2C lcd(0x3F, 20, 4)`
- [Amazon](https://www.amazon.com/dp/B0D2L9VGQ9)
- [DigiKey (SunFounder)](https://www.digikey.com/en/products/detail/sunfounder/CN0296D/18668625)
- [PWM backlight brightness control (Reddit)](https://www.reddit.com/r/arduino/comments/17o387b/any_way_to_pwm_control_the_backlight_of_this_lcd/)

---

### Speaker
**2" 4Ω 3W Speaker** — $2
- [Amazon](https://www.amazon.com/dp/B0DP9DVCD9)

---

### MP3 Player
**DFRobot DFPlayer Pro (DFR0768)** — $8.90
- [Wiki / datasheet](https://wiki.dfrobot.com/DFPlayer_PRO_SKU_DFR0768)
- [DigiKey](https://www.digikey.com/en/products/detail/dfrobot/DFR0768/13978502)

---

### Power Supply — $9.99
- [Amazon](https://www.amazon.com/dp/B0BP9V6WXX)

---

### Rotary Encoder with Push Button
Any will do — $1
- [Amazon](https://www.amazon.com/dp/B0C6Q67V97)

---

### Miscellaneous
| Item | Notes |
|---|---|
| Audio taper potentiometer, 1kΩ | Volume control |
| Capacitor, 147µF | Across MP3 player power inputs to remove audio pops |
| Enclosure | — |
| Speaker grill | — |
| Standoffs | — |
| Bus boards | Common power bus, I2C bus, etc. |
| [Adafruit Swirly Aluminum Mounting Grid 5×5 \[ID:5779\]](https://www.adafruit.com/product/5779) | $7.90 |
| [Adafruit Perma-Proto Quarter-sized Breadboard PCB \[ID:1608\]](https://www.adafruit.com/product/1608) | $8.85 |

---

## Wiring Color Convention

| Color | Signal |
|---|---|
| Red | 5V+ |
| Orange | 3.3V+ |
| Black | Ground |
| Green | I2C SCL |
| Blue | I2C SDA |
| Yellow | TX (Arduino) / RX (Device) — Serial |
| Purple | RX (Arduino) / TX (Device) — Serial |
| White | Speaker + / Log volume pot |
| Black | Speaker − |




<BR>
<img width="715" height="418" alt="image" src="https://github.com/user-attachments/assets/724d5121-b44d-4c10-b50e-d3957e572378" />
<BR>
<img width="838" height="457" alt="image" src="https://github.com/user-attachments/assets/a47f6c65-3a25-4f25-a78a-7144d3985e76" />
<BR>
<img width="866" height="667" alt="image" src="https://github.com/user-attachments/assets/0ff9269a-36ec-46c7-a911-5ecafa88eee9" />
<BR>

<BR>
<img width="821" height="449" alt="image" src="https://github.com/user-attachments/assets/fc665fb1-74f8-4578-b44e-41cc72476d86" />
<BR>
