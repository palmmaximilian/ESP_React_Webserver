#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include "wsEventHandler.h"
#include <global_variables.h>

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
    if (receivedJson.containsKey("ssid"))
    {
      String ssid = receivedJson["ssid"];
      Serial.println("Received new SSID: " + ssid);
      preferences.begin("Wordclock", false);
      preferences.putString("SSID", ssid);
      preferences.end();
    }
    if (receivedJson.containsKey("password"))
    {
      String password = receivedJson["password"];
      Serial.println("Received new password: " + password);
      preferences.begin("Wordclock", false);
      preferences.putString("Password", password);
      preferences.end();
    }

    Serial.println("Received message");
    // get the target LED state

    // print out json data
    // Create a string to hold the serialized JSON
    String jsonString;

    // Serialize the JSON document to the string
    serializeJson(receivedJson, jsonString);

    // Print the serialized JSON string
    Serial.println(jsonString);

    bool led = receivedJson["LED"];

    digitalWrite(2, led);
    // send ACK
    client->text(dataBuffer, len);
    // alert all other clients
    for (int i = 0; i < 16; ++i)
      if (clients[i] != NULL && clients[i] != client)
        clients[i]->text(dataBuffer, len);
  }
  else if (type == WS_EVT_CONNECT)
  {
    Serial.println("Websocket client connection received");
    // ACK with current state
    client->text(dataBuffer);
    // store connected client
    for (int i = 0; i < 16; ++i)
      if (clients[i] == NULL)
      {
        clients[i] = client;
        break;
      }

    sendMessageToClients("connectionStatus", wifiConnectionState);
    sendMessageToClients("ssid", WIFISSID);
    sendMessageToClients("password", WIFIPassword);

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

void sendMessageToClients(String id, String value)
{
  String message = "{\"type\":\"message\",\"" + id + "\":" + value + "}";
  int tempLen = message.length();
  for (int i = 0; i < tempLen; ++i)
    dataBuffer[i] = message[i];
  dataBuffer[tempLen] = '\0';
  for (int i = 0; i < 16; ++i)
    if (clients[i] != NULL)
      clients[i]->text(dataBuffer, tempLen);
}