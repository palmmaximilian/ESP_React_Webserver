// useWebSocket.ts
import { useRef, useEffect, useCallback, useContext } from "react";
import { w3cwebsocket, IMessageEvent } from "websocket";
import { useWordclockData, WordclockDataContext } from '../AppContext/AppContext.tsx'; // Import the WordclockDataContext


const useWebSocket = (url: string, onMessage: (data: any) => void) => {
  const websocket = useRef<w3cwebsocket | null>(null);
  const WordclockData = useContext(WordclockDataContext);

  useEffect(() => {
    websocket.current = new w3cwebsocket(url);
    websocket.current.onmessage = (message: IMessageEvent) => {
        const dataFromServer = JSON.parse(message.data.toString());
        if (dataFromServer.type === "message") {
          console.log("data received:"+dataFromServer);
        }
      };
    websocket.current.onclose = () => {
      console.log("WebSocket closed, attempting to reconnect...");
    //   setTimeout(() => connectWebSocket(url, onMessage), 1000); // Attempt to reconnect after 1 second
    };
    return () => websocket.current?.close();
  }, [url, onMessage]);

  const sendMessage = useCallback((message: any) => {
    if (websocket.current && websocket.current.readyState === websocket.current.OPEN) {
      websocket.current.send(JSON.stringify(message));
    } else {
      console.error("WebSocket is not open. Unable to send message.");
    }
  }, []);
  

  const updateWifiCredentials=()=>
  {
    console.log("Sending SSID and password : "+WordclockData?.ssid+" "+WordclockData?.password);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        ssid: WordclockData?.ssid,
        password: WordclockData?.password
      })
    );
  }

};

export default useWebSocket;


