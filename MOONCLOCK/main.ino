/* MAIN.INO
 * 
 * Runs all the initializations and then runs the main function in repeat
 * Revision: Filip Hanus, 30/07/2022
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
  clear_display();
  testdisplay();

  // Connect to Wi-Fi
  connectwifi();

  // Wait for half a second
  vTaskDelay(5000 / portTICK_PERIOD_MS);

  // Clear displays
  clear_display();
}

void loop() {
  Serial.print("SSSSSSSSSSS");
  Serial.print(String(crypto_choice));
  Serial.print("SSASASASAASA");
  if ((WiFi.status() == WL_CONNECTED)){
    App_Selector(String(app_choice),String(crypto_choice),String(fiat_choice),String(timezone_choice),String(latitude_choice),String(longitude_choice),contrast_after_sunrise_choice,contrast_after_sunset_choice,hide_seconds);
  }
  else{
    testdisplay();
  }
}
