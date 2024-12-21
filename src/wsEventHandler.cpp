#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <ESPAsyncWebServer.h>
#include "wsEventHandler.h"
#include <global_variables.h>
#include "main.h"

// allocate memory for recieved json data
#define BUFFER_SIZE 512
StaticJsonDocument<BUFFER_SIZE> receivedJson;
// initial device state

char dataBuffer[BUFFER_SIZE] = "{\"type\":\"message\",\"LED\":false}";
AsyncWebSocketClient *clients[16];

void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_DATA)
  {
    // save the response as newest device state
    for (int i = 0; i < len; ++i)
      dataBuffer[i] = data[i];
    dataBuffer[len] = '\0';
#ifdef VERBOSE
    Serial.println(dataBuffer);
#endif
    // parse the recieved json data
    Serial.println("Parsing JSON");
    DeserializationError error = deserializeJson(receivedJson, (char *)data, len);
    Serial.println("Parsed JSON");
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    if (strcmp(receivedJson["type"], "message") != 0)
    {
      Serial.println("Not a message");
      return;
    }

    Serial.println("Received message");
    String jsonString;
    serializeJson(receivedJson, jsonString);
    Serial.println(jsonString);
// {"type":"message","datetime":"2024-11-25T21:22:04.741Z"}
    if(receivedJson["datetime"].is<const char *>())
    {
      String datetime = receivedJson["datetime"];
      Serial.println("Received new datetime: " + datetime);
      hours = datetime.substring(11, 13).toInt();
      minutes = datetime.substring(14, 16).toInt();
      seconds = datetime.substring(17, 19).toInt();
      years = datetime.substring(0, 4).toInt();
      months = datetime.substring(5, 7).toInt();
      days = datetime.substring(8, 10).toInt();
      
    }

    if (receivedJson["ssid"].is<const char *>())
    {
      String ssid = receivedJson["ssid"];
      Serial.println("Received new SSID: " + ssid);
      preferences.begin("Wordclock", false);
      preferences.putString("SSID", ssid);
      WIFISSID = ssid;

      String password = receivedJson["password"];
      Serial.println("Received new password: " + password);
      preferences.putString("Password", password);
      preferences.end();
      WIFIPassword = password;
      reconnectionCounter = 0;
      reconnecttimer = millis();
      Wifireconnect();
    }
    if (receivedJson["timezone"].is<const char *>())
    {
      String temptimezone = receivedJson["timezone"];
      TimezoneUMT = temptimezone;
      Serial.println("Received new timezone: " + TimezoneUMT);
      preferences.begin("Wordclock", false);
      preferences.putString("Timezone", TimezoneUMT);
      preferences.end();
    }
    if (receivedJson["nightmode"].is<const char *>())
    {
      String tempNightmode = receivedJson["nightmode"];
      NightmodeActive = tempNightmode;
      Serial.println("Received new nightmode: " + NightmodeActive);
      preferences.begin("Wordclock", false);
      preferences.putString("Nightmode", NightmodeActive);
      preferences.end();
    }
    if (receivedJson["nightmodeFrom"].is<const char *>())
    {
      String tempNightmodeFrom = receivedJson["nightmodeFrom"];
      NightmodeFrom = tempNightmodeFrom;
      Serial.println("Received new nightmodeFrom: " + NightmodeFrom);
      preferences.begin("Wordclock", false);
      preferences.putString("NightmodeFrom", NightmodeFrom);
      preferences.end();
    }
    if (receivedJson["nightmodeTo"].is<const char *>())
    {
      String tempNightmodeTo = receivedJson["nightmodeTo"];
      NightmodeTo = tempNightmodeTo;
      Serial.println("Received new nightmodeTo: " + NightmodeTo);
      preferences.begin("Wordclock", false);
      preferences.putString("NightmodeTo", NightmodeTo);
      preferences.end();
    }
    if (receivedJson["nightmodebrightness"].is<const char *>())
    {
      String tempNightmodeBrightness = receivedJson["nightmodebrightness"];
      NightmodeBrightnessPercent = tempNightmodeBrightness;
      Serial.println("Received new nightmodeBrightness: " + NightmodeBrightnessPercent);
      preferences.begin("Wordclock", false);
      preferences.putString("NightPerc", NightmodeBrightnessPercent);
      preferences.end();
    }
    if (receivedJson["brightness"].is<const char *>())
    {
      String tempBrightness = receivedJson["brightness"];
      BrightnessPercent = tempBrightness;
      Serial.println("Received new brightness: " + BrightnessPercent);
      preferences.begin("Wordclock", false);
      preferences.putString("Perc", BrightnessPercent);
      preferences.end();
    }
    if (receivedJson["color"].is<const char *>())
    {
      String tempColor = receivedJson["color"];
      ColorInHex = tempColor;
      Serial.println("Received new color: " + ColorInHex);
      preferences.begin("Wordclock", false);
      preferences.putString("ColorHEX", ColorInHex);
      preferences.end();
    }
    if (receivedJson["language"].is<const char *>())
    {
      String tempLanguage = receivedJson["language"];
      Language = tempLanguage;
      Serial.println("Received new language: " + Language);
      preferences.begin("Wordclock", false);
      preferences.putString("Language", Language);
      preferences.end();
    }
    refreshInterface();
  }
  else if (type == WS_EVT_CONNECT)
  {
    Serial.println("Websocket client connection received");
    sendMessageToClient("connectionStatus", wifiConnectionState, client);
    String ssidList = "";
    for (int i = 0; i < numberWlan; i++)
    {
      ssidList += Wlanssid[i];
      if (i < numberWlan - 1)
        ssidList += ",";
    }
    sendMessageToClient("ssidList", ssidList, client);
    sendMessageToClient("timezone", TimezoneUMT, client);
    sendMessageToClient("nightmode", NightmodeActive, client);
    sendMessageToClient("nightmodeFrom", NightmodeFrom, client);
    sendMessageToClient("nightmodeTo", NightmodeTo, client);
    sendMessageToClient("nightmodeBrightness", NightmodeBrightnessPercent, client);
    sendMessageToClient("brightness", BrightnessPercent, client);
    sendMessageToClient("color", ColorInHex, client);
    sendMessageToClient("language", Language, client);
    sendMessageToClient("connectionStatus", wifiConnectionState, client);

    for (int i = 0; i < 16; ++i)
      if (clients[i] == NULL)
      {
        clients[i] = client;
        break;
      }
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.println("Client disconnected");
    // remove client from storage
    for (int i = 0; i < 16; ++i)
      if (clients[i] == client)
        clients[i] = NULL;
  }
}

void sendMessageToAllClients(String id, String value)
{
  String message = "{\"type\":\"message\",\"" + id + "\":\"" + value + "\"}";
  int tempLen = message.length();
  for (int i = 0; i < tempLen; ++i)
    dataBuffer[i] = message[i];
  dataBuffer[tempLen] = '\0';

  for (int i = 0; i < 16; ++i)
    if (clients[i] != NULL)
      clients[i]->text(dataBuffer);
  return;
}

void sendMessageToClient(String id, String value, AsyncWebSocketClient *client)
{
  String message = "{\"type\":\"message\",\"" + id + "\":\"" + value + "\"}";
  Serial.println("Sending message to clients: " + message);
  int tempLen = message.length();
  for (int i = 0; i < tempLen; ++i)
    dataBuffer[i] = message[i];
  dataBuffer[tempLen] = '\0';

  client->text(dataBuffer);
  return;
}

void toDataBuffer(String id, String value)
{
  String message = "{\"type\":\"message\",\"" + id + "\":" + value + "}";
  Serial.println("Sending message to clients: " + message);
  int tempLen = message.length();
  for (int i = 0; i < tempLen; ++i)
    dataBuffer[i] = message[i];
  dataBuffer[tempLen] = '\0';
}