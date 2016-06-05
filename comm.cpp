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
#include "comm.h"
#include "global.h"
#include <ESP8266WiFi.h>

// ------------------------------------------------------------------
// call URL, return statuscode and JSON
reply callUrl(String sUrl)
{
  reply r;

  r.iStatusCd = 0;
  r.sReply = "";
  
  Serial.print("Connecting to ");
  Serial.println(SERVER);
  
  // Use WiFiClient class to create TCP connections
  // todo: change to WiFiClientSecure when ready so as to make sniffing a bit more complex
  WiFiClient client;

  // todo: we should probably wait here or sleep here until we can connect
  // so we do not drain the battery too much
  if (!client.connect(SERVER, PORT)) {
    Serial.println("Connection failed, trying later...");
    return r;
  }
  
  // We now create a URI for the request
  Serial.print("Requesting URL: ");
  Serial.println(sUrl);
  
  // This will send the request to the server
  client.print(String("GET ") + sUrl + " HTTP/1.1\r\n" + "Host: " + SERVER + "\r\n" + "Connection: close\r\n\r\n");
               
  // just a short delay to give other components the chance to do something else
  delay(50);

  // handling of response
  Serial.println("");
  Serial.println("Response:");
  
  // Read line by line
  while(client.available())
  {
    String sLine = client.readStringUntil('\r');

    // make sure to not have whitespaces so that the very basic parser will not fail
    Serial.print(sLine);
    sLine.trim();

    if (sLine.startsWith("HTTP/1.1 ")) 
    {
        String sStatus = sLine.substring(9, 12);

        // Serial.print("Status: ");
        // Serial.println(sStatus);

        r.iStatusCd = (int)sStatus.toInt();
    } 
    // todo: how can we detect the JSON payload?
    else if (sLine.startsWith("{ \"m1\": "))
    {
      r.sReply = sLine;
    }
  }

  Serial.println("EOF.");
  
  if (!client.connected()) 
  {
    // free up memory
    client.stop();
  }
  
  return r;
}

//EOF
