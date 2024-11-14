// AppContext.tsx
import { createContext, useState, ReactNode, useEffect } from "react";

interface WordclockDataContextType {
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
}

export const WordclockDataContext = createContext<
  WordclockDataContextType | undefined
>(undefined);

export const WordclockDataProvider = ({
  children,
}: {
  children: ReactNode;
}) => {
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

  // Effect to run whenever any of the values change
  useEffect(() => {
    console.log("WordclockDataContext values changed");
    console.log(JSON.stringify({
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
    }));
    // Place any other logic here, such as updating a database, logging, etc.
  }, [
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
  ]);

  return (
    <WordclockDataContext.Provider
      value={{
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
      }}
    >
      {children}
    </WordclockDataContext.Provider>
  );
};
