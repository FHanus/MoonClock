/* APPS.H
 * 
 * Contains all the functions for the apps
 * Revision: Filip Hanus, 25/08/2022
 */

/* App: AutoContrastApp
 * Required: setContrast, set_contrast, get_time, get_risetimes (helper functions)
 * Inputs: String timezone, int contrast_after_sunrise, int contrast_after_sunset,String latitude,String longitude
 * 
 * Functionality: Auto sets contrast of the displays
 * Revision: Filip Hanus, 16/08/2022
 */

void setContrast(Adafruit_SSD1306 *display,int index, uint8_t contrast)
// Set contrast for a selected display
{   
  // Select the display using the multiplexer
  TCA9548A(index);
  // Access contrast settings for the specific display and change it
  display->ssd1306_command(SSD1306_SETCONTRAST);
  display->ssd1306_command(contrast);
}

void set_contrast(int contrast)
// Call the set display function for all the displays
{ 
  setContrast(&display1,1, contrast);
  setContrast(&display2,2, contrast);
  setContrast(&display3,3, contrast);
  setContrast(&display4,4, contrast);
  setContrast(&display5,5, contrast);
}

String get_time(String timezone)
// Obtain time using the HTTP request, returns a string variable
{
  // Create a structure to store time data in
  struct tm timeinfo;

  // Obtain time data
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return "00:00:00";
  }

  // Format time data
  char currenttime[10];
  strftime(currenttime,10, "%H:%M:%S", &timeinfo);
  
  // Return current time
  return currenttime;
}

String get_risetimes(String latitude = "50.073611",String longitude = "14.435664",bool return_option=false)
// Obtain sunrise and sunset times for the specified latitude and longitude
{
  // Setup request url and request data
  String request_url = ("http://api.sunrise-sunset.org/json?lat="+latitude+"&lng="+longitude+"&date=today&formatted=0");

  // Create a json variable for the data
  const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
  DynamicJsonDocument doc(sized_buffer);
  
  // Request data
  String payload = request_data(request_url);

  // Process the requested data
  const char* json_data = payload.c_str();
  deserializeJson(doc, json_data);
 
  String sunrise = doc["results"]["sunrise"];
  String sunset = doc["results"]["sunset"];

  String sunrise_time = String(String(sunrise[11])+String(sunrise[12]));
  String sunset_time = String(String(sunset[11])+String(sunset[12]));

  // Return sunrise and sunset data
  if(return_option==false){
    return sunrise_time;
  }
  else if(return_option==true){
    return sunset_time;
  }
  else{
    return "00";
  }
}

void AutoContrastApp()
// Setup display contrast automatically
{
  // If TERMINAL output is allowed then print out status message
  if(DEBUG)
    Serial.print("Running autocontrast");
  
  String timestamp = get_time(timezone_choice);
  String stamp = String(timestamp[0])+String(timestamp[1]);
  String sunrise_time = get_risetimes(latitude_choice,longitude_choice,false);
  String sunset_time = get_risetimes(latitude_choice,longitude_choice,true);

  // If TERMINAL output is allowed then print out status message
  if(DEBUG)
    Serial.print("Sunset: ");
    Serial.print(sunset_time.toInt());
    Serial.print("Sunrise: ");
    Serial.print(sunrise_time.toInt());
    Serial.print("Time: ");
    Serial.print(stamp.toInt());
    Serial.print("\n");

  // Set after sunset contrast if the sunset hour is smaller than the current hour
  if (sunset_time.toInt() <= stamp.toInt()){
    set_contrast(contrast_after_sunset_choice);

    
    // If TERMINAL output is allowed then print out status message
    if(DEBUG)
      Serial.print("changing contrast after sunset");
  }
  // Set after sunrise contrast if the sunrise hour is smaller than the current hour
  else if (sunrise_time.toInt() <= stamp.toInt()){
    set_contrast(contrast_after_sunrise_choice);

    // If TERMINAL output is allowed then print out status message
    if(DEBUG)  
      Serial.print("changing contrast after sunrise");
  }
  // Wait for the selected time duration, note that every API has a limit to the amount of requests per minute
  vTaskDelay((100) / portTICK_RATE_MS);
} 

/* App: BlockHeight
 * Required: display_BlockHeight (helper function)
 * 
 * Functionality: Displays current block height
 * Revision: Filip Hanus, 16/08/2022
 */

void display_BlockHeight(String blockheight)
// Display the blockheight data on the displays
{
  // Process the blockheight data and split it for the display positions
  String D6 = String(blockheight[0]);
  String D5 = String(blockheight[1]);
  String D4 = String(blockheight[2]);
  String D3 = String(blockheight[3]);
  String D2 = String(blockheight[4]);
  String D1 = String(blockheight[5]);

  // Display each one of the characters on each one of the displays
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

void BlockHeight()
// Obtain blockheight data and display it on the displays
{
  // Setup request url
  String request_url = ("https://mempool.space/api/blocks/tip/height");
  
  // Create a json variable for the data
  const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
  DynamicJsonDocument doc(sized_buffer);
  
  // Request data
  String payload = request_data(request_url);
  
  // Process the requested data
  const char* json_data = payload.c_str();

  if(DEBUG)
    Serial.print("Block Height: ");
    Serial.println(json_data);
    
  
  // Display the block height
  display_BlockHeight(json_data);

  // Wait for the selected time duration, note that every API has a limit to the amount of requests per minute
  vTaskDelay(1500 / portTICK_PERIOD_MS);
}

/* App: CryptoApp
 * Required: display_price (helper function)
 * Input: String selected_coin,String selected_fiat
 * 
 * Functionality: Displays current price
 * Revision: Filip Hanus, 16/08/2022
 */

void display_price(float number = 0,bool decimals=true,String selected_coin = "bitcoin" ,String selected_fiat = "usd")
// Display price data
{
  String str_value_f, d2, d3, d4;

  // Decide how many decimals will be kept based on the lenght of the number
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

  // Process the digits of the price and split them in order to fit them to the displays
  
  unsigned int len = str_value_f.length(); 
  
  String D6 = String(str_value_f[len-1]);
  String D5 = String(str_value_f[len-2]);
  String D4 = String(str_value_f[len-3]);
  String D3 = String(str_value_f[len-4]);
  String D2 = String(str_value_f[len-5]);
  String D1 = String(str_value_f[len-6]);

  // Print the digits and symbols onto the displays
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

void CryptoApp(String selected_coin,String selected_fiat)
// Obtain the crypto price data and display it
{ 
  // Setup request url
  String request_url = ("https://api.coingecko.com/api/v3/simple/price?ids=" + selected_coin + "&vs_currencies=" + selected_fiat);

  // Create a json variable for the data
  const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
  DynamicJsonDocument doc(sized_buffer);
  
  // Request data
  String payload = request_data(request_url);

  // Process the requested data
  const char* json_data = payload.c_str();
  deserializeJson(doc, json_data);
  JsonObject json_coin = doc[selected_coin];
  float selected_coin_f = json_coin[selected_fiat];

  // Request overflow temporary management
  if(selected_coin_f != 0.000){last_call = selected_coin_f;}
  else{selected_coin_f = last_call;}
  
  // If TERMINAL output is allowed then print out status message
  if(DEBUG)
    Serial.print("Price: " + String(selected_coin_f) + " " + String(selected_coin));

  // Display data
  display_price(selected_coin_f,true,selected_coin,selected_fiat);

  // Wait for a selected amount of time, note that the coingecko allows only 50 requests per minute (hence 1s delay is too short)
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}

/* App: Difficulty
 * Required: display_difficulty (helper function)
 * 
 * Functionality: Displays current difficulty (%)
 * Revision: Filip Hanus, 16/08/2022
 */

void display_difficulty(String message)
// Split the message that is to be displayed into separate digits and show it on the displays
{
  // Process the digits of the difficulty and split them in order to fit them to the displays
  String D5 = String(message[4]);
  String D4 = String(message[3]);
  String D3 = String(message[2]);
  String D2 = String(message[1]);
  String D1 = String(message[0]);

  // Display the digits on the displays
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

void Difficulty()
// Obtain difficulty data and display it
{
  // Setup request url
  String request_url = ("https://mempool.space/api/v1/difficulty-adjustment");

  // Create a json variable for the data
  const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
  DynamicJsonDocument doc(sized_buffer);
  
  // Request data
  String payload = request_data(request_url);

  // Process the requested data
  const char* json_data = payload.c_str();
  deserializeJson(doc, json_data);
  String selected_coin_f = doc["difficultyChange"];

  // Print data
  if(DEBUG)
    Serial.print("Difficulty: " + String(selected_coin_f));

  // Display the difficulty
  display_difficulty(selected_coin_f);

  // Wait for the selected time duration, note that every API has a limit to the amount of requests per minute
  vTaskDelay(1500 / portTICK_PERIOD_MS);
}

/* App: Fees
 * Required: display_fees (helper function)
 * 
 * Functionality: Displays current fees
 * Revision: Filip Hanus, 16/08/2022
 */

void display_fees(String fastest_fee, String hour_fee)
// Display obtained fees
{
  // Prepare message to be displayed
  String message = String(fastest_fee) + " " + String(hour_fee);

  // Separate the message for the displays
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

  // Display the separated digits of the fees
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

void Fees()
// Obtain and print out fees
{
  // Setup request url
  String request_url = ("https://mempool.space/api/v1/fees/recommended");

  // Create a json variable for the data
  const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
  DynamicJsonDocument doc(sized_buffer);
  
  // Request data
  String payload = request_data(request_url);

  // Process the requested data
  const char* json_data = payload.c_str();
  deserializeJson(doc, json_data);

  String fastest_fee = doc["fastestFee"];
  String hour_fee = doc["hourFee"];   

  // Print data
  if(DEBUG)
    Serial.println("Fastest fee: " + String(fastest_fee) + " Hour fee: " + String(hour_fee));

  // Display the fees
  display_fees(fastest_fee,hour_fee);

  // Wait for the selected time duration, note that every API has a limit to the amount of requests per minute
  vTaskDelay(1500 / portTICK_PERIOD_MS);
}

/* App: Halving
 * Required: display_halving (helper function)
 * 
 * Functionality: Displays current halving for mining
 * Revision: Filip Hanus, 16/08/2022
 */

void display_halving(String halving)
// Display obtained halving on the displays
{
  // Separate the data into the separate digits for the displays
  int len = halving.length();
    
  String D8 = String(halving[len-1]);
  String D7 = String(halving[len-2]);
  String D6 = String(halving[len-3]);
  String D5 = String(halving[len-4]);
  String D4 = String(halving[len-5]);
  String D3 = String(halving[len-6]);
  String D2 = String(halving[len-7]);
  String D1 = String(halving[len-8]);

  // Display the digits on the displays
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

void Halving()
// Obtain and display halving
{
  // Setup request url
  String request_url = ("https://mempool.space/api/blocks/tip/height");

  // Request data
  String payload = request_data(request_url);

  // Format data
  String json_data = payload.c_str();
  int height_i = json_data.toInt();  

  String halving_i = String(210000 - int(height_i) % 210000);

  // Display data
  if(DEBUG)
    Serial.println("Halving: " + String(halving_i));

  // Display the halving
  display_halving(halving_i);

  // Wait for the selected time duration, note that every API has a limit to the amount of requests per minute
  vTaskDelay(1500 / portTICK_PERIOD_MS);
}

/* App: MarketCap
 * Required: get_MarketCap, display_marketcap (helper functions)
 * Inputs: String selected_crypto,String selected_fiat
 * 
 * Functionality: Displays current marketcap
 * Revision: Filip Hanus, 16/08/2022
 */

void display_marketcap(String message, String sel_crypto, String sel_fiat)
// Display obtained marketcap on the displays 
{ 
  // Separate the data into the separate digits for the displays 
  int len = message.length();
  
  String D1 = String(message[len-1]);
  String D2 = String(message[len-2]);
  String D3 = String(message[len-3]);
  String D4 = String(message[len-4]);
  String D5 = String(message[len-5]);
  String D6 = String(message[len-6]);

  // Display the digits on the displays
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

void MarketCap(String selected_crypto,String selected_fiat)
// Obtain and display market cap
{
  // Setup request url
  String request_url = ("https://api.coingecko.com/api/v3/coins/"+selected_crypto+"?localization=false&tickers=false&market_data=true&community_data=false&developer_data=false&sparkline=false");
 
  // Create a json variable for the data
  const size_t sized_buffer = 3*JSON_ARRAY_SIZE(0) + JSON_ARRAY_SIZE(1) + 2*JSON_ARRAY_SIZE(2) + 3*JSON_ARRAY_SIZE(3) + JSON_ARRAY_SIZE(10) + 2*JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(11) + JSON_OBJECT_SIZE(28) + JSON_OBJECT_SIZE(42) + 23*JSON_OBJECT_SIZE(61);
  DynamicJsonDocument doc(sized_buffer);
  
  // Request data
  String payload = request_data(request_url);

  // Process the requested data
  deserializeJson(doc,payload,DeserializationOption::NestingLimit(150));
  String selected_coin_f = doc["market_data"]["current_price"][selected_fiat];

  // Print data
  if(DEBUG)
    Serial.println("Market Cap: " + String(selected_coin_f) + " Crypto: " + String(selected_crypto));

  // Display the marketcap
  display_marketcap(selected_coin_f,selected_crypto,selected_fiat);

  // Wait for the selected time duration, note that every API has a limit to the amount of requests per minute
  vTaskDelay(1500 / portTICK_PERIOD_MS);
}

/* App: MoscowTimeApp
 * Required: display_moscowtime (helper function)
 * 
 * Functionality: Displays current "MoscowTime" (= 100 000 000 / Current BTC price)
 * Revision: Filip Hanus, 16/08/2022
 */

void display_moscowtime(String str_value)
// Display obtained moscow time on the displays
{  
  // Separate the data into the separate digits for the displays 
  int len = str_value.length();

  String D4 = String(str_value[len-1]);
  String D3 = String(str_value[len-2]);
  String D2 = String(str_value[1]);
  String D1 = String(str_value[0]);

  // Display the digits on the displays
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

void MoscowTimeApp()
// Obtain and display moscow time
{
   // Setup request url
  String request_url = ("https://api.coingecko.com/api/v3/simple/price?ids=bitcoin&vs_currencies=usd");

  // Create a json variable for the data
  const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
  DynamicJsonDocument doc(sized_buffer);
  
  // Request data
  String payload = request_data(request_url);

  // Process the requested data
  const char* json_data = payload.c_str();
  deserializeJson(doc, json_data);
  int price = doc["bitcoin"]["usd"];
  int p_price = (100000000 / price);
  String str_price = String(int(p_price));

  // Print data
  if(DEBUG)
    Serial.println("Moscow time: " + String(str_price));

  // Display the moscowtime
  display_moscowtime(str_price);

  // Wait for the selected time duration, note that every API has a limit to the amount of requests per minute
  vTaskDelay(1500 / portTICK_PERIOD_MS);
}

/* App: TimeApp
 * Required: display_time (helper function)
 * Inputs: String timezone, bool hide_sec
 * 
 * Functionality: Displays current time
 * Revision: Filip Hanus, 16/08/2022
 */

void display_time(String time_unparsed,bool hide_sec)
// Display obtained halving on the displays 
{
  // Separate the data into the separate digits for the displays 
  String str_value_f = String(time_unparsed);
  
  String D6 = String(str_value_f[7]);
  String D5 = String(str_value_f[6]);
  String D4 = String(str_value_f[4]);
  String D3 = String(str_value_f[3]);
  String D2 = String(str_value_f[1]);
  String D1 = String(str_value_f[0]);

  // Display the digits on the displays
  // Display without seconds
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
  // Display with seconds
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

void TimeApp(String timezone= "Europe/Prague", bool hide_sec=true)
// Obtain and display time
{
  // Store current processor time
  unsigned long lastMillis_second = millis();

   // Request the current time and format it
  String timestamp = String(get_time(timezone_choice));

  // Print current time
  if(DEBUG)
    Serial.println("Time: " + String(timestamp));
   
  // If seconds are supposed to be hidden
  if(hide_sec==false){
    if(timestamp[6] == 0){
      // Display the current time
      display_time(timestamp,hide_sec);

      // Wait until the next minute
      unsigned int delay_i = (60*1000) - (millis()-lastMillis_second);
      if (delay_i < (60*1000)){vTaskDelay( delay_i / portTICK_PERIOD_MS);}  
    }
  }
  // If seconds are supposed to be shown
  else if(hide_sec==true){
    // Display the current time
    display_time(timestamp,hide_sec);

    // Wait until the next second
    unsigned int delay_i = 1000 - (millis()-lastMillis_second);
    if (delay_i < 1000){vTaskDelay( delay_i / portTICK_PERIOD_MS);}
  }
}

/* App: Temperature
 * Required: display_temp (helper function)
 * Inputs: String city, String api_key, String units
 * 
 * Functionality: Displays current temperature
 * Revision: Filip Hanus, 16/08/2022
 * 
 * Notes: Untested! Was not able to get the API requirements to properly test the function
 */

void display_temp(String temp)
// Display obtained temperature on the displays 
{
  // Separate the data into the separate digits for the displays // Display the digits on the displays
  int len = temp.length();
  
  String D3 = String(temp[len-1]);
  String D2 = String(temp[len-2]);
  String D1 = String(temp[len-3]);

  // Separate the data into the separate digits for the displays // Display the digits on the displays
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

void Temperature(String city, String api_key, String units)
// Obtain and display temperature
{
  // Setup request url
  String request_url = ("http://api.openweathermap.org/data/2.5/weather?q=" + city + "&appid=" + api_key + "&units={}" + units);

  // Create a json variable for the data
  const size_t sized_buffer = JSON_OBJECT_SIZE(2) + 2 * JSON_OBJECT_SIZE(6) + 150;
  DynamicJsonDocument doc(sized_buffer);
  
  // Request data
  String payload = request_data(request_url);

  // Process the requested data
  const char* json_data = payload.c_str();
  deserializeJson(doc, json_data);
  String temp = doc["main"]["temp"];

  if(DEBUG)
    Serial.println("Temperature: " + String(temp));

  // Display the temperature
  display_temp(temp);

  // Wait for the selected time duration, note that every API has a limit to the amount of requests per minute
  vTaskDelay((3600 * 1000)/ portTICK_PERIOD_MS);
}

/* App: App_Selector
 * Inputs: String selected_app
 * 
 * Functionality: Displays selected app
 * Revision: Filip Hanus, 16/08/2022
 */

void App_Selector()
// Run the selected app
{
  // Print currently selected app
  if(DEBUG)
    Serial.print("Running App Selector: ");
    Serial.print(app_choice);

  // Run selected app (compare the char array containing app selection with all the apps and run the one that matches)
  if(strcmp(app_choice, "BlockHeight") == 0){BlockHeight();}
  else if(strcmp(app_choice, "CryptoApp") == 0){CryptoApp(String(crypto_choice),String(fiat_choice));}
  else if(strcmp(app_choice, "Difficulty") == 0){Difficulty();}
  else if(strcmp(app_choice, "Fees") == 0){Fees();}
  else if(strcmp(app_choice, "Halving") == 0){Halving();}
  else if(strcmp(app_choice, "MarketCap") == 0){MarketCap(String(crypto_choice),String(fiat_choice));}
  else if(strcmp(app_choice, "MoscowTimeApp") == 0){MoscowTimeApp();}
  else if(strcmp(app_choice, "TimeApp") == 0){TimeApp(String(timezone_choice), bool(hide_seconds));}
}



 
