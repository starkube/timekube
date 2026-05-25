TimeKube Project
 - Video history of project: https://youtu.be/rv1fuwQbApo

<BR>
 Build BOM

 3D Print Files
 https:cad.onshape.com/documents/f47f5ee7ce2854b4617142ce/w/78f16faa0b1682fba33c5218/e/67465c017aa9228f80cce16c


 Curcuit diagram: https:app.cirkitdesigner.com/project/91a7f5a8-7523-460a-bbd8-373d6684543a
 https:app.cirkitdesigner.com/project/91a7f5a8-7523-460a-bbd8-373d6684543a
 Google sheet: https:docs.google.com/spreadsheets/d/1GN6ld3xGk032MxlrJK5zELR0rM6Zr99TY1Dd1nu3Fkk/edit?usp=sharing


 Arduino Due (chosen due to multiple hardware serial ports, fast processor)
      https:store-usa.arduino.cc/products/arduino-due  $48.40
 RTC 3231 (more accurate, make sure doens't have the L on the chip,  RTC DS1307 is older less accurate)
      +3.3 You can get these a lot cheaper, but had the lower quality chip when purchased from amazon, choosling the right
      Chosing the right RTC: https:hackaday.com/2020/09/30/choosing-the-right-rtc-for-your-project/
      https:www.adafruit.com/product/3013   $17.55
      https:www.amazon.com/HiLetgo-AT24C32-Arduino-Without-Battery/dp/B00LX3V7F0  $13 for 5 = ~$3
 WWVb Receiver - EverSet ES100 WWVB BPSK Atomic Clock Starter Kit
     +3.3  https:www.universal-solder.ca/product/everset-es100-wwvb-bpsk-atomic-clock-starter-kit/  CAD 32.99
 GPS Receiver
      HiLetgo GY-NEO6MV2 NEO-6M  https:www.amazon.com/dp/B01D1D0F5M  $8.99
        Could also use: Adafruit Ultimate GPS Breakout - 66 channel w/10 Hz updates (PA1616S) $29.95 - Haven't tried
 LCD Display - 2004 LCD Display Module with IIC I2C Serial Interface Adapter
    Want red, but struggeling to get it bright enough, might stick with blue
    https:www.amazon.com/dp/B0D2L9VGQ9
    https:www.digikey.com/en/products/detail/sunfounder/CN0296D/18668625
    LiquidCrystal_I2C lcd(0X3F, 20, 4);
    LCD brightness control: https:www.reddit.com/r/arduino/comments/17o387b/any_way_to_pwm_control_the_backlight_of_this_lcd/#:~:text=You'll%20need%20an%20NPN,LOW%20turns%20on%20the%20backlight!
 Speaker - 2" 4Ohm, 3 W
    https:www.amazon.com/dp/B0DP9DVCD9  $2
 MP3 Player - DFROBOT DFPLAYER PRO
    https:wiki.dfrobot.com/DFPlayer_PRO_SKU_DFR0768
    Digikey  - https:www.digikey.com/en/products/detail/dfrobot/DFR0768/13978502  $8.90
 Power Supply
      https:www.amazon.com/dp/B0BP9V6WXX $9.99
 Encoder w/push button (for menus) - Any will do, cheapest on Amazon
    https:www.amazon.com/dp/B0C6Q67V97  $1
 Audio Taper POT - Volume Control - 1kohm
 Capacitor 147uf - across MP3 player power inputs to remove "pops"
 Enclosure
 Speaker Grill
 Standoffs
 Bus boards (for making common power bus, I2C bus, etc.)
 Mounting boards
  Adafruit Swirly Aluminum Mounting Grid for 0.1" Spaced PCBs (5x5) [ID:5779] = $7.90
  Adafruit Perma-Proto Quarter-sized Breadboard PCB - Single[ID:1608] = $8.85

  Wire
    Red       5V+
    Orange    3.3V+
    Black     Ground
    Green     I2C Serial SCL
    Blue      I2C Serial SDA
              TX Arduino /RX Device Serial
              RX Arduino /TX Device Serial
    White
              Speaker +
              Log Volume POT
    Black     Speaker -





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
