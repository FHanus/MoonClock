/* APPS.H
 * 
 * Contains all the functions for the apps
 * Revision: Filip Hanus, 30/07/2022
 */

/* App: AutoContrastApp
 * Required: setContrast, set_contrast, get_time, get_risetimes (helper functions)
 * Inputs: String timezone, int contrast_after_sunrise, int contrast_after_sunset,String latitude,String longitude
 * 
 * Functionality: Auto sets contrast of the displays
 * Revision: Filip Hanus, 30/07/2022
 */

void setContrast(Adafruit_SSD1306 *display,int index, uint8_t contrast){   
    TCA9548A(index);
    display->ssd1306_command(SSD1306_SETCONTRAST);
    display->ssd1306_command(contrast);
}

void set_contrast(int contrast){ 
  setContrast(&display1,1, contrast);
  setContrast(&display2,2, contrast);
  setContrast(&display3,3, contrast);
  setContrast(&display4,4, contrast);
  setContrast(&display5,5, contrast);
}

String get_time(String timezone){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    String timezone = "Europe/Prague";
    // URL TemplateEurope/Prague
    String apiUrl("https://www.timeapi.io/api/TimeZone/zone?timeZone=" + timezone);
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }
    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    const char* json_data = payload.c_str();

    deserializeJson(doc, json_data);

    String time_e = doc["currentLocalTime"];

    http.end();
    return time_e;
  }
  else{
    return " ";
  }
}

String get_risetimes(String latitude = "50.073611",String longitude = "14.435664",bool return_option=false){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;

    String apiUrl("http://api.sunrise-sunset.org/json?lat="+latitude+"&lng="+longitude+"&date=today&formatted=0");
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }

    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    const char* json_data = payload.c_str();

    deserializeJson(doc, json_data);

    String sunrise = doc["results"]["sunrise"];
    String sunset = doc["results"]["sunset"];

    String sunrise_time = String(String(sunrise[11])+String(sunrise[12]));
    String sunset_time = String(String(sunset[11])+String(sunset[12]));

    http.end();

    if(return_option==false){
      return sunrise_time;
    }
    else if(return_option==true){
      return sunset_time;
    }  
  }
  return " ";
}

void AutoContrastApp(){
  // If TERMINAL output is allowed then print out status message
  if(DEBUG)
    Serial.print("Running autocontrast");
  
  String timestamp = get_time(timezone_choice);
  String stamp = String(timestamp[11])+String(timestamp[12]);
  String sunrise_time = get_risetimes("50.073611","14.435664",false);
  String sunset_time = get_risetimes("50.073611","14.435664",true);

  if(DEBUG)
    Serial.print("Sunset: ");
    Serial.print(sunset_time.toInt());
    Serial.print("Sunrise: ");
    Serial.print(sunrise_time.toInt());
    Serial.print("Time: ");
    Serial.print(stamp.toInt());

  if (sunset_time.toInt() <= timestamp.toInt()){
    set_contrast(contrast_after_sunset_choice);

    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.print("changing contrast after sunset");
  }
  else if (sunrise_time.toInt() <= timestamp.toInt()){
    set_contrast(contrast_after_sunrise_choice);

    // If TERMINAL output is allowed then print out status message
    if(DEBUG)  
      Serial.print("changing contrast after sunrise");
  }
  // Waiting for an hour would be: (1000*3600)
  vTaskDelay(1000 / portTICK_RATE_MS);
} 

/* App: BlockHeight
 * Required: display_BlockHeight (helper function)
 * 
 * Functionality: Displays current block height
 * Revision: Filip Hanus, 30/07/2022
 */

void display_BlockHeight(String blockheight){
  
  String D6 = String(blockheight[0]);
  String D5 = String(blockheight[1]);
  String D4 = String(blockheight[2]);
  String D3 = String(blockheight[3]);
  String D2 = String(blockheight[4]);
  String D1 = String(blockheight[5]);


  TCA9548A(1);
  display1.clearDisplay();
  display_draw_bitmap(&display1,1,bitmap_selector("chain"),"mid_wide");
  display1.display();
  
  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector(D1),"left");
  display_draw_bitmap(&display2,2,bitmap_selector(D2),"right");
  display2.display(); 
  
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector(D3),"left");
  display_draw_bitmap(&display3,3,bitmap_selector(D4),"right");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector(D5),"left");
  display_draw_bitmap(&display4,4,bitmap_selector(D6),"right");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display_draw_bitmap(&display5,5,bitmap_selector("bitcoin"),"mid_wide");
  display5.display();
}

void BlockHeight(){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    // URL TemplateEurope/Prague
    String apiUrl("https://mempool.space/api/blocks/tip/height");
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }

    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    String json_data = payload.c_str();

    display_BlockHeight(json_data);
      
    http.end();
  }
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

/* App: CryptoApp
 * Required: display_price (helper function)
 * Input: String selected_coin,String selected_fiat
 * 
 * Functionality: Displays current price
 * Revision: Filip Hanus, 30/07/2022
 */

void display_price(float number = 0,bool decimals=true,String selected_coin = "bitcoin" ,String selected_fiat = "usd"){
  String str_value_f, d2, d3, d4;

  if(decimals != false){
    if(number < 0.1){str_value_f = String(number,4);}
    else if(number < 1){str_value_f = String(number,3);}
    else if(number < 10){str_value_f = String(number,2);}
    else if(number < 100){str_value_f = String(number,1);}
    else{str_value_f = String(number,0);}
  }
  else{
    str_value_f = String(number,0);    
  }
  
  unsigned int len = str_value_f.length(); 
  
  String D6 = String(str_value_f[len-1]);
  String D5 = String(str_value_f[len-2]);
  String D4 = String(str_value_f[len-3]);
  String D3 = String(str_value_f[len-4]);
  String D2 = String(str_value_f[len-5]);
  String D1 = String(str_value_f[len-6]);

  TCA9548A(1);
  display1.clearDisplay();
  display_draw_bitmap(&display1,1,bitmap_selector(selected_fiat),"mid");
  display1.display();
  
  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector(D1),"left");
  display_draw_bitmap(&display2,2,bitmap_selector(D2),"right");
  display2.display(); 
  
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector(D3),"left");
  display_draw_bitmap(&display3,3,bitmap_selector(D4),"right");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector(D5),"left");
  display_draw_bitmap(&display4,4,bitmap_selector(D6),"right");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display_draw_bitmap(&display5,5,bitmap_selector(selected_coin),"mid_wide");
  display5.display();  
}

void CryptoApp(String selected_coin,String selected_fiat){ 
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;

    // URL Template
    String apiUrl("https://api.coingecko.com/api/v3/simple/price?ids=" + selected_coin + "&vs_currencies=" + selected_fiat);
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }

    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    const char* json_data = payload.c_str();
   

    deserializeJson(doc, json_data);


    JsonObject json_coin = doc[selected_coin];
    float selected_coin_f = json_coin[selected_fiat];

    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.print("Price: " + String(selected_coin_f));

    display_price(selected_coin_f,true,selected_coin,selected_fiat);
    http.end();
  }
  else{
    Serial.print("WIFI NOT CONNECTED!");
  }
  
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

/* App: Difficulty
 * Required: display_difficulty (helper function)
 * 
 * Functionality: Displays current difficulty (%)
 * Revision: Filip Hanus, 30/07/2022
 */

void display_difficulty(String message){

  String D5 = String(message[4]);
  String D4 = String(message[3]);
  String D3 = String(message[2]);
  String D2 = String(message[1]);
  String D1 = String(message[0]);


  TCA9548A(1);
  display1.clearDisplay();
  display1.display();
  
  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector(D1),"left");
  display_draw_bitmap(&display2,2,bitmap_selector(D2),"right");
  display2.display(); 
  
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector(D3),"left");
  display_draw_bitmap(&display3,3,bitmap_selector(D4),"right");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector(D5),"left");
  display_draw_bitmap(&display4,4,bitmap_selector("percent"),"right");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display5.display();
}

void Difficulty(){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    // URL TemplateEurope/Prague
    String apiUrl("https://mempool.space/api/v1/difficulty-adjustment");
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }

    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    const char* json_data = payload.c_str();    

    deserializeJson(doc, json_data);


    String selected_coin_f = doc["difficultyChange"];

    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.print(selected_coin_f);
      
    display_difficulty(selected_coin_f);
      
    http.end();
  }
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

/* App: Fees
 * Required: display_fees (helper function)
 * 
 * Functionality: Displays current fees
 * Revision: Filip Hanus, 30/07/2022
 */

void display_fees(String fastest_fee, String hour_fee){
 // 
  String message = String(fastest_fee) + " " + String(hour_fee);
    
  int len = message.length();
  
  String D1 = String(message[len-1]);
  String D2 = String(message[len-2]);
  String D3 = String(message[len-3]);
  String D4 = String(message[len-4]);
  String D5 = String(message[len-5]);
  String D6 = String(message[len-6]);
  String D7 = String(message[len-7]);
  String D8 = String(message[len-8]);
  String D9 = String(message[len-9]);
  String D10 = String(message[len-10]);

  
  if(len>=8){
    TCA9548A(1);
    display1.clearDisplay();
    display_draw_bitmap(&display1,1,bitmap_selector(D10),"left");
    display_draw_bitmap(&display1,1,bitmap_selector(D9),"right");
    display1.display();
  }
  else{
    TCA9548A(1);
    display1.clearDisplay();
    display_draw_bitmap(&display1,1,bitmap_selector("money_bag"),"mid_wide");
    display1.display();

  }
  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector(D8),"left");
  display_draw_bitmap(&display2,2,bitmap_selector(D7),"right");
  display2.display(); 
  
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector(D6),"left");
  display_draw_bitmap(&display3,3,bitmap_selector(D5),"right");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector(D4),"left");
  display_draw_bitmap(&display4,4,bitmap_selector(D3),"right");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display_draw_bitmap(&display5,5,bitmap_selector(D2),"left");
  display_draw_bitmap(&display5,5,bitmap_selector(D1),"right");
  display5.display();  
}

void Fees(){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    // URL TemplateEurope/Prague
    String apiUrl("https://mempool.space/api/v1/fees/recommended");
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }

    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();

    const char* json_data = payload.c_str();
    deserializeJson(doc, json_data);
    String fastest_fee = doc["fastestFee"];
    String hour_fee = doc["hourFee"];    

    display_fees(fastest_fee,hour_fee);
      
    http.end();
  }
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

/* App: Halving
 * Required: display_halving (helper function)
 * 
 * Functionality: Displays current halving for mining
 * Revision: Filip Hanus, 30/07/2022
 */

void display_halving(String halving){

  int len = halving.length();
    
  String D8 = String(halving[len-1]);
  String D7 = String(halving[len-2]);
  String D6 = String(halving[len-3]);
  String D5 = String(halving[len-4]);
  String D4 = String(halving[len-5]);
  String D3 = String(halving[len-6]);
  String D2 = String(halving[len-7]);
  String D1 = String(halving[len-8]);


  TCA9548A(1);
  display1.clearDisplay();
  display_draw_bitmap(&display1,1,bitmap_selector("halving"),"mid_wide");
  display1.display();
  
  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector(D1),"left");
  display_draw_bitmap(&display2,2,bitmap_selector(D2),"right");
  display2.display(); 
  
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector(D3),"left");
  display_draw_bitmap(&display3,3,bitmap_selector(D4),"right");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector(D5),"left");
  display_draw_bitmap(&display4,4,bitmap_selector(D6),"right");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display_draw_bitmap(&display5,5,bitmap_selector(D7),"left");
  display_draw_bitmap(&display5,5,bitmap_selector(D8),"right");
  display5.display();
}

void Halving(){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    // URL TemplateEurope/Prague
    String apiUrl("https://mempool.space/api/blocks/tip/height");
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }

    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    String json_data = payload.c_str();
    int height_i = json_data.toInt();  
    
    String halving_i = String(210000 - int(height_i) % 210000);

    display_halving(halving_i);
      
    http.end();
  }
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

/* App: MarketCap
 * Required: get_MarketCap, display_marketcap (helper functions)
 * Inputs: String selected_crypto,String selected_fiat
 * 
 * Functionality: Displays current marketcap
 * Revision: Filip Hanus, 30/07/2022
 */

void display_marketcap(String message, String sel_crypto, String sel_fiat){ 
  int len = message.length();
  
  String D1 = String(message[len-1]);
  String D2 = String(message[len-2]);
  String D3 = String(message[len-3]);
  String D4 = String(message[len-4]);
  String D5 = String(message[len-5]);
  String D6 = String(message[len-6]);

  TCA9548A(1);
  display1.clearDisplay();
  display_draw_bitmap(&display1,1,bitmap_selector(sel_fiat),"mid");
  display1.display();

  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector(D6),"left");
  display_draw_bitmap(&display2,2,bitmap_selector(D5),"right");
  display2.display(); 
  
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector(D4),"left");
  display_draw_bitmap(&display3,3,bitmap_selector(D3),"right");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector(D2),"left");
  display_draw_bitmap(&display4,4,bitmap_selector(D1),"right");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display_draw_bitmap(&display5,5,bitmap_selector(sel_crypto),"mid_wide");
  display5.display();  
}

String get_MarketCap(String selected_crypto,String selected_fiat){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    // URL TemplateEurope/Prague
    String apiUrl("https://api.coingecko.com/api/v3/coins/"+selected_crypto+"?localization=false&tickers=false&market_data=true&community_data=false&developer_data=false&sparkline=false");
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }
    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = 3*JSON_ARRAY_SIZE(0) + JSON_ARRAY_SIZE(1) + 2*JSON_ARRAY_SIZE(2) + 3*JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(10) + 2*JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(11) + JSON_OBJECT_SIZE(28) + JSON_OBJECT_SIZE(42) + 23*JSON_OBJECT_SIZE(61);
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();

    deserializeJson(doc,payload,DeserializationOption::NestingLimit(150));

    String selected_coin_f = doc["market_data"]["current_price"][selected_fiat];

    http.end();
    
    return selected_coin_f;
  }
  return "  ";
}

void MarketCap(String selected_crypto,String selected_fiat){
  String market_cap = get_MarketCap(selected_crypto,selected_fiat);
  display_marketcap(market_cap,selected_crypto,selected_fiat);
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

/* App: MoscowTimeApp
 * Required: display_moscowtime (helper function)
 * 
 * Functionality: Displays current "MoscowTime" (= 100 000 000 / Current BTC price)
 * Revision: Filip Hanus, 30/07/2022
 */

void display_moscowtime(String str_value){  

  int len = str_value.length();

  String D4 = String(str_value[len-1]);
  String D3 = String(str_value[len-2]);
  String D2 = String(str_value[1]);
  String D1 = String(str_value[0]);

  TCA9548A(1);
  display1.clearDisplay();
  display_draw_bitmap(&display1,1,bitmap_selector("moscow"),"mid_wide");
  display1.display();
  
  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector(D1),"left");
  display_draw_bitmap(&display2,2,bitmap_selector(D2),"right");
  display2.display(); 
  
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector("widecolon"),"mid_wide");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector(D3),"left");
  display_draw_bitmap(&display4,4,bitmap_selector(D4),"right");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display_draw_bitmap(&display5,5,bitmap_selector("moscow"),"mid_wide");
  display5.display();
}

void MoscowTimeApp(){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    // URL TemplateEurope/Prague
    String apiUrl("https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }

    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    const char* json_data = payload.c_str();

    deserializeJson(doc, json_data);

    int price = doc["bitcoin"]["usd"];


    int p_price = (100000000 / price);

    String str_price = String(int(p_price));

    display_moscowtime(str_price);

    http.end();
  }
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

/* App: TimeApp
 * Required: display_time (helper function)
 * Inputs: String timezone, bool hide_sec
 * 
 * Functionality: Displays current time
 * Revision: Filip Hanus, 30/07/2022
 */

void display_time(const char *time_unparsed,bool hide_sec){
  String str_value_f = String(time_unparsed);
  
  String D6 = String(str_value_f[18]);
  String D5 = String(str_value_f[17]);
  String D4 = String(str_value_f[15]);
  String D3 = String(str_value_f[14]);
  String D2 = String(str_value_f[12]);
  String D1 = String(str_value_f[11]);

  if(hide_sec){
    TCA9548A(1);
    display1.clearDisplay();
    display_draw_bitmap(&display1,1,bitmap_selector(D1),"left");
    display_draw_bitmap(&display1,1,bitmap_selector(D2),"right");
    display1.display();
    
    TCA9548A(2);
    display2.clearDisplay();
    display_draw_bitmap(&display2,2,bitmap_selector("widecolon"),"mid_wide");
    display2.display(); 
    
    TCA9548A(3);
    display3.clearDisplay();
    display_draw_bitmap(&display3,3,bitmap_selector(D3),"left");
    display_draw_bitmap(&display3,3,bitmap_selector(D4),"right");
    display3.display();
    
    TCA9548A(4);
    display4.clearDisplay();
    display_draw_bitmap(&display4,4,bitmap_selector("widecolon"),"mid_wide");
    display4.display();
    
    TCA9548A(5);
    display5.clearDisplay();
    display_draw_bitmap(&display5,5,bitmap_selector(D5),"left");
    display_draw_bitmap(&display5,5,bitmap_selector(D6),"right");
    display5.display();
  }
  else{
    TCA9548A(1);
    display1.clearDisplay();
    display1.display();
    
    TCA9548A(2);
    display2.clearDisplay();
    display_draw_bitmap(&display2,2,bitmap_selector(D1),"left");
    display_draw_bitmap(&display2,2,bitmap_selector(D2),"right");
    display2.display(); 
    
    TCA9548A(3);
    display3.clearDisplay();
    display_draw_bitmap(&display3,3,bitmap_selector("widecolon"),"mid_wide");
    display3.display();
    
    TCA9548A(4);
    display4.clearDisplay();
    display_draw_bitmap(&display4,4,bitmap_selector(D3),"left");
    display_draw_bitmap(&display4,4,bitmap_selector(D4),"right");
    display4.display();
    
    TCA9548A(5);
    display5.clearDisplay();
    display5.display();
    
  }
}

void TimeApp(String timezone= "Europe/Prague", bool hide_sec=true){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    // URL TemplateEurope/Prague
    String apiUrl("https://www.timeapi.io/api/TimeZone/zone?timeZone=" + timezone);
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }
    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    const char* json_data = payload.c_str();

    deserializeJson(doc, json_data);

    const char *time_e = doc["currentLocalTime"];

    display_time(time_e,hide_sec);

    http.end();
  }
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

/* App: Temperature
 * Required: display_temp (helper function)
 * Inputs: String city, String api_key, String units
 * 
 * Functionality: Displays current temperature
 * Revision: Filip Hanus, 30/07/2022
 * 
 * Notes: Untested! Was not able to get the API requirements to properly test the function
 */

void display_temp(String temp){

  int len = temp.length();
  
  String D3 = String(temp[len-1]);
  String D2 = String(temp[len-2]);
  String D1 = String(temp[len-3]);

  TCA9548A(1);
  display1.clearDisplay();
  display1.display();
  
  TCA9548A(2);
  display2.clearDisplay();
  display_draw_bitmap(&display2,2,bitmap_selector(D1),"right");
  display_draw_bitmap(&display2,2,bitmap_selector(D2),"left");
  display2.display(); 
  TCA9548A(3);
  display3.clearDisplay();
  display_draw_bitmap(&display3,3,bitmap_selector(D3),"left");
  display_draw_bitmap(&display3,3,bitmap_selector("degree"),"right");
  display3.display();
  
  TCA9548A(4);
  display4.clearDisplay();
  display_draw_bitmap(&display4,4,bitmap_selector("c"),"left");
  display4.display();
  
  TCA9548A(5);
  display5.clearDisplay();
  display5.display();
}

void Temperature(String city, String api_key, String units){
  if ((WiFi.status() == WL_CONNECTED)){
    HTTPClient http;
    String timezone = "Europe/Prague";
    // URL TemplateEurope/Prague
    String apiUrl("http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + api_key + "&units={}" + units);
    http.begin (apiUrl);

    int httpCode = http.GET();

    if (httpCode > 0) {
    }

    if(DEBUG)
      Serial.println ("\nStatuscode: " + String(httpCode));
      
    vTaskDelay(100 / portTICK_PERIOD_MS);

    const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
    DynamicJsonDocument doc(sized_buffer);
    String payload = http.getString();
    const char* json_data = payload.c_str();

    deserializeJson(doc, json_data);

    String temp = doc["main"]["temp"];

    display_temp(temp);

    http.end();
  }
  vTaskDelay(300000 / portTICK_PERIOD_MS);
}

/* App: App_Selector
 * Inputs: String selected_app
 * 
 * Functionality: Displays selected app
 * Revision: Filip Hanus, 30/07/2022 
 */

void App_Selector(){
  if(DEBUG)
    Serial.print("Running App Selector: ");
    Serial.print(app_choice);
  
  if(strcmp(app_choice, "BlockHeight") == 0){BlockHeight();}
  else if(strcmp(app_choice, "CryptoApp") == 0){CryptoApp(String(crypto_choice),String(fiat_choice));}
  else if(strcmp(app_choice, "Difficulty") == 0){Difficulty();}
  else if(strcmp(app_choice, "Fees") == 0){Fees();}
  else if(strcmp(app_choice, "Halving") == 0){Halving();}
  else if(strcmp(app_choice, "MarketCap") == 0){MarketCap(String(crypto_choice),String(fiat_choice));}
  else if(strcmp(app_choice, "MoscowTimeApp") == 0){MoscowTimeApp();}
  else if(strcmp(app_choice, "TimeApp") == 0){TimeApp(String(timezone_choice), bool(hide_seconds));}
}
 
