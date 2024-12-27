// AppContext.tsx
import {
  createContext,
  useState,
  ReactNode,
  useEffect,
  useRef,
  useContext,
} from "react";
import { Color } from "react-color";
import { w3cwebsocket, IMessageEvent } from "websocket";

interface WordclockDataContextType {
  connectionStatus: string;
  ssidList: string[];
  ssid: string;
  password: string;
  timezone: string;
  timezoneCode: string;
  language: string;
  nightmode: boolean;
  nightmodebrightness: string;
  nightmodeFrom: string;
  nightmodeTo: string;
  color: Color;
  brightness: string;
  setConnectionStatus: (connectionStatus: string) => void;
  setSsidList: (ssidList: string[]) => void;
  setSsid: (ssid: string) => void;
  setPassword: (password: string) => void;
  setTimezone: (timezone: string) => void;
  setTimezoneCode: (timezoneCode: string) => void;
  setLanguage: (language: string) => void;
  setNightmode: (nightmode: boolean) => void;
  setNightmodebright: (brightness: string) => void;
  setNightmodeFrom: (from: string) => void;
  setNightmodeTo: (to: string) => void;
  setColor: (color: string) => void;
  setBrightness: (brightness: string) => void;
  updateWifiCredentials: () => void;
  updateTimezone: (tempTimezone: string) => void;
  updateTimezoneCode: (tempTimezoneCode: string) => void
  updateLanguage: (tempLang: string) => void;
  updateNightmode: (tempNightmode: boolean) => void;
  updateNightmodebrightness: (tempNightmodebrightness: string) => void;
  updateNightmodeFrom: (tempNightmodeFrom: string) => void;
  updateNightmodeTo: (tempNightmodeTo: string) => void;
  updateColor: (tempColor: string) => void;
  updateBrightness: (tempBrightness: string) => void;
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
  const [connectionStatus, setConnectionStatus] = useState("No connection to Wordclock");
  const [ssid, setSsid] = useState("");
  const [ssidList, setSsidList] = useState<string[]>([
    "ssid1",
    "ssid2",
    "ssid3",
  ]);
  const [password, setPassword] = useState("");
  const [timezone, setTimezone] = useState("Select Timezone");
  const [timezoneCode, setTimezoneCode] = useState("");
  const [language, setLanguage] = useState("1");
  const [nightmode, setNightmode] = useState(false);
  const [nightmodebrightness, setNightmodebright] = useState("50");
  const [nightmodeFrom, setNightmodeFrom] = useState("22:00");
  const [nightmodeTo, setNightmodeTo] = useState("06:00");
  const [color, setColor] = useState("#ffffff");
  const [brightness, setBrightness] = useState("50");

  // const Serveradress = "ws://192.168.2.1/ws";
  // const Serveradress = "ws://esp32-31E544/ws";
  
  const Serveradress = "ws://wordclock/ws";
  const updateTimezone = (tempTimezone: string) => {
    console.log("Sending timezone: " + tempTimezone);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        timezone: tempTimezone,
      })
    );
  };

  const updateTimezoneCode = (tempTimezoneCode: string) => {
    console.log("Sending timezoneCode: " + tempTimezoneCode);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        timezoneCode: tempTimezoneCode,
      })
    );
  }

  const updateLanguage = (tempLang: string) => {
    console.log("Sending language: " + tempLang);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        language: tempLang,
      })
    );
  };

  const updateNightmode = (tempNightmode: boolean) => {
    console.log("Sending nightmode: " + tempNightmode);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        nightmode: tempNightmode ? "1" : "0",
      })
    );
  };

  const updateNightmodebrightness = (tempNightmodebrightness: string) => {
    console.log("Sending nightmodebrightness: " + tempNightmodebrightness);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        nightmodebrightness: tempNightmodebrightness,
      })
    );
  };

  const updateNightmodeFrom = (tempNightmodeFrom: string) => {
    console.log("Sending nightmodeFrom: " + tempNightmodeFrom);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        nightmodeFrom: tempNightmodeFrom,
      })
    );
  };

  const updateNightmodeTo = (tempNightmodeTo: string) => {
    console.log("Sending nightmodeTo: " + tempNightmodeTo);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        nightmodeTo: tempNightmodeTo,
      })
    );
  };


  const updateColor = (tempColor: string) => {
    console.log("Sending color: " + tempColor);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        color: tempColor,
      })
    );
  };

  const updateBrightness = (tempBrightness: string) => {
    console.log("Sending brightness: " + tempBrightness);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        brightness: tempBrightness,
      })
    );
  };

  useEffect(() => {
    websocket.current = new w3cwebsocket(Serveradress);
    websocket.current.onopen = () => {
      console.log("Connected to server");
      // send over system time and date to the server
      websocket.current?.send(
        JSON.stringify({
          type: "message",
          datetime: new Date().toISOString(),
        })
      );
    };
    websocket.current.onmessage = (message: IMessageEvent) => {
      const dataFromServer = JSON.parse(message.data.toString());
      if (dataFromServer.type === "message") {
        console.log("data received:" + JSON.stringify(dataFromServer));

        if (dataFromServer.ssid) {
          setSsid(dataFromServer.ssid);
        }
        if (dataFromServer.ssidList) {
          // list arrives like this "ssid1,ssid2,ssid3"
          setSsidList(dataFromServer.ssidList.split(","));
        }
        if (dataFromServer.password) {
          setPassword(dataFromServer.password);
        }
        if (dataFromServer.connectionStatus) {
          setConnectionStatus(dataFromServer.connectionStatus);
        }
        if (dataFromServer.timezone) {
          setTimezone(dataFromServer.timezone);
        }
        if (dataFromServer.language) {
          setLanguage(dataFromServer.language);
        }
        if (dataFromServer.nightmode) {
          setNightmode(dataFromServer.nightmode);
        }
        if (dataFromServer.nightmodebrightness) {
          setNightmodebright(dataFromServer.nightmodebrightness);
        }
        if (dataFromServer.nightmodeFrom) {
          // might arrive like hh:mm or hh:m or h:mm or h:m but should always be hh:mm
          const from = dataFromServer.nightmodeFrom.split(":");
          if (from.length === 2) {
            setNightmodeFrom(
              from[0].padStart(2, "0") + ":" + from[1].padStart(2, "0")
            );
          }
          // setNightmodeFrom(dataFromServer.nightmodeFrom);
        }
        if (dataFromServer.nightmodeTo) {
          // might arrive like hh:mm or hh:m or h:mm or h:m but should always be hh:mm
          const to = dataFromServer.nightmodeTo.split(":");
          if (to.length === 2) {
            setNightmodeTo(
              to[0].padStart(2, "0") + ":" + to[1].padStart(2, "0")
            );
          }
          // setNightmodeTo(dataFromServer.nightmodeTo);
        }
        if (dataFromServer.color) {
          // might arrive like #ffffff so it needs to be converted to a color object with the hex value
          setColor(dataFromServer.color);
        }
        if (dataFromServer.brightness) {
          setBrightness(dataFromServer.brightness);
        }
      }
    };
    return () => websocket.current?.close();
  }, []);

  const updateWifiCredentials = () => {
    console.log("Sending SSID and password : " + ssid + " " + password);
    websocket.current?.send(
      JSON.stringify({
        type: "message",
        ssid: ssid,
        password: password,
      })
    );
  };

  return (
    <WordclockDataContext.Provider
      value={{
        connectionStatus,
        ssidList,
        ssid,
        password,
        timezone,
        timezoneCode,
        language,
        nightmode,
        nightmodebrightness,
        nightmodeFrom,
        nightmodeTo,
        color,
        brightness,
        setConnectionStatus,
        setSsidList,
        setSsid,
        setPassword,
        setTimezone,
        setTimezoneCode,
        setLanguage,
        setNightmode,
        setNightmodebright,
        setNightmodeFrom,
        setNightmodeTo,
        setColor,
        setBrightness,
        updateWifiCredentials,
        updateTimezone,
        updateTimezoneCode,
        updateLanguage,
        updateNightmode,
        updateNightmodebrightness,
        updateNightmodeFrom,
        updateNightmodeTo,
        updateColor,
        updateBrightness,
      }}
    >
      {children}
    </WordclockDataContext.Provider>
  );
};

export const useWordclockData = () => {
  const context = useContext(WordclockDataContext);
  if (!context) {
    throw new Error(
      "useWordclockData must be used within a WordclockDataProvider"
    );
  }
  return context;
};
