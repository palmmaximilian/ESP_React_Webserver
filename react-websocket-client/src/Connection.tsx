
import { useContext } from "react";
import { useWordclockData, WordclockDataContext } from './AppContext/AppContext.tsx'; // Import the WordclockDataContext

export function Connection() {
  const WordclockData = useContext(WordclockDataContext);
  if (!WordclockData) {
    throw new Error("WordclockDataContext is not available");
  }
  const {ssid, setSsid} = useWordclockData();
  const {password, setPassword} = useWordclockData();
  const {connectionStatus} = useWordclockData();

  

  return (
    <div>
      <div className="row-view">
        <p className="title-text">Wifi</p>
      </div>

      <div className="row-view">
        <p>Is Wifi connected?:</p>
        <p>{connectionStatus}</p>
      </div>

      <div style={{ paddingBottom: "20px" }}></div>

      <div className="row-view">
        <label>SSID:</label>
        <input
          type="text"
          value={ssid}
          onChange={(e) => setSsid(e.target.value)}
          placeholder={ssid}
        />
      </div>

      <div className="row-view">
        <label>Password:</label>
        <input
          type="password"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          placeholder="Password"
        />
      </div>

      <div className="row-view">
        <button
          onClick={() => {
            if (ssid !== "") {
              setSsid(ssid);
            }
            if (password !== "") {
              setPassword(password);
            }
          WordclockData.updateWifiCredentials();
          }}
        >
          Set Wifi Connection
        </button>
      </div>
    </div>
  );
}
