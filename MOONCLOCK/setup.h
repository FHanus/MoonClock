/* SETUP.H
 * 
 * Contains all the initializations of libraries and components
 * Revision: Filip Hanus, 30/07/2022
 */

// Wi-Fi and HTTP requests related libraries
//#include "WiFi.h"
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <HTTPClient.h>

// Include WiFiManager Library
#include <WiFiManager.h>

// Display related libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "symbols.h"

// Display size definitions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Display structure definitions
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display3(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display4(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 display5(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Turn debugging print outs on/off
#define DEBUG true

// Global variable used to turn on/off the web handler for config updater
bool shouldSaveConfig = true;
bool configuration = true;

// Enter your WiFi SSID and password
char chSSID[] = "TALKTALK694114";             // your network SSID (name)
char chPassword[] = "9E6JAYT6";    // your network password (use for WPA, or use as key for WEP)

char app_choice[15]= "CryptoApp";
char crypto_choice [15] = "bitcoin";
char fiat_choice [15]= "usd";
char timezone_choice[20] = "Europe/Prague";
char latitude_choice[15] = "50.073611";
char longitude_choice[15] = "14.435664";
int contrast_after_sunrise_choice = 100;
int contrast_after_sunset_choice = 0;
bool hide_seconds = true;

unsigned long lastMillis = 0;
