#include <libraries.h>
#include <global_variables.h>
#include <LittleFS.h>
#include "main.h"
#include "esp_log.h"

#define CAPTIVE_DOMAIN "http://wordclock" // This is the SSID that ESP32 will broadcast
// Uncomment the following line to enable password in the wifi acces point
// #define PASSWORD "12345678" // password should be atleast 8 characters to make it work
#define DNS_PORT 53
// Options to enable serial prin ting
#define VERBOSE

// #define SSID "ESP32 SoftAP" // This is the SSID that ESP32 will broadcast
const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

String testTimezone = "CET-1CEST,M3.5.0,M10.5.0/3";

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");

String split(String s, char parser, int index)
{
  String rs = "";
  // int parserIndex = index;
  int parserCnt = 0;
  int rFromIndex = 0, rToIndex = -1;
  while (index >= parserCnt)
  {
    rFromIndex = rToIndex + 1;
    rToIndex = s.indexOf(parser, rFromIndex);
    if (index == parserCnt)
    {
      if (rToIndex == 0 || rToIndex == -1)
        return "";
      return s.substring(rFromIndex, rToIndex);
    }
    else
      parserCnt++;
  }
  return rs;
}

/**
 * @brief Connects to a WiFi network using the provided SSID and password.
 *
 * This function attempts to connect to a WiFi network with the given SSID and password.
 * It waits for up to 5 seconds for the connection to be established. If the connection
 * is successful, it sets the hostname and prints the local IP address to the serial monitor.
 *
 * @param txtSSID The SSID of the WiFi network to connect to.
 * @param txtPassword The password of the WiFi network to connect to.
 * @return int Returns 1 if the connection is successful, -1 if the connection fails.
 */
int WiFi_RouterNetworkConnect(String txtSSID, String txtPassword)
{
  char *SSID2 = const_cast<char *>(txtSSID.c_str());
  char *Password2 = const_cast<char *>(txtPassword.c_str());
  int success = 1;
  String message = "Trying to connect to " + txtSSID;

  WiFi.setHostname(HOSTNAME);
  WiFi.begin(SSID2, static_cast<const char *>(Password2));
  // print hostname
  Serial.println(String("Hostname: ") + WiFi.getHostname());

  // we wait until connection is established
  // or 5 seconds are gone

  int WiFiConnectTimeOut = 0;
  while ((WiFi.status() != WL_CONNECTED) && (WiFiConnectTimeOut < 5))
  {
    delay(1000);
    WiFiConnectTimeOut++;
  }

  // not connected
  if (WiFi.status() != WL_CONNECTED)
  {
    success = -1;
    wificonnected = false;
    String message = "Connection to Wifi failed";
  }
  else
  {
    wificonnected = true;
    String message = "Connection to Wifi successful";
    Serial.println(message);
    Serial.println(WiFi.getHostname());
  }

  Serial.println(WiFi.localIP());

  return success;
}

/**
 * @brief Sets the color values from a hexadecimal color code.
 *
 * This function converts a hexadecimal color code (in the format #RRGGBB)
 * to its corresponding red, green, and blue values. The color code is
 * expected to be stored in the global variable `ColorInHex`.
 *
 * @note The global variables `redval`, `blueval`, and `greenval` are
 *       updated with the respective color values.
 */
void setColor()
{

  long number = (long)strtol(&ColorInHex[1], NULL, 16);
  redval = number >> 16;
  blueval = number >> 8 & 0xFF;
  greenval = number & 0xFF;
}

/**
 * @brief Sets the color of a list of LEDs to the specified RGB values.
 *
 * @param LedList An array of integers representing the indices of the LEDs to be set.
 * @param length The number of LEDs in the LedList array.
 * @param redval The red component of the color (0-255).
 * @param greenval The green component of the color (0-255).
 * @param blueval The blue component of the color (0-255).
 */
void setLedList(int LedList[], int length, int redval, int greenval, int blueval)
{
  for (int i = 0; i < length; i++)
  {
    leds[(LedList[i])] = CRGB(redval, greenval, blueval);
  }
}

/**
 * @brief Sets the LED display to indicate the given hour in the specified language.
 *
 * This function converts the given hour to a 12-hour format if necessary and then
 * sets the LED display to show the corresponding hour in the specified language.
 *
 * @param h The hour to set (0-23). The function will convert this to a 12-hour format.
 * @param lang The language index to use for the LED display.
 *
 * The function handles the following conversions:
 * - 0 is converted to 12 (midnight).
 * - 13 is converted to 1 (1 PM).
 * - Any hour greater than 12 is converted to a 12-hour format.
 *
 * The function then uses a switch statement to call setLedList with the appropriate
 * hour array and its length for the specified language.
 */
void sethour(int h, int lang) // hours setzen
{

  if (h == 13)
  {
    h = 1;
  }
  if (h == 0)
  {
    h = 12;
  }
  if (h > 12)
  {
    h = h - 12;
  }

  switch (h)
  {
  case 1:
    setLedList(hour1[lang], len_hour1[lang], redval, blueval, greenval);
    break;
  case 2:
    setLedList(hour2[lang], len_hour2[lang], redval, blueval, greenval);
    break;
  case 3:
    setLedList(hour3[lang], len_hour3[lang], redval, blueval, greenval);
    break;
  case 4:
    setLedList(hour4[lang], len_hour4[lang], redval, blueval, greenval);
    break;
  case 5:
    setLedList(hour5[lang], len_hour5[lang], redval, blueval, greenval);
    break;
  case 6:
    setLedList(hour6[lang], len_hour6[lang], redval, blueval, greenval);
    break;
  case 7:
    setLedList(hour7[lang], len_hour7[lang], redval, blueval, greenval);
    break;
  case 8:
    setLedList(hour8[lang], len_hour8[lang], redval, blueval, greenval);
    break;
  case 9:
    setLedList(hour9[lang], len_hour9[lang], redval, blueval, greenval);
    break;
  case 10:
    setLedList(hour10[lang], len_hour10[lang], redval, blueval, greenval);
    break;
  case 11:
    setLedList(hour11[lang], len_hour11[lang], redval, blueval, greenval);
    break;
  case 12:
    setLedList(hour12[lang], len_hour12[lang], redval, blueval, greenval);
    break;
  }
}

/**
 * @brief Displays the time on an LED matrix.
 *
 * This function updates the LED matrix to display the current time in a human-readable format.
 * It uses various predefined LED patterns to represent different time intervals.
 *
 * @param m The current minute (0-59).
 * @param h The current hour (0-23).
 * @param lang The language index for the LED patterns.
 *
 * The function performs the following steps:
 * 1. Clears the LED matrix.
 * 2. Displays "It is" in the selected language.
 * 3. Determines the appropriate time interval and updates the LED matrix accordingly.
 * 4. Adjusts the hour if necessary.
 * 5. Sets the hour on the LED matrix.
 * 6. Updates the LED matrix to show the exact minute using corner LEDs.
 * 7. Shows the updated LED matrix.
 * 8. Introduces a short delay.
 */
void displayTime(int m, int h, int lang) // minutes setzen
{
  if (h > 12)
  {
    h = h - 12;
  }

  FastLED.clear();

  // ES IST
  setLedList(ItIs[lang], len_ItIs[lang], redval, blueval, greenval);

  int e = 0;
  e = m % 10;

  if ((m >= 0 && m < 5) || m == 60)
  {

    setLedList(OClock[lang], len_OClock[lang], redval, blueval, greenval);
    if (h == 1 and time == 0)
    {
      leds[56] = CRGB::Black;
    }
  }

  if (m >= 5 && m < 10)
  {

    setLedList(five[lang], len_five[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 10 && m < 15)
  {

    setLedList(ten[lang], len_ten[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 15 && m < 20)
  {

    setLedList(quarter[lang], len_quarter[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 20 && m < 25)
  {

    setLedList(twenty[lang], len_twenty[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 25 && m < 30)
  {

    setLedList(five[lang], len_five[lang], redval, blueval, greenval);
    setLedList(twenty[lang], len_twenty[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 30 && m < 35)
  {

    // DE_half
    setLedList(half[lang], len_half[lang], redval, blueval, greenval);
  }

  if (m >= 35 && m < 40)
  {

    setLedList(five[lang], len_five[lang], redval, blueval, greenval);
    setLedList(twenty[lang], len_twenty[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 40 && m < 45)
  {
    setLedList(twenty[lang], len_twenty[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 45 && m < 50)
  {
    setLedList(quarter[lang], len_quarter[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 50 && m < 55)
  {
    setLedList(ten[lang], len_ten[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 55 && m < 60)
  {
    setLedList(five[lang], len_five[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 30 && m < 60)
  {
    h++;
  }
  sethour(h, lang);

  if (e >= 5)
  {
    e = e - 5;
  }

  switch (e)
  { // ecken setzten

  case 1:
    setLedList(min1, 1, redval, blueval, greenval);
    break;
  case 2:
    setLedList(min2, 2, redval, blueval, greenval);
    break;
  case 3:
    setLedList(min3, 3, redval, blueval, greenval);
    break;
  case 4:
    setLedList(min4, 4, redval, blueval, greenval);
    break;
  }

  FastLED.show();
  delay(50);
}

/**
 * @brief Initializes values from non-volatile storage (NVS) and stores them in RAM.
 *
 * This function retrieves stored configuration values from NVS and assigns them to
 * corresponding variables in RAM. If a value is not found in NVS, it stores the
 * default value back to NVS.
 *
 * The following values are initialized:
 * - SSID: WiFi SSID
 * - Password: WiFi password
 * - Timezone: Timezone in UMT format
 * - DST: Daylight Saving Time (Summertime) active status
 * - ColorHEX: Color in hexadecimal format
 * - NightmodeActive: Night mode active status
 * - NightmodeFrom: Night mode start time
 * - NightmodeTo: Night mode end time
 * - Perc: Brightness percentage
 * - NightPerc: Night mode brightness percentage
 * - Language: Language setting

 */
void initvalues()
{
  preferences.begin("Wordclock", false);

  // takeout Strings out of the Non-volatile storage
  String strSSID = preferences.getString("SSID", "");
  String strPassword = preferences.getString("Password", "");
  String strTimezone = preferences.getString("Timezone", "");
  String strColorHex = preferences.getString("ColorHEX", "");
  String strNightmodeActive = preferences.getString("NightmodeActive", "");
  String strNightmodeFrom = preferences.getString("NightmodeFrom", "");
  String strNightmodeTo = preferences.getString("NightmodeTo", "");
  String strBrightnessPercent = preferences.getString("Perc", "");
  String strNightmodeBrightness = preferences.getString("NightPerc", "");
  String strLanguage = preferences.getString("Language", "");

  // put the NVS stored values in RAM for the program if they contain information
  if (strColorHex != "")
  {
    ColorInHex = strColorHex;
  }
  else
  {
    preferences.putString("ColorHEX", ColorInHex);
  }

  if (strBrightnessPercent != "")
  {
    BrightnessPercent = strBrightnessPercent;
  }
  else
  {
    preferences.putString("Perc", BrightnessPercent);
  }

  if (strTimezone != "")
  {
    TimezoneUMT = strTimezone;
  }
  else
  {
    preferences.putString("Timezone", TimezoneUMT);
  }

 

  if (strNightmodeFrom != "")
  {
    NightmodeFrom = strNightmodeFrom;
  }
  else
  {
    preferences.putString("NightmodeFrom", NightmodeFrom);
  }

  if (strNightmodeTo != "")
  {
    NightmodeTo = strNightmodeTo;
  }
  else
  {
    preferences.putString("NightmodeTo", NightmodeTo);
  }

  if (strNightmodeActive != "")
  {
    NightmodeActive = strNightmodeActive;
  }
  else
  {
    preferences.putString("NightmodeActive", NightmodeActive);
  }

  if (strNightmodeBrightness != "")
  {
    NightmodeBrightnessPercent = strNightmodeBrightness;
  }
  else
  {
    preferences.putString("NightPerc", BrightnessPercent);
  }

  if (strSSID != "")
  {
    WIFISSID = strSSID;
  }
  else
  {
    preferences.putString("SSID", WIFISSID);
  }

  if (strPassword != "")
  {
    WIFIPassword = strPassword;
  }
  else
  {
    preferences.putString("Password", WIFIPassword);
  }
  if (strLanguage != "")
  {
    Language = strLanguage;
  }
  else
  {
    preferences.putString("Language", Language);
  }
}

/**
 * @brief Adjusts the brightness of the LEDs based on the current time and night mode settings.
 *
 * This function determines whether it is currently nighttime based on the provided start and end times
 * for night mode. If night mode is active and the current time falls within the specified night mode
 * period, the LED brightness is set to the night mode brightness percentage. Otherwise, it is set to
 * the normal brightness percentage.
 *
 * The function uses the following global variables:
 * - NightmodeFrom: A string representing the start time of night mode in "HH:MM" format.
 * - NightmodeTo: A string representing the end time of night mode in "HH:MM" format.
 * - NightmodeActive: A string representing whether night mode is active ("1" for active, "0" for inactive).
 * - NightmodeBrightnessPercent: A string representing the brightness percentage during night mode.
 * - BrightnessPercent: A string representing the normal brightness percentage.
 * - hours: An integer representing the current hour.
 * - minutes: An integer representing the current minute.
 *
 * The function uses the FastLED library to set the brightness of the LEDs.
 */
void setBrightness()
{
  boolean nighttime = false;

  String temphstart = NightmodeFrom;
  String temphend = NightmodeTo;
  String tempmstart = NightmodeFrom;
  String tempmend = NightmodeTo;

  int temphourstart = split(temphstart, ':', 0).toInt();
  int temphourend = split(temphend, ':', 0).toInt();
  int tempminstart = NightmodeFrom.substring(3, 5).toInt();
  int tempminend = NightmodeTo.substring(3, 5).toInt();

  if (NightmodeActive.toInt() == 1)
  {
    if (temphourstart > temphourend)
    {
      if (hours >= temphourstart || hours <= temphourend)
      {
        // DE_pastthelligkeit
        if (hours != temphourstart && hours != temphourend)
        {
          nighttime = true;
        }

        if (hours == temphourstart && minutes >= tempminstart)
        {
          nighttime = true;
        }
        if (hours == temphourend && minutes <= tempminend)
        {
          nighttime = true;
        }
      }
    }

    if (temphourstart == temphourend)
    {
      if (hours == temphourstart && hours == temphourend)
      {

        if (minutes >= tempminstart && minutes <= tempminend)
        {
          nighttime = true;
        }
      }
    }

    if (temphourstart < temphourend)
    {

      if (hours >= temphourstart && hours <= temphourend)
      {
        // DE_pastt
        if (hours != temphourstart && hours != temphourend)
        {
          nighttime = true;
        }
        if (hours == temphourstart && minutes >= tempminstart)
        {
          nighttime = true;
        }
        if (hours == temphourend && minutes <= tempminend)
        {
          nighttime = true;
        }
      }
    }
  }

  if (nighttime == true)
  {
    FastLED.setBrightness(map(NightmodeBrightnessPercent.toInt(), 0, 100, 0, 255));
  }
  else
  {
    FastLED.setBrightness(map(BrightnessPercent.toInt(), 0, 100, 0, 255));
  }
}

void initTime(String timezone)
{
  struct tm timeinfo;

  Serial.println("Setting up time");
  configTime(0, 0, "pool.ntp.org"); // First connect to NTP server, with 0 TZ offset
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("  Failed to obtain time");
    return;
  }
  Serial.println("  Got the time from NTP");
  // Now we can set the real timezone
  setTimezone(timezone);
}

void setTimezone(String timezone)
{
  Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1); //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void updateNTPTime(String timezone)
{
  const char *ntpServer = "pool.ntp.org";

  // const long gmtOffset_sec = 0;
  // const int daylightOffset_sec = 0;

  // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  initTime(timezone);
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return;
  }
  hours = timeinfo.tm_hour;
  minutes = timeinfo.tm_min;
  seconds = timeinfo.tm_sec;
  years = timeinfo.tm_year + 1900;
  months = timeinfo.tm_mon + 1;
  days = timeinfo.tm_mday;
}

void Wifireconnect()
{
  if (reconnecttimer - millis() > 5000 && reconnectionCounter < 5)
  {
    Serial.println("Attempting to reconnect");
    String strSSID = preferences.getString("SSID", "");
    String strPassword = preferences.getString("Password", "");

    char *txtSSID = const_cast<char *>(strSSID.c_str());
    char *txtPassword = const_cast<char *>(strPassword.c_str());
    WiFi.disconnect(true);
    WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE);  
    WiFi.mode(WIFI_STA);
    WiFi.setHostname(HOSTNAME);
    WiFi.begin(txtSSID, txtPassword);
    // print hostname
    Serial.println("Hostname: " + String(WiFi.getHostname()));
    int WiFiConnectTimeOut = 0;
    while ((WiFi.status() != WL_CONNECTED) && (WiFiConnectTimeOut < 5))
    {
      delay(1000);
      WiFiConnectTimeOut++;
    }
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Reconnection successful");
      Serial.println(WiFi.getHostname());
      wifiConnectionState = "Connected to: " + strSSID;
      wificonnected = true;
      reconnectionCounter = 0;
    }
    else
    {
      Serial.println("Reconnection failed, starting AP mode");
      WiFi.mode(WIFI_AP);
      WiFi.softAPsetHostname(HOSTNAME);
      WiFi.softAP("Wordclock Portal");
      WiFi.softAPConfig(apIP, apIP, gateway);
      dnsServer.start(DNS_PORT, "*", apIP);
      wificonnected = false;
      reconnectionCounter++;
      wifiConnectionState = "Disconnected";
    }
    reconnecttimer = millis();
    sendMessageToAllClients("connectionStatus", wifiConnectionState);
  }
}

void scanWifiNetworks()
{
  numberWlan = WiFi.scanNetworks();
  if (numberWlan == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(numberWlan);
    Serial.println(" networks found");
    for (int i = 0; i < numberWlan; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      Wlanssid[i] = WiFi.SSID(i);
      Wlanrssi[i] = WiFi.RSSI(i);
      Wlanauth[i] = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*";
      delay(10);
    }
  }
  Serial.println("");
}
void setup()
{
  pinMode(2, OUTPUT);
#ifdef VERBOSE
  Serial.begin(115200);
#endif

  esp_log_level_set("WiFi", ESP_LOG_NONE);
  // INITIALIZE LEDs
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // Initialize RTC
  if (!rtc.begin())
  {
    Serial.println("Couldn't find RTC");
  }
  else
  {
    rtc_found = true;
  }

  if (rtc.lostPower())
  {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Retrieve Configuration from FLASH
  initvalues();

  WiFi.disconnect();   // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_OFF); // added to start with the wifi off, avoid crashing
  scanWifiNetworks();

  // int connection = WiFi_RouterNetworkConnect(WIFISSID, WIFIPassword);
  Wifireconnect();

  //   hours = 12;
  //   minutes = 34;
  //   refreshInterface();
  //   if (connection == -1)
  //   {
  //     Serial.println("No connection to Wifi");
  //     Serial.println("Starting AP Mode");
  //     WiFi.mode(WIFI_AP);
  //     WiFi.softAP("Wordclock");
  //     WiFi.softAPConfig(apIP, apIP, gateway);
  //     dnsServer.start(DNS_PORT, "*", apIP);
  // #ifdef VERBOSE
  //     Serial.println("\nWiFi AP is now running\nIP address: ");
  //     Serial.println(WiFi.softAPIP());
  // #endif
  //   }

  if (!LittleFS.begin())
  {
#ifdef VERBOSE
    Serial.println("An Error has occurred while mounting LittleFS, formatting...");
#endif
    // LittleFS.format();     // Format LittleFS
    if (!LittleFS.begin()) // Try to mount again after formatting
    {
#ifdef VERBOSE
      Serial.println("Failed to initialize LittleFS after formatting!");
#endif

      return;
    }
  }
  // bind websocket to async web server
  websocket.onEvent(wsEventHandler);
  server.addHandler(&websocket);
  // setup statuc web server
  server.serveStatic("/", LittleFS, "/www/")
      .setDefaultFile("index.html");

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
                      request->redirect("/"); // Redirect all unhandled routes to index.html
                    });
  // server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->redirect("/"); });E
  server.begin();

#ifdef VERBOSE
  Serial.println("Server Started");
#endif

  initTime("CET-1CEST,M3.5.0,M10.5.0/3");
}

void checkWifiConnection()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    if (wificonnected == true)
    {
      wificonnected = false;
      Serial.println("Wifi connection lost");
    }
    Wifireconnect();
  }
  else
  {
    if (wificonnected == false)
    {
      wificonnected = true;
      reconnectionCounter = 0;
      Serial.println("Wifi connection reestablished");
    }
    updateNTPTime(testTimezone);
  }
}

void refreshInterface()
{

  setColor();
  setBrightness();

  // This sets the leds to display the time
  int language = Language.toInt();
  displayTime(minutes, hours, language);

  Serial.print(hours);
  Serial.print(':');
  Serial.print(minutes);
  Serial.print(':');
  Serial.print(seconds);
  Serial.println();

}

void loop()
{
  // serve DNS request for captive portal
  dnsServer.processNextRequest();
  vTaskDelay(1);


  if (rtc_found && millis() - maintimer > 1000)
  {
    // Updates time from RTC
    Serial.println("Getting time from RTC");
    refreshInterface();
    maintimer = millis();
  }

  else if (!rtc_found && millis() - maintimer > 5000)
  {
    Serial.println("Getting time from Wifi");
    // Checks to see if wifi is connected and if yes, updates the time
    // check if any client is currently connected to the AP
    int clientConnected = WiFi.softAPgetStationNum();
    if (clientConnected == 0)
    {
      checkWifiConnection();
    }
    refreshInterface();
    maintimer = millis();
  }

  if (rtc_found && millis() - RTCUpdateTimer > 600000)
  {
    // Updates RTC from Internet Time
    Serial.println("Updating RTC");
    RTCUpdateTimer = millis();
  }
}
