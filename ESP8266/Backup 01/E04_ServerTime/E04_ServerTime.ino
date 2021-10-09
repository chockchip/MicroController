#include "defines.h"
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <TridentTD_LineNotify.h>
/*
List of virtual pin for blynk
V0: LED pin D4
V1: Temperature
V2: Humidity
V3: Pump
V4: Pump SW
V5: Pump Timer
V6: Time
V7: Date
V8: SW use pump timer *** Waiting create function
*/


/*
Blynk server

User: component@gmail.com
Pass: Neng@1325
Server: blank.iot-cm.com:9443
Command: http://49.229.108.138:8080/XhrSKV2HFQi-3YOIf8NsL5_caLPW_aS0/update/D2?value=1
*/

///////////////////////////////////////////////////////////////////
#if ( USING_ESP8266_CORE_VERSION >= 30000 )
  // Only to deal with ESP8266 core v3.0.0 warning in Ticker library
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wcast-function-type"
  
  #include <Ticker.h>
  #pragma GCC diagnostic pop
#else
  #include <Ticker.h>
#endif
///////////////////////////////////////////////////////////////////

#include <Wire.h>
#include <Adafruit_AM2315.h>        // To install Adafruit AM2315 library

// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock (PIN_D1 / SCL)
// Connect YELLOW to i2c data (PIN_D2 / SDA)

/* Parameter setup */

// Temperature and Humidity sensor
Adafruit_AM2315 AM2315;
#define AM2315_DEBUG     true
#define READ_INTERVAL        30000          //read AM2315 interval 30s

// Line Token
#define LINE_TOKEN "BH0OMQ3UYZlUCIrrIhFxagPYs9RFLUbGhnfFmxptshQ"

// Blynk RTC
WidgetRTC rtc;

// Bylnk parameter
BlynkTimer timer;
Ticker     led_ticker;

// LED parameter
int led_pin = 2; // D4
int previous_led_status = 99;
int current_led_status = 0;

// Pump parameter
int pump_status = 0;
int sw_pump = 0;
int timer_pump = 0;

/* Function setup */

void clockDisplay()
{
  // You can call hour(), minute(), ... at any time
  // Please see Time library examples for details

  String currentTime = String(hour()) + ":" + minute() + ":" + second();
  String currentDate = String(day()) + " " + month() + " " + year();
  Serial.print("Current time: ");
  Serial.print(currentTime);
  Serial.print(" ");
  Serial.print(currentDate);
  Serial.println();

  // Send time to the App
  Blynk.virtualWrite(V6, currentTime);
  // Send date to the App
  Blynk.virtualWrite(V7, currentDate);
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}

// Read temperature and humidity from AM2315
void ReadData()
{
  static float temperature, humidity;

  if (!AM2315.readTemperatureAndHumidity(&temperature, &humidity))
  {
#if AM2315_DEBUG
    Serial.println("Failed to read data from AM2315");
#endif

    return;
  }

#if AM2315_DEBUG
  Serial.println("Temp *C: " + String(temperature));
  Serial.println("Humid %: " + String(humidity));
#endif

  //V1 and V2 are Blynk Display widgets' VPIN
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
}

// Set LED status
void set_led(byte status)
{ 
  digitalWrite(LED_BUILTIN, status);
}

#if USING_CUSTOMS_STYLE
const char NewCustomsStyle[] /*PROGMEM*/ = "<style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}\
button{background-color:blue;color:white;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style>";
#endif

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(200);

#if ( USE_LITTLEFS || USE_SPIFFS)
  Serial.print(F("\nStarting AM2315_ESP8266 using "));
  Serial.print(CurrentFileFS);
#else
  Serial.print("\nStarting AM2315_ESP8266 using EEPROM");
#endif

#if USE_SSL
  Serial.print(F(" with SSL on ")); Serial.println(ARDUINO_BOARD);
#else
  Serial.print(F(" without SSL on ")); Serial.println(ARDUINO_BOARD);
#endif

#if USE_BLYNK_WM
  Serial.println(ESP8266_CORE_VERSION);
  Serial.println(BLYNK_WM_VERSION);
  Serial.println(ESP_DOUBLE_RESET_DETECTOR_VERSION);
#endif

  if (!AM2315.begin())
  {
    Serial.println(F("Sensor not found, check wiring & pullups!"));
  }

#if USE_BLYNK_WM
  // From v1.0.5
  // Set config portal SSID and Password
  Blynk.setConfigPortal("TestPortal", "TestPortalPass");
  // Set config portal IP address
  Blynk.setConfigPortalIP(IPAddress(192, 168, 200, 1));
  // Set config portal channel, defalut = 1. Use 0 => random channel from 1-13
  Blynk.setConfigPortalChannel(0);

  // From v1.0.5, select either one of these to set static IP + DNS
  Blynk.setSTAStaticIPConfig(IPAddress(192, 168, 2, 220), IPAddress(192, 168, 2, 1), IPAddress(255, 255, 255, 0));
  //Blynk.setSTAStaticIPConfig(IPAddress(192, 168, 2, 220), IPAddress(192, 168, 2, 1), IPAddress(255, 255, 255, 0),
  //                           IPAddress(192, 168, 2, 1), IPAddress(8, 8, 8, 8));
  //Blynk.setSTAStaticIPConfig(IPAddress(192, 168, 2, 220), IPAddress(192, 168, 2, 1), IPAddress(255, 255, 255, 0),
  //                           IPAddress(4, 4, 4, 4), IPAddress(8, 8, 8, 8));

//////////////////////////////////////////////
  
#if USING_CUSTOMS_STYLE
  Blynk.setCustomsStyle(NewCustomsStyle);
#endif

#if USING_CUSTOMS_HEAD_ELEMENT
  Blynk.setCustomsHeadElement("<style>html{filter: invert(10%);}</style>");
#endif

#if USING_CORS_FEATURE  
  Blynk.setCORSHeader("Your Access-Control-Allow-Origin");
#endif

//////////////////////////////////////////////
  
  // Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
  //Blynk.begin();
  // Use this to personalize DHCP hostname (RFC952 conformed)
  // 24 chars max,- only a..z A..Z 0..9 '-' and no '-' as last char
  //Blynk.begin("AM2315_ESP8266");
  Blynk.begin(HOST_NAME);
#else
  WiFi.begin(ssid, pass);

#if USE_LOCAL_SERVER
  Blynk.config(auth, blynk_server, BLYNK_HARDWARE_PORT);
#else
  Blynk.config(auth);
#endif

  Blynk.connect();
#endif

  if (Blynk.connected())
  {
#if ( USE_LITTLEFS || USE_SPIFFS)
    Serial.print(F("\nBlynk ESP8288 using "));
    Serial.print(CurrentFileFS);
    Serial.println(F(" connected."));
#else
    {
      Serial.println(F("\nBlynk ESP8288 using EEPROM connected."));
      Serial.printf("EEPROM size = %d bytes, EEPROM start address = %d / 0x%X\n", EEPROM_SIZE, EEPROM_START, EEPROM_START);
    }
#endif

#if USE_BLYNK_WM
    Serial.print(F("Board Name : ")); Serial.println(Blynk.getBoardName());
#endif
  }

  // Line setup
  LINE.setToken(LINE_TOKEN);
  // LINE.notify("Your device is reset");
  LINE.notifyPicture("Your device is reset", "https://s3-eu-central-1.amazonaws.com/cartoons-s3/styles/product_detail_image/s3/cartoons/2019/02/reset__paolo_calleri.jpg?itok=JAqFCOSz");
  LINE.notifySticker("Done", 6325, 10979904);

  //RTC setup
  setSyncInterval(10 * 60); // Sync interval in seconds (10 minutes)
  timer.setInterval(10000L, clockDisplay); // Display digital clock every 10 seconds

  timer.setInterval(READ_INTERVAL, ReadData);

  // Clear pump status when reset ESP8266
  Blynk.virtualWrite(V3, 0);
  Blynk.virtualWrite(V4, 0);
  Blynk.virtualWrite(V5, 0);

  // Clear LED when reset 
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

#if (USE_BLYNK_WM && USE_DYNAMIC_PARAMETERS)
void displayCredentials()
{
  Serial.println(F("\nYour stored Credentials :"));

  for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
  {
    Serial.print(myMenuItems[i].displayName);
    Serial.print(F(" = "));
    Serial.println(myMenuItems[i].pdata);
  }
}

void displayCredentialsInLoop()
{
  static bool displayedCredentials = false;

  if (!displayedCredentials)
  {
    for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
    {
      if (!strlen(myMenuItems[i].pdata))
      {
        break;
      }

      if ( i == (NUM_MENU_ITEMS - 1) )
      {
        displayedCredentials = true;
        displayCredentials();
      }
    }
  }
}

#endif

// Update LED statsu to Blynk
void updateLedStatus()
{
  current_led_status = digitalRead(led_pin);
  if (current_led_status == previous_led_status)
  {
    return;
  }

  previous_led_status = current_led_status;
  Blynk.virtualWrite(V0, current_led_status * 255);
  Serial.print("LED status: ");
  Serial.print(current_led_status);
}

int block_clicking = 0;
void block_double_click()
{
  block_clicking = 1; 
}

// Read SW pump
BLYNK_WRITE(V4)
{   
  // For protect double clicking
  if (block_clicking == 1)
  {
    block_clicking = 0;
    Serial.println("Block Clicking");
    return;
  }

  if (pump_status == 255)
  {
    Blynk.virtualWrite(V3, 0);
    pump_status = 0;
    timer_pump = 0;
    Serial.println("The pump is turn off");
    block_double_click();
    return;
  }
  
  if (pump_status == 0)
  {
    Blynk.virtualWrite(V3, 255);
    pump_status = 255;
    timer_pump = 0;
    Serial.println("The pump is turn on");
    LINE.notify("Pump is started");
    block_double_click();
    return;
  }

  //sw_pump = param.asInt(); // Get value as integer
  //Blynk.virtualWrite(V3, 255);
  //Serial.print("Pump status: ");
  //Serial.println(sw_pump);
}

// Read Timer pump
BLYNK_WRITE(V5)
{   
  timer_pump = param.asInt(); // Get value as integer
  Blynk.virtualWrite(V3, timer_pump * 255);
  pump_status = timer_pump * 255;
  Serial.print("The pump: ");
  Serial.println(timer_pump);
 
}

void loop()
{
  
  updateLedStatus();
 
  Blynk.run();
  timer.run();
  //check_status();

#if (USE_BLYNK_WM && USE_DYNAMIC_PARAMETERS)
  displayCredentialsInLoop();
#endif
}
