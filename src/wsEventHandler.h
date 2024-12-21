void wsEventHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
void sendMessageToAllClients(String id, String value);
void sendMessageToClient(String id, String value, AsyncWebSocketClient *client);
void toDataBuffer(String id, String value);
extern AsyncWebSocketClient *clients[16];