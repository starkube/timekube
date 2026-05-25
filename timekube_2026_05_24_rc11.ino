// Patrick Van Vickle
// Code for timekube

// 2025.02.06 - Got the encoder added, can read the encoder values on the screen
// 2025.02.07 - Added the encoder menu code, also added
// 2025.02.08 - Added DST transition info to WWVb info screen
// 2025.02.15 - Adding GPS information
// 2025.02.16 - Switched to Red Display - didn't work for some reason
// 2025.02.17 - Added GPS Lat/Long/Alt screen, GPS time screeen, general page cleanup
// 2025.02.19 - Completed timezone and DST shift from UTC based on RTC rather than GPS, fixed the GPS coords with decimal
// 2025.02.20 - Created distance from Ft. Colins, Great Circle Distance, and time of flight screen, also corrected local time date display
// 2025.02.23 - Fixed button click to set local time, extracted and printed local tz (EST vs EDT)
// 2025.02.24 - Updated time compare between GPS and RTC
// 2025.02.25 - Added notation for time sync type between GPS and WWVB to sync page, rearranged pages
// 2025.04.26 - Adding in audio board, using https://wwv.mcodes.org/ - https://www.youtube.com/results?search_query=wwv - https://github.com/kalafut/wwv
// 2025.04.27 - Debugging input and output, used https://audiomass.co/ to help debug some audio, won't play short tick sound
// 2025.04.28 - Used ChatGPT to write code to concat string of folder/<time>/.mp3
// 2025.04.29 - Struggeling with case vs. if statements for audio
// 2025.04.30 - If statements for voice mostly working, working out delays.  Added direct 3V to board from supply board, 5V direct to display
//            - Added encoder for volume
// 2025.06.02 - Set volume to max and added 1kohm audio taper pot to control volume
// 2025.06.05 - Added delay check while loops to WWV audio loops, added encoder escape, refined playback, discovered issue with tick mp3 file.
//              Captured new file from: https://freesound.org/people/Mxsmanic/sounds/139323/
//              Edited to cut tick with: https://audiomass.co/
//              Edited to increase volume of mp3 to match: https://mp3cut.net/change-volume
// 2025.06.06 - Placed code for WWVH, updated links to correct audio files for WWVH
//              Saved as Release Candidate 0 - renamed to timekube
// 2025.06.07-09 First prototype unit soldered and assembled. Discovered that the LCD backpack will interfere with antenna placement, otherwise good.
// 2025.06.10 -`Discovered that I failed to solder the IRQ and EN from the ES100, also in doing research to diagnosse an issue discovered this fact
//              The analysis of response times excludes tests made between 10 to 16 minutes after the beginning of each hour, and 40 to 46 minutes after the beginning of each hour. Image 2 shows the hourly time blocks when reception of a time signal should be avoided.
// 2025.06.13 - Found an issue where the WWVb sync will not take, but not a hardware issue, some kind of antenna issue.
//            - Upgraded to new LCD library for auto find of the LCD HEX address #include <hd44780.h>
//            - Realized that the light goes off on the mp3 player sometimes, diagnosing
// 2025.06.15 - Discovered that having a serial output to the comm port right after the beep at 0 somehow solves the hang issues with the mp3 player.  Leaving for now, only downside is that it skips 01 sec on display.
// 2025.06.16 - Added timeout and default to GPS for fail read of WWVb
//            - Resynced WWVH to WWV logic
// 2025.06.17 - Added back in the mp3 pause on encoder change to escape
//            - Added code to allow ident to play all the way through without inturruption from regular time code
// 2025.08.01 - soldered second prototype with liams 5mm larger (each side) enclosure
// 2025.08.02 - addition of temp/humidity I2C has caused lots of problems, removing for now
// 2025.08.10 - Added pot for volume and read to input to DF volume
// 2025.08.12 - Added 2.2k pull up resistors to both SCL and SDA to try and fix comm issues, may need to decrease ohm further
// 2025.08.13 - Added PWM from pin 8 to top pin of LCD for brightness control
// 2025.08.15 - Added feature on last WWVB update screent to press to force update rather than waiting for nightly update
// 2025.08.19 - Updates to the WWVB forced sync, works on set number of attempts, based on variable "attempt"
// 2025.08.20 - Updated to rotate through events on dedicated screen, moved the WWV and WWVH audio to last two menu items
// 2025.08.22 - Updated text on main screen to prompt to switch to local time, changed screen saver to not rotate off UTC page 2 scrren as well as first screen
//            - Added brightness selection on Events page, checking for brightness from the pot during other screens messed with the timing of the processor and discplayed seconds wrong
// 2025.08.26 - Updated encoder routine away from mod function to more straightforward counter method.
// 2025.09.19 - Fixed the trailing "y" on the main display when switching from long day to short day
// 2025.09.22 - Added message on main screen showing event
// 2025.11.09 - Fixed event not clearing after date
// 2025.11.10 - Fixed not performing nightly WWVB update
// 2025.11.12 - Fixed display of event clearing after WWVB update and moved events to seperate include file called events.h
// 2025.11.13 - Added dimmer mode with custom moon character while in dim mode
//              Declaring stable!
// 2025.11.15 - Added check on boot for event rather than waiting for next nightly check
//              Fixed some screen flickering on too many screen clear events on 0 and 1 case screens
//              Commented all calls to seria.print
// 2025.11.21 - Splash screen work and aduio on startup https://audiomass.co/?local=5o1dp
// 2025.11.22 - Fixed no increment on count when in WWVB receive status
//              syncs made between 10 to 16 minutes after the beginning of each hour, and 40 to 46 minutes after the beginning of each hour. Image 2 shows the hourly time blocks when reception of a time signal should be avoided.
//              https://sites.google.com/site/wwvbreceiverwitharduino/
// 2025.11.22 -   eb1.useQuadPrecision(false); toggle to true for differnet encoder
// 2025.11.23 - Fixed issue on date change leaving training text on longer days
// 2025.12.02 - Fixed issue on volume control on splash, added "Karl Edition"
//            - Added dots for splash
//            - Added IRQ timer to check for no antenna connected, stop loop lock waiting for IRQ
//            - Added longpress and doublepress handlers for encoder
// 2025.12.03 - Upgraded IRQ timer
//            - Added toggle for DD and DMS Coords, and speed from GPS
// 2025.12.04 - Added temp toggle on UTC time screen to show RTC temp
// 2025.12.05 - Added new audio
// 2025.12.06 - More work on the timeout function for IRQ ES100
// 2025.12.10 - Discovered in SFO that decimal degrees method wasn't sufficient, 119 deg threw off placement and math, reworked
// 2025.05.23 - Added code to handle if failure to sync WWVB then sync auto to GPS.
// 2025.05.24 - Turned Claude loose



// Build BOM

// Curcuit diagram: https://app.cirkitdesigner.com/project/91a7f5a8-7523-460a-bbd8-373d6684543a
// https://app.cirkitdesigner.com/project/91a7f5a8-7523-460a-bbd8-373d6684543a
// Google sheet: https://docs.google.com/spreadsheets/d/1GN6ld3xGk032MxlrJK5zELR0rM6Zr99TY1Dd1nu3Fkk/edit?usp=sharing
//
//
// Arduino Due (chosen due to multiple hardware serial ports, fast processor)
//      https://store-usa.arduino.cc/products/arduino-due  $48.40
// RTC 3231 (more accurate, make sure doens't have the L on the chip,  RTC DS1307 is older less accurate)
//      +3.3 You can get these a lot cheaper, but had the lower quality chip when purchased from amazon, choosling the right
//      Chosing the right RTC: https://hackaday.com/2020/09/30/choosing-the-right-rtc-for-your-project/
//      https://www.adafruit.com/product/3013   $17.55
//      https://www.amazon.com/HiLetgo-AT24C32-Arduino-Without-Battery/dp/B00LX3V7F0  $13 for 5 = ~$3
// WWVb Receiver - EverSet ES100 WWVB BPSK Atomic Clock Starter Kit
//     +3.3  https://www.universal-solder.ca/product/everset-es100-wwvb-bpsk-atomic-clock-starter-kit/  CAD 32.99
// GPS Receiver
//      HiLetgo GY-NEO6MV2 NEO-6M  https://www.amazon.com/dp/B01D1D0F5M  $8.99
//        Could also use: Adafruit Ultimate GPS Breakout - 66 channel w/10 Hz updates (PA1616S) $29.95 - Haven't tried
// LCD Display - 2004 LCD Display Module with IIC I2C Serial Interface Adapter
//    Want red, but struggeling to get it bright enough, might stick with blue
//    https://www.amazon.com/dp/B0D2L9VGQ9
//    https://www.digikey.com/en/products/detail/sunfounder/CN0296D/18668625
//    LiquidCrystal_I2C lcd(0X3F, 20, 4);
//    LCD brightness control: https://www.reddit.com/r/arduino/comments/17o387b/any_way_to_pwm_control_the_backlight_of_this_lcd/#:~:text=You'll%20need%20an%20NPN,LOW%20turns%20on%20the%20backlight!
// Speaker - 2" 4Ohm, 3 W
//    https://www.amazon.com/dp/B0DP9DVCD9  $2
// MP3 Player - DFROBOT DFPLAYER PRO
//    https://wiki.dfrobot.com/DFPlayer_PRO_SKU_DFR0768
//    Digikey  - https://www.digikey.com/en/products/detail/dfrobot/DFR0768/13978502  $8.90
// Power Supply
//      https://www.amazon.com/dp/B0BP9V6WXX $9.99
// Encoder w/push button (for menus) - Any will do, cheapest on Amazon
//    https://www.amazon.com/dp/B0C6Q67V97  $1
// Audio Taper POT - Volume Control - 1kohm
// Capacitor 147uf - across MP3 player power inputs to remove "pops"
// Enclosure
// Speaker Grill
// Standoffs
// Bus boards (for making common power bus, I2C bus, etc.)
// Mounting boards
//  Adafruit Swirly Aluminum Mounting Grid for 0.1" Spaced PCBs (5x5) [ID:5779] = $7.90
//  Adafruit Perma-Proto Quarter-sized Breadboard PCB - Single[ID:1608] = $8.85

//  Wire
//    Red       5V+
//    Orange    3.3V+
//    Black     Ground
//    Green     I2C Serial SCL
//    Blue      I2C Serial SDA
//              TX Arduino /RX Device Serial
//              RX Arduino /TX Device Serial
//    White
//              Speaker +
//              Log Volume POT
//    Black     Speaker -
//
//

// What works is DF1201S.playSpecFile("/filename.mp3");, with conditions given below.

// -Files are relative to the root, e.g. "/yourfile.mp3" will attempt to play ROOT/yourfile.mp3.
// -Leading forward slash is required in string, but ignored when counting filename char length.
// -File naming must be in 8.3 format. e.g. "/test.mp3" is OK. "/testlong.mp3" is OK. "/testlonger.mp3" is not (testlonger is 10 chars).
// -Subfolders only work if the total length of the path (including slashes) is less than 8 chars e.g. "/test/test.mp3" WILL NOT WORK as "test/test" is 9 chars. "/test/tes.mp3" seems OK. More investigation required.
// If any of these conditions are not met, the chip always falls back to playing file 1 (the first file copied to the disk, regardless of location/naming).

// Audio Files (Other than main screen files and standard ident)


// wwvh_h.mp3:  WWVH history, Steve Johnston, AWA Communication Tech Museum (https://www.youtube.com/watch?v=sOmm4fG4DoI&t=163s)
// wwvh_ri.mp3: 1980s retro wwvh id (https://www.youtube.com/watch?v=T9ddpYcxvsw) - crooner2007
// wwv_ri.mp3: 1980s retro wwh id (https://www.youtube.com/watch?v=T9ddpYcxvsw) - crooner2007
// splsh_s.mp3: combination of mac boot sound and old radio shack add featuring twilight zone theme (https://www.youtube.com/watch?v=1sUp3RKpidU)
// W0FXD.mp3:  Computer created code audio (https://morse-coder.com/)
// W0ZE.mp3:    Computer created code audio  (https://morse-coder.com/)
// KB0BEI.mp3: Computer created code audio  (https://morse-coder.com/)

// Historical recordings of WWV and WWVH: https://swling.com/blog/2015/11/updated-release-at-the-tone-a-little-history-of-nist-radio-stations-wwv-wwvh/
// History and behind the scenese at WWVH (Steve Johnston): https://www.youtube.com/watch?v=sOmm4fG4DoI&t=163s


#include <Wire.h>
#include <hd44780.h>                        // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>  // i2c expander i/o class header


hd44780_I2Cexp lcd;  // declare lcd object: auto locate & auto config expander chip

#include <EncoderButton.h>
#include <DFRobot_DF1201S.h>   // audio board
#define DF1201SSerial Serial2  // audio board
DFRobot_DF1201S DF1201S;       // audio board

#include <Timezone.h>  // https://github.com/JChristensen/Timezone
#include <UnixTime.h>

//moon character for dimmer mode
byte customChar[] = {
  B01110,
  B11100,
  B11100,
  B11100,
  B01110,
  B00000,
  B00000,
  B00000
};

// custom characters for "static" on splash screen
byte dot[8] = {
  B10000,
  B00100,
  B00000,
  B00101,
  B10000,
  B00010,
  B10001,
  B01000
};

byte invdot[8] = {
  B00000,
  B10010,
  B00000,
  B00001,
  B00000,
  B01000,
  B00010,
  B10001
};

byte invdota[8] = {
  B10000,
  B01010,
  B00001,
  B01000,
  B10001,
  B00010,
  B01000,
  B10010
};

byte dota[8] = {
  B01010,
  B10001,
  B00000,
  B01001,
  B00000,
  B11010,
  B00000,
  B01001
};

byte block[] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte blockclear[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};


// LCD geometry
const int LCD_COLS = 20;
const int LCD_ROWS = 4;

UnixTime stamp(0);
UnixTime gps_stamp(0);

double beginTime = 0;
double endTime = 0;
double elapsedTime = 0;
double Elapsed_Time_in_Minutes = 0;  // my variable kg
double sofar_time = 0;
unsigned long previousMillis = 0;  // Stores the last time the LCD was updated
unsigned long currentMillis = 0;   // Get the current time

// Main settings XXX (bookmark)
const long interval = 500;      // Interval at which to LCD refresh (milliseconds)
int attempt = 20;               // Number of attempts for WWVB to try, ~2.3 min/attempt = 2min14sec
int splash = 0;                 // spash screen control, 0 off, 1 on
int screen_saver_timeout = 5;   //minutes before kicking back to screen local or utc
int irq_timeout = 135000;       // timeout to indicate no ES100/antenna connection 133990
int time_inIRQ_limit = 134700;  //133111 seems to be the normal return time, setting some padding
int count = 0;


// time_inIRQ = 133909 (with ES100)  133204
// time_inIRQ = 134000 (without ES100)

int lcd_pot_value = 0;
int lcd_bright = 0;
int lcd_bright_dimm = 0;

int event_today = 0;
int event_checked = 0;
int time_here = 0;
int time_inIRQ = 0;

int coord_sys = 0;
float velNorth_cms = 0;
int rtc_toggle = 0;

//Imprtant user variables ******************************************************

char timezone_selection[7][5] = { "UTC", "usET", "usCT", "usMT", "usPT", "CE", "UK" };

// US Eastern Time Zone (New York, Detroit)
//TimeChangeRule myDST = { "EDT", Second, Sun, Mar, 2, -240 };  //Daylight time = UTC - 4 hours
//TimeChangeRule mySTD = { "EST", First, Sun, Nov, 2, -300 };   //Standard time = UTC - 5 hours
//Timezone myTZ(myDST, mySTD);

// Australia Eastern Time Zone (Sydney, Melbourne)
TimeChangeRule aEDT = { "AEDT", First, Sun, Oct, 2, 660 };  // UTC + 11 hours
TimeChangeRule aEST = { "AEST", First, Sun, Apr, 3, 600 };  // UTC + 10 hours
Timezone ausET(aEDT, aEST);

// Moscow Standard Time (MSK, does not observe DST)
TimeChangeRule msk = { "MSK", Last, Sun, Mar, 1, 180 };
Timezone tzMSK(msk);

// Central European Time (Frankfurt, Paris)
TimeChangeRule CEST = { "CEST", Last, Sun, Mar, 2, 120 };  // Central European Summer Time
TimeChangeRule CET = { "CET ", Last, Sun, Oct, 3, 60 };    // Central European Standard Time
Timezone CE(CEST, CET);

// United Kingdom (London, Belfast)
TimeChangeRule BST = { "BST", Last, Sun, Mar, 1, 60 };  // British Summer Time
TimeChangeRule GMT = { "GMT", Last, Sun, Oct, 2, 0 };   // Standard Time
Timezone UK(BST, GMT);

// UTC
TimeChangeRule utcRule = { "UTC", Last, Sun, Mar, 1, 0 };  // UTC
Timezone UTC(utcRule);

// US Eastern Time Zone (New York, Detroit)
TimeChangeRule usEDT = { "EDT", Second, Sun, Mar, 2, -240 };  // Eastern Daylight Time = UTC - 4 hours
TimeChangeRule usEST = { "EST", First, Sun, Nov, 2, -300 };   // Eastern Standard Time = UTC - 5 hours
Timezone usET(usEDT, usEST);

// US Central Time Zone (Chicago, Houston)
TimeChangeRule usCDT = { "CDT", Second, Sun, Mar, 2, -300 };
TimeChangeRule usCST = { "CST", First, Sun, Nov, 2, -360 };
Timezone usCT(usCDT, usCST);

// US Mountain Time Zone (Denver, Salt Lake City)
TimeChangeRule usMDT = { "MDT", Second, Sun, Mar, 2, -360 };
TimeChangeRule usMST = { "MST", First, Sun, Nov, 2, -420 };
Timezone usMT(usMDT, usMST);

// Arizona is US Mountain Time Zone but does not use DST
TimeChangeRule usAZT = { "AZT", First, Sun, Nov, 2, -420 };
Timezone usAZ(usAZT);

// US Pacific Time Zone (Las Vegas, Los Angeles)
TimeChangeRule usPDT = { "PDT", Second, Sun, Mar, 2, -420 };
TimeChangeRule usPST = { "PST", First, Sun, Nov, 2, -480 };
Timezone usPT(usPDT, usPST);

// If TimeChangeRules are already stored in EEPROM, comment out the three
// lines above and uncomment the line below.
//Timezone myTZ(100);       //assumes rules stored at EEPROM address 100

TimeChangeRule *tcr;  //pointer to the time change rule, use to get TZ abbrev

const char *monthNames[] = {
  "",  // Index 0 is often left empty for 1-based indexing of months
  "January", "February", "March", "April", "May", "June",
  "July", "August", "September", "October", "November", "December"
};

#include <NMEAGPS.h>


//======================================================================
//  Program: NMEAtimezone.ino
//
//  Description:  This program shows how to offset the GPS dateTime member
//          into your specific timezone.  GPS devices do not know which
//          timezone they are in, so they always report a UTC time.  This
//          is the same as GMT.
//
//  Prerequisites:
//     1) NMEA.ino works with your device
//     2) GPS_FIX_TIME is enabled in GPSfix_cfg.h
//     3) NMEAGPS_PARSE_RMC is enabled in NMEAGPS_cfg.h.  You could use
//        any sentence that contains a time field.  Be sure to change the
//        "if" statement in GPSloop from RMC to your selected sentence.
//
//  'Serial' is for debug output to the Serial Monitor window.
//
//  License:
//    Copyright (C) 2014-2017, SlashDevin
//
//    This file is part of NeoGPS
//
//    NeoGPS is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    NeoGPS is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with NeoGPS.  If not, see <http://www.gnu.org/licenses/>.
//
//======================================================================

static NMEAGPS gps;  // This parses received characters
static gps_fix fix;  // This contains all the parsed pieces

//--------------------------
// CHECK CONFIGURATION

#if !defined(GPS_FIX_TIME) | !defined(GPS_FIX_DATE)
#error You must define GPS_FIX_TIME and DATE in GPSfix_cfg.h!
#endif

#if !defined(NMEAGPS_PARSE_RMC) & !defined(NMEAGPS_PARSE_ZDA)
#error You must define NMEAGPS_PARSE_RMC or ZDA in NMEAGPS_cfg.h!
#endif

//------------------------------------------------------------
// Check that the config files are set up properly

#if !defined(NMEAGPS_PARSE_RMC)
#error You must uncomment NMEAGPS_PARSE_RMC in NMEAGPS_cfg.h!
#endif

#if !defined(GPS_FIX_LOCATION_DMS)
#error You must uncomment GPS_FIX_LOCATION_DMS in GPSfix_cfg.h!
#endif

#if !defined(GPS_FIX_TIME)
#error You must uncomment GPS_FIX_TIME in GPSfix_cfg.h!
#endif

#if !defined(GPS_FIX_LOCATION)
#error You must uncomment GPS_FIX_LOCATION in GPSfix_cfg.h!
#endif

#if !defined(GPS_FIX_SPEED)
#error You must uncomment GPS_FIX_SPEED in GPSfix_cfg.h!
#endif

#if !defined(GPS_FIX_VELNED)
#error You must uncomment GPS_FIX_VELNED in GPSfix_cfg.h!
#endif

#if !defined(GPS_FIX_SATELLITES)
#error You must uncomment GPS_FIX_SATELLITES in GPSfix_cfg.h!
#endif

#ifdef NMEAGPS_INTERRUPT_PROCESSING
#error You must *NOT* define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
#endif

#include <GPSport.h>
#include <Streamers.h>

//--------------------------
// Set these values to the offset of your timezone from GMT

static const int32_t zone_hours = -5L;   // EST
static const int32_t zone_minutes = 0L;  // usually zero
static const NeoGPS::clock_t zone_offset =
  zone_hours * NeoGPS::SECONDS_PER_HOUR + zone_minutes * NeoGPS::SECONDS_PER_MINUTE;

// Uncomment one DST changeover rule, or define your own:
#define USA_DST
//#define EU_DST

#if defined(USA_DST)
static const uint8_t springMonth = 3;
static const uint8_t springDate = 14;  // latest 2nd Sunday
static const uint8_t springHour = 2;
static const uint8_t fallMonth = 11;
static const uint8_t fallDate = 7;  // latest 1st Sunday
static const uint8_t fallHour = 2;
#define CALCULATE_DST

#elif defined(EU_DST)
static const uint8_t springMonth = 3;
static const uint8_t springDate = 31;  // latest last Sunday
static const uint8_t springHour = 1;
static const uint8_t fallMonth = 10;
static const uint8_t fallDate = 31;  // latest last Sunday
static const uint8_t fallHour = 1;
#define CALCULATE_DST
#endif


static void printL(Print &outs, int32_t degE7);
static void printL(Print &outs, int32_t degE7) {
  // Extract and print negative sign
  if (degE7 < 0) {
    degE7 = -degE7;
    outs.print('-');
  }

  // Whole degrees
  int32_t deg = degE7 / 10000000L;
  outs.print(deg);
  outs.print('.');

  // Get fractional degrees
  degE7 -= deg * 10000000L;

  // Print leading zeroes, if needed
  int32_t factor = 1000000L;
  while ((degE7 < factor) && (factor > 1L)) {
    outs.print('0');
    factor /= 10L;
  }

  // Print fractional degrees
  outs.print(degE7);
}

static void doSomeWork(const gps_fix &fix);
static void doSomeWork(const gps_fix &fix) {
  //  This is the best place to do your time-consuming work, right after
  //     the RMC sentence was received.  If you do anything in "loop()",
  //     you could cause GPS characters to be lost, and you will not
  //     get a good lat/lon.
  //  For this example, we just print the lat/lon.  If you print too much,
  //     this routine will not get back to "loop()" in time to process
  //     the next set of GPS data.

  if (fix.valid.location) {

    if (fix.dateTime.seconds < 10)
      DEBUG_PORT.print('0');
    DEBUG_PORT.print(fix.dateTime.seconds);
    DEBUG_PORT.print(',');

    // DEBUG_PORT.print( fix.latitude(), 6 ); // floating-point display
    // DEBUG_PORT.print( fix.latitudeL() ); // integer display
    printL(DEBUG_PORT, fix.latitudeL());  // prints int like a float
    DEBUG_PORT.print(',');
    // DEBUG_PORT.print( fix.longitude(), 6 ); // floating-point display
    // DEBUG_PORT.print( fix.longitudeL() );  // integer display
    printL(DEBUG_PORT, fix.longitudeL());  // prints int like a float

    DEBUG_PORT.print(',');
    if (fix.valid.satellites)
      DEBUG_PORT.print(fix.satellites);

    DEBUG_PORT.print(',');
    DEBUG_PORT.print(fix.speed(), 6);
    DEBUG_PORT.print(F(" kn = "));
    DEBUG_PORT.print(fix.speed_mph(), 6);
    DEBUG_PORT.print(F(" mph"));





  } else {
    // No valid location data yet!
    DEBUG_PORT.print('?');
  }

  DEBUG_PORT.println();

}  // doSomeWork


static void doSomeWorkDMS(const gps_fix &fix);
static void doSomeWorkDMS(const gps_fix &fix) {
  //  This is the best place to do your time-consuming work, right after
  //     the RMC sentence was received.  If you do anything in "loop()",
  //     you could cause GPS characters to be lost, and you will not
  //     get a good lat/lon.
  //  For this example, we just print the lat/lon.  If you print too much,
  //     this routine will not get back to "loop()" in time to process
  //     the next set of GPS data.

  if (fix.valid.location) {

    DEBUG_PORT << fix.latitudeDMS;
    DEBUG_PORT.print(fix.latitudeDMS.NS());
    DEBUG_PORT.write(' ');
    if (fix.longitudeDMS.degrees < 100)
      DEBUG_PORT.write('0');
    DEBUG_PORT << fix.longitudeDMS;
    DEBUG_PORT.print(fix.longitudeDMS.EW());

  } else {
    // No valid location data yet!
    DEBUG_PORT.print('?');
  }

  DEBUG_PORT.println();

}  // doSomeWork


static void doSomeWorkVel(const gps_fix &fix);
static void doSomeWorkVel(const gps_fix &fix) {
  //  This is the best place to do your time-consuming work, right after
  //     the RMC sentence was received.  If you do anything in "loop()",
  //     you could cause GPS characters to be lost, and you will not
  //     get a good lat/lon.
  //  For this example, we just print the lat/lon.  If you print too much,
  //     this routine will not get back to "loop()" in time to process
  //     the next set of GPS data.

  if (fix.valid.location) {

    if (fix.dateTime.seconds < 10)
      DEBUG_PORT.print('0');
    DEBUG_PORT.print(fix.dateTime.seconds);
    DEBUG_PORT.print(',');

    // DEBUG_PORT.print( fix.latitude(), 6 ); // floating-point display
    // DEBUG_PORT.print( fix.latitudeL() ); // integer display
    printL(DEBUG_PORT, fix.latitudeL());  // prints int like a float
    DEBUG_PORT.print(',');
    // DEBUG_PORT.print( fix.longitude(), 6 ); // floating-point display
    // DEBUG_PORT.print( fix.longitudeL() );  // integer display
    printL(DEBUG_PORT, fix.longitudeL());  // prints int like a float

    DEBUG_PORT.print(',');
    if (fix.valid.satellites)
      DEBUG_PORT.print(fix.satellites);

    DEBUG_PORT.print(',');
    DEBUG_PORT.print(fix.speed(), 6);
    DEBUG_PORT.print(F(" kn = "));
    DEBUG_PORT.print(fix.speed_mph(), 6);
    DEBUG_PORT.print(F(" mph"));
    DEBUG_PORT.print(fix.velocity_north, 6);

  } else {
    // No valid location data yet!
    DEBUG_PORT.print('?');
  }

  DEBUG_PORT.println();

}  // doSomeWork

//------------------------------------

static void GPSloop_loc();
static void GPSloop_loc() {
  while (gps.available(gpsPort))
    doSomeWork(gps.read());

}  // GPSloop

static void GPSloop_loc_DMS() {
  while (gps.available(gpsPort))
    doSomeWorkDMS(gps.read());

}  // GPSloop

static void GPSloop_loc_Vel() {
  while (gps.available(gpsPort))
    doSomeWorkVel(gps.read());

}  // GPSloop

//--------------------------

void printTwoDigit(int col, int row, int val) {
  lcd.setCursor(col, row);
  if (val < 10) lcd.print('0');
  lcd.print(val, DEC);
}

void printYear(int col, int row, int year) {
  lcd.setCursor(col, row);
  lcd.print(year, DEC);
}

//--------------------------

void adjustTime(NeoGPS::time_t &dt) {
  NeoGPS::clock_t seconds = dt;  // convert date/time structure to seconds

#ifdef CALCULATE_DST
    //  Calculate DST changeover times once per reset and year!
  static NeoGPS::time_t changeover;
  static NeoGPS::clock_t springForward, fallBack;

  if ((springForward == 0) || (changeover.year != dt.year)) {

    //  Calculate the spring changeover time (seconds)
    changeover.year = dt.year;
    changeover.month = springMonth;
    changeover.date = springDate;
    changeover.hours = springHour;
    changeover.minutes = 0;
    changeover.seconds = 0;
    changeover.set_day();
    // Step back to a Sunday, if day != SUNDAY
    changeover.date -= (changeover.day - NeoGPS::time_t::SUNDAY);
    springForward = (NeoGPS::clock_t)changeover;

    //  Calculate the fall changeover time (seconds)
    changeover.month = fallMonth;
    changeover.date = fallDate;
    changeover.hours = fallHour - 1;  // to account for the "apparent" DST +1
    changeover.set_day();
    // Step back to a Sunday, if day != SUNDAY
    changeover.date -= (changeover.day - NeoGPS::time_t::SUNDAY);
    fallBack = (NeoGPS::clock_t)changeover;
  }
#endif

  //  First, offset from UTC to the local timezone
  seconds += zone_offset;

#ifdef CALCULATE_DST
  //  Then add an hour if DST is in effect
  if ((springForward <= seconds) && (seconds < fallBack))
    seconds += NeoGPS::SECONDS_PER_HOUR;
#endif

  dt = seconds;  // convert seconds back to a date/time structure

}  // adjustTime

//--------------------------

static void GPSloop_time() {
  while (gps.available(gpsPort)) {
    fix = gps.read();
    // Display the local time

    if (fix.valid.time && fix.valid.date) {
      //  adjustTime(fix.dateTime);

      // DEBUG_PORT << fix.dateTime;
    }
    // DEBUG_PORT.println();
  }

}  // GPSloop

//--------------------------

#ifdef NMEAGPS_INTERRUPT_PROCESSING
static void GPSisr(uint8_t c) {
  gps.handle(c);
}
#endif

//--------------------------


// Autor / Auteur : Zineb KANDOUSSI (https://www.facebook.com/SchoolSurSeineFR/)
// Creation / Création : Le 21/04/2023
// Description : Display the current time, date and temperature on the 20x4 LCD screen / Affichage de l'heure actuelle, la date et la température sur l'écran LCD 20x4
// Inspired from this code : File => Examples => RTClib => ds3231 / inspiré de ce code : Fichier => Exemples => RTClib => ds3231

//#include <LiquidCrystal_I2C.h>  // Not happy with this, want to convert to a different recommended  <hd44780.h>
#include "RTClib.h"

//LiquidCrystal_I2C lcd(0X3F, 20, 4);  // 20 columns, 4 rows (starts at 0)
RTC_DS3231 rtc;  //Uses the DS3231, the 1307 isn't accurate enough.  Even the chip on the 3231 has to be examined. Had to send some back to get the right chip.




char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
byte char_temp[8] = { B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110 };  // for thermometer icon / pour l'icône du thermomètre
char timezone_local;
int sync_type = 0;
DateTime wwvb_timestamp;


//Menu Encoder setup
EncoderButton eb1(2, 3, 4);
//int encoderPosition;
int encoderdiff;
int menuSize = 10;
int currentPosition;
int newPosition = 0;



//uint8_t page_number=0; // tried this variable type due to issues with switch statement
int page_number = 0;
int page_number_menu = 0;
int timezone_count = 0;
int timezone_count_num = 0;
int value = 0;
int value2 = 0;


int tick = 0;
int last_tick = 0;
int last = 0;
int last2 = 0;
int screen_saver = 0;

int is_playing = 0;
const char *dir_part_v = "/v/";
const char *dir_part_h = "/h/";
int time_value = 0;
const char *mp3_ext = ".mp3";
//String finalString = "";
char finalString[16];  // fixed buffer replaces String — avoids heap fragmentation

int data_dst_next_month = 0;
int data_dst_next_day = 0;
int data_dst_next_hour = 0;

#define january 1
#define february 2
#define march 3
#define april 4
#define may 5
#define june 6
#define july 7
#define august 8
#define september 9
#define october 10
#define november 11
#define december 12

// Event arrays
// Load with month of 'event',  day of event and description to be displayed

#include "events.h"
//#include "C:\Users\psvan\OneDrive\Documents\Arduino\holidays\holidays.h"

//------------------------------------------------------------------------------
// MCU constants - USER TO MODIFY
//------------------------------------------------------------------------------

// GPIO pins
// #define GPIO_EN 4
// #define GPIO_IRQ 2
#define GPIO_EN 22   // Important, just be placed in these ports on the board
#define GPIO_IRQ 24  // Important, just be placed in these ports on the board

//------------------------------------------------------------------------------
// constants
//------------------------------------------------------------------------------

// I2C slave address
#define ES100_SLAVE_ADDR 0x32

// enable delay time
//#define ENABLE_DELAY_US 1500
#define ENABLE_DELAY_US 100000  // 100ms: module version 1 only

// I2C clock high/low time
#define SCL_TIME_US 2

// ES100 API register addresses
#define ES100_CONTROL0_REG 0x00
#define ES100_CONTROL1_REG 0x01
#define ES100_IRQ_STATUS_REG 0x02
#define ES100_STATUS0_REG 0x03
#define ES100_YEAR_REG 0x04
#define ES100_MONTH_REG 0x05
#define ES100_DAY_REG 0x06
#define ES100_HOUR_REG 0x07
#define ES100_MINUTE_REG 0x08
#define ES100_SECOND_REG 0x09
#define ES100_NEXT_DST_MONTH_REG 0x0A
#define ES100_NEXT_DST_DAY_REG 0x0B
#define ES100_NEXT_DST_HOUR_REG 0x0C

//------------------------------------------------------------------------------
// array to store date and time
//------------------------------------------------------------------------------

#define DT_STATUS 0
#define DT_YEAR 1
#define DT_MONTH 2
#define DT_DAY 3
#define DT_HOUR 4
#define DT_MINUTE 5
#define DT_SECOND 6
#define DT_NEXT_DST_MONTH 7
#define DT_NEXT_DST_DAY 8
#define DT_NEXT_DST_HOUR 9

#define DT_LENGTH 10



// Credit https://gist.github.com/jrleeman/3b7c10712112e49d8607
// Needed a day of the year to display the next event to follow and cycle through

int calculateDayOfYear(int day, int month, int year) {

  // Given a day, month, and year (4 digit), returns
  // the day of year. Errors return 999.
  //result = calculateDayOfYear(29,2,2000);

  int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  // Verify we got a 4-digit year
  if (year < 1000) {
    return 999;
  }

  // Check if it is a leap year, this is confusing business
  // See: https://support.microsoft.com/en-us/kb/214019
  if (year % 4 == 0) {
    if (year % 100 != 0) {
      daysInMonth[1] = 29;
    } else {
      if (year % 400 == 0) {
        daysInMonth[1] = 29;
      }
    }
  }

  // Make sure we are on a valid day of the month
  if (day < 1) {
    return 999;
  } else if (day > daysInMonth[month - 1]) {
    return 999;
  }

  int doy = 0;
  for (int i = 0; i < month - 1; i++) {
    doy += daysInMonth[i];
  }

  doy += day;
  return doy;
}

/**
 * A function to handle the 'clicked' event
 * Can be called anything but requires EncoderButton& 
 * as its only parameter.
 * I tend to prefix with 'on' and suffix with the 
 * event type.
 */
void onEb1LongPress(EncoderButton &eb) {
  Serial.print("button1 longPressCount: ");
  Serial.println(eb.longPressCount());


  if (page_number == 0) {
    DF1201S.playSpecFile("/cv_kiss.mp3");
  }
  if (page_number == 1) {
    DF1201S.playSpecFile("/cv_full.mp3");
  }

  if (page_number == 8) {

    // monitor volume pot, set volume
    DF1201S.setVol(((analogRead(A0) * (40) / 1023)));

    lcd.setCursor(0, 2);
    lcd.print("Historical Audio");
    lcd.setCursor(0, 3);
    lcd.print("               ");
    DF1201S.playSpecFile("/coll.mp3");
    unsigned long startTime = millis();
    unsigned long elapsedTime = millis();
    elapsedTime = millis() - startTime;

    while ((elapsedTime < 487000) && (page_number == 8)) {
      elapsedTime = millis() - startTime;

      //break if encoder changes

      unsigned long currentMillis = 487000 - elapsedTime;
      unsigned long seconds = currentMillis / 1000;
      unsigned long minutes = seconds / 60;
      unsigned long hours = minutes / 60;
      unsigned long days = hours / 24;
      currentMillis %= 1000;
      seconds %= 60;
      minutes %= 60;
      hours %= 24;

      lcd.setCursor(0, 3);
      lcd.print("Remaining: ");
      // int hr = (588000-elapsedTime)/3600;                                                        //Number of seconds in an hour
      //int mins = ((588000-elapsedTime)/60000);                                             //Remove the number of hours and calculate the minutes.
      //int sec = (588000-elapsedTime)-(mins*60)/1000;
      lcd.setCursor(12, 3);
      lcd.print(minutes);
      lcd.setCursor(13, 3);
      lcd.print(":");
      lcd.setCursor(14, 3);
      if (seconds < 10) {

        lcd.print("0");
        lcd.setCursor(15, 3);
      }
      lcd.print(seconds);
      if (minutes > 7) {
        lcd.setCursor(3, 1);
        lcd.print("(303) 499-7111");
      }

      eb1.update();
      DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
      value = page_number;
      if ((value != last)) {
        lcd.clear();
        last = value;
        DF1201S.pause();
      }
    }
  }

  if (page_number == 9) {

    // monitor volume pot, set volume
    DF1201S.setVol(((analogRead(A0) * (40) / 1023)));

    lcd.setCursor(0, 2);
    lcd.print("History of WWVH");
    lcd.setCursor(0, 3);
    lcd.print("               ");
    DF1201S.playSpecFile("/wwvh_h.mp3");
    unsigned long startTime = millis();
    unsigned long elapsedTime = millis();
    elapsedTime = millis() - startTime;

    while ((elapsedTime < 588000) && (page_number == 9)) {
      elapsedTime = millis() - startTime;

      //break if encoder changes

      unsigned long currentMillis = 588000 - elapsedTime;
      unsigned long seconds = currentMillis / 1000;
      unsigned long minutes = seconds / 60;
      unsigned long hours = minutes / 60;
      unsigned long days = hours / 24;
      currentMillis %= 1000;
      seconds %= 60;
      minutes %= 60;
      hours %= 24;

      lcd.setCursor(0, 3);
      lcd.print("Remaining: ");
      // int hr = (588000-elapsedTime)/3600;                                                        //Number of seconds in an hour
      //int mins = ((588000-elapsedTime)/60000);                                             //Remove the number of hours and calculate the minutes.
      //int sec = (588000-elapsedTime)-(mins*60)/1000;
      lcd.setCursor(12, 3);
      lcd.print(minutes);
      lcd.setCursor(13, 3);
      lcd.print(":");
      lcd.setCursor(14, 3);
      if (seconds < 10) {

        lcd.print("0");
        lcd.setCursor(15, 3);
      }
      lcd.print(seconds);
      if (minutes < 1) {
        lcd.setCursor(3, 1);
        lcd.print("(800) 335-4363");
      }

      eb1.update();
      DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
      value = page_number;
      if ((value != last)) {
        lcd.clear();
        last = value;
        DF1201S.pause();
      }
    }
  }
  // Do something
  lcd.clear();
}

void onEb1DoubleClicked(EncoderButton &eb) {
  Serial.println("double clicked ");



  if (page_number == 8) {
    // monitor volume pot, set volume
    DF1201S.setVol(((analogRead(A0) * (40) / 1023)));

    lcd.setCursor(0, 2);
    lcd.print("Ident...1980s retro");
    lcd.setCursor(0, 3);
    lcd.print("               ");
    DF1201S.playSpecFile("/wwv_ri.mp3");
    unsigned long startTime = millis();
    unsigned long elapsedTime = millis();
    elapsedTime = millis() - startTime;
    while (elapsedTime < 54000) {
      elapsedTime = millis() - startTime;
      //break if encoder changes
      eb1.update();
      DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
      value = page_number;
      if ((value != last)) {
        lcd.clear();
        last = value;
        DF1201S.pause();
      }
    }
    // Do something
    lcd.clear();
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print("WWV (Ft.Collins, CO)");
    lcd.setCursor(0, 2);
    lcd.print("UTC:");
    lcd.setCursor(6, 2);
    if (now.hour() <= 9) {
      lcd.print("0");
      lcd.setCursor(7, 2);
      lcd.print(now.hour(), DEC);
    } else {
      lcd.print(now.hour(), DEC);
    }
    lcd.setCursor(8, 2);
    lcd.print(":");
    lcd.setCursor(9, 2);
    if (now.minute() <= 9) {
      lcd.print("0");
      lcd.setCursor(10, 2);
      lcd.print(now.minute(), DEC);
    } else {
      lcd.print(now.minute(), DEC);
    }
    lcd.setCursor(11, 2);
    lcd.print(":");
    lcd.setCursor(12, 2);

    if (now.second() <= 9) {
      lcd.print("0");
      lcd.setCursor(13, 2);
      lcd.print(now.second(), DEC);

    } else {
      lcd.print(now.second(), DEC);
    }
    page_number = 8;
  }



  if (page_number == 9) {

    // monitor volume pot, set volume
    DF1201S.setVol(((analogRead(A0) * (40) / 1023)));

    lcd.setCursor(0, 2);
    lcd.print("Ident...1980s retro");
    lcd.setCursor(0, 3);
    lcd.print("               ");
    DF1201S.playSpecFile("/wwvh_ri.mp3");
    unsigned long startTime = millis();
    unsigned long elapsedTime = millis();
    elapsedTime = millis() - startTime;
    while (elapsedTime < 48000) {
      elapsedTime = millis() - startTime;
      //break if encoder changes
      eb1.update();
      DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
      value = page_number;
      if ((value != last)) {
        lcd.clear();
        last = value;
        DF1201S.pause();
      }
    }
    // Do something
    lcd.clear();
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print("WWV (Ft.Collins, CO)");
    lcd.setCursor(0, 2);
    lcd.print("UTC:");
    lcd.setCursor(6, 2);
    if (now.hour() <= 9) {
      lcd.print("0");
      lcd.setCursor(7, 2);
      lcd.print(now.hour(), DEC);
    } else {
      lcd.print(now.hour(), DEC);
    }
    lcd.setCursor(8, 2);
    lcd.print(":");
    lcd.setCursor(9, 2);
    if (now.minute() <= 9) {
      lcd.print("0");
      lcd.setCursor(10, 2);
      lcd.print(now.minute(), DEC);
    } else {
      lcd.print(now.minute(), DEC);
    }
    lcd.setCursor(11, 2);
    lcd.print(":");
    lcd.setCursor(12, 2);

    if (now.second() <= 9) {
      lcd.print("0");
      lcd.setCursor(13, 2);
      lcd.print(now.second(), DEC);

    } else {
      lcd.print(now.second(), DEC);
    }
  }

  lcd.clear();
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("WWV (Ft.Collins, CO)");
  lcd.setCursor(0, 2);
  lcd.print("UTC:");
  lcd.setCursor(6, 2);
  if (now.hour() <= 9) {
    lcd.print("0");
    lcd.setCursor(7, 2);
    lcd.print(now.hour(), DEC);
  } else {
    lcd.print(now.hour(), DEC);
  }
  lcd.setCursor(8, 2);
  lcd.print(":");
  lcd.setCursor(9, 2);
  if (now.minute() <= 9) {
    lcd.print("0");
    lcd.setCursor(10, 2);
    lcd.print(now.minute(), DEC);
  } else {
    lcd.print(now.minute(), DEC);
  }
  lcd.setCursor(11, 2);
  lcd.print(":");
  lcd.setCursor(12, 2);

  if (now.second() <= 9) {
    lcd.print("0");
    lcd.setCursor(13, 2);
    lcd.print(now.second(), DEC);

  } else {
    lcd.print(now.second(), DEC);
  }
  page_number = 9;
}

// What to do if Encoder is pressed //
void onEb1Clicked(EncoderButton &eb) {

  if (page_number == 0) {
    timezone_count = timezone_count + 1;
    timezone_count_num = (timezone_count % 7);
    lcd.setCursor(0, 3);
    lcd.print("                ");
    lcd.setCursor(6, 0);
    lcd.print("         ");
  }

  if (page_number == 1) {
    lcd.setCursor(0, 3);
    lcd.print("                   ");
    rtc_toggle++;
    if (rtc_toggle > 1) { rtc_toggle = 0; }
  }

  if (page_number == 2) {


    event_checked = 0;
    event_today = 0;
    // local variables
    int current_timer_value;
    int dt_array[DT_LENGTH];

    // initialize MCU
    mcu_init();
    // initialize gpios
    mcu_gpio_set_low(GPIO_EN);
    // receive time from WWVB

    current_timer_value = es100_receive(dt_array);
    //     Serial.print("time_inIRQ/time_inIRQ_limit: ");
    // Serial.print(time_inIRQ);
    // Serial.print(" / ");
    // Serial.println(time_inIRQ_limit);
    // Serial.print("count / sync_type:");
    // Serial.print(count);
    // Serial.print(" /");
    // Serial.println(sync_type);
    if (count < attempt) { sync_type = 1; }
    //Serial.print("after sync type chedk: ");
    // Serial.println(sync_type);
    if ((time_inIRQ < time_inIRQ_limit) && (sync_type > 0)) {
      wwvb_timestamp = rtc.now();
    } else {
      if (fix.satellites > 3) {
        lcd.clear();
        lcd.setCursor(1, 1);
        lcd.print("GPS Time Sync");
        GPSloop_time();
        rtc.adjust(DateTime(fix.dateTime.year, fix.dateTime.month, fix.dateTime.date, fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds));
        sync_type = 2;
        wwvb_timestamp = rtc.now();
        delay(3000);
      }
    }
  }


  if (page_number == 3) {
    GPSloop_time();
    rtc.adjust(DateTime(fix.dateTime.year, fix.dateTime.month, fix.dateTime.date, fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds));
    sync_type = 2;
    wwvb_timestamp = rtc.now();
  }

  if (page_number == 4) {
    coord_sys++;
    if (coord_sys > 2) { coord_sys = 0; }
    lcd.clear();
  }


  if (page_number == 7) {

    // Need to determine where in the stack we are currently
    int iii = 0;
    while (iii < eventnumber) {
      // First pass: upcoming events (day-of-year >= today)
      for (int ii = 0; ii < eventnumber; ii++) {
        if (calculateDayOfYear(events[ii].day, events[ii].month, stamp.year) >= calculateDayOfYear(stamp.day, stamp.month, stamp.year)) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(monthNames[events[ii].month]);
          lcd.print(" ");
          lcd.print(events[ii].day);
          lcd.setCursor(0, 1);
          lcd.print(events[ii].description);
          iii++;
          delay(900);
        }
      }
      // Second pass: past events (wrap around to start of year)
      for (int ii = 0; ii < eventnumber; ii++) {
        if (calculateDayOfYear(events[ii].day, events[ii].month, stamp.year) < calculateDayOfYear(stamp.day, stamp.month, stamp.year)) {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(monthNames[events[ii].month]);
          lcd.print(" ");
          lcd.print(events[ii].day);
          lcd.setCursor(0, 1);
          lcd.print(events[ii].description);
          iii++;
          delay(900);
        }
      }
    }
    lcd.clear();
  }


  if (page_number == 8) {
    // monitor volume pot, set volume
    DF1201S.setVol(((analogRead(A0) * (40) / 1023)));

    lcd.setCursor(0, 2);
    lcd.print("Identifying...");
    lcd.setCursor(0, 3);
    lcd.print("               ");
    DF1201S.playSpecFile("/v/ident.mp3");

    unsigned long startTime = millis();
    unsigned long elapsedTime = millis();
    ;
    // ... some code ...
    elapsedTime = millis() - startTime;
    while (elapsedTime < 33000) {  // Check if 1 second has passed
                                   // monitor volume pot, set volume
      DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
      elapsedTime = millis() - startTime;

      if ((elapsedTime > 11000) && (elapsedTime < 12000)) {
        lcd.setCursor(0, 3);
        lcd.print("2.5,");
      }
      if ((elapsedTime > 12000) && (elapsedTime < 13000)) {
        lcd.setCursor(4, 3);
        lcd.print("5,");
      }
      if ((elapsedTime > 13000) && (elapsedTime < 14000)) {
        lcd.setCursor(6, 3);
        lcd.print("10,");
      }
      if ((elapsedTime > 14000) && (elapsedTime < 14700)) {
        lcd.setCursor(9, 3);
        lcd.print("15,");
      }
      if ((elapsedTime > 14700) && (elapsedTime < 15000)) {
        lcd.setCursor(12, 3);
        lcd.print("20 MHz");
      }

      //break if encoder changes
      eb1.update();
      value = page_number;
      if ((value != last)) {
        lcd.clear();
        last = value;
        DF1201S.pause();
      }
      // Do something
    }
    startTime = millis();  // Reset the start time
    lcd.clear();
    //break;
    lcd.clear();
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print("WWV (Ft.Collins, CO)");
    lcd.setCursor(0, 2);
    lcd.print("UTC:");
    lcd.setCursor(6, 2);
    if (now.hour() <= 9) {
      lcd.print("0");
      lcd.setCursor(7, 2);
      lcd.print(now.hour(), DEC);
    } else {
      lcd.print(now.hour(), DEC);
    }
    lcd.setCursor(8, 2);
    lcd.print(":");
    lcd.setCursor(9, 2);
    if (now.minute() <= 9) {
      lcd.print("0");
      lcd.setCursor(10, 2);
      lcd.print(now.minute(), DEC);
    } else {
      lcd.print(now.minute(), DEC);
    }
    lcd.setCursor(11, 2);
    lcd.print(":");
    lcd.setCursor(12, 2);

    if (now.second() <= 9) {
      lcd.print("0");
      lcd.setCursor(13, 2);
      lcd.print(now.second(), DEC);

    } else {
      lcd.print(now.second(), DEC);
    }
  }

  if (page_number == 9) {

    // monitor volume pot, set volume
    DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
    //DF1201S.playSpecFile("/h/ident.mp3");
    lcd.setCursor(0, 2);
    lcd.print("Identifying...");
    lcd.setCursor(0, 3);
    lcd.print("               ");
    DF1201S.playSpecFile("/h/ident.mp3");

    unsigned long startTime = millis();
    unsigned long elapsedTime = millis();
    ;
    // ... some code ...
    elapsedTime = millis() - startTime;
    while (elapsedTime < 38000) {  // Check if 1 second has passed
                                   // monitor volume pot, set volume
      DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
      elapsedTime = millis() - startTime;

      if ((elapsedTime > 11000) && (elapsedTime < 12000)) {
        lcd.setCursor(0, 3);
        lcd.print("2.5,");
      }
      if ((elapsedTime > 12000) && (elapsedTime < 13000)) {
        lcd.setCursor(4, 3);
        lcd.print("5,");
      }
      if ((elapsedTime > 13000) && (elapsedTime < 14000)) {
        lcd.setCursor(6, 3);
        lcd.print("10,");
      }
      if ((elapsedTime > 14000) && (elapsedTime < 14700)) {
        lcd.setCursor(9, 3);
        lcd.print("15");
      }
      // if (elapsedTime == 14700) {
      //  lcd.setCursor(12, 3);
      //   lcd.print("20 MHz");
      // }

      //break if encoder changes
      eb1.update();
      value = page_number;
      if ((value != last)) {
        lcd.clear();
        last = value;
        DF1201S.pause();
      }
      // Do something
    }
    startTime = millis();  // Reset the start time
    lcd.clear();
  }
}

int oldPosition = 0;

// What to do if Encoder is turned  //

void onEb1Encoder(EncoderButton &eb) {

  //Serial.print("oldPosition/newPosition/currentPosition: ");
  // Serial.print(oldPosition);
  newPosition = eb.position();
  // Serial.print(newPosition);
  //    newPosition =(2* eb.increment())+newPosition;
  if (newPosition > oldPosition) {
    page_number_menu--;
    //page_number_menu--;
    // page_number_menu++;
    if (page_number_menu < 0) page_number_menu = menuSize - 1;
  }
  if (newPosition < oldPosition) {
    //currentPosition--;
    page_number_menu++;
    //page_number_menu++;
    // page_number_menu=page_number_menu+2;
    if (page_number_menu >= menuSize) {
      page_number_menu = 0;
    }
  }

  oldPosition = newPosition;
  page_number = page_number_menu;

  // Serial.print("Page number change to: ");  // my test statement
  // Serial.print(page_number);
  // Serial.print("\n");
}

//------------------------------------------------------------------------------
// mcu functions - USER TO MODIFY
//------------------------------------------------------------------------------

void mcu_init(void) {
  // optional code to initialize GPIO, timer and other MCU functions
  pinMode(GPIO_EN, OUTPUT);
  pinMode(GPIO_IRQ, INPUT);
  Wire.begin();
  Serial.begin(9600);
}

void mcu_gpio_set_high(int pin) {
  // set pin to a logic one
  digitalWrite(pin, HIGH);
}

void mcu_gpio_set_low(int pin) {
  // set pin to a logic zero
  digitalWrite(pin, LOW);
}

int mcu_gpio_read(int pin) {
  // read current state of input pinFf
  return (digitalRead(pin));
}

int mcu_timer_read(void) {
  // read timer value and return as integer
  return (millis());
}

void mcu_timer_wait_us(int count) {
  // wait 1 microsecond or greater for each count
  delayMicroseconds(count);
}

//------------------------------------------------------------------------------
// write I2C data - USER TO MODIFY
//------------------------------------------------------------------------------

void i2c_write(uint8_t slave_addr, uint8_t num_bytes, uint8_t *ptr) {

  int i;

  Wire.beginTransmission(slave_addr);

  for (i = 0; i < num_bytes; i++) {
    Wire.write(ptr[i]);
  }

  Wire.endTransmission();
}

//------------------------------------------------------------------------------
// read I2C data - USER TO MODIFY
//------------------------------------------------------------------------------

void i2c_read(uint8_t slave_addr, uint8_t num_bytes, uint8_t *ptr) {

  int i;
  const uint8_t stop_flag = 1;

  Wire.requestFrom(slave_addr, num_bytes, stop_flag);
  for (i = 0; (i < num_bytes && Wire.available()); i++) {
    ptr[i] = Wire.read();
  }
}


//NIST's WWVB 60 Khz signal broadcasts 24/7. However, during the hour cycle, the PM (Phase Modulation) signal changes as follows [1]:

//Every half-hour, for a duration of six minutes, the normal WWVB-PM 1-minute frames are replaced by the WWVBPM extended-mode time code sequences. The ES100 is not capable of receiving during these six-minute intervals that occur from HH:10 to HH:16 and HH:40 to HH:46 each hour (i.e. HH= 00, 01,…, 23).


//------------------------------------------------------------------------------
// write data to an ES100 API register
//------------------------------------------------------------------------------

void es100_write_register(uint8_t addr, uint8_t data) {
  uint8_t es100_write_array[2];

  es100_write_array[0] = addr;
  es100_write_array[1] = data;
  i2c_write(ES100_SLAVE_ADDR, 0x2, es100_write_array);
}

//------------------------------------------------------------------------------
// read data from an ES100 API register
//------------------------------------------------------------------------------

uint8_t es100_read_register(uint8_t addr) {
  uint8_t data;

  i2c_write(ES100_SLAVE_ADDR, 0x1, &addr);
  i2c_read(ES100_SLAVE_ADDR, 0x1, &data);

  return (data);
}

//------------------------------------------------------------------------------
// enable ES100
//------------------------------------------------------------------------------

void es100_enable(void) {
  mcu_gpio_set_high(GPIO_EN);
}

//------------------------------------------------------------------------------
// disable ES100
//------------------------------------------------------------------------------

void es100_disable(void) {
  mcu_gpio_set_low(GPIO_EN);
}

//------------------------------------------------------------------------------
// start reception
//------------------------------------------------------------------------------

void es100_start_rx() {
  es100_write_register(ES100_CONTROL0_REG, 0x01);

  // // perform read of control register for i2c debug only
  // es100_read_register(ES100_CONTROL0_REG);
  //  Serial.print("ES100_CONTROL0_REG: ");
  //  Serial.println(ES100_CONTROL0_REG);

  //    es100_read_register(ES100_CONTROL1_REG);
  //  Serial.print("ES100_CONTROL1_REG: ");
  //  Serial.println(ES100_CONTROL1_REG);

  //  Serial.print("Wire.available(): ");
  //  Serial.println(Wire.available());
}

//------------------------------------------------------------------------------
// wait for falling edge of IRQ
//------------------------------------------------------------------------------

void es100_wait_for_irq() {


  time_here = millis();

  while ((mcu_gpio_read(GPIO_IRQ) && ((millis() - time_here) < irq_timeout)))
    time_inIRQ = millis() - time_here;
  ;  // wait until IRQ is low
}

//------------------------------------------------------------------------------
// read IRQ status register
//------------------------------------------------------------------------------

uint8_t es100_get_irq_status() {
  //Read IRQ status register
  return (es100_read_register(ES100_IRQ_STATUS_REG));
}

uint8_t decToBcd(int val) {
  return (uint8_t)((val / 10 * 16) + (val % 10));
}

uint8_t bcdToDec(uint8_t val) {
  return (uint8_t)((val / 16 * 10) + (val % 16));
}

int hexToDec(String hexString) {

  unsigned int decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {

    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    nextInt = constrain(nextInt, 0, 15);

    decValue = (decValue * 16) + nextInt;
  }

  return decValue;
}

//------------------------------------------------------------------------------
// read date and time from API registers - Set local RTC clock
//------------------------------------------------------------------------------

void es100_read_time(int dt_array[]) {
  dt_array[DT_STATUS] = es100_read_register(ES100_STATUS0_REG);
  dt_array[DT_YEAR] = es100_read_register(ES100_YEAR_REG);
  dt_array[DT_MONTH] = es100_read_register(ES100_MONTH_REG);
  dt_array[DT_DAY] = es100_read_register(ES100_DAY_REG);
  dt_array[DT_HOUR] = es100_read_register(ES100_HOUR_REG);
  dt_array[DT_MINUTE] = es100_read_register(ES100_MINUTE_REG);
  dt_array[DT_SECOND] = es100_read_register(ES100_SECOND_REG);
  dt_array[DT_NEXT_DST_MONTH] = es100_read_register(ES100_NEXT_DST_MONTH_REG);
  dt_array[DT_NEXT_DST_DAY] = es100_read_register(ES100_NEXT_DST_DAY_REG);
  dt_array[DT_NEXT_DST_HOUR] = es100_read_register(ES100_NEXT_DST_HOUR_REG);

  // Convert from BCD to DEC
  int year = (int)bcdToDec(dt_array[DT_YEAR]);
  int month = (int)bcdToDec(dt_array[DT_MONTH]);
  int day = (int)bcdToDec(dt_array[DT_DAY]);
  int hours = (int)bcdToDec(dt_array[DT_HOUR]);
  int minutes = (int)bcdToDec(dt_array[DT_MINUTE]);
  int seconds = (int)bcdToDec(dt_array[DT_SECOND]);
  int dst_next_month = (int)bcdToDec(dt_array[DT_NEXT_DST_MONTH]);
  int dst_next_day = (int)bcdToDec(dt_array[DT_NEXT_DST_DAY]);
  int dst_next_hour = (int)bcdToDec(dt_array[DT_NEXT_DST_HOUR]);

  // Conver to unsigned 8 bit integer rather than relying on type compiler defines for int
  int data_year = (uint8_t)year;
  int data_month = (uint8_t)month;
  int data_day = (uint8_t)day;
  int data_hour = (uint8_t)hours;
  int data_minute = (uint8_t)minutes;
  int data_second = (uint8_t)seconds;
  data_dst_next_month = (uint8_t)dst_next_month;
  data_dst_next_day = (uint8_t)dst_next_day;
  data_dst_next_hour = (uint8_t)dst_next_hour;

  rtc.adjust(DateTime(data_year, data_month, data_day, data_hour, data_minute, data_second));
}

//------------------------------------------------------------------------------
// top level function to receive time from WWVB
//------------------------------------------------------------------------------

int es100_receive(int dt_array[]) {
  // local variables
  int irq_status = 0;
  int current_timer_value;
  count = 0;
  beginTime = millis();

  // enable and delay
  // Serial.println("Enabling es100");
  es100_enable();
  mcu_timer_wait_us(ENABLE_DELAY_US);
  // Serial.println("Turning off LCD Backlight");
  //lcd.noBacklight();
  analogWrite(8, 70);  // 128 is approximately 50% of 255

  // start reception
  es100_start_rx();

  // Serial.print("Done with es100_start_rx, starting while(irq_status != 0x01) \n");
  // loop until time received
  while ((irq_status != 0x01) && (count < attempt)) {


    // wait for interrupt
    //Serial.print("Count in while(irq_status != 0x01) = ");
    //Serial.print(count++);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WWVB Time Sync");

    lcd.setCursor(0, 1);
    lcd.print("In Receive Status");  // overwrite old data
    lcd.setCursor(0, 2);
    lcd.print("Time(min): ");  // overwrite old data
    lcd.setCursor(11, 2);
    //lcd.print(millis());
    sofar_time = millis();
    //endTime = millis();
    elapsedTime = sofar_time - beginTime;

    lcd.print((double)(elapsedTime / 1000 / 60));
    //lcd.setCursor(0, 4);
    count++;
    //lcd.print(count_cycle);
    if (time_inIRQ > time_inIRQ_limit) {
      Serial.println("Possible Antenna Problem");
      Serial.println("Time_inIRQ");
      Serial.println(time_inIRQ);
      lcd.setCursor(0, 3);
      lcd.print("Antenna Not Detected");
      attempt = 4;  // overwrite old data


    } else {

      lcd.setCursor(0, 3);
      lcd.print("Attempt:");  // overwrite old data
      lcd.setCursor(9, 3);    // reset the cursor
      lcd.print(count);
      lcd.setCursor(12, 3);  // reset the cursor
      lcd.print("of");
      lcd.setCursor(15, 3);  // reset the cursor
      lcd.print(attempt);
    }


    // Serial.print("\nIRQ status going in to wait for irq_status(...)... HEX = ");  // my test statement
    // Serial.print(irq_status, HEX);
    // Serial.print(" ---- DEC =  ");
    // Serial.print(irq_status, DEC);
    // Serial.print("\n");

    // time_inIRQ = 133909 (with ES100)  133204
    // time_inIRQ = 134000 (without ES100)


    Serial.print("Waiting for interrupt ... ");  // The following three are my test statements
    es100_wait_for_irq();

    if (time_inIRQ > time_inIRQ_limit) {
      Serial.println("Possible Antenna Problem");
      Serial.println("Time_inIRQ");
      Serial.println(time_inIRQ);
      lcd.setCursor(0, 3);
      lcd.print("Antenna Not Detected");
      attempt = 4;
      sync_type = 0;
    } else {

      lcd.setCursor(0, 3);
      lcd.print("Attempt:");  // overwrite old data
      lcd.setCursor(9, 3);    // reset the cursor
      lcd.print(count);
      lcd.setCursor(12, 3);  // reset the cursor
      lcd.print("of");
      lcd.setCursor(15, 3);  // reset the cursor
      lcd.print(attempt);
    }


    Serial.print("Elapsed time to here...");
    elapsedTime = millis() - beginTime;
    Serial.print(elapsedTime);
    Serial.print("\n");

    Serial.print("Time_inIRQ: ");
    Serial.println(time_inIRQ);
    // interrupt defines second boundary, so save current timer value
    current_timer_value = mcu_timer_read();
    // read interrupt status
    irq_status = es100_get_irq_status();
    // Serial.print("IRQ status = 0x");
    // Serial.println(irq_status, HEX);
  }  // End while

  if (irq_status == 0x01) {
    // Serial.print("IRQ status 0x01 received, now outside of while loop in function es100_receive(int dt_array[ ]) --------------------\n");
    endTime = millis();
    elapsedTime = endTime - beginTime;
    if (time_inIRQ > time_inIRQ_limit) {
      sync_type = 0;
    } else {
      es100_read_time(dt_array);
      time_here = 0;
    }
  }

  // disable ES100
  es100_disable();
  lcd.clear();
  analogWrite(8, lcd_bright_dimm);


  // return timer value when interrupt occurred
  return (current_timer_value);
}



NeoGPS::Location_t base(406806940L, -1050406110L);  // WWVB Ft. Collins CO
//NeoGPS::Location_t base( -253448688L, 1310324914L ); // Ayers Rock, AU



void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS);  // Init the LCD screen
  lcd.createChar(9999, customChar);
  lcd.createChar(1, dot);         // Store the 'dot' pattern at location 0
  lcd.createChar(2, invdot);      // Store the 'dot' pattern at location 0
  lcd.createChar(3, invdota);     // Store the 'dot' pattern at location 0
  lcd.createChar(4, dota);        // Store the 'dot' pattern at location 0
  lcd.createChar(5, block);       // Store the 'dot' pattern at location 0
  lcd.createChar(6, blockclear);  // Store the 'dot' pattern at location 0


  // Dim the backlight to 50% brightness
  analogWrite(8, 128);  // 128 is approximately 50% of 255

  // You can vary this value to change brightness
  // For example, to fade in and out:
  for (int brightness = 0; brightness <= 255; brightness++) {
    analogWrite(8, brightness);
    delay(10);
  }
  for (int brightness = 255; brightness >= 0; brightness--) {
    analogWrite(8, brightness);
    delay(10);
  }
  analogWrite(8, 255);  // 128 is approximately 50% of 255



  eb1.useQuadPrecision(false);
  gpsPort.begin(9600);
  Serial.begin(9600);
  DF1201SSerial.begin(115200);

  //DF1201SSerial.begin(115200);

  //Serial.begin(115200);

  while (!DF1201S.begin(DF1201SSerial)) {
    //   Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }

  //read volume pot
  // stream of numbers ranging from 0-1023
  int vol_pot_value = analogRead(A0);
  int vol = ((vol_pot_value * (40) / 1023));
  DF1201S.setVol(vol);

  // Serial.print("vol pot value: ");
  // Serial.println(vol_pot_value);
  // Serial.print("vol: ");
  // Serial.println(vol);




  /*Set volume to 20*/
  //    * @param vol 0-30
  // DF1201S.setVol(/*VOL = */ 10);
  //DF1201S.setVol(vol);
  // Serial.print("VOL:");
  /*Get volume*/
  //Serial.println(DF1201S.getVol());
  /*Enter music mode*/

  DF1201S.switchFunction(DF1201S.MUSIC);
  /*Wait for the end of the prompt tone */
  //delay(2000);
  /*Set playback mode to "repeat all"*/
  DF1201S.setPlayMode(DF1201S.SINGLE);
  //Serial.print("PlayMode:");
  /*Get playback mode*/
  //Serial.println(DF1201S.getPlayMode());



#ifdef NMEAGPS_INTERRUPT_PROCESSING
  gpsPort.attachInterrupt(GPSisr);
#endif
  //}


  //Link the event(s) to your function
  eb1.setClickHandler(onEb1Clicked);
  eb1.setEncoderHandler(onEb1Encoder);
  eb1.setLongPressHandler(onEb1LongPress, true);  //Second arg means repeat the long click
  eb1.setDoubleClickHandler(onEb1DoubleClicked);


  rtc.begin();
  // Only use rtc.adjust if you need to hard code the RTC clock to a new date
  // rtc.adjust(DateTime(2000, 01, 01, 00, 00, 00));  // Just do this to test not getting date from computer and really updating from wwvb
  //lcd.begin(LCD_COLS, LCD_ROWS);  // Init the LCD screen




  if (splash == 1) {
    // splash screen test
    // int vol_pot_value = analogRead(A0);
    // int vol = ((vol_pot_value * (40) / 1023));
    // DF1201S.setVol(vol);


    DF1201S.setVol((analogRead(A0) * (40) / 1023));

    DF1201S.playSpecFile("/splsh_s.mp3");

    // lcd.init();       // Init the LCD screen old lib
    lcd.backlight();  // Turn on backlight
    lcd.createChar(0, char_temp);
    delay(2000);

    for (int num = 0; num < 1000; num++) {
      lcd.setCursor(random(0, 20), random(0, 4));
      lcd.write(1);
      lcd.setCursor(random(0, 20), random(0, 4));
      lcd.write(2);
      lcd.setCursor(random(0, 20), random(0, 4));
      lcd.write(3);
      lcd.setCursor(random(0, 20), random(0, 4));
      lcd.write(4);

      // if(num>200){
      //   lcd.setCursor(3,0);
      //     lcd.write("W0FXD"); }

      //DF1201S.setVol((analogRead(A0) * (40) / 1023));

      if (num > 400) {
        lcd.setCursor(5, 1);
        lcd.write(" TimeKube ");
        DF1201S.setVol((analogRead(A0) * (40) / 1023));
      }


      if (num > 600) {
        lcd.setCursor(4, 2);
        lcd.write("Karl Edition");
      }
      if (num > 700) {
        lcd.setCursor(5, 3);
        lcd.write("Crapgadget");
      }
    }


    lcd.clear();

    //lcd.setCursor(3,0);     lcd.write("W0FXD");
    //  lcd.setCursor(7,1);   lcd.write("W0ZE");
    //  lcd.setCursor(10,2);  lcd.write("TimeKube");
    lcd.setCursor(0, 0);
    lcd.write("Loading:");
    delay(1000);
    for (int num = 8; num < 20; num++) {
      DF1201S.setVol((analogRead(A0) * (40) / 1023));
      lcd.setCursor(num, 0);
      // lcd.write(5);
      lcd.write(byte(255));
      delay(700);
      if (num == 8) {
        lcd.setCursor(1, 1);
        lcd.write("Arduino");
      }
      if (num == 11) {
        lcd.setCursor(1, 2);
        lcd.write("WWVB Receiver");
      }


      if (num == 13) {
        lcd.setCursor(1, 3);
        lcd.write("GPS");
      }
      if (num == 15) {
        lcd.setCursor(5, 3);
        lcd.write("RTC");
      }
      if (num == 18) {
        lcd.setCursor(9, 3);
        lcd.write("Audio");
      }
    }
    //if (splash == 1) {
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    delay(1000);
    lcd.clear();
    int code_delay = 750;
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    //Startup Splash Screen
    DF1201S.playSpecFile("/W0FXD.mp3");
    analogWrite(8, 255);  // 128 is approximately 50% of 255
    lcd.setCursor(0, 0);
    delay(500);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print(" W");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("0");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("F");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("X");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("D");
    delay(2000);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));

    DF1201S.playSpecFile("/W0ZE.mp3");
    lcd.setCursor(4, 1);
    delay(500);
    lcd.print(" W");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("0");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("Z");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("E");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    delay(1000);

    // //delay(2000);

    DF1201S.playSpecFile("/KB0BEI.mp3");
    lcd.setCursor(4, 1);
    delay(500);
    lcd.setCursor(7, 2);

    lcd.print(" K");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("B");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("0");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("B");
    delay(code_delay);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("E");
    delay(500);
    DF1201S.setVol((analogRead(A0) * (40) / 1023));
    lcd.print("I");

    delay(1000);



    lcd.setCursor(12, 3);
    lcd.print("HomeBrew");
    delay(2000);
    // for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    //   // scroll one position left:
    //   lcd.scrollDisplayLeft();
    //   lcd.clear();
    // }
  }
  //delay(2000);
  lcd.clear();

  lcd_pot_value = analogRead(A1);
  lcd_bright = ((lcd_pot_value * (255) / 1023));
  lcd_bright_dimm = lcd_bright / 3;
  analogWrite(8, lcd_bright);  // 128 is approximately 50% of 255
}

void loop() {

  GPSloop_time();
  eb1.update();
  // onEb1Encoder();

  void getDateTime(uint32_t t);
  DateTime now = rtc.now();
  uint32_t utc;
  uint32_t utc_gps;
  uint32_t utc_gps_unix;
  uint32_t utc_diff;

  time_t local;
  float range;
  DateTime local_datetime;
  int local_dayofweek;
  float range_great_circle;
  float r;
  r = 3963.1;
  float range_great_circle_tof;


  utc = now.unixtime();

  //------------------------------Temperature display - Affichage de la température-----------------------------//



  value = page_number;


  if ((value != last)) {
    lcd.clear();
    last = value;
    delay(100);
    screen_saver = now.minute();
  }

  if ((page_number > 1) && (abs(screen_saver - now.minute()) > screen_saver_timeout)) {
    page_number = 0;
    lcd.clear();
  }  // Screen saver puts back to clock after 1 min


  TimeChangeRule *tcr;  // pointer to the time change rule, use to get the TZ abbrev

  //   time_t local = tz.toLocal(utc, &tcr);
  if (timezone_count_num == 0) {
    local = UTC.toLocal(utc, &tcr);
    printDateTime(local, tcr->abbrev);
  }
  if (timezone_count_num == 1) {
    local = usET.toLocal(utc, &tcr);
    printDateTime(local, tcr->abbrev);
  }
  if (timezone_count_num == 2) {
    local = usCT.toLocal(utc, &tcr);
    printDateTime(local, tcr->abbrev);
  }
  if (timezone_count_num == 3) {
    local = usMT.toLocal(utc, &tcr);
    printDateTime(local, tcr->abbrev);
  }
  if (timezone_count_num == 4) {
    local = usAZ.toLocal(utc, &tcr);
    printDateTime(local, tcr->abbrev);
  }
  if (timezone_count_num == 5) {
    local = usPT.toLocal(utc, &tcr);
    printDateTime(local, tcr->abbrev);
  }
  if (timezone_count_num == 6) {
    local = CE.toLocal(utc, &tcr);
    printDateTime(local, tcr->abbrev);
  }
  if (timezone_count_num == 7) {
    local = UK.toLocal(utc, &tcr);
    printDateTime(local, tcr->abbrev);
  }

  stamp.getDateTime(local);

  // Set time once per day from NIST, at 0 UTC / 6 EDT
  //if ((now.hour() == 6) && (now.minute() == 1)) {

  // Time set daily XXX (bookmark)

  if ((stamp.hour == 0) && (stamp.minute == 1)) {
    //------------------Let's get the time
    Serial.println("Entering check time loop");
    Serial.print("Count: ");
    Serial.println(count);
    Serial.print(stamp.hour);
    Serial.print(":");
    Serial.println(stamp.minute);

    stamp.getDateTime(local);
    lcd.clear();
    lcd.setCursor(1, 1);
    lcd.print("WWVb Time Sync");
    // local variables
    int current_timer_value;
    int dt_array[DT_LENGTH];
    // initialize MCU
    mcu_init();
    // initialize gpios
    mcu_gpio_set_low(GPIO_EN);

    current_timer_value = es100_receive(dt_array);
    if (count < attempt) { sync_type = 1; }
    if ((time_inIRQ < time_inIRQ_limit) && (sync_type == 1)) {
      wwvb_timestamp = rtc.now();
      //      Serial.println(wwvb_timestamp);
    }
    Serial.println("After receive attempt");
    Serial.print("Count: ");
    Serial.println(count);
    Serial.print(stamp.hour);
    Serial.print(":");
    Serial.println(stamp.minute);

    if (count == attempt) {
      if (fix.satellites > 3) {
        lcd.clear();
        lcd.setCursor(1, 1);
        lcd.print("GPS Time Sync");
        GPSloop_time();
        utc = now.unixtime();

        rtc.adjust(DateTime(fix.dateTime.year, fix.dateTime.month, fix.dateTime.date, fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds));
        Serial.println("In GPS backup set condition");
        Serial.print("Count: ");
        Serial.println(count);
        Serial.print(stamp.hour);
        Serial.print(":");
        Serial.println(stamp.minute);
        //    rtc.adjust(DateTime(fix.dateTime.year, fix.dateTime.month, fix.dateTime.date, fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds));
        Serial.println(fix.dateTime.minutes);
        sync_type = 2;
        wwvb_timestamp = rtc.now();
        delay(3000);
      }
    }
    //}

    lcd.clear();  // clear the screen, otherwise leaves odd artifact on day of week
    //lcd.backlight();


  }  //end IF for time set




  switch (page_number) {
    case 0:  //Main UTC/Local time screen with

      // dimmer - print moon char if between 10 and 7
      if ((stamp.hour > 21) || (stamp.hour < 7)) {
        lcd.setCursor(0, 0);
        lcd.write(9999);
        analogWrite(8, lcd_bright_dimm);  // set to dimm value
      } else {
        analogWrite(8, lcd_bright);
        lcd.setCursor(0, 0);
        lcd.print(" ");
      }

      TimeChangeRule *tcr;  // pointer to the time change rule, use to get the TZ abbrev

      //   time_t local = tz.toLocal(utc, &tcr);
      if (timezone_count_num == 0) {
        local = UTC.toLocal(utc, &tcr);
        printDateTime(local, tcr->abbrev);
      }
      if (timezone_count_num == 1) {
        local = usET.toLocal(utc, &tcr);
        printDateTime(local, tcr->abbrev);
      }
      if (timezone_count_num == 2) {
        local = usCT.toLocal(utc, &tcr);
        printDateTime(local, tcr->abbrev);
      }
      if (timezone_count_num == 3) {
        local = usMT.toLocal(utc, &tcr);
        printDateTime(local, tcr->abbrev);
      }
      if (timezone_count_num == 4) {
        local = usAZ.toLocal(utc, &tcr);
        printDateTime(local, tcr->abbrev);
      }
      if (timezone_count_num == 5) {
        local = usPT.toLocal(utc, &tcr);
        printDateTime(local, tcr->abbrev);
      }
      if (timezone_count_num == 6) {
        local = CE.toLocal(utc, &tcr);
        printDateTime(local, tcr->abbrev);
      }
      if (timezone_count_num == 7) {
        local = UK.toLocal(utc, &tcr);
        printDateTime(local, tcr->abbrev);
      }

      stamp.getDateTime(local);

      if (event_checked == 0) {  //Put this back in once done testing
                                 //if ((stamp.hour != 0)) {  // Take this out once one testing
                                 // lcd.setCursor(6, 0);
                                 // lcd.print("         ");
        event_today = 0;
        int iii = 0;

        while (iii < eventnumber) {
          if (calculateDayOfYear(events[iii].day, events[iii].month, stamp.year) == calculateDayOfYear(stamp.day, stamp.month, stamp.year)) {
            event_today = iii;
          }
          iii++;
        }

        Serial.print("event_today: ");
        Serial.print(calculateDayOfYear(events[event_today].day, events[event_today].month, stamp.year));
        Serial.print(" current day num: ");
        Serial.println(calculateDayOfYear(stamp.day, stamp.month, stamp.year));
        event_checked = 1;
      }




      if ((timezone_count_num == 0) && (event_today == 0)) {
        lcd.setCursor(0, 3);
        lcd.print("Press for Local      ");
      }


      currentMillis = millis();  // Get the current time
      if ((event_today != 0) && (timezone_count_num == 0)) {

        if (stamp.second == 0) {
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            lcd.setCursor(0, 3);
            lcd.print("                   ");
          }
        }

        if ((stamp.second >= 6) && (stamp.second <= 9)) {
          if (stamp.second == 6) {
            if (currentMillis - previousMillis >= interval) {
              previousMillis = currentMillis;
              lcd.setCursor(0, 3);
              lcd.print("                   ");
            }

            lcd.setCursor(0, 3);
            lcd.print("Press for Local      ");
          }
        } else {
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            if (stamp.second == 10) {
              lcd.setCursor(0, 3);
              lcd.print("                   ");
            }
            lcd.setCursor(0, 3);
            lcd.print(events[event_today].description);
          }
        }
      }

      if ((event_today != 0) && (timezone_count_num != 0)) {
        if (currentMillis - previousMillis >= interval) {
          previousMillis = currentMillis;

          lcd.setCursor(0, 3);
          lcd.print(events[event_today].description);
        }
      }
      //}



      lcd.setCursor(6, 0);
      local_dayofweek = weekday(local);
      lcd.print(daysOfTheWeek[local_dayofweek - 1]);

      if ((stamp.hour == 0) && (stamp.second == 0)) {
        lcd.setCursor(6, 0);
        lcd.print("         ");
        event_checked = 0;
        event_today = 0;
      }

      lcd.setCursor(0, 1);
      lcd.print("Date:");
      printYear(6, 1, stamp.year);
      lcd.setCursor(10, 1);
      lcd.print(":");
      printTwoDigit(11, 1, stamp.month);
      lcd.setCursor(13, 1);
      lcd.print(":");
      printTwoDigit(14, 1, stamp.day);

      //------------------------------Time dsiplay - Affichage du temps-----------------------------//
      lcd.setCursor(0, 2);
      lcd.print("Time:");
      printTwoDigit(6, 2, stamp.hour);
      lcd.setCursor(8, 2);
      lcd.print(":");
      printTwoDigit(9, 2, stamp.minute);
      lcd.setCursor(11, 2);
      lcd.print(":");
      printTwoDigit(12, 2, stamp.second);







      break;



    case 1:  // Time in UTC


      if ((stamp.hour == 0) && (stamp.hour == 0) && (stamp.second == 0)) {
        lcd.setCursor(6, 0);
        lcd.print("         ");
      }

      // dimmer - print moon char if between 10 and 7
      if ((stamp.hour > 21) || (stamp.hour < 7)) {
        lcd.setCursor(0, 0);
        lcd.write(9999);
        analogWrite(8, lcd_bright_dimm);  // set to dimm value
      } else {
        analogWrite(8, lcd_bright);
        lcd.setCursor(0, 0);
        lcd.print(" ");
      }

      //------------------------------Date display - Affichage de la date-----------------------------//

      lcd.setCursor(6, 0);
      lcd.print(daysOfTheWeek[now.dayOfTheWeek()]);
      lcd.setCursor(15, 0);
      lcd.print("(UTC)");
      lcd.setCursor(0, 1);
      lcd.print("Date:");
      printYear(6, 1, now.year());
      lcd.setCursor(10, 1);
      lcd.print(":");
      printTwoDigit(11, 1, now.month());
      lcd.setCursor(13, 1);
      lcd.print(":");
      printTwoDigit(14, 1, now.day());

      //------------------------------Time dsiplay - Affichage du temps-----------------------------//

      lcd.setCursor(0, 2);
      lcd.print("Time:");
      printTwoDigit(6, 2, now.hour());
      lcd.setCursor(8, 2);
      lcd.print(":");
      printTwoDigit(9, 2, now.minute());
      lcd.setCursor(11, 2);
      lcd.print(":");
      printTwoDigit(12, 2, now.second());


      if (rtc_toggle == 1) {
        lcd.setCursor(0, 3);
        lcd.print("RTC Temp: ");
        lcd.setCursor(11, 3);
        // lcd.print(rtc.getTemperature());
        // lcd.print((char)223);  // degree symbol
        // lcd.print("C");
        lcd.print((rtc.getTemperature() * 9 / 5) + 32);
        lcd.print((char)223);  // degree symbol
        lcd.print("F");
      }


      break;




    case 2:  // ------------------ Last Sync Time ------------------
      lcd.setCursor(0, 0);
      lcd.print("Last Sync: ");
      lcd.setCursor(11, 0);
      if (sync_type == 1) { lcd.print("(WWVB)"); }
      if (sync_type == 2) { lcd.print("(GPS)"); }
      lcd.setCursor(0, 1);
      lcd.print("Date:");
      printYear(6, 1, wwvb_timestamp.year());
      lcd.setCursor(10, 1);
      lcd.print(":");
      printTwoDigit(11, 1, wwvb_timestamp.month());
      lcd.setCursor(13, 1);
      lcd.print(":");
      printTwoDigit(14, 1, wwvb_timestamp.day());


      lcd.setCursor(0, 2);
      lcd.print("Time:");
      printTwoDigit(6, 2, wwvb_timestamp.hour());
      lcd.setCursor(8, 2);
      lcd.print(":");
      printTwoDigit(9, 2, wwvb_timestamp.minute());
      lcd.setCursor(11, 2);
      lcd.print(":");
      printTwoDigit(12, 2, wwvb_timestamp.second());


      lcd.setCursor(16, 2);
      lcd.print("UTC");

      lcd.setCursor(0, 3);
      lcd.print("Press to Sync to WWVB");

      break;


      /////////// Clock compare

    case 3:            // ------------------ GPS Time------------------
GPSloop_time();
lcd.setCursor(0, 0);
lcd.print("Clock Compare (UTC)");

// GPS Clock
lcd.setCursor(0, 1);
lcd.print("GPS:");
printTwoDigit(6, 1, fix.dateTime.hours);
lcd.setCursor(8, 1);
lcd.print(":");
printTwoDigit(9, 1, fix.dateTime.minutes);
lcd.setCursor(11, 1);
lcd.print(":");
printTwoDigit(12, 1, fix.dateTime.seconds);

// RTC Clock
lcd.setCursor(0, 2);
lcd.print("RTC:");
printTwoDigit(6, 2, now.hour());
lcd.setCursor(8, 2);
lcd.print(":");
printTwoDigit(9, 2, now.minute());
lcd.setCursor(11, 2);
lcd.print(":");
printTwoDigit(12, 2, now.second());

      // Compare RTC to GPS



      utc_diff = utc - utc_gps;

      lcd.setCursor(0, 3);
      lcd.print("Press to Sync to GPS");
      break;

    case 4:  // ------------------ gps coords ------------------

      currentMillis = millis();  // Get the current time

      // Check if the interval has passed

      switch (coord_sys) {

        case 1:
          // if (coord_sys == 0) {
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            GPSloop_loc();  //Go get GPS location


            lcd.setCursor(0, 0);
            lcd.print("GPS Coords (DD)DMS");


            lcd.setCursor(0, 1);
            lcd.print("Lat:");

            //Latitude print checking for digits and sign

            lcd.setCursor(7, 1);

            // // scaling to put the decimal after lat
            // int scale_lat;
            // int scale_lat_after_decimal;

            // scale_lat = ((int)(fix.latitudeL() / 1E7));
            // lcd.print(scale_lat);
            // lcd.setCursor(9, 1);
            // lcd.print(".");
            // lcd.setCursor(10, 1);
            // scale_lat_after_decimal = (int)(fix.latitudeL()) - (1E7 * scale_lat);
            // lcd.print(scale_lat_after_decimal);

            // // scaling to put the decimal after long
            // // int scale_long;
            // //int scale_long_after_decimal;
            // long min_long = fix.latitudeDMS.minutes;


            //XXXXXXXXX

            long int number = fix.latitudeDMS.degrees;  // Can handle large numbers
            int count = 0;
            long int temp = number;  // Use a temporary variable so the original number is preserved

            if (temp == 0) {
              count = 1;  // Zero has one digit
            } else {
              // Handle negative numbers by converting to positive
              if (temp < 0) {
                temp = -temp;
              }
              while (temp > 0) {
                temp = temp / 10;  // Integer division removes the last digit
                count++;           // Increment the count for each digit removed
              }
            }
            lcd.setCursor(0, 1);
            lcd.print("Lat:");
            //determine the sign of the coordinate
            int sign = 0;
            number = fix.latitudeL();
            if (number > 0) {
              sign = 1;  // Positive
            } else if (number < 0) {
              sign = -1;  // Negative
            } else {
              sign = 0;  // Zero
            }

            if (count == 2) {

              if (sign < 0) {
                lcd.setCursor(6, 1);
                lcd.print("-");
              }


              lcd.setCursor(7, 1);
              // scale_long = ((int)(fix.longitudeL() / 1E6));
              int scale_long = ((fix.latitudeL()));

              long scale_long_before_decimal = 0;
              int scale_long_after_decimal = 0;
              scale_long_before_decimal = (((int)(fix.latitudeDMS.degrees * 1E7)));
              scale_long_after_decimal = ((fix.latitudeL() - scale_long_before_decimal * (sign)));
              lcd.print(((int)fix.latitudeDMS.degrees));
              lcd.setCursor(9, 1);
              lcd.print(".");
              lcd.print(scale_long_after_decimal * sign);
            }


            if (count == 3) {

              if (sign < 0) {
                lcd.setCursor(6, 1);
                lcd.print("-");
              } else {
                lcd.setCursor(6, 1);
                lcd.print("+");
              }
              lcd.setCursor(6, 1);
              // scale_long = ((int)(fix.longitudeL() / 1E6));
              int scale_long = ((fix.latitudeL()));
              long scale_long_before_decimal = 0;
              int scale_long_after_decimal = 0;
              scale_long_before_decimal = (((int)(fix.latitudeDMS.degrees * 1E7)));
              scale_long_after_decimal = ((fix.latitudeL() - scale_long_before_decimal * (sign)));
              lcd.print(((int)fix.latitudeDMS.degrees));
              lcd.setCursor(8, 1);
              lcd.print(".");
              lcd.print(scale_long_after_decimal * sign);
            }

            //Longitude

            //determine the number of digits in the degrees
            number = fix.longitudeDMS.degrees;  // Can handle large numbers
            count = 0;
            temp = number;  // Use a temporary variable so the original number is preserved

            if (temp == 0) {
              count = 1;  // Zero has one digit
            } else {
              // Handle negative numbers by converting to positive
              if (temp < 0) {
                temp = -temp;
              }
              while (temp > 0) {
                temp = temp / 10;  // Integer division removes the last digit
                count++;           // Increment the count for each digit removed
              }
            }
            lcd.setCursor(0, 2);
            lcd.print("Long:");
            Serial.print("Number of digits (long): ");
            Serial.println(count);

            //determine the sign of the coordinate
            sign = 0;
            number = fix.longitudeL();
            if (number > 0) {
              sign = 1;  // Positive
            } else if (number < 0) {
              sign = -1;  // Negative
            } else {
              sign = 0;  // Zero
            }

            if (count == 2) {

              if (sign < 0) {
                lcd.setCursor(6, 2);
                lcd.print("-");
              }


              lcd.setCursor(7, 2);
              // scale_long = ((int)(fix.longitudeL() / 1E6));
              int scale_long = ((fix.longitudeL()));

              long scale_long_before_decimal = 0;
              int scale_long_after_decimal = 0;
              scale_long_before_decimal = (((int)(fix.longitudeDMS.degrees * 1E7)));
              scale_long_after_decimal = ((fix.longitudeL() - scale_long_before_decimal * (sign)));
              lcd.print(((int)fix.longitudeDMS.degrees));

              lcd.setCursor(9, 2);
              lcd.print(".");
              lcd.print(scale_long_after_decimal * sign);
            }


            if (count == 3) {

              if (sign < 0) {
                lcd.setCursor(5, 2);
                lcd.print("-");
              }
              lcd.setCursor(6, 2);
              // scale_long = ((int)(fix.longitudeL() / 1E6));
              int scale_long = ((fix.longitudeL()));
              long scale_long_before_decimal = 0;
              int scale_long_after_decimal = 0;
              scale_long_before_decimal = (((int)(fix.longitudeDMS.degrees * 1E7)));
              scale_long_after_decimal = ((fix.longitudeL() - scale_long_before_decimal * (sign)));
              lcd.print(((int)fix.longitudeDMS.degrees));
              lcd.setCursor(9, 2);
              lcd.print(".");
              lcd.print(scale_long_after_decimal * sign);
            }

            lcd.setCursor(0, 3);
            lcd.print("Alt(ft):");
            lcd.setCursor(9, 3);
            lcd.print(fix.altitude_ft());
          }
          break;
          // }


        case 0:
          //  if(coord_sys == 1) {
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;

            GPSloop_loc_DMS();  //Go get GPS location

            lcd.setCursor(0, 0);
            lcd.print("GPS Coords DD(DMS)");

            // Print Latitude in DMS format
            lcd.setCursor(0, 1);
            lcd.print("Lat:");
            lcd.setCursor(4, 1);
            lcd.print(fix.latitudeDMS.degrees);

            lcd.print((char)223);  // degree symbol
            lcd.print(fix.latitudeDMS.minutes);
            lcd.print('\'');                           // minute symbol
            lcd.print(fix.latitudeDMS.secondsF(), 3);  // seconds as float with 3 decimal places
            lcd.print('"');                            // second symbol
            lcd.setCursor(19, 1);
            lcd.print(fix.latitudeDMS.NS());  // Hemisphere (N/S)

            // // Print Longitude in DMS format
            lcd.setCursor(0, 2);
            lcd.print("Lon:");
            lcd.setCursor(4, 2);

            lcd.print(fix.longitudeDMS.degrees);
            lcd.print((char)223);  // degree symbol
            lcd.print(fix.longitudeDMS.minutes);
            lcd.print('\'');
            lcd.print(fix.longitudeDMS.secondsF(), 3);
            lcd.print('"');
            lcd.setCursor(19, 2);
            lcd.print(fix.longitudeDMS.EW());  // Hemisphere (E/W)

            lcd.setCursor(0, 3);
            lcd.print("Alt(ft):");
            lcd.setCursor(9, 3);
            lcd.print(fix.altitude_ft());
          }
          break;
          // }

          //   if (coord_sys == 2) {
        case 2:

          GPSloop_loc_Vel();  //Go get GPS location

          lcd.setCursor(0, 0);
          lcd.print("GPS (Vel)");
          lcd.setCursor(11, 0);
          lcd.print("Sats: ");
          lcd.print(fix.satellites);



          //kn
          lcd.setCursor(0, 1);
          lcd.print(fix.speed());
          lcd.setCursor(6, 1);
          lcd.print("(kn)");

          //km/hr
          lcd.setCursor(0, 2);
          lcd.print(fix.speed_kph());
          lcd.setCursor(6, 2);
          lcd.print("(km/hr)");

          //mph
          lcd.setCursor(0, 3);
          lcd.print(fix.speed_mph());
          lcd.setCursor(6, 3);
          lcd.print("(mph)");


          break;

        // velocity components in the North, East and Down directions, accessed with
        // fix.velocity_north, in integer cm/s
        // fix.velocity_east, in integer cm/s
        // fix.velocity_down, in integer cm/s

        case 3:
          //     if (coord_sys == 3) {

          GPSloop_loc_Vel();  //Go get GPS location

          lcd.setCursor(0, 0);
          lcd.print("GPS (Vel - Spacial)");

          //north
          velNorth_cms = fix.velocity_north;
          lcd.setCursor(0, 1);
          lcd.print("(north)");
          lcd.setCursor(8, 1);
          lcd.print(velNorth_cms);

          //east
          lcd.setCursor(0, 2);
          lcd.print("(east)");
          lcd.setCursor(8, 2);
          lcd.print(fix.velocity_east);

          //down
          lcd.setCursor(0, 3);
          lcd.print("(down)");
          lcd.setCursor(8, 3);
          lcd.print(fix.velocity_down);

          break;
      }


      break;

    case 5:
      // ---------------------DST Transition Info ---------------------

      // int data_dst_next_month = (uint8_t)dst_next_month;
      // int data_dst_next_day = (uint8_t)dst_next_day;
      // int data_dst_next_hour
      lcd.setCursor(0, 0);
      lcd.print("Next DST Update");

      lcd.setCursor(0, 1);
      lcd.print("Month: ");

      lcd.setCursor(7, 1);
      lcd.print(data_dst_next_month, DEC);


      lcd.setCursor(0, 2);
      lcd.print("Day: ");

      lcd.setCursor(7, 2);
      if (now.day() <= 9) {
        lcd.print("0");
        lcd.setCursor(8, 2);
        lcd.print(data_dst_next_day, DEC);
      } else {
        lcd.print(data_dst_next_day, DEC);
      }

      //------------------------------DST Transition Information-----------------------------//

      lcd.setCursor(0, 3);
      lcd.print("Hour: ");
      lcd.setCursor(7, 3);
      lcd.print(data_dst_next_hour, DEC);
      break;

    case 6:
      // ---------------------Ft. Collins Distance Info---------------------
      lcd.setCursor(0, 0);
      lcd.print("Dist/TOF(Ft.Collins)");
      // When we have a location, calculate how far away we are from the base location.
      if (fix.valid.location) {
        range = fix.location.DistanceMiles(base);
      }
      // float range = fix.location.DistanceMiles( base );

      lcd.setCursor(0, 1);
      lcd.print("Dist(miles):");
      lcd.setCursor(13, 1);
      lcd.print(range);

      range_great_circle = 2 * r * asin(range / (2 * r));
      lcd.setCursor(0, 2);
      lcd.print("GC(miles):");
      lcd.setCursor(13, 2);
      lcd.print(range_great_circle);

      range_great_circle_tof = range_great_circle / 186282;
      lcd.setCursor(0, 3);
      lcd.print("TOF(sec):");
      lcd.setCursor(13, 3);
      lcd.print(range_great_circle_tof);
      break;

    case 7:
      //lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Events");
      lcd.setCursor(0, 1);
      lcd.print("Press to Review");


      lcd.setCursor(0, 2);
      lcd.print("Brightness");
      lcd.setCursor(0, 3);
      lcd.print("Rotate to Set");


      lcd_pot_value = analogRead(A1);
      lcd_bright = ((lcd_pot_value * (255) / 1023));
      analogWrite(8, lcd_bright);  // 128 is approximately 50% of 255

      //    int barLength = map(lcd_pot_value, 0, 1023, 0, 7);

      // Clear the bar graph row to redraw it
      lcd.setCursor(13, 3);
      for (int i = 1; i < 8; i++) {
        lcd.print(" ");  // Print spaces to clear the previous bar
      }
      lcd.setCursor(13, 3);
      for (int i = 1; i < lcd_bright / (255 / 8); i++) {
        lcd.write(byte(255));  // Print a solid block character (ASCII 255)
      }


      break;

    case 8:

      lcd.setCursor(0, 0);
      lcd.print("WWV (Ft.Collins, CO)");
      lcd.setCursor(0, 2);
      lcd.print("UTC:");
      printTwoDigit(6, 2, now.hour());
      lcd.setCursor(8, 2);
      lcd.print(":");
      printTwoDigit(9, 2, now.minute());
      lcd.setCursor(11, 2);
      lcd.print(":");
      printTwoDigit(12, 2, now.second());

      //delay(2000);
      now = rtc.now();

      tick = now.second();

      if (tick == 0) {

        time_value = now.minute();

        lcd.setCursor(8, 2);
        lcd.print(":");
        printTwoDigit(9, 2, time_value);

        lcd.setCursor(12, 2);
        lcd.print("00");

        DF1201S.playSpecFile("/t/1_5k.mp3");
        if (DF1201S.isPlaying()) {
          //  Serial.println("0 tone");
        } else {
          lcd.setCursor(12, 2);
          lcd.print("01");
        }
      }
      now = rtc.now();
      tick = now.second();

      if ((tick > 0) && (tick < 44)) {

        lcd.setCursor(0, 3);
        lcd.print("Press for Ident");

        DF1201S.playSpecFile("/t/v_440.mp3");
        //     if(DF1201S.isPlaying()){Serial.println("1-44 tone");}else{ Serial.println("no 1-44 tone"); }
        while (((now.second() > 0) && (now.second() < 44))) {

          //int vol_pot_value = analogRead(A0);
          // int vol = ((analogRead(A0)* (40) / 1023));
          DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
          //break if encoder changes
          eb1.update();
          value = page_number;
          if ((value != last)) {
            lcd.clear();
            last = value;
            DF1201S.pause();
            break;
          }

          now = rtc.now();

          tick = now.second();
          printTwoDigit(12, 2, now.second());
        }
      }

      now = rtc.now();
      tick = now.second();

      if ((tick > 44) && (tick < 51)) {
        DF1201S.playSpecFile("/t/al.mp3");
        //delay(4500);
        //      if(DF1201S.isPlaying()){Serial.println("tick tone");}else{ Serial.println("no tick tone"); }
        now = rtc.now();
        tick = now.second();

        while (((tick > 44) && (tick < 51))) {
          //break if encoder changes
          eb1.update();
          value = page_number;


          // monitor volume pot, set volume
          DF1201S.setVol(((analogRead(A0) * (40) / 1023)));


          if ((value != last)) {
            DF1201S.pause();
            break;
            lcd.clear();
            last = value;
          }

          now = rtc.now();
          tick = now.second();
          printTwoDigit(12, 2, now.second());
        }

        // delay(50000 - (tick * 1000));
      }
      now = rtc.now();

      tick = now.second();

      if ((tick > 51) && (tick < 56)) {
        lcd.setCursor(0, 3);
        lcd.print("                ");

        DF1201S.playSpecFile("/v/attone.mp3");
        // delay(1500);
        //   if(DF1201S.isPlaying()){Serial.println("att");}else{ Serial.println("no att tone"); }


        // monitor volume pot, set volume
        DF1201S.setVol(((analogRead(A0) * (40) / 1023)));


        while (((tick > 51) && (tick < 53))) {
          //delay(900);
          // DateTime now = rtc.now();
          now = rtc.now();
          tick = now.second();
          printTwoDigit(12, 2, now.second());
        }

        time_value = now.hour();
        // time_value_min=now.minute();
        if ((now.hour() == 23) && (now.minute() == 59)) {
          time_value = 0;
        } else {
          if (now.minute() == 59) { time_value++; }
        }

        snprintf(finalString, sizeof(finalString), "/v/%d.mp3", time_value);
        DF1201S.playSpecFile(finalString);
        //Serial.print("Hours:");
        // Serial.println(finalString);
        delay(700);

        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        DF1201S.playSpecFile("/v/hours.mp3");
        delay(1000);


        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());


        time_value = now.minute();
        if (time_value == 59) {
          time_value = 0;
        } else {
          time_value++;
        }
        snprintf(finalString, sizeof(finalString), "/v/%d.mp3", time_value);
        DF1201S.playSpecFile(finalString);
        //Serial.print("Minute:");
        //Serial.println(finalString);
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        delay(800);
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        DF1201S.playSpecFile("/v/mins.mp3");

        delay(800);
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        DF1201S.playSpecFile("/v/utc.mp3");
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());





        //delay(900);
      }



      break;

      case 9:
     lcd.setCursor(0, 0);
      lcd.print("WWVH (Kauai, Hawaii)");
      // monitor volume pot, set volume
      DF1201S.setVol(((analogRead(A0) * (40) / 1023)));

      lcd.setCursor(0, 2);
      lcd.print("UTC:");
      printTwoDigit(6, 2, now.hour());
      lcd.setCursor(8, 2);
      lcd.print(":");
      printTwoDigit(9, 2, now.minute());
      lcd.setCursor(11, 2);
      lcd.print(":");
      printTwoDigit(12, 2, now.second());

      now = rtc.now();
      tick = now.second();

           // --- Second 0: top-of-minute 1.5kHz tone ---
      if (tick == 0) {

        time_value = now.minute();

        lcd.setCursor(8, 2);
        lcd.print(":");
        printTwoDigit(9, 2, time_value);

        lcd.setCursor(12, 2);
        lcd.print("00");

        DF1201S.playSpecFile("/t/1_5k.mp3");
        if (DF1201S.isPlaying()) {
          // Serial.println("0 tone");
        } else {
          lcd.setCursor(12, 2);
          lcd.print("01");
        }
      }


      now = rtc.now();
      tick = now.second();

      // --- Seconds 1-43: 440Hz continuous tone ---
      if ((tick > 0) && (tick < 44)) {

        lcd.setCursor(0, 3);
        lcd.print("Press for Ident");

        DF1201S.playSpecFile("/t/v_440.mp3");
        while (((now.second() > 0) && (now.second() < 44))) {
          // monitor volume pot, set volume
          DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
          // break if encoder changes
          eb1.update();
          value = page_number;
          if ((value != last)) {
            lcd.clear();
            last = value;
            DF1201S.pause();
            break;
          }

          now = rtc.now();
          tick = now.second();
          printTwoDigit(12, 2, now.second());
        }
      }


      now = rtc.now();
      tick = now.second();

      // --- Seconds 45-50: alert tone ---
      if ((tick > 44) && (tick < 51)) {

        DF1201S.playSpecFile("/t/al.mp3");
        now = rtc.now();
        tick = now.second();

        while (((tick > 44) && (tick < 51))) {
          // monitor volume pot, set volume
          DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
          // break if encoder changes
          eb1.update();
          value = page_number;
          if ((value != last)) {
            DF1201S.pause();
            lcd.clear();
            last = value;
            break;
          }

          now = rtc.now();
          tick = now.second();
          printTwoDigit(12, 2, now.second());
        }
      }

      now = rtc.now();
      tick = now.second();

      if ((tick > 51) && (tick < 56)) {
        // monitor volume pot, set volume
        DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
        lcd.setCursor(0, 3);
        lcd.print("                ");

        DF1201S.playSpecFile("/h/attone.mp3");

        while (((tick > 51) && (tick < 53))) {
          // monitor volume pot, set volume
          DF1201S.setVol(((analogRead(A0) * (40) / 1023)));
          //delay(900);
          // DateTime now = rtc.now();
          now = rtc.now();
          tick = now.second();
          printTwoDigit(12, 2, now.second());
        }


        // Find Hour and create string to call mp3
        time_value = now.hour();
        // time_value_min=now.minute();
        if ((now.hour() == 23) && (now.minute() == 59)) {
          time_value = 0;
        } else {
          if (now.minute() == 59) { time_value++; }
        }

        snprintf(finalString, sizeof(finalString), "/h/%d.mp3", time_value);
        DF1201S.playSpecFile(finalString);
        //Serial.print("Hours:");
        // Serial.println(finalString);
        delay(700);

        //Update clock before calling minutes
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());




        DF1201S.playSpecFile("/h/hours.mp3");
        delay(1000);

        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        // Find minutes and create string to call mp3
        time_value = now.minute();
        if (time_value == 59) {
          time_value = 0;
        } else {
          time_value++;
        }
        snprintf(finalString, sizeof(finalString), "/h/%d.mp3", time_value);
        DF1201S.playSpecFile(finalString);
        //Serial.print("Minute:");
        //Serial.println(finalString);
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        delay(800);
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        DF1201S.playSpecFile("/h/mins.mp3");

        delay(800);
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        DF1201S.playSpecFile("/h/utc.mp3");
        now = rtc.now();
        tick = now.second();
        printTwoDigit(12, 2, now.second());

        break;
      }
  }
}

// format and print a time_t value, with a time zone appended.
// format and print a time_t value, with a time zone appended.
void printDateTime(time_t t, const char *tz) {
  char buf[5];

  sprintf(buf, "%s", tz);
  // Serial.println(buf);
  // timezone_local = tz;
  if (page_number == 0) {
    lcd.setCursor(15, 0);
    lcd.print("(");
    lcd.print(tz);
    lcd.setCursor(19, 0);
    lcd.print(")");
  }
}
