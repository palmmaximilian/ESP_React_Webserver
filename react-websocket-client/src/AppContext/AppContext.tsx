// AppContext.tsx
import { createContext, useState, ReactNode, useEffect, useRef, useContext } from "react";
import { w3cwebsocket, IMessageEvent } from "websocket";

interface WordclockDataContextType {
  connectionStatus: string;
  isConnected: boolean;
  ssid: string;
  password: string;
  timezone: string;
  language: string;
  nightmode: boolean;
  nightmodebrightness: number;
  nightmodeFrom: string;
  nightmodeTo: string;
  summertime: boolean;
  setConnectionStatus: (connectionStatus: string) => void;
  setIsConnected: (isConnected: boolean) => void;
  setSsid: (ssid: string) => void;
  setPassword: (password: string) => void;
  setTimezone: (timezone: string) => void;
  setLanguage: (language: string) => void;
  setNightmode: (nightmode: boolean) => void;
  setNightmodebright: (brightness: number) => void;
  setNightmodeFrom: (from: string) => void;
  setNightmodeTo: (to: string) => void;
  setSummertime: (summertime: boolean) => void;
  updateWifiCredentials: () => void;
}

export const WordclockDataContext = createContext<
  WordclockDataContextType | undefined
>(undefined);

export const WordclockDataProvider = ({
  children,
}: {
  children: ReactNode;
}) => {

  const websocket = useRef<w3cwebsocket | null>(null);
  const [connectionStatus, setConnectionStatus] = useState("%");
  const [isConnected, setIsConnected] = useState(false);
  const [ssid, setSsid] = useState("");
  const [password, setPassword] = useState("");
  const [timezone, setTimezone] = useState("0");
  const [language, setLanguage] = useState("1");
  const [nightmode, setNightmode] = useState(false);
  const [nightmodebrightness, setNightmodebright] = useState(50);
  const [nightmodeFrom, setNightmodeFrom] = useState("22:00");
  const [nightmodeTo, setNightmodeTo] = useState("06:00");
  const [summertime, setSummertime] = useState(false);
  

  // Effect to run whenever any of the values changes to send a json file with type=message and the value that changed
  // useEffect(() => {
  //   console.log("Sending SSID: "+ssid);
  //   websocket.current?.send(
  //     JSON.stringify({
  //       type: "message",
  //       ssid: ssid
  //     })
  //   );
  // }
  // , [ssid]);

  // useEffect(() => {
    
  //   console.log("Sending Password: "+password);
  //   websocket.current?.send(
  //     JSON.stringify({
  //       type: "message",
  //       password: password
  //     })
  //   );
  // }
  // , [password]);

  useEffect(() => {
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        timezone: timezone
      })
    );
  }
  , [timezone]);



  

  useEffect(() => {
    websocket.current = new w3cwebsocket("ws://192.168.2.1/ws");
    websocket.current.onmessage = (message: IMessageEvent) => {
      const dataFromServer = JSON.parse(message.data.toString());
      if (dataFromServer.type === "message") {
        console.log("data received:"+dataFromServer);

        if (dataFromServer.ssid) {
          setSsid(dataFromServer.ssid);
        }
        if (dataFromServer.password) {
          setPassword(dataFromServer.password);
        }
        if (dataFromServer.connectionStatus) {
          setConnectionStatus(dataFromServer.connectionStatus);
        }
      }
    };
    return () => websocket.current?.close();
  }, []);
  

  const updateWifiCredentials=()=>
    {
      console.log("Sending SSID and password : "+ssid+" "+password);
      websocket.current?.send(
        JSON.stringify({
          type: "message",
          ssid: ssid,
          password: password
        })
      );
    }



  return (
    <WordclockDataContext.Provider
      value={{
        connectionStatus,
        isConnected,
        ssid,
        password,
        timezone,
        language,
        nightmode,
        nightmodebrightness,
        nightmodeFrom,
        nightmodeTo,
        summertime,
        setConnectionStatus,
        setIsConnected,
        setSsid,
        setPassword,
        setTimezone,
        setLanguage,
        setNightmode,
        setNightmodebright,
        setNightmodeFrom,
        setNightmodeTo,
        setSummertime,
        updateWifiCredentials
      }}
    >
      {children}
    </WordclockDataContext.Provider>
  );
};

export const useWordclockData = () => {
  const context = useContext(WordclockDataContext);
  if (!context) {
    throw new Error("useWordclockData must be used within a WordclockDataProvider");
  }
  return context;
};

