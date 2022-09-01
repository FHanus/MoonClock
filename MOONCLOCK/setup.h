/* SETUP.H
 * 
 * Contains all the initializations of libraries and components
 * Revision: Filip Hanus, 01/09/2022
 */

// Wi-Fi and HTTP requests related libraries
// Include WiFiManager Library
#include <WiFiManager.h>
//#include <WiFi.h>

#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <HTTPClient.h>
#include <time.h>
#include <sntp.h>

char ssid[] = "Pixel_Filip";  //  your network SSID (name)
char pass[] = "sendmemes"; // your network password

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

// NTP server for time
const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";

// User default app choices
char app_choice[15]= "CryptoApp";
char crypto_choice [15] = "bitcoin";
char fiat_choice [15]= "usd";
<<<<<<< Updated upstream
char timezone_choice[55] = "NST+3:30NDT+2:30,M3.2.0/00:01:00,M11.1.0/00:01:00";  
=======
char timezone_choice[55] = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";
>>>>>>> Stashed changes
char latitude_choice[15] = "50.073611";
char longitude_choice[15] = "14.435664";
int contrast_after_sunrise_choice = 100;
int contrast_after_sunset_choice = 0;
bool show_seconds = true;

// WiFi config variable
bool portalRunning = false;

// Global variable used to turn on/off the web handler for config updater
bool shouldSaveConfig = true;
bool configuration = true;

// Timer variables
unsigned long startTime = 0;
unsigned long lastMillis = 0;
float last_call = 0;
