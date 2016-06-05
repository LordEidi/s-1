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
#ifndef H_CONST
#define H_CONST

#include <Arduino.h> //needed for Serial.println
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// change these to your setup
static const char* SSID  = "b1";
static const char* PWD   = "b1";

// device_id
extern String sDeviceId;

// where your hardac server is running
static const char* SERVER  = "192.168.100.12";
static const int PORT      = 6262;

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS D4

static const int SECONDS_TO_MILLIS = 1000;

#endif // H_CONST
