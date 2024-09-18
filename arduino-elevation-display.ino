// Elevation display by Dan Anderson
// Forked from https://github.com/klinquist/Arduino-Elevation-Display by Kris Linquist

// Tested and works great with the Adafruit Mini GPS PA1010D Breakout board
// using MTK3333 chipset
//    ------> https://www.adafruit.com/product/4415

//And the Adafruit 7 segment I2C LCD

#include <Wire.h>
#include <SoftwareSerial.h>

// v1.6.1
#include <TimeLib.h>
// Mild modification required for Sunrise v2.0.4 to get it to compile with Arduino UNO (see README)
#include <SunRise.h>
// v1.5.1
#include "Adafruit_LEDBackpack.h"
// v1.7.5
#include <Adafruit_GPS.h>

Adafruit_7segment matrix = Adafruit_7segment();

SunRise sr;

tmElements_t te;  //Time elements structure
time_t unixTime; // a time stamp


int daytimeBrightness = 10;
int nighttimeBrightness = 0;
int currentBrightness;

// Connect to the GPS on the hardware I2C port (STEMMA QT)
Adafruit_GPS GPS(&Wire);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  false

// Set brightness based on if it's daytime or nighttime
void setBrightness()
{
  te.Second = GPS.seconds;
  te.Hour = GPS.hour;
  te.Minute = GPS.minute;
  te.Day = GPS.day;
  te.Month = GPS.month;
  int actualYear = 2000 + GPS.year;
  te.Year = actualYear - 1970;
  unixTime = makeTime(te);

  float lata = GPS.latitude/100;
  float longi = 0 - (GPS.longitude/100);

  sr.calculate(lata, longi, unixTime);
  Serial.print("Unixtime: ");
  Serial.println(unixTime);

  if (sr.isVisible){
    Serial.println("It's daytime!");
    if (currentBrightness != daytimeBrightness) {
      Serial.println("Setting brightness to day");
      matrix.setBrightness(daytimeBrightness);
      currentBrightness = daytimeBrightness;
    }
  } else {
    Serial.println("It's nighttime!");
    if (currentBrightness != nighttimeBrightness) {
      Serial.println("Setting brightness to night");
      matrix.setBrightness(nighttimeBrightness);
      currentBrightness = nighttimeBrightness;
    }   
  }
}

void setup()
{

  // Print out to serial port at 115200 so we can read the GPS fast enough and echo without dropping chars
  Serial.begin(115200);

  // Default I2C address for display is 0x70
  matrix.begin(0x70);
  matrix.clear();

  Wire.setClock(100000);

  // Just show dashes until we get a GPS fix
  matrix.print("----");
  matrix.writeDisplay();

  Serial.print("Startup: setting brightness to: ");
  Serial.println(nighttimeBrightness);
  matrix.setBrightness(nighttimeBrightness);
  currentBrightness = nighttimeBrightness;
  delay(5000);
 

  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  // The I2C address to use is 0x10
  GPS.begin(0x10);  

  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time

  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);
  // Ask for firmware version
  GPS.println(PMTK_Q_RELEASE);
  
}

uint32_t timer = millis();
void loop()                     // run over and over again
{
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c) && (GPSECHO))
    Serial.write(c);

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, update the display
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {

      setBrightness();

      // Calculate and display altitude
      double doubFt = GPS.altitude * 3.28084;
      // Round to nearest 10
      int mod = (int) doubFt % 10;
      if (mod < 5) {
      doubFt = doubFt - mod;
      } else {
        doubFt = doubFt + (10 - mod);
      }
      int intAltitude = (int)doubFt;

      // If it's greater than 9,999, display in the thousands instead (e.g. 11300 will be 11.3K)
      if (intAltitude > 9999){
        matrix.writeDigitNum(0, (intAltitude / 10000U) % 10);
        matrix.writeDigitNum(1, (intAltitude / 1000U) % 10, true);
        matrix.writeDigitNum(3, (intAltitude / 100U) % 10);
        // This is the ASCII code for the letter "K"
        matrix.writeDigitAscii(4, 75);
      } else {
        matrix.print(intAltitude);
      }
      matrix.writeDisplay(); 

      Serial.print("Altitude: "); Serial.println(intAltitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
    } 
  }
}
