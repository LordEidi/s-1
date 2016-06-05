/*
 ====================================================================

 S-1

 --------------------------------------------------------------------
 Copyright (c) 2016 by
 SwordLord - the coding crew - http://www.swordlord.com
 and contributing authors

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Affero General Public License as
 published by the Free Software Foundation, either version 3 of the
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Affero General Public License for more details.

 You should have received a copy of the GNU Affero General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ====================================================================
 */

#include "Esp.h" // deep sleep and system info
#include "global.h"
#include "handler.h"

// ESP8266 SDK headers are in C, they need to be included externally or else 
// they will throw an error compiling/linking.
//
// all SDK .c files required can be added between the { }
//extern "C" {
// #include "user_interface.h"
//}

String sDeviceId = "notinitialised";
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// ------------------------------------------------------------------
// ------------------------------------------------------------------
void setup() 
{
 // start serial port
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("S-1 booting up");

   // --------------------
  // start wifi
  WiFi.mode(WIFI_STA);
  
  // get the wifi up and running
  WiFi.begin(SSID, PWD);

  Serial.print("Connecting to network: ");
  Serial.println(SSID);

  // TODO: this loops until things get connected to the wireless network
  // should we cancel after a few attempts and sleep instead to not drain
  // the battery too much when out of reach?
  unsigned long lStart = millis();
  while (WiFi.status() != WL_CONNECTED && lStart + (20 * SECONDS_TO_MILLIS) > millis()) 
  {
    delay(500);
    Serial.print(".");
  }

  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("");
    Serial.println("Wifi not available, going to sleep.");

    ESP.deepSleep(1 * 10 * 1000000, WAKE_RF_DEFAULT);
    delay(1000);  
  }

    // print out stats
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(" Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  sDeviceId = String(ESP.getChipId()) + "_" + String(ESP.getFlashChipId());
  Serial.println("DeviceId: " + sDeviceId);

  Serial.println("telling the B-1 about our Temperature");

  handleTemperatureRead(sensors);
  
  Serial.println("Everything done, going to DeepSleep");

  ESP.deepSleep(1 * 10 * 1000000, WAKE_RF_DEFAULT);
  delay(1000);  
}

// ------------------------------------------------------------------
// ------------------------------------------------------------------
// not needed
void loop() 
{
}

//EOF
