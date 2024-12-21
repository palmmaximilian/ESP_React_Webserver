import { useContext } from "react";
import { WordclockDataContext } from "./AppContext/AppContext.tsx"; // Import the WordclockDataContext

import "./App.css"; // Import CSS styles for styling the components

export function Settings() {
  const WordclockData = useContext(WordclockDataContext);
  if (!WordclockData) {
    throw new Error("WordclockDataContext is not available");
  }

  const { timezone, setTimezone } = WordclockData;
  const { nightmode, setNightmode } = WordclockData;
  const { nightmodebrightness, setNightmodebright } = WordclockData;
  const { nightmodeFrom, setNightmodeFrom } = WordclockData;
  const { nightmodeTo, setNightmodeTo } = WordclockData;
  const { language, setLanguage } = WordclockData;

  const UTCList = [
    { label: "UTC-12", value: "-12" },
    { label: "UTC-11", value: "-11" },
    { label: "UTC-10", value: "-10" },
    // Add the rest of the timezones here...
  ];

  const LanguageList = [
    { label: "German", value: "0" },
    { label: "English", value: "1" },
  ];

  return (
    <div>
      <div className="row-view">
        <h2>Time</h2>
      </div>

      <div className="row-view">
        <label>Timezone</label>
        <select
          value={timezone}
          onChange={(e) => {
            setTimezone(e.target.value);
            WordclockData.updateTimezone(e.target.value);
          }}
        >
          {UTCList.map((timezone) => (
            <option key={timezone.value} value={timezone.value}>
              {timezone.label}
            </option>
          ))}
        </select>
      </div>


      <hr className="divider" />

      <div className="row-view">
        <h2>Nightmode</h2>
      </div>

      <div className="row-view">
        <label>From</label>
        <input
          type="time"
          value={nightmodeFrom}
          onChange={(e) => {
            setNightmodeFrom(e.target.value);
            WordclockData.updateNightmodeFrom(e.target.value);
            console.log("Setting nightmode from: " + e.target.value);
          }}
        />
      </div>

      <div className="row-view">
        <label>To</label>
        <input
          type="time"
          value={nightmodeTo}
          onChange={(e) => {
            setNightmodeTo(e.target.value);
            WordclockData.updateNightmodeTo(e.target.value);
            console.log("Setting nightmode from: " + e.target.value);
          }}
        />
      </div>

      <div className="row-view">
        <label>Activate?</label>
        <input
          type="checkbox"
          checked={nightmode}
          onChange={(e) => {
            setNightmode(e.target.checked);
            WordclockData.updateNightmode(e.target.checked);
          }}
        />
      </div>

      <div className="row-view">
        <label>Brightness</label>
        <p>Value: {nightmodebrightness}%</p>
        <input
          type="range"
          min="0"
          max="100"
          step="5"
          value={nightmodebrightness}
          onChange={(e) => {
            setNightmodebright(e.target.value);
            WordclockData.updateNightmodebrightness(e.target.value);
          }}
        />
      </div>

      <hr className="divider" />

      <div className="row-view">
        <h2>Language</h2>
        <select
          value={language}
          onChange={(e) => {
            setLanguage(e.target.value);
            WordclockData.updateLanguage(e.target.value);
          }}
        >
          {LanguageList.map((lang) => (
            <option key={lang.value} value={lang.value}>
              {lang.label}
            </option>
          ))}
        </select>
      </div>
    </div>
  );
}

export default Settings;
