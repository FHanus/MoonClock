/* UTILS.H
 * 
 * Contains all the initialization functions
 * Revision: Filip Hanus, 25/08/2022
 */

void TCA9548A(uint8_t bus)
// Select I2C BUS (for the I2C multiplexer)
{
  // buffer starts at 0, we initiate the displays starting from 1
  bus = bus-1;

  // This is the I2C address of the multiplexer, begin serial comms with this address
  Wire.beginTransmission(0x70);  // TCA9548A address
  
  // Senf a byte to the selected adress
  Wire.write(1 << bus);

  // End the transmission
  Wire.endTransmission();

  // If debugging is turned on display the transmitted message
  if(DEBUG)
    Serial.print(bus);
}

void setupdisplays()
// Allocate displays
{
  // Start I2C communication with the Multiplexer
  Wire.begin();

  // Init OLED display on bus number 2 (display 1)
  TCA9548A(1);

  // This statement secures the scenario if the allocation fails
  if(!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // If debugging is turned on print the error message
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display1.clearDisplay();

  // Init OLED display on bus number 3
  TCA9548A(2);
  
  // This statement secures the scenario if the allocation fails
  if(!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // If debugging is turned on print the error message
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display2.clearDisplay();

  // Init OLED display on bus number 4
  TCA9548A(3);
  
  // This statement secures the scenario if the allocation fails
  if(!display3.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // If debugging is turned on print the error message
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display3.clearDisplay();

  // Init OLED display on bus number 5
  TCA9548A(4);
  
  // This statement secures the scenario if the allocation fails
  if(!display4.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // If debugging is turned on print the error message
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display4.clearDisplay();

  // Init OLED display on bus number 5
  TCA9548A(5);
  
  // This statement secures the scenario if the allocation fails
  if(!display5.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // If debugging is turned on print the error message
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display5.clearDisplay();  
}

void display_draw_bitmap(Adafruit_SSD1306 *display_d,int index,const unsigned char *bitmap,String pos)
// Function to draw bitmap on the display
{
  // Different positioning options
  // MID_WIDE position handles the icons that take up a whole display
  if(pos=="mid_wide"){
    display_d->drawBitmap(0, 0,bitmap, 128, 64, WHITE);}
  // LEFT half of display display, for symbols that take up half the display in size
  else if(pos=="left"){
    display_d->drawBitmap(0, 0,bitmap, 48, 64, WHITE);}
  // RIGHT half of the display, for symbols that take up half the display in size
  else if(pos=="right"){
    display_d->drawBitmap(80, 0,bitmap, 48, 64, WHITE);}
  // MIDDLE part of the display, for symbols that take up half the display in size
  else if(pos=="mid"){
    display_d->drawBitmap(40, 0,bitmap, 48, 64, WHITE);}
}

void display_clear(Adafruit_SSD1306 *display_d,int index)
// Function to clear selected display
{
  // Select multiplexer address
  TCA9548A(index);

  // Clear selected display
  display_d->clearDisplay();

  // Apply changes in the buffer to the display comms
  display_d->display();
}

void displays_clear()
// Function to clear all displays. Helper function only used for startup
{
  display_clear(&display1,1);
  display_clear(&display2,2);
  display_clear(&display3,3);
  display_clear(&display4,4); 
  display_clear(&display5,5);
}

void display_fill(Adafruit_SSD1306 *display_d,int index,String bitmap)
// Redundant function for drawing on one display
{
  // Select multiplexer address
  TCA9548A(index);

  // Clear selected display
  display_d->clearDisplay();

  // Fill the display with desired bits
  display_draw_bitmap(display_d,index,bitmap_selector(bitmap),"mid_wide");

  // Apply set buffer changes to the display comms
  display_d->display();  
}

void displays_fill(String bitmap)
// Function to test all displays during start-up. Helper function only used for startup
{
  display_fill(&display1,1,bitmap);
  display_fill(&display2,2,bitmap);
  display_fill(&display3,3,bitmap);
  display_fill(&display4,4,bitmap);
  display_fill(&display5,5,bitmap); 
}

void printWifiStatus()
// Print out Wi-Fi status
{
  // print the SSID of the network you're attached to
  // If TERMINAL output is allowed then print out status message
  if(DEBUG){
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  
    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
  
    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  }
  else{
    ;
  }
}

String request_data(String request_url)
// Start HTTP client and request data from a desired URL, return the requested data in json format
{
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    // Setup url for requests
    String apiUrl(request_url);
    http.begin (apiUrl);
    
    int httpCode = http.GET();
    
    if (httpCode > 0) {
    }
      
    //vTaskDelay(100 / portTICK_PERIOD_MS);
    
    // Request data from the url
    String payload = http.getString();

    // End the http connection
    http.end();

    // Return data
    return payload;
  }
  else{
    return "{""bitcoin"":{""usd"":66666}}";
  }
}
 
void saveConfigCallback () 
//callback notifying us of the need to save config
{
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void run_config_manager()
// Setup Wi-Fi connection and run config manager
{
  // Debug info
  if(DEBUG)
    Serial.println("mounting FS...");
    
  // Mount SPIFFS memory and load the last stored choices
  if (SPIFFS.begin()) {
    // Debug info
    if(DEBUG)
      Serial.println("mounted file system");
      
    // If there is file called config:
    if (SPIFFS.exists("/config.json")){      
      // Debug info
      if(DEBUG)
        Serial.println("reading config file");
        
      //file exists, reading and loading 
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile){
        if(DEBUG)
          Serial.println("opened config file");
        // Get the file size   
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        // Read the file
        configFile.readBytes(buf.get(), size);

        // Format the file
        DynamicJsonDocument json(1024);
        auto deserializeError = deserializeJson(json, buf.get());
        serializeJson(json, Serial);

        // Process the variables in the file
        if ( ! deserializeError ) {
          if(DEBUG)
            Serial.println("\nparsed json");
            
          strcpy(app_choice, json["app_choice"]);
          strcpy(crypto_choice, json["crypto_choice"]);
          strcpy(fiat_choice, json["fiat_choice"]);
          strcpy(timezone_choice, json["timezone_choice"]);
          strcpy(latitude_choice, json["latitude_choice"]);
          strcpy(longitude_choice, json["longitude_choice"]);
          contrast_after_sunrise_choice = int(json["contrast_after_sunrise_choice"]);
          contrast_after_sunset_choice = int(json["contrast_after_sunset_choice"]);
          hide_seconds = bool(json["hide_seconds"]);

        } else {
          if(DEBUG)
            Serial.println("failed to load json config");
        }
        // Close the file
        configFile.close();
      }
    }
  } else {
    if(DEBUG)
      Serial.println("failed to mount FS");
  }

  // Define a text boxes for the custom settings - STRINGS
  WiFiManagerParameter custom_text_box_app("key_text1", "Enter selected app here", app_choice, 50); // 50 == max length
  WiFiManagerParameter custom_text_box_cry("key_text2", "Enter selected crypto here", crypto_choice, 50); // 50 == max length
  WiFiManagerParameter custom_text_box_fia("key_text3", "Enter selected fiat here", fiat_choice, 50); // 50 == max length
  WiFiManagerParameter custom_text_box_tim("key_text4", "Enter timezone here", timezone_choice, 50); // 50 == max length
  WiFiManagerParameter custom_text_box_lat("key_text5", "Enter latitude here", latitude_choice, 15); // 15 == max length
  WiFiManagerParameter custom_text_box_lon("key_text6", "Enter longitude here", longitude_choice, 15); // 15 == max length 

   // Define a text boxes for the custom settings - NUMBERS
  char convertedValue1[15];
  sprintf(convertedValue1, "%d", contrast_after_sunrise_choice); // Need to convert to string to display a default value.
  WiFiManagerParameter custom_text_box_ris("key_num7", "Enter day display contrast here", convertedValue1, 15); 

  char convertedValue2[15];
  sprintf(convertedValue2, "%d", contrast_after_sunset_choice); // Need to convert to string to display a default value.
  WiFiManagerParameter custom_text_box_set("key_num8", "Enter night display contrast here", convertedValue2, 15);

   // Define a text boxes for the custom settings - BOOL  
  char convertedValue3[10];
  if(hide_seconds){strcpy(convertedValue3, "true");}
  else{strcpy(convertedValue3, "false");}
  WiFiManagerParameter custom_text_box_sho("key_bool9", "Hide seconds in clock app?", convertedValue3, 15);

  // Create WiFiManager object
  WiFiManager wf;

  //set config save notify callback
  wf.setSaveConfigCallback(saveConfigCallback);
 
  // Supress Debug information
  if(DEBUG)
    wf.setDebugOutput(true);
  else
    wf.setDebugOutput(false);

  // Setup all the parameters
  wf.addParameter(&custom_text_box_app);
  wf.addParameter(&custom_text_box_cry);
  wf.addParameter(&custom_text_box_fia);
  wf.addParameter(&custom_text_box_tim);
  wf.addParameter(&custom_text_box_lat);
  wf.addParameter(&custom_text_box_lon);
  wf.addParameter(&custom_text_box_ris);
  wf.addParameter(&custom_text_box_set);
  wf.addParameter(&custom_text_box_sho);

  // Remove any previous network settings
  wf.resetSettings();

  //If no access point name has been previously entered disable timeout.
  //wf.setTimeout(120);

  // START the config portal
  //wf.startConfigPortal("MoonClock Configuration","123456789");
  //wf.autoConnect("MoonClock Configuration");
  if (!wf.autoConnect("MoonClock Configuration")){
    // If the debug is turned on and the statrup failed -> print error message
    if(DEBUG)
      Serial.println("failed to connect and hit timeout");
    
    // Reset, wait and try again
    ESP.restart();
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
 
  // If debug is turned on print out the success message
  if(DEBUG){
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  
  // Deal with the user config values
  strncpy(app_choice,  custom_text_box_app.getValue(), sizeof(app_choice));
  strncpy(crypto_choice,  custom_text_box_cry.getValue(), sizeof(crypto_choice));
  strncpy(fiat_choice,  custom_text_box_fia.getValue(), sizeof(fiat_choice));
  strncpy(timezone_choice,  custom_text_box_tim.getValue(), sizeof(timezone_choice));
  strncpy(latitude_choice,  custom_text_box_lat.getValue(), sizeof(latitude_choice));
  strncpy(longitude_choice,  custom_text_box_lon.getValue(), sizeof(longitude_choice));
  contrast_after_sunrise_choice = atoi(custom_text_box_ris.getValue());
  contrast_after_sunset_choice = atoi(custom_text_box_set.getValue());
  hide_seconds = (strncmp(custom_text_box_sho.getValue(), "t", 1));

  // Save the custom parameters to FS
  if (shouldSaveConfig) {
    if(DEBUG)
      Serial.println("saving config");

    // Save into the json structure
    DynamicJsonDocument json(1024);
    json["app_choice"] = app_choice;
    json["crypto_choice"] = crypto_choice;
    json["timezone_choice"] = timezone_choice;
    json["latitude_choice"] = latitude_choice;
    json["longitude_choice"] = longitude_choice;
    json["contrast_after_sunrise_choice"] = contrast_after_sunrise_choice;
    json["contrast_after_sunset_choice"] = contrast_after_sunset_choice;
    json["hide_seconds"] = String(hide_seconds);

    // Open file for writing
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      if(DEBUG)
        Serial.println("failed to open config file for writing");
    }

    // Enter the data and close
    serializeJson(json, Serial);
    serializeJson(json, configFile);

    configFile.close();
    //end save
  }
  // Print debug wifi status
  printWifiStatus();
}
