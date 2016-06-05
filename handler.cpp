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
#include "global.h"
#include "handler.h"

// ------------------------------------------------------------------
void printWIFIStrength()
{
  Serial.print("My IP is: ");
  Serial.print(WiFi.localIP());
  Serial.print("Signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}

// ------------------------------------------------------------------
void handleUpdateReply(reply r)
{
  Serial.println("");
  Serial.println("Handling Update Reply");
      
  switch (r.iStatusCd) 
  {
    case 200:
      Serial.println("Request successfull.");
      break;
    case 400:
      Serial.println("Not all parameters were given.");
      break;
    default:
      Serial.println("That status code is not known (yet): '" + String(r.iStatusCd) + "' with line: " + r.sReply);
    break;
  }

  // { "m1": "F0020002F0020002F0020002", "m2": "0F0200020F0200020F020002", "mrc": "0", "cl": "1000" }
  // m1: movie 1, m2: movie 2, mrc: movie replay count, cl: cycle length
  if(r.iStatusCd != 0 && r.sReply.length() > 24)
  {
    DynamicJsonBuffer jsonBuffer;
    Serial.println("JSON Buffer allocated");
    
    JsonObject& root = jsonBuffer.parseObject(r.sReply);
    Serial.println("JSON Object parsed");

    /*
    const char* colourLeft = root["m1"];
    Serial.println("m1 read");

    const char* colourRight = root["m2"];
    Serial.println("m2 read");

    // Movie replay count. 0 for endless
    const short mrc = root["mrc"];
    gs->sMovieReplayCount = mrc;
    Serial.println("mrc read");

    // the possibility to define how long a cycle is
    gs->iCycleLength = root["cl"];
    Serial.println("cl read");

    Serial.print("m1: ");
    Serial.println(colourLeft);
    Serial.print("m2: ");
    Serial.println(colourRight);
    Serial.print("mrc: ");
    Serial.println(gs->sMovieReplayCount);
    Serial.print("cl: ");
    Serial.println(gs->iCycleLength);
    */
  }
  else
  {
    Serial.println("JSON seems not to be valid.");
  }
}

// ------------------------------------------------------------------
void handleTemperatureRead(DallasTemperature sensors)
{
  Serial.print("handleTemperature");

  // Start up the library
  sensors.begin();
  sensors.setResolution(TEMP_12_BIT);

  Serial.print(" Requesting temperatures...");
  
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  sensors.requestTemperatures(); // Send the command to get temperatures

  // sending data out
  for(int i = 0; i < 2; i++)
  {
    float fTemperature = sensors.getTempCByIndex(i);
    
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire
    Serial.print("Temperature for Device ");
    Serial.print(i);
    Serial.print(" is: ");
    Serial.print(fTemperature);
    Serial.println(" C.");

    delay(100);
  
    // Send temperature to server
    String sUrl = "/d/" + sDeviceId + "/" + String(i) + "/temp/" + String(fTemperature, 2) + "/";
    
    Serial.println(sUrl);
  
    reply r = callUrl(sUrl);

    delay(100);
  
    handleUpdateReply(r);
  }
}

//EOF
