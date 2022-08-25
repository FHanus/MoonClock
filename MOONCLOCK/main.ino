/* MAIN.INO

   Runs all the initializations and then runs the main function in repeat
   Revision: Filip Hanus, 25/08/2022
*/

// Include file with all the library and file initializations
#include "setup.h"

// Include file with all the utility functions
#include "utils.h"

// Inlude file with all the apps
#include "apps.h"

// Setup
void setup() {
  WiFi.mode(WIFI_STA);
  // If TERMINAL output is needed, initialize it
  if (DEBUG)
    Serial.begin(115200);

  // Debug info
  if (DEBUG)
    Serial.println("Setting up NTP");
  
  // config time ntp server
  sntp_servermode_dhcp(1);
  configTzTime(timezone_choice, ntpServer1, ntpServer2);

  // Debug info
  if (DEBUG)
    Serial.println("Setting up displays");
  
  // Setup displays
  setupdisplays();

  // Clear displays
  displays_clear();
  displays_fill("wifi");

  // Debug info
  if (DEBUG)
    Serial.println("Starting config manager");

  // Connect to Wi-Fi
  run_config_manager();

  // Wait for the selected amount of miliseconds
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  // Set contrast
  AutoContrastApp();

  // Clear displays
  displays_clear();
}

// Main loop
void loop() {
  // Run AutoContrast app every hour
  if (millis() - lastMillis >= 3600 * 1000) { //3600*1000
    lastMillis = millis();
    AutoContrastApp();
  }

  // If WiFi connects run the selected app
  if ((WiFi.status() == WL_CONNECTED)){
    App_Selector();}
  else{
    displays_fill("cross");
  }
  
}
