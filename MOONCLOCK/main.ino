/* MAIN.INO
 * 
 * Runs all the initializations and then runs the main function in repeat
 * Revision: Filip Hanus, 12/08/2022
 */

// Include file with all the library and file initializations
#include "setup.h"

// Include file with all the utility functions
#include "utils.h"

// Inlude file with all the apps
#include "apps.h"

// Setup
void setup() {
  // If TERMINAL output is needed, initialize it
  if(DEBUG)
    Serial.begin(115200);

  // Setup displays
  setupdisplays();

  // Clear displays
  displays_clear();
  displays_fill("wifi");

  // Connect to Wi-Fi
  connectwifi();

  // Wait for 5 seconds
  vTaskDelay(5000 / portTICK_PERIOD_MS);

  // Clear displays
  displays_clear();
}

void loop() {
  // If Wi-Fi connects then start an app that has been selected during the settings
  if ((WiFi.status() == WL_CONNECTED)){
    App_Selector(String(app_choice),String(crypto_choice),String(fiat_choice),String(timezone_choice),String(latitude_choice),String(longitude_choice),contrast_after_sunrise_choice,contrast_after_sunset_choice,hide_seconds);}
  // IF Wi-Fi disconnects then display corresponding symbol
  else{
    displays_fill("wifi");}
}
