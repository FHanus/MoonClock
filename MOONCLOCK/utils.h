/* UTILS.H
 * 
 * Contains all the initialization functions
 * Revision: Filip Hanus, 30/07/2022
 */

// Select I2C BUS (for the I2C multiplexer)
void TCA9548A(uint8_t bus){
  bus = bus-1;
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
  if(DEBUG)
    Serial.print(bus);
}

// Allocate displays
void setupdisplays(){
  // Start I2C communication with the Multiplexer
  Wire.begin();

  // Init OLED display on bus number 2 (display 1)
  TCA9548A(1);
  if(!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display1.clearDisplay();

  // Init OLED display on bus number 3
  TCA9548A(2);
  if(!display2.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display2.clearDisplay();

  // Init OLED display on bus number 4
  TCA9548A(3);
  if(!display3.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display3.clearDisplay();

  // Init OLED display on bus number 5
  TCA9548A(4);
  if(!display4.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display4.clearDisplay();

  // Init OLED display on bus number 5
  TCA9548A(5);
  if(!display5.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    if(DEBUG)
      Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  } 
  // Clear the buffer
  display5.clearDisplay();  
}

// Function to draw bitmap on the display
void display_draw_bitmap(Adafruit_SSD1306 *display_d,int index,const unsigned char *bitmap,String pos){
  if(pos=="mid_wide"){
    display_d->drawBitmap(0, 0,bitmap, 128, 64, WHITE);
  }
  else if(pos=="left"){
    display_d->drawBitmap(0, 0,bitmap, 48, 64, WHITE);
  }
  else if(pos=="right"){
    display_d->drawBitmap(80, 0,bitmap, 48, 64, WHITE);
  }
  else if(pos=="mid"){
    display_d->drawBitmap(40, 0,bitmap, 48, 64, WHITE);
  }
}

// Function to clear displays
void clear_display(){
  TCA9548A(1);
  display1.clearDisplay();
  display1.display();
  
  TCA9548A(2);
  display2.clearDisplay();
  display2.display(); 
  
  TCA9548A(3);
  display3.clearDisplay();
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display5.display();
}

// Function to test displays during start-up
void testdisplay(){
  TCA9548A(1);
  display1.clearDisplay();
  display_draw_bitmap(&display1,1,bitmap_selector("wifi"),"mid_wide");
  display1.display();
  
  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector("wifi"),"mid_wide");
  display2.display(); 
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector("wifi"),"mid_wide");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector("wifi"),"mid_wide");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display_draw_bitmap(&display5,5,bitmap_selector("wifi"),"mid_wide");
  display5.display();
}

// Print out Wi-Fi status
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  
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

// Setup Wi-Fi connection
bool connectwifi(){
  // Create WiFiManager object
  WiFiManager wf;
 
  // Supress Debug information
  wf.setDebugOutput(false);
 
  // Remove any previous network settings
  wf.resetSettings();
 
  // Define a text box, 50 characters maximum
  // Text box (String)
  WiFiManagerParameter custom_text_box_app("key_text1", "Enter selected app here", app_choice, 50); // 50 == max length

  // Text box (String)
  WiFiManagerParameter custom_text_box_cry("key_text2", "Enter selected crypto here", crypto_choice, 50); // 50 == max length

  // Text box (String)
  WiFiManagerParameter custom_text_box_fia("key_text3", "Enter selected fiat here", fiat_choice, 50); // 50 == max length

  // Text box (String)
  WiFiManagerParameter custom_text_box_tim("key_text4", "Enter timezone here", timezone_choice, 50); // 50 == max length

  // Text box (Number)
  WiFiManagerParameter custom_text_box_lat("key_text5", "Enter latitude here", latitude_choice, 15);

  // Text box (Number)
  WiFiManagerParameter custom_text_box_lon("key_text6", "Enter longitude here", longitude_choice, 15); 

  // Text box (Number)
  char convertedValue1[15];
  sprintf(convertedValue1, "%d", contrast_after_sunrise_choice); // Need to convert to string to display a default value.
  WiFiManagerParameter custom_text_box_ris("key_num7", "Enter day display contrast here", convertedValue1, 15); 

  // Text box (Number)
  char convertedValue2[15];
  sprintf(convertedValue2, "%d", contrast_after_sunset_choice); // Need to convert to string to display a default value.
  WiFiManagerParameter custom_text_box_set("key_num8", "Enter night display contrast here", convertedValue2, 15);

  // Text box (Number)
  char convertedValue3[10];
  if(hide_seconds){
    strcpy(convertedValue3, "true");
  }
  else{
    strcpy(convertedValue3, "false");
  }
 
  WiFiManagerParameter custom_text_box_sho("key_bool9", "Hide seconds in clock app?", convertedValue3, 15);

 
  //add all your parameters here
  wf.addParameter(&custom_text_box_app);
  wf.addParameter(&custom_text_box_cry);
  wf.addParameter(&custom_text_box_fia);
  wf.addParameter(&custom_text_box_tim);
  wf.addParameter(&custom_text_box_lat);
  wf.addParameter(&custom_text_box_lon);
  wf.addParameter(&custom_text_box_ris);
  wf.addParameter(&custom_text_box_set);
  wf.addParameter(&custom_text_box_sho);

  //wf.setTimeout(120); //If no access point name has been previously entered disable timeout.
 
  if (!wf.startConfigPortal("MoonClock Configuration")) {
    // Did not connect, print error message
    Serial.println("failed to connect and hit timeout");
 
    // Reset and try again
    ESP.restart();
    delay(1000);
  }
 
  // Connected!
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("\nApp: ");
  
  // Lets deal with the user config values
  strncpy(app_choice,  custom_text_box_app.getValue(), sizeof(app_choice));
  strncpy(crypto_choice,  custom_text_box_cry.getValue(), sizeof(crypto_choice));
  strncpy(fiat_choice,  custom_text_box_fia.getValue(), sizeof(fiat_choice));
  strncpy(timezone_choice,  custom_text_box_tim.getValue(), sizeof(timezone_choice));
  strncpy(latitude_choice,  custom_text_box_lat.getValue(), sizeof(latitude_choice));
  strncpy(longitude_choice,  custom_text_box_lon.getValue(), sizeof(longitude_choice));
  contrast_after_sunrise_choice = atoi(custom_text_box_ris.getValue());
  contrast_after_sunset_choice = atoi(custom_text_box_set.getValue());
  hide_seconds = (strncmp(custom_text_box_sho.getValue(), "t", 1));

  printWifiStatus();
  return true;
}
