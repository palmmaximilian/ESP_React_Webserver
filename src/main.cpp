#include <libraries.h>
#include <global_variables.h>
#include <LittleFS.h>

// #define CAPTIVE_DOMAIN "http://domain-name-to-show" // This is the SSID that ESP32 will broadcast
// Uncomment the following line to enable password in the wifi acces point
// #define PASSWORD "12345678" // password should be atleast 8 characters to make it work
#define DNS_PORT 53
// Options to enable serial prin ting
#define VERBOSE

#define SSID "ESP32 SoftAP" // This is the SSID that ESP32 will broadcast
const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");

void refreshInterface();
void setColor();

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

String randomString(int length)
{
  String randString = "";
  for (int i = 0; i < length; i++)
  {
    byte randomValue = random(0, 36);
    char letter = randomValue + 'a';
    if (randomValue > 26)
      letter = (randomValue - 26) + '0';
    randString += letter;
  }
  return randString;
}

int WiFi_RouterNetworkConnect(String txtSSID, String txtPassword)
{
  char *SSID2 = const_cast<char *>(txtSSID.c_str());
  char *Password2 = const_cast<char *>(txtPassword.c_str());
  int success = 1;
  String message = "Trying to connect to " + txtSSID;

  WiFi.begin(SSID2, static_cast<const char *>(Password2));
  WiFi.setHostname(HOSTNAME);

  // we wait until connection is established
  // or 10 seconds are gone

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
  }

  Serial.println(WiFi.localIP());

  return success;
}

void redirectToIndex(AsyncWebServerRequest *request)
{
#ifdef CAPTIVE_DOMAIN
  request->redirect(CAPTIVE_DOMAIN);
#else
  request->redirect("http://" + apIP.toString());
#endif
}
void setColor()
{

  long number = (long)strtol(&ColorInHex[1], NULL, 16);
  redval = number >> 16;
  blueval = number >> 8 & 0xFF;
  greenval = number & 0xFF;
}

void setLedList(int LedList[], int length, int redval, int greenval, int blueval)
{
  for (int i = 0; i < length; i++)
  {
    leds[(LedList[i])] = CRGB(redval, greenval, blueval);
  }
}

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

void displayTime(int m, int h, int lang) // minutes setzen
{

  FastLED.clear();

  // ES IST
  setLedList(ItIs[lang], len_ItIs[lang], redval, blueval, greenval);

  int e = 0;
  e = m % 10;

  if ((m >= 0 && m < 5) || m == 60)
  {
    sethour(h, lang);

    setLedList(OClock[lang], len_OClock[lang], redval, blueval, greenval);
    if (h == 1 and time == 0)
    {
      leds[56] = CRGB::Black;
    }
  }

  if (m >= 5 && m < 10)
  {
    sethour(h, lang);
    setLedList(five[lang], len_five[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 10 && m < 15)
  {
    sethour(h, lang);
    setLedList(ten[lang], len_ten[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 15 && m < 20)
  {
    sethour(h, lang);
    setLedList(quarter[lang], len_quarter[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 20 && m < 25)
  {
    sethour(h, lang);
    setLedList(twenty[lang], len_twenty[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 25 && m < 30)
  {
    sethour(h, lang);
    setLedList(five[lang], len_five[lang], redval, blueval, greenval);
    setLedList(twenty[lang], len_twenty[lang], redval, blueval, greenval);
    setLedList(past[lang], len_past[lang], redval, blueval, greenval);
  }

  if (m >= 30 && m < 35)
  {

    sethour(h + 1, lang);

    // DE_half
    setLedList(half[lang], len_half[lang], redval, blueval, greenval);
  }

  if (m >= 35 && m < 40)
  {
    sethour(h + 1, lang);

    setLedList(five[lang], len_five[lang], redval, blueval, greenval);
    setLedList(twenty[lang], len_twenty[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 40 && m < 45)
  {
    sethour(h + 1, lang);
    setLedList(twenty[lang], len_twenty[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 45 && m < 50)
  {
    sethour(h + 1, lang);
    setLedList(quarter[lang], len_quarter[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 50 && m < 55)
  {
    sethour(h + 1, lang);
    setLedList(ten[lang], len_ten[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

  if (m >= 55 && m < 60)
  {
    sethour(h + 1, lang);
    setLedList(five[lang], len_five[lang], redval, blueval, greenval);
    setLedList(to[lang], len_to[lang], redval, blueval, greenval);
  }

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

void initvalues()
{
  preferences.begin("Wordclock", false);

  // takeout Strings out of the Non-volatile storage
  String strSSID = preferences.getString("SSID", "");
  String strPassword = preferences.getString("Password", "");
  String strTimezone = preferences.getString("Timezone", "");
  String strSummertimeActive = preferences.getString("DST", "");
  String strColorHex = preferences.getString("ColorHEX", "");
  String strNightmodeActive = preferences.getString("NightmodeActive", "");
  String strNightmodeFrom = preferences.getString("NightmodeFrom", "");
  String strNightmodeTo = preferences.getString("NightmodeTo", "");
  String strBrightnessPercent = preferences.getString("Perc", "");
  String strNightmodeBrightness = preferences.getString("NightPerc", "");
  String strLanguage = preferences.getString("Language", "");
  String strDeviceName = preferences.getString("DeviceName", "");

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

  if (strSummertimeActive != "")
  {
    SummertimeActive = strSummertimeActive;
  }
  else
  {
    preferences.putString("DST", SummertimeActive);
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

  if (strDeviceName != "")
  {
    DeviceName = strDeviceName;
  }
  else
  {
    DeviceName = "Wordclock_" + randomString(5);
    preferences.putString("DeviceName", DeviceName);
  }
}

void dealwithnighttime()
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

void correctTime()
{
  // if (SummertimeActive.toInt() == 1)
  // {
  //   clockGen();
  // }

  int extraminutes = ((TimezoneUMT.toFloat()) - TimezoneUMT.toInt()) * 100;
  int extrahours = TimezoneUMT.toInt();

  minutes += extraminutes;

  if (minutes > 60)
  {
    minutes -= 60;
    hours += 1;
  }

  hours = hours + extrahours;

  // pretty sure this does nothing
  if (hours < 12)
  {
    hours = hours + 24;
  }

  if (hours > 24)
  {
    hours = hours - 24;
  }

  dealwithnighttime();

  if (hours > 12)
  {
    hours = hours - 12;
  }
}

void updateNTPTime()
{
  const char *ntpServer = "pool.ntp.org";

  const long gmtOffset_sec = 0;
  const int daylightOffset_sec = 0;

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
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
  if (reconnecttimer - millis() > 5000)
  {
    //   Serial.println("Attempting to reconnect");
    String strSSID = preferences.getString("SSID", "");
    String strPassword = preferences.getString("Password", "");

    char *txtSSID = const_cast<char *>(strSSID.c_str());
    char *txtPassword = const_cast<char *>(strPassword.c_str());

    WiFi.begin(txtSSID, txtPassword);
    WiFi.setHostname(HOSTNAME);
    reconnecttimer = millis();
  }
}
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.path(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
void setup()
{
  pinMode(2, OUTPUT);
#ifdef VERBOSE
  Serial.begin(115200);
#endif

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
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // Retrieve Configuration from FLASH
  initvalues();
  Serial.println(DeviceName);

  WiFi.disconnect();   // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_OFF); // added to start with the wifi off, avoid crashing
  int connection = WiFi_RouterNetworkConnect(WIFISSID, WIFIPassword);
  hours = 12;
  minutes = 34;
  refreshInterface();
  if (connection == -1)
  {
    Serial.println("No connection to Wifi");
    Serial.println("Starting AP Mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(SSID);
    WiFi.softAPConfig(apIP, apIP, gateway);
    dnsServer.start(DNS_PORT, "*", apIP);
#ifdef VERBOSE
    Serial.println("\nWiFi AP is now running\nIP address: ");
    Serial.println(WiFi.softAPIP());
#endif
  }
  // if (!MDNS.begin("esp32"))
  // { // Set hostname to "esp32.local"
  //   Serial.println("Error starting mDNS");
  //   return;
  // }
  // Serial.println("mDNS responder started");

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
  listDir(LittleFS, "/", 1);
  // bind websocket to async web server
  websocket.onEvent(wsEventHandler);
  server.addHandler(&websocket);
  // setup statuc web server
  server.serveStatic("/", LittleFS, "/www/")
      .setDefaultFile("index.html");
  // Captive portal to keep the client
  // server.onNotFound(redirectToIndex);

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
                      request->redirect("/index.html"); // Redirect all unhandled routes to index.html
                    });
  server.on("/hotspot-detect.html", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->redirect("index.html.gz"); });
  server.begin();

#ifdef VERBOSE
  Serial.println("Server Started");
#endif
}

void checkWifiConnection()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    if (wificonnected == true)
    {
    }
    wificonnected = false;
    Wifireconnect();
  }
  else
  {
    if (wificonnected == false)
    {
      wificonnected = true;
    }
    updateNTPTime();
    correctTime();
  }
}

void refreshInterface()
{

  setColor();

  // This sets the leds to display the time
  int language = Language.toInt();
  displayTime(minutes, hours, language);

  Serial.print(hours);
  Serial.print(':');
  Serial.print(minutes);
  Serial.println();
}

void getRTCTime()
{
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
    getRTCTime();
    correctTime();
    refreshInterface();
    maintimer = millis();
  }

  else if (!rtc_found && millis() - maintimer > 5000)
  {
    Serial.println("Getting time from Wifi");
    // Checks to see if wifi is connected and if yes, updates the time
    // checkWifiConnection();
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
