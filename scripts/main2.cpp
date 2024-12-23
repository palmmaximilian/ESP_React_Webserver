#include <Arduino.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include "wsEventHandler.h"

#define SSID "ESP32 SoftAP" // This is the SSID that ESP32 will broadcast
// #define CAPTIVE_DOMAIN "http://domain-name-to-show" // This is the SSID that ESP32 will broadcast
// Uncomment the following line to enable password in the wifi acces point
// #define PASSWORD "12345678" // password should be atleast 8 characters to make it work
#define DNS_PORT 53
// Options to enable serial printing
#define VERBOSE
const IPAddress apIP(192, 168, 2, 1);
const IPAddress gateway(255, 255, 255, 0);

DNSServer dnsServer;
AsyncWebServer server(80);
AsyncWebSocket websocket("/ws");

void redirectToIndex(AsyncWebServerRequest *request)
{
#ifdef CAPTIVE_DOMAIN
  request->redirect(CAPTIVE_DOMAIN);
#else
  request->redirect("http://" + apIP.toString());
#endif
}
void listDir(const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = LittleFS.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }

  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
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
        listDir(file.name(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
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

  WiFi.disconnect();   // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_OFF); // added to start with the wifi off, avoid crashing
  WiFi.mode(WIFI_AP);
#ifndef PASSWORD
  WiFi.softAP(SSID);
#else
  WiFi.softAP(SSID, PASSWORD);
#endif
  WiFi.softAPConfig(apIP, apIP, gateway);
  dnsServer.start(DNS_PORT, "*", apIP);

#ifdef VERBOSE
  Serial.println("\nWiFi AP is now running\nIP address: ");
  Serial.println(WiFi.softAPIP());
#endif

  if (!LittleFS.begin())
  {
#ifdef VERBOSE
    Serial.println("An Error has occurred while mounting LittleFS, formatting...");
#endif
    LittleFS.format();     // Format LittleFS
    if (!LittleFS.begin()) // Try to mount again after formatting
    {
#ifdef VERBOSE
      Serial.println("Failed to initialize LittleFS after formatting!");
#endif
      return;
    }
  }
  listDir("/", 0); // List all files and directories from the root level

  // bind websocket to async web server
  websocket.onEvent(wsEventHandler);
  server.addHandler(&websocket);
  // setup statuc web server
  server.serveStatic("/", LittleFS, "/www/")
      .setDefaultFile("index.html");
  // Captive portal to keep the client
  server.onNotFound(redirectToIndex);
  server.begin();

#ifdef VERBOSE
  Serial.println("Server Started");
#endif
}

void loop()
{
  // serve DNS request for captive portal
  dnsServer.processNextRequest();
  vTaskDelay(1);
}
