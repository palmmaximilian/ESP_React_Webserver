import { useState, useContext, useEffect } from "react";
import { WordclockDataContext } from "./AppContext/AppContext.tsx"; // Import the WordclockDataContext

import "./App.css"; // Import CSS styles for styling the components

export function Settings() {
  const WordclockData = useContext(WordclockDataContext);
  if (!WordclockData) {
    throw new Error("WordclockDataContext is not available");
  }

  const [sliderState, setSliderState] = useState(0);
  const [dateFrom, setDateFrom] = useState(new Date());
  const [dateTo, setDateTo] = useState(new Date());
  const [newTimezone, setNewTimezone] = useState(WordclockData.timezone);
  const [newLanguage, setNewLanguage] = useState(WordclockData.language);

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

  useEffect(() => {
    setSliderState(WordclockData.nightmodebrightness);
    const tempDateFrom = new Date();
    const valuesFrom = WordclockData.nightmodeFrom.split(":");
    tempDateFrom.setHours(Number(valuesFrom[0]));
    tempDateFrom.setMinutes(Number(valuesFrom[1]));
    setDateFrom(tempDateFrom);

    const tempDateTo = new Date();
    const valuesTo = WordclockData.nightmodeTo.split(":");
    tempDateTo.setHours(Number(valuesTo[0]));
    tempDateTo.setMinutes(Number(valuesTo[1]));
    setDateTo(tempDateTo);
  }, [WordclockData]);

  return (
    <div>
      <div className="row-view">
        <h2>Time</h2>
      </div>

      <div className="row-view">
        <label>Timezone</label>
        <select
          value={newTimezone}
          onChange={(e) => {
            setNewTimezone(e.target.value);
          }}
        >
          {UTCList.map((timezone) => (
            <option key={timezone.value} value={timezone.value}>
              {timezone.label}
            </option>
          ))}
        </select>
      </div>

      <div className="row-view">
        <label>Automatic Summertime (Europe)?</label>
        <input
          type="checkbox"
          checked={WordclockData.summertime}
          onChange={(e) => WordclockData.setSummertime(e.target.checked)}
        />
      </div>

      <hr className="divider" />

      <div className="row-view">
        <h2>Nightmode</h2>
      </div>

      <div className="row-view">
        <label>From</label>
        <input
          type="time"
          value={`${String(dateFrom.getHours()).padStart(2, "0")}:${String(
            dateFrom.getMinutes()
          ).padStart(2, "0")}`}
          onChange={(e) => {
            const selectedDate = new Date(dateFrom);
            const [hours, minutes] = e.target.value.split(":");
            selectedDate.setHours(Number(hours), Number(minutes));
            setDateFrom(selectedDate);
            WordclockData?.setNightmodeFrom(`${hours}:${minutes}`);
          }}
        />
      </div>

      <div className="row-view">
        <label>To</label>
        <input
          type="time"
          value={`${String(dateTo.getHours()).padStart(2, "0")}:${String(
            dateTo.getMinutes()
          ).padStart(2, "0")}`}
          onChange={(e) => {
            const selectedDate = new Date(dateTo);
            const [hours, minutes] = e.target.value.split(":");
            selectedDate.setHours(Number(hours), Number(minutes));
            setDateTo(selectedDate);
            WordclockData?.setNightmodeTo(`${hours}:${minutes}`);
          }}
        />
      </div>

      <div className="row-view">
        <label>Activate?</label>
        <input
          type="checkbox"
          checked={WordclockData.nightmode}
          onChange={(e) => WordclockData.setNightmode(e.target.checked)}
        />
      </div>

      <div className="row-view">
        <label>Brightness</label>
        <p>Value: {sliderState}%</p>
        <input
          type="range"
          min="0"
          max="100"
          step="5"
          value={sliderState}
          onChange={(e) => setSliderState(Number(e.target.value))}
          onMouseUp={() => WordclockData.setNightmodebright(sliderState)}
        />
      </div>

      <hr className="divider" />

      <div className="row-view">
        <h2>Language</h2>
        <select
          value={newLanguage}
          onChange={(e) => {
            setNewLanguage(e.target.value);
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
